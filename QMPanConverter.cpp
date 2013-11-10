#include "QMPanConverter.h"
#include "QMFrameSoftwareRenderYuv.h"
#include "log4qt/logger.h"
#include <QVideoProbe>
#include <QMediaPlayer>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QFileInfo>

class QMVideoSurfaceFormatGrabber: public QAbstractVideoSurface {
public:

    QMVideoSurfaceFormatGrabber() {
    }

    bool start(const QVideoSurfaceFormat &format) {
        return QAbstractVideoSurface::start(format);
    }

    void stop() {

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

        return true;
    }

};


class QMPanConverterPrivate
{
public:
    QMPanConverter *self;
    Log4Qt::Logger *log;
    QMVideoSurfaceFormatGrabber* mFormatGrabber;
    QVideoProbe *mVideoProbe;
    QMediaPlayer *mMediaPlayer;
    QString mFileName;
    qint32 mCount;
    QProgressBar *mProgressBar;
    QMFrameSoftwareRenderYuv *mFrameConverter;

    void initialize() {
        log = Log4Qt::Logger::logger("QMPanConverter");
        mMediaPlayer = new QMediaPlayer(self);
        mVideoProbe = new QVideoProbe(self);
        mMediaPlayer->setVideoOutput(new QMVideoSurfaceFormatGrabber());
        mVideoProbe->setSource(mMediaPlayer);

        mCount = 0;
        mProgressBar = Q_NULLPTR;
        mFrameConverter = new QMFrameSoftwareRenderYuv(self);

        QObject::connect(mVideoProbe, &QVideoProbe::videoFrameProbed, self, &QMPanConverter::videoFrameLoaded);
        QObject::connect(mMediaPlayer, &QMediaPlayer::positionChanged, self, &QMPanConverter::positionChanged);
    }



    void videoFrameLoaded(const QVideoFrame &frame) {
        log->trace("ENTER videoFrameLoaded");


        log->debug("Pixel format: %1", frame.pixelFormat());
        QImage image;
        if (frame.pixelFormat() == QVideoFrame::Format_RGB32) {
            // Copy const QVideoFrame to mutable QVideoFrame.
            QVideoFrame nonConstFrame = frame;
            // Unlock for reading the stack frame (increment ref pointer)
            nonConstFrame.map(QAbstractVideoBuffer::ReadOnly);
            // Create new image from the frame bits
            image = QImage(
                    nonConstFrame.bits(),
                    nonConstFrame.width(),
                    nonConstFrame.height(),
                    nonConstFrame.bytesPerLine(),
                    QVideoFrame::imageFormatFromPixelFormat(nonConstFrame.pixelFormat()));
            nonConstFrame.unmap();
        } else {
            image = QImage(frame.size(), QImage::Format_RGB32);
            mFrameConverter->convertFrame(frame, &image);
        }
        log->debug("File converted");

        QString imgFileName = QString("%1.%2.png").arg(mFileName).arg(++mCount, 2, 10, QChar('0'));
        //QFile file(imgFileName);
        //file.open(QFile::WriteOnly);

        bool saved = image.save(imgFileName, "png");
        if (saved) {
            log->info("File: %1 saved", imgFileName);
        } else {
            log->info("File: %1 not saved", imgFileName);
        }


        log->trace("LEAVE videoFrameLoaded");
    }

    void acceptUrl(QUrl url) {
        if(mProgressBar) {
            mProgressBar->setRange(0, 100);
            mProgressBar->setValue(0);
        }
        mFileName = url.toLocalFile();
        //mFileName.chop(4);

        mMediaPlayer->setNotifyInterval(1000 / 60);
        mMediaPlayer->setMedia(url);
        mMediaPlayer->setMuted(true);
        mMediaPlayer->setPlaybackRate(1.0);
        mMediaPlayer->play();
    }

     void positionChanged(int pos) {
        int duration = mMediaPlayer->duration();
        if (duration && mProgressBar) {
            mProgressBar->setRange(0, duration);
            mProgressBar->setValue(pos);
        }
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

QProgressBar *QMPanConverter::progressBar()
{
    return p->mProgressBar;
}

void QMPanConverter::setProgressBar(QProgressBar *progressBar)
{
    p->mProgressBar = progressBar;
    emit progressBarChanged();
}


void QMPanConverter::videoFrameLoaded(const QVideoFrame &frame)
{
    p->videoFrameLoaded(frame);
}

void QMPanConverter::positionChanged(int pos)
{
    p->positionChanged(pos);
}

void QMPanConverter::acceptUrl(QUrl url)
{
    p->acceptUrl(url);
}
