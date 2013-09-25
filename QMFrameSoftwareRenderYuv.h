#ifndef QMFRAMESOFTWARERENDERYUV_H
#define QMFRAMESOFTWARERENDERYUV_H

#include <QObject>
#include <QVideoFrame>

class QMFrameSoftwareRenderYuvPrivate;

class QMFrameSoftwareRenderYuv : public QObject
{
    Q_OBJECT
public:
    explicit QMFrameSoftwareRenderYuv(QObject *parent = 0);
    ~QMFrameSoftwareRenderYuv();

    void convertFrame(QVideoFrame frame, QImage  *image);
signals:

public slots:

private:
    QScopedPointer<QMFrameSoftwareRenderYuvPrivate> p;
};

#endif // QMFRAMESOFTWARERENDERYUV_H
