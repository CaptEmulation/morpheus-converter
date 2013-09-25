#include "QMFrameGlRenderYuv.h"
#include "log4qt/logger.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>


static const GLfloat sTexCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };

class QMFrameGlRenderYuvPrivate {
public:

    QMFrameGlRenderYuv *self;
    Log4Qt::Logger *log;
    QOpenGLShaderProgram *mShaderProgram;
    GLint mUniformSamplers[3];
    GLuint mTextureIds[3];
    GLfloat mVertices[8];
    GLint  mUniformMatrix;

    enum {
        ATTRIBUTE_VERTEX,
        ATTRIBUTE_TEXCOORD
    };

    void loadProgram() {
        mShaderProgram = new QOpenGLShaderProgram(self);
        QOpenGLShader *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, self);
        bool fragementCompiled = fragmentShader->compileSourceFile(":/shader/yuvFragment.glsl");
        QOpenGLShader *vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, self);
        bool vertexCompiled = vertexShader->compileSourceFile(":/shader/vertexShader.glsl");
        if(fragementCompiled && vertexCompiled) {
            if(mShaderProgram->addShader(fragmentShader) && mShaderProgram->addShader(vertexShader)) {
                mShaderProgram->bindAttributeLocation("position", ATTRIBUTE_VERTEX);
                mShaderProgram->bindAttributeLocation("texcord", ATTRIBUTE_TEXCOORD);
                if(!mShaderProgram->link()) {
                    log->error("Failed to link shader program");
                    return;
                }
                mUniformMatrix = mShaderProgram->uniformLocation("modelViewProjectionMatrix");
                mUniformSamplers[0] = mShaderProgram->uniformLocation("s_texture_y");
                mUniformSamplers[1] = mShaderProgram->uniformLocation("s_texture_u");
                mUniformSamplers[2] = mShaderProgram->uniformLocation("s_texture_v");
            } else {
                log->error("Failed to add shader program");
            }
        } else {
            log->error("Failed to compile shader program");
        }
    }

    void acceptFrame(const QVideoFrame &frame) {


        const qint32 frameWidth = frame.width();
        const qint32 frameHeight = frame.height();
        const qint32 frameSize = frameHeight * frameWidth;
        const uchar *lumaYBytes = frame.bits();
        const uchar *chromaUBytes = lumaYBytes + frameSize;
        const uchar *chromaVBytes = chromaUBytes + (frameSize / 4);

        updateVertices(frame);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(3, mTextureIds);
        const uchar *pixels[3] = { lumaYBytes, chromaUBytes, chromaVBytes };
        const int widths[3] = { frameWidth, frameWidth / 2, frameWidth / 2 };
        const int heights[3] = { frameHeight, frameHeight / 2, frameHeight / 2 };
        for (int i = 0; i < 3; i++) {
            glBindTexture(GL_TEXTURE_2D, mTextureIds[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, widths[i], heights[i], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        QOpenGLFramebufferObject *fbo = new QOpenGLFramebufferObject(QSize(frameWidth, frameHeight), GL_TEXTURE_2D);

        fbo->bind();

        glViewport(0, 0, frameWidth, frameHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        mShaderProgram->bind();
        for (int i = 0; i < 3; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextureIds[i]);
            glUniform1i(mUniformSamplers[i], i);
        }
        glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, 0, 0, mVertices);
        glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
        glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, sTexCoords);
        glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);

        if (prepareRender()) {
            GLfloat modelviewProj[16];
            mat4f_LoadOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, modelviewProj);
            glUniformMatrix4fv(mUniformMatrix, 1, GL_FALSE, modelviewProj);

            glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, 0, 0, mVertices);
            glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
            glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, sTexCoords);
            glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
        }

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void updateVertices(const QVideoFrame &frame) {
        const float w  = frame.width();
        const float h  = frame.height();

        mVertices[0] = - w;
        mVertices[1] = - h;
        mVertices[2] =   w;
        mVertices[3] = - h;
        mVertices[4] = - w;
        mVertices[5] =   h;
        mVertices[6] =   w;
        mVertices[7] =   h;
    }

    bool prepareRender() {
        if (mTextureIds[0] == 0)
            return false;

        for (int i = 0; i < 3; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextureIds[i]);
            glUniform1i(mUniformSamplers[i], i);
        }

        return true;
    }

    void mat4f_LoadOrtho(float left, float right, float bottom, float top, float near, float far, float* mout) {
        float r_l = right - left;
        float t_b = top - bottom;
        float f_n = far - near;
        float tx = - (right + left) / (right - left);
        float ty = - (top + bottom) / (top - bottom);
        float tz = - (far + near) / (far - near);

        mout[0] = 2.0f / r_l;
        mout[1] = 0.0f;
        mout[2] = 0.0f;
        mout[3] = 0.0f;

        mout[4] = 0.0f;
        mout[5] = 2.0f / t_b;
        mout[6] = 0.0f;
        mout[7] = 0.0f;

        mout[8] = 0.0f;
        mout[9] = 0.0f;
        mout[10] = -2.0f / f_n;
        mout[11] = 0.0f;

        mout[12] = tx;
        mout[13] = ty;
        mout[14] = tz;
        mout[15] = 1.0f;
    }
};

QMFrameGlRenderYuv::QMFrameGlRenderYuv(QObject *parent) :
    QObject(parent),
    p(new QMFrameGlRenderYuvPrivate)
{
    p->self = this;
    p->log = Log4Qt::Logger::logger("QMFrameGlRenderYuv");
    p->loadProgram();
}

QMFrameGlRenderYuv::~QMFrameGlRenderYuv()
{

}

void QMFrameGlRenderYuv::acceptFrame(const QVideoFrame &frame)
{
    p->acceptFrame(frame);
}
