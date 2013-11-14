#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QMPanConverter.h"
#include "QMDirConverter.h"

#include <QFileDialog>

class MainWindowPrivate
{
public:
    MainWindow *self;
    QScopedPointer<Ui::MainWindow> ui;
    QMPanConverter *mConverter;

    void initialize() {
        ui.reset(new Ui::MainWindow);
        ui->setupUi(self);

        QObject::connect(ui->actionConvertMovie, SIGNAL(triggered()), self, SLOT(openSingleFileDialog()));
        QObject::connect(ui->actionConvertFolder, SIGNAL(triggered()), self, SLOT(openFolderDialog()));
    }



};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    p(new MainWindowPrivate)
{
    p->self = this;
    p->initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::openSingleFileDialog()
{
    const QFileDialog::Options options = QFlag(0);
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open PAN movie"),
                                tr(""),
                                tr("All Files (*);;Movie Files (*.mp4)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists()) {
            QMPanConverter *converter = new QMPanConverter(this);
            converter->setProgressBar(p->ui->fileProgressBar);
            converter->acceptUrl(QUrl::fromLocalFile(fileName));
        }

    }
}

void MainWindow::openFolderDialog()
{
    QString selectedFilter;
    QString folderName = QFileDialog::getExistingDirectory(this,
                                tr("Open Folder"));
    if (!folderName.isEmpty()) {
        QMDirConverter *converter = new QMDirConverter(this);
        converter->setProgressBar(p->ui->totalProgressBar);
        QMPanConverter *panConverter = new QMPanConverter(this);
        panConverter->setProgressBar(p->ui->fileProgressBar);

        QStringList filters;
        filters.append("*PAN");
        converter->acceptUrl(QUrl::fromLocalFile(folderName), filters, panConverter);

    }
}
