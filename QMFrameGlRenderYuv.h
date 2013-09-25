#ifndef QMFRAMEGLRENDERYUV_H
#define QMFRAMEGLRENDERYUV_H

#include <QObject>
#include <QVideoFrame>

class QMFrameGlRenderYuvPrivate;

class QMFrameGlRenderYuv : public QObject
{
    Q_OBJECT
public:
    explicit QMFrameGlRenderYuv(QObject *parent = 0);
    ~QMFrameGlRenderYuv();

signals:

public slots:
    void acceptFrame(const QVideoFrame &frame);

private:
    QScopedPointer<QMFrameGlRenderYuvPrivate> p;
};

#endif // QMFRAMEGLRENDERYUV_H
