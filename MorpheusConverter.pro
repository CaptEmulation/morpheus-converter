#-------------------------------------------------
#
# Project created by QtCreator 2013-07-27T19:17:52
#
#-------------------------------------------------

#Log4Qt
include(../log4qt-code/src/log4qt/log4qt.pri)

QT       += core gui multimedia multimediawidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MorpheusConverter
TEMPLATE = app

shader.source = shader
shader.target = shader
DEPLOYMENTFOLDERS += shader

SOURCES += main.cpp\
        MainWindow.cpp \
        QMPanConverter.cpp \
    QMDirConverter.cpp \
    QMConverterRouter.cpp \
    QMFrameGlRenderRgb.cpp \
    QMFrameGlRenderYuv.cpp \
    QMFrameSoftwareRenderYuv.cpp

HEADERS  += MainWindow.h \
        QMPanConverter.h \
    QMDirConverter.h \
    QMConverterRouter.h \
    QMIAcceptUrl.h \
    QMFrameGlRenderRgb.h \
    QMFrameGlRenderYuv.h \
    QMFrameSoftwareRenderYuv.h

FORMS    += MainWindow.ui

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD_WIN}\\$${FILE} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    }
}
unix {
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
    }
}

RESOURCES += \
    shaders.qrc
