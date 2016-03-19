#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T20:15:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NTUST-CG-Pro1
TEMPLATE = app

# header of libraries
# INCLUDEPATH += A \
#                B \
#

# 3-party, compiled files
# LIBS += \
#         \
#

SOURCES += src/main.cpp \
           src/MB.cpp \
           src/Application.cpp \
           src/OpenglRenderThread.cpp \
           src/OpenglWidget.cpp \
           src/qt_opengl_framework.cpp

HEADERS += src/Application.h \
           src/ClassDefine.h \
           src/MB.h \
           src/OpenglRenderThread.h \
           src/OpenglWidget.h \
           src/qt_opengl_framework.h

FORMS   += src/qt_opengl_framework.ui \
           src/prompt.ui

# RESOURCES = *.qrc\
