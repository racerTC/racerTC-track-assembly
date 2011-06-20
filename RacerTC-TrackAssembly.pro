#-------------------------------------------------
#
# Project created by QtCreator 2011-02-22T21:24:44
#
#-------------------------------------------------

QT       += core gui xml

TARGET = RacerTC-TrackAssembly
TEMPLATE = app


SOURCES += src/main.cpp\
        src/AssemblyMainWindow.cpp \
    src/TrackView.cpp \
    src/Track.cpp \
    src/QuadTree.cpp \
    src/TrackDataContainer.cpp \
    src/TrackData.cpp \
    src/TrackArray.cpp

HEADERS  += AssemblyMainWindow.h \
    src/TrackView.h \
    src/Track.h \
    src/QuadTree.h \
    src/TrackDataContainer.h \
    src/TrackData.h \
    src/TrackArray.h \
    src/Matrix.h

FORMS    += src/AssemblyMainWindow.ui
