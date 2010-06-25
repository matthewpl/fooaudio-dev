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

#ifndef FOOPLAYERMANAGER_HPP
#define FOOPLAYERMANAGER_HPP

#include <QObject>

#include "abstractaudioplugin.h"
#include "fooplaylistmanager.hpp"
#include "fooplaybackorder.hpp"

class FooPlayerManager : public QObject
{
Q_OBJECT
public:
	 explicit FooPlayerManager(FooPlaylistManager *playlistManager, FooAudio::AbstractAudioPlugin *engine, QObject *parent = 0);
	 ~FooPlayerManager();

	 qint64 getTotalTime();
	 qint64 getCurrentTrackTime();
	 bool isMuted();
	 int getVolume();
	 FooPlaybackOrder::FooPlaybackOrder getPlaybackOrder();

private:
	 FooPlaylistManager *playlistManager;
	 FooAudio::AbstractAudioPlugin *engine;

	 qint64 totalTrackTime;
	 qint64 currentTrackTime;
	 FooPlaybackOrder::FooPlaybackOrder playbackOrder;

signals:
	 void enqueueNextTrack(QUrl);
	 void muted(bool);
	 void currentTrackTimeChanged(qint64 position, qint64 totalTime);
	 void seek(qint64);

public slots:
	void next();
	void previous();
	void pause();
	void play();
	void stop();
	void mute();
	void random();

	void enqueueNextTrack();

	void setVolume(int);

	void changeTotalTrackTime(qint64 t);
	void changeCurrentTrackTime(qint64 t);
	void changePlaybackOrder(FooPlaybackOrder::FooPlaybackOrder);
};

#endif // FOOPLAYERMANAGER_HPP
