#include "QMConverterRouter.h"

class QMConverterRouterPrivate
{
public:

    QObjectList mConverters;
};

QMConverterRouter::QMConverterRouter(QObject *parent) :
    QObject(parent),
    p(new QMConverterRouterPrivate)
{

}

QMConverterRouter::~QMConverterRouter()
{
}

QObjectList QMConverterRouter::converters()
{
    return p->mConverters;
}

void QMConverterRouter::addConverter(QObject *converter)
{
    p->mConverters.append(converter);
}

void QMConverterRouter::acceptFile(QString filePath)
{
}

void QMConverterRouter::acceptDir(QString dirPath)
{
}
