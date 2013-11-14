#ifndef QMDIRCONVERTER_H
#define QMDIRCONVERTER_H

#include <QObject>
#include <QProgressBar>

#include "QMPanConverter.h"

class QMDirConverterPrivate;

class QMDirConverter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QProgressBar *progressBar READ progressBar WRITE setProgressBar NOTIFY progressBarChanged)

public:
    explicit QMDirConverter(QObject *parent = 0);
    ~QMDirConverter();

    QProgressBar *progressBar() const;
    void setProgressBar(QProgressBar* progressBar);

signals:
    void progressBarChanged();

public slots:
    void acceptUrl(QUrl url, QStringList filters, QMPanConverter *converter);

private:
    QScopedPointer<QMDirConverterPrivate> p;
};

#endif // QMDIRCONVERTER_H
