#ifndef QMPANCONVERTER_H
#define QMPANCONVERTER_H

#include <QObject>
#include <QVideoWidget>
#include <QVideoFrame>
#include <QFile>
#include <QProgressBar>

class QMPanConverterPrivate;

class QMPanConverter : public QObject
{
    friend class QMPanConverterPrivate;
    Q_OBJECT
    Q_PROPERTY(QProgressBar *progressBar READ progressBar WRITE setProgressBar NOTIFY progressBarChanged);

public:
    explicit QMPanConverter(QObject *parent = 0);
    ~QMPanConverter();

    QProgressBar *progressBar();
    void setProgressBar(QProgressBar* progressBar);
    
signals:
    void progressBarChanged();
    
public slots:
    void acceptFile(QString movieFileName);

protected slots:
    void videoFrameLoaded(const QVideoFrame &frame);
    void positionChanged(int pos);

private:
    QScopedPointer<QMPanConverterPrivate> p;
};

#endif // QMPANCONVERTER_H
