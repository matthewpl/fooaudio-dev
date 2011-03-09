TEMPLATE = lib
DEPENDPATH += . ../../fooaudio
INCLUDEPATH += . ../../fooaudio
DESTDIR = ../../../lib
CONFIG += plugin

HEADERS += \
    fooaudioplaylistplsformat.hpp \
    fooaudioplaylistplsformatplugin.hpp \
    ../../fooaudio/fooplaylistformat.hpp \
    ../../fooaudio/fooplaylistformatplugin.hpp

SOURCES += \
    fooaudioplaylistplsformat.cpp \
    fooaudioplaylistplsformatplugin.cpp
