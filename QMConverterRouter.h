#ifndef QMCONVERTERROUTER_H
#define QMCONVERTERROUTER_H

#include <QObject>
#include <QUrl>

class QMConverterRouterPrivate;

class QMConverterRouter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObjectList converters READ converters)
public:
    explicit QMConverterRouter(QObject *parent = 0);
    ~QMConverterRouter();
    
    QObjectList converters();
    void addConverter(QObject *converter);
signals:
    
public slots:
    void acceptFile(QUrl fileUrl);
    void acceptDir(QUrl dirUrl);

private:
    QScopedPointer<QMConverterRouterPrivate> p;
};

#endif // QMCONVERTERROUTER_H
