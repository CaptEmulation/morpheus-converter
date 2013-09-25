#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

void setUpLog4Qt() {
    QSettings s;

    // Set logging level for Log4Qt to TRACE
    s.beginGroup("Log4Qt");
    s.setValue("Debug", "INFO");

    // Configure logging to log to the file C:/myapp.log using the level TRACE
    s.beginGroup("Properties");
    s.setValue("log4j.appender.A1", "org.apache.log4j.FileAppender");
    s.setValue("log4j.appender.A1.file", "morpheusConverter.log");
    s.setValue("log4j.appender.A1.layout", "org.apache.log4j.TTCCLayout");
    s.setValue("log4j.appender.A1.layout.DateFormat", "ISO8601");
    s.setValue("log4j.rootLogger", "DEBUG, A1");

    // Settings will become active on next application startup
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Morpheus-Converter");
    a.setOrganizationName("Soapbubble Productions");
    a.setOrganizationDomain("www.5555555555.com");

    setUpLog4Qt();

    MainWindow w;
    w.show();
    
    return a.exec();
}
