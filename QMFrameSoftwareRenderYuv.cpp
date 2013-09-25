#include "QMFrameSoftwareRenderYuv.h"

class QMFrameSoftwareRenderYuvPrivate {
public:
    QMFrameSoftwareRenderYuv *self;

    void convertFrame(QVideoFrame &frame, QImage *image) {
        if(frame.pixelFormat() == QVideoFrame::Format_YUV420P && frame.map(QAbstractVideoBuffer::ReadOnly)) {
            const qint32 frameWidth = frame.width();
            const qint32 frameHeight = frame.height();
            const qint32 frameSize = frameHeight * frameWidth;
            const uchar *lumaYBytes = frame.bits();
            const uchar *chromaUBytes = lumaYBytes + frameSize;
            const uchar *chromaVBytes = chromaUBytes + (frameSize / 4);

            for (int y = 0; y < frameHeight; y++) {
                for (int x = 0; x < frameWidth; x++) {
                    const int Y = lumaYBytes[y * frameWidth + x];
                    const int U = chromaUBytes[(y / 2) * (frameWidth / 2) + (x / 2)];
                    const int V = chromaVBytes[(y / 2) * (frameWidth / 2) + (x / 2)];
                    const int r = qBound(0.0, 1.164 * (Y - 16) + 1.596 * (V - 128), 255.0);
                    const int g = qBound(0.0, 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128), 255.0);
                    const int b = qBound(0.0, 1.164 * (Y - 16) + 2.018 * (U - 128), 255.0);

                    image->setPixel(x, y, qRgb(r, g, b));
                }
            }
            frame.unmap();
        }
    }
};

QMFrameSoftwareRenderYuv::QMFrameSoftwareRenderYuv(QObject *parent) :
    QObject(parent),
    p(new QMFrameSoftwareRenderYuvPrivate)
{
    p->self = this;
}

QMFrameSoftwareRenderYuv::~QMFrameSoftwareRenderYuv()
{

}

void QMFrameSoftwareRenderYuv::convertFrame(QVideoFrame frame, QImage  *image)
{
    p->convertFrame(frame, image);
}
