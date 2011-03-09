/**********************************************************************
 *
 * fooaudio
 * Copyright (C) 2009-2010  fooaudio team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "phononengine.h"

#include <QtDebug>
#include <QtPlugin>

#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/backendcapabilities.h>

#include "footrack.hpp"
#include "footrackinfo.hpp"

#include <taglib/tag.h>
#include <taglib/fileref.h>

const int DEFAUL_INTERVAL   = 250;

class PhononEngine::PhononEnginePrivate
{
public:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	Phonon::MediaObject *metaInformation;
};

PhononEngine::PhononEngine(QObject * parent) : FooAudioEngine(parent)
{
	d = new PhononEnginePrivate;

	d->audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	d->mediaObject = new Phonon::MediaObject(this);
	d->metaInformation = new Phonon::MediaObject(this);

	Phonon::createPath(d->mediaObject, d->audioOutput);

	d->mediaObject->setTickInterval(DEFAUL_INTERVAL);

	connect(d->mediaObject, SIGNAL(tick(qint64)), this, SIGNAL (progress(qint64)));
	connect(d->mediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
	connect(d->mediaObject, SIGNAL(totalTimeChanged(qint64)), this, SIGNAL(changeTotalTime(qint64)));
	connect(d->metaInformation, SIGNAL(metaDataChanged()), this, SLOT(newMetaData()));
	connect(d->audioOutput, SIGNAL(mutedChanged(bool)), this, SIGNAL(muteChanged(bool)));
	connect(this, SIGNAL(seek(qint64)), d->mediaObject, SLOT(seek(qint64)));
}

PhononEngine::~PhononEngine()
{
	delete d;
}

bool PhononEngine::isPlaying()
{
	return d->mediaObject->state() == Phonon::PlayingState;
}

bool PhononEngine::isStopped()
{
	Phonon::State state = d->mediaObject->state();
	return state == Phonon::StoppedState || state == Phonon::LoadingState;
}

bool PhononEngine::isPaused()
{
	return d->mediaObject->state() == Phonon::PausedState;
}

bool PhononEngine::isMuted()
{
	return d->audioOutput->isMuted();
}

void PhononEngine::mute(bool mute)
{
	d->audioOutput->setMuted(mute);
}

void PhononEngine::stop()
{
	d->mediaObject->stop();
}

void PhononEngine::play()
{
	d->mediaObject->play();
}

void PhononEngine::pause()
{
	d->mediaObject->pause();
}

void PhononEngine::clearQueue()
{
	d->mediaObject->clearQueue();
}

void PhononEngine::enqueueNextFile(const QUrl file)
{
	qDebug() << "PhononEngine::enqueueNextFile"
			<< "Next song: " << file.toString();

	if (!file.isEmpty())
	{
		d->mediaObject->enqueue(file.toString());
		emit willPlayNow(file);
	}
	else
	{
		d->mediaObject->stop();
		d->mediaObject->clearQueue();
	}
}

void PhononEngine::playFile(const QUrl file)
{
	qDebug() << "PhononEngine::playfile";

	if (!file.isEmpty())
	{
		qDebug() << "PhononEngine::playFile: is not Empty: "
				<< file.toString();
		emit willPlayNow(file);
		d->mediaObject->stop();
		d->mediaObject->clearQueue();
		d->mediaObject->setCurrentSource(file.toString());
		d->mediaObject->play();
	}
	else
	{
		qDebug() << "FooPhononAudioEngine::playFile: is Empty";
		d->mediaObject->stop();
	}
}

void PhononEngine::setVolume(const int volume)
{
	qreal v = qreal(volume) / 100;
	d->audioOutput->setVolume(v);
}

void PhononEngine::seekTrack(qint64 t)
{
	d->mediaObject->seek(t);
}

int PhononEngine::getVolume()
{
	return int(d->audioOutput->volume() * 100);
}

void PhononEngine::metaData(Fooaudio::FooTrack& track)
{
	TagLib::FileRef f(track.file().toString().toLocal8Bit());

	if (!f.isNull())
	{
		QString trackNumber;
		QString date;
		QString channels;
		QString sampleRate;
		QString bitrate;
		QString length;

		trackNumber.setNum(f.tag()->track());
		date.setNum(f.tag()->year());
		channels.setNum(f.audioProperties()->channels());
		sampleRate.setNum(f.audioProperties()->sampleRate());
		bitrate.setNum(f.audioProperties()->bitrate());
		length.setNum(f.audioProperties()->length());

		track.setTitle(f.tag()->title().toCString(true));
		track.setAlbum(f.tag()->album().toCString(true));
		track.setArtist(f.tag()->artist().toCString(true));
		track.setTrackNumber(trackNumber);
		track.setGenre(f.tag()->genre().toCString(true));
		track.setDate(date);
		track.setLenght(f.audioProperties()->length());
	}
}

QStringList PhononEngine::mimeTypes()
{
	return Phonon::BackendCapabilities::availableMimeTypes();
}

void PhononEngine::newMetaData()
{
	//		emit metaDataChanged(d->metaInformation->metaData(), d->metaInformation->currentSource().url());
}
