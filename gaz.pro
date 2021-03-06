#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T09:56:08
#
#-------------------------------------------------

QT       += core gui opengl script

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS_RELEASE *= -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE *= -O3

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
    scripteditor.cc \
    paircorfunwidget.cc

HEADERS  += glwidget.hh \
    system.hh \
    particle.hh \
    vec3.hh \
    random.hh \
    glsphere.hh \
    mainwindow.hh \
    scriptreader.hh \
    scripteditor.hh \
    paircorfunwidget.hh

FORMS += \
    scripteditor.ui
