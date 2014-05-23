#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T09:56:08
#
#-------------------------------------------------

QT       += core gui opengl script

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gaz
TEMPLATE = app


SOURCES += main.cc\
        glwidget.cc \
    system.cc \
    particle.cc \
    vec3.cc \
    glsphere.cc \
    mainwindow.cc \
    scriptreader.cc \
    scripteditor.cc

HEADERS  += glwidget.hh \
    system.hh \
    particle.hh \
    vec3.hh \
    random.hh \
    glsphere.hh \
    mainwindow.hh \
    scriptreader.hh \
    scripteditor.hh

FORMS += \
    scripteditor.ui
