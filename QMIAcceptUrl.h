#ifndef QMIACCEPTURL_H
#define QMIACCEPTURL_H

#include <QObject>
#include <QUrl>

class QMIAcceptUrl {
public:
    virtual void acceptUrl(QUrl url);
};

Q_DECLARE_INTERFACE(
        QMIAcceptUrl,
        "com.soapbubble.QMIAcceptUrl/1.0"
        )

#endif // QMIACCEPTFILE_H
