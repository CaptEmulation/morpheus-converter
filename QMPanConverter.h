#ifndef QMPANCONVERTER_H
#define QMPANCONVERTER_H

#include <QObject>
#include <QVideoWidget>
#include <QVideoFrame>
#include <QFile>

class QMPanConverterPrivate;

class QMPanConverter : public QObject
{
    Q_OBJECT
public:
    explicit QMPanConverter(QObject *videoWidget);
    ~QMPanConverter();
    
signals:
    
public slots:
    void videoFrameLoaded(const QVideoFrame &frame);
    void processMovie(QString movieFileName);

private:
    QScopedPointer<QMPanConverterPrivate> p;
};

#endif // QMPANCONVERTER_H
