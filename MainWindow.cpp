#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QMPanConverter.h"

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
        ui->progressBar->setValue(0);
        QObject::connect(ui->actionConvertMovie, SIGNAL(triggered()), self, SLOT(openFileDialog()));
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

void MainWindow::openFileDialog()
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
        QMPanConverter *converter = new QMPanConverter(this);
        converter->setProgressBar(p->ui->progressBar);
        converter->processMovie(fileName);
    }
}
