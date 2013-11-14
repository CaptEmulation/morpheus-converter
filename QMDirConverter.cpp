#include "QMDirConverter.h"

#include <QDir>

class QMDirConverterPrivate
{
public:
    QMDirConverter *self;
    QProgressBar *mProgressBar;

    QProgressBar *progressBar() const {
        return mProgressBar;
    }

    void setProgressBar(QProgressBar* progressBar) {
        if (progressBar != mProgressBar) {
            mProgressBar = progressBar;
            emit self->progressBarChanged();
        }
    }

    void acceptUrl(QUrl url, QStringList filters, QMPanConverter *converter) {
        QDir dir(url.toLocalFile());

        if (dir.exists()) {
            if (mProgressBar) {
                mProgressBar->setRange(0, getFileCount(dir, filters));
                mProgressBar->setValue(0);
            }
            recurseDir(dir, filters, converter);
        }
    }

    int getFileCount(QDir dir, QStringList filters) {
        int items = dir.entryList(filters).length();
        // Look for folders
        foreach (QString item, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QDir dirEntry(dir.filePath(item));
            if (dirEntry.exists()) {
                items += getFileCount(dirEntry, filters);
            }
        }

        return items;
    }

    void recurseDir(QDir dir, QStringList filters, QMPanConverter *converter) {
        foreach (QString entry, dir.entryList(filters)) {
            QFileInfo info(dir.filePath(entry));
            if (info.exists()) {
                QUrl url = QUrl::fromLocalFile(info.absoluteFilePath());
                mProgressBar->setValue(mProgressBar->value() + 1);
                converter->acceptUrl(url);
            }
        }

        foreach (QString entry, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QDir dirEntry(dir.filePath(entry));
            if (dirEntry.exists()) {
                recurseDir(dirEntry, filters, converter);
            }
        }

    }

};

QMDirConverter::QMDirConverter(QObject *parent) :
    QObject(parent),
    p(new QMDirConverterPrivate)
{
    p->self = this;
    p->mProgressBar = Q_NULLPTR;
}

QMDirConverter::~QMDirConverter()
{

}

QProgressBar *QMDirConverter::progressBar() const
{
    return p->progressBar();

}

void QMDirConverter::setProgressBar(QProgressBar *progressBar)
{
    p->setProgressBar(progressBar);
}

void QMDirConverter::acceptUrl(QUrl url, QStringList filters, QMPanConverter *converter)
{
    p->acceptUrl(url, filters, converter);
}
