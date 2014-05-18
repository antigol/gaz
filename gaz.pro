#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T09:56:08
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gaz
TEMPLATE = app


SOURCES += main.cc\
        glwidget.cc \
    system.cc \
    particle.cc \
    vec3.cc \
    glsphere.cc \
    mainwindow.cc

HEADERS  += glwidget.hh \
    system.hh \
    particle.hh \
    vec3.hh \
    random.hh \
    glsphere.hh \
    mainwindow.hh

QMAKE_CXXFLAGS += -std=c++11

FORMS += \
    widgettool.ui

RESOURCES += \
    res.qrc
