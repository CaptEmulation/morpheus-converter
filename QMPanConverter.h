#ifndef QMPANCONVERTER_H
#define QMPANCONVERTER_H

#include "QMIAcceptUrl.h"

#include <QObject>
#include <QVideoWidget>
#include <QVideoFrame>
#include <QUrl>
#include <QProgressBar>

class QMPanConverterPrivate;

class QMPanConverter : public QObject, public QMIAcceptUrl
{
    friend class QMPanConverterPrivate;
    Q_OBJECT
    Q_INTERFACES(QMIAcceptUrl)
    Q_PROPERTY(QProgressBar *progressBar READ progressBar WRITE setProgressBar NOTIFY progressBarChanged)

public:
    explicit QMPanConverter(QObject *parent = 0);
    ~QMPanConverter();

    QProgressBar *progressBar() const;
    void setProgressBar(QProgressBar* progressBar);
    
signals:
    void progressBarChanged();
    
public slots:
    void acceptUrl(QUrl url);

protected slots:
    void videoFrameLoaded(const QVideoFrame &frame);
    void positionChanged(int pos);

private:
    QScopedPointer<QMPanConverterPrivate> p;
};

#endif // QMPANCONVERTER_H
