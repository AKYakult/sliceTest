#-------------------------------------------------
#
# Project created by QtCreator 2022-10-07T11:15:26
#
#-------------------------------------------------

QT       += core gui
QT       +=core gui opengl concurrent

LIBS     +=-lopengl32 -lglu32
LIBS += -L$$PWD/assimp/lib64/ -lassimp
INCLUDEPATH += $$PWD/assimp
DEPENDPATH += $$PWD/assimp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GreyImage
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mesh.cpp \
    model.cpp \
    sliceable.cpp \
    slicer.cpp \
    ed.cpp \
    edcolor.cpp \
    edpf.cpp

HEADERS += \
        mainwindow.h \
    mesh.h \
    model.h \
    sliceable.h \
    slicer.h \
    ed.h \
    edcolor.h \
    edlib.h \
    edpf.h \
    rdp.h \
    reject_sharp_turn.h \
    detinflexpt.h \
    mytools.h

FORMS += \
        mainwindow.ui

RESOURCES +=

DISTFILES +=



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world460
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world460d

INCLUDEPATH += $$PWD/opencv/build/x64/vc15
DEPENDPATH += $$PWD/opencv/build/x64/vc15

INCLUDEPATH += $$PWD/opencv/build/include
DEPENDPATH += $$PWD/opencv/build/include

INCLUDEPATH += $$PWD/eigen-3.4.0


