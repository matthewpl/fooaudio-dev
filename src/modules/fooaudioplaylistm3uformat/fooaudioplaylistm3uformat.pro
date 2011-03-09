TEMPLATE = lib
DEPENDPATH += . ../../fooaudio
INCLUDEPATH += . ../../fooaudio
DESTDIR = ../../../lib
CONFIG += plugin

HEADERS += \
    fooaudioplaylistm3uformat.hpp \
    fooaudioplaylistm3uformatplugin.hpp \
    ../../fooaudio/fooplaylistformatplugin.hpp \
    ../../fooaudio/fooplaylistformat.hpp

SOURCES += \
    fooaudioplaylistm3uformat.cpp \
    fooaudioplaylistm3uformatplugin.cpp
