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

#include "footrackinfo.hpp"

#include <taglib/tag.h>
#include <taglib/fileref.h>

namespace FooAudio
{
	const int DEFAUL_INTERVAL   = 250;

	class PhononEngine::PhononEnginePrivate
	{
	public:
		Phonon::MediaObject *mediaObject;
		Phonon::AudioOutput *audioOutput;
		Phonon::MediaObject *metaInformation;
	};

	PhononEngine::PhononEngine(QObject * parent) : AbstractAudioPlugin(parent)
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

	void PhononEngine::setMuted(bool mute)
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

	QMap<FooTrackInfo::FooTrackInfo, QString> PhononEngine::metaData(const QUrl url)
	{
		QMap<FooTrackInfo::FooTrackInfo, QString> meta;

		TagLib::FileRef f(url.toString().toStdString().c_str());

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

			meta.insert(FooTrackInfo::title, f.tag()->title().toCString(true));
			meta.insert(FooTrackInfo::album, f.tag()->album().toCString(true));
			meta.insert(FooTrackInfo::artist, f.tag()->artist().toCString(true));
			meta.insert(FooTrackInfo::comment, f.tag()->comment().toCString(true));
			meta.insert(FooTrackInfo::tracknumber, trackNumber);
			meta.insert(FooTrackInfo::genre, f.tag()->genre().toCString(true));
			meta.insert(FooTrackInfo::date, date);
			meta.insert(FooTrackInfo::channels, channels);
			meta.insert(FooTrackInfo::samplerate, sampleRate);
			meta.insert(FooTrackInfo::bitrate, bitrate);
			meta.insert(FooTrackInfo::length, length);
		}

		return meta;
	}

	QString PhononEngine::metaData(const FooTrackInfo::FooTrackInfo key, const QUrl url)
	{
		//d->metaInformation->setCurrentSource(url);
		//return d->metaInformation->metaData(key);
		return QString();
	}

	QStringList PhononEngine::mimeTypes()
	{
		return Phonon::BackendCapabilities::availableMimeTypes();
	}

	void PhononEngine::newMetaData()
	{
		//		emit metaDataChanged(d->metaInformation->metaData(), d->metaInformation->currentSource().url());
	}
};

