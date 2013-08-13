#include "QMPanConverter.h"
#include <QVideoProbe>
#include <QMediaPlayer>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QFileInfo>

class QMVideoSurfaceFormatGrabber: public QAbstractVideoSurface
{
public:
    QVideoSurfaceFormat mFormat;
    QVideoSurfaceFormat format() { return mFormat; }

    QMVideoSurfaceFormatGrabber() {
    }

    bool start(const QVideoSurfaceFormat &format) {
        mFormat = format;
        return QAbstractVideoSurface::start(format);
    }

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const {
        if (handleType == QAbstractVideoBuffer::NoHandle) {
            return QList<QVideoFrame::PixelFormat>()
                    << QVideoFrame::Format_RGB32
                    << QVideoFrame::Format_ARGB32
                    << QVideoFrame::Format_ARGB32_Premultiplied
                    << QVideoFrame::Format_RGB565
                    << QVideoFrame::Format_RGB555;
        } else {
            return QList<QVideoFrame::PixelFormat>();
        }
    }

    bool present(const QVideoFrame &frame) {
        // do nothing
        return true;
    }

};

class QMPanConverterPrivate
{
public:
    QMPanConverter *self;
    QScopedPointer<QMVideoSurfaceFormatGrabber> mFormatGrabber;
    QVideoProbe *mVideoProbe;
    QMediaPlayer *mMediaPlayer;
    QString mFileName;
    QImage::Format mFormat;
    qint32 mCount;

    void initialize() {
        mFormatGrabber.reset(new QMVideoSurfaceFormatGrabber());
        mMediaPlayer = new QMediaPlayer(self);
        mVideoProbe = new QVideoProbe(self);
        mMediaPlayer->setVideoOutput(mFormatGrabber.data());
        mVideoProbe->setSource(mMediaPlayer);

        mCount = 0;
        QObject::connect(mVideoProbe, &QVideoProbe::videoFrameProbed, self, &QMPanConverter::videoFrameLoaded);
    }

    void videoFrameLoaded(const QVideoFrame &frame) {
        // TODO save video frame or something

        // Copy const QVideoFrame to mutable QVideoFrame.
        QVideoFrame nonConstFrame = frame;
        // Unlock for reading the stack frame (increment ref pointer)
        nonConstFrame.map(QAbstractVideoBuffer::ReadOnly);
        // Create new image from the frame bits
        QImage image(
                nonConstFrame.bits(),
                nonConstFrame.width(),
                nonConstFrame.height(),
                nonConstFrame.bytesPerLine(),
                mFormat);

        QString imgFileName = QString("%1.%2.PNG").arg(mFileName).arg(mCount++);
        image.save(imgFileName);

        // Decrement ref pointer
        nonConstFrame.unmap();
    }

    void processMovie(QString movieFileName) {
        mFileName = movieFileName;
        mFileName.chop(4);
        QFileInfo fileInfo(mFileName);
        if (fileInfo.exists()) {

        }
        QUrl url = QUrl::fromLocalFile(mFileName);

        mMediaPlayer->setMedia(url);
        mMediaPlayer->play();
    }
};

QMPanConverter::QMPanConverter(QObject *parent) :
    QObject(parent),
    p(new QMPanConverterPrivate)
{
    p->self = this;
    p->initialize();
}

QMPanConverter::~QMPanConverter()
{
}


void QMPanConverter::videoFrameLoaded(const QVideoFrame &frame)
{
    p->videoFrameLoaded(frame);
}

void QMPanConverter::processMovie(QString movieFileName)
{
    p->processMovie(movieFileName);
}
