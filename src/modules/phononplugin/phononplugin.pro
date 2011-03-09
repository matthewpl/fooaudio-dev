QT += phonon
TEMPLATE = lib
LIBS += -L/usr/lib -ltag
DEPENDPATH += . ../../fooaudio
INCLUDEPATH += . ../../fooaudio
DESTDIR = ../../../lib
CONFIG += plugin

HEADERS +=  phononengine.h \
				phononplugin.h \
	 ../../fooaudio/footrack.hpp \
	 ../../fooaudio/fooaudioengineplugin.hpp \
	 ../../fooaudio/fooaudioengine.hpp

SOURCES +=  phononengine.cpp \
				phononplugin.cpp \
	 ../../fooaudio/footrack.cpp
