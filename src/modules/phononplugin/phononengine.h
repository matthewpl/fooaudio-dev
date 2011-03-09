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

#ifndef PHONONENGINE_HPP
#define PHONONENGINE_HPP

#include <fooaudioengine.hpp>

#include "footrackinfo.hpp"
#include <QObject>
#include <QUrl>

class PhononEngine : public Fooaudio::FooAudioEngine
{
	Q_OBJECT

public:
	PhononEngine(QObject *parent = 0);
	~PhononEngine();

	bool isPlaying();
	bool isStopped();
	bool isPaused();
	bool isMuted();
	int getVolume();

	void metaData(Fooaudio::FooTrack& track);
	QStringList mimeTypes();

signals:
	void aboutToFinish();
	void progress(const qint64 time);
	void changeTotalTime(qint64 time);
	void willPlayNow(const QUrl file);
	void metaDataChanged(QMultiMap<Fooaudio::FooTrackInfo::FooTrackInfo, QString> newMetaData, QUrl url);
	void muteChanged(bool mute);

public slots:
	void stop();
	void play();
	void pause();
	void clearQueue();
	void enqueueNextFile(const QUrl file);
	void playFile(const QUrl file);
	void setVolume(const int volume);
	void seekTrack(qint64);
	void mute(bool mute);

private slots:
	void newMetaData();

private:
	class PhononEnginePrivate;
	PhononEnginePrivate * d;
};

#endif // PHONONENGINE_HPP
