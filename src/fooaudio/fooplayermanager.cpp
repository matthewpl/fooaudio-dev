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

#include "fooplayermanager.hpp"
#include "fooplayback.hpp"

#include <QDebug>

namespace Fooaudio
{
	FooPlayerManager::FooPlayerManager(FooPlaylistManager *playlistManager, FooAudioEngine *engine, QObject *parent) : QObject(parent)
	{
		this->engine = engine;
		this->playlistManager = playlistManager;

		m_mute = false;

		currentTrackTime = 0;
		totalTrackTime = 0;
		playbackOrder = FooPlaybackOrder::Default;

		connect(engine, SIGNAL(aboutToFinish()), this, SLOT(enqueueNextTrack()));
		connect(engine, SIGNAL(progress(qint64)), this, SLOT(changeCurrentTrackTime(qint64)));
		connect(engine, SIGNAL(changeTotalTime(qint64)), this, SLOT(changeTotalTrackTime(qint64)));
		connect(engine, SIGNAL(muteChanged(bool)), this, SIGNAL(muted(bool)));
		connect(this, SIGNAL(mute(bool)), engine, SLOT(mute(bool)));
		connect(this, SIGNAL(enqueueNextTrack(QUrl)), engine, SLOT(enqueueNextFile(QUrl)));
		connect(this, SIGNAL(seek(qint64)), engine, SIGNAL(seek(qint64)));
	}

	FooPlayerManager::~FooPlayerManager()
	{
	}

	int FooPlayerManager::getVolume()
	{
		return engine->getVolume();
	}

	FooPlaybackOrder::FooPlaybackOrder FooPlayerManager::getPlaybackOrder()
	{
		return playbackOrder;
	}

	void FooPlayerManager::changePlaybackOrder(FooPlaybackOrder::FooPlaybackOrder order)
	{
		playbackOrder = order;
	}

	qint64 FooPlayerManager::getTotalTime()
	{
		return totalTrackTime;
	}

	void FooPlayerManager::next()
	{
		engine->playFile(playlistManager->getNextTrack(playbackOrder, FooPlayback::next));
	}

	void FooPlayerManager::previous()
	{
		engine->playFile(playlistManager->getNextTrack(playbackOrder, FooPlayback::prev));
	}

	void FooPlayerManager::pause()
	{
		if (engine->isPaused())
		{
			engine->play();
		}
		else if (engine->isPlaying())
		{
			engine->pause();
		}
	}

	void FooPlayerManager::play()
	{
		if (engine->isPaused())
		{
			engine->play();
		}
		else if (!engine->isPlaying())
		{
			engine->playFile(playlistManager->getNextTrack(playbackOrder, FooPlayback::play));
		}
		else if (engine->isPlaying())
		{
			engine->seekTrack(0);
			engine->play();
		}
	}

	void FooPlayerManager::stop()
	{
		engine->stop();
	}

	void FooPlayerManager::mute()
	{
		m_mute = !m_mute;

		emit mute(m_mute);

		if (!m_mute)
		{
			setVolume(m_volume);
		}
	}

	bool FooPlayerManager::isMuted()
	{
		return m_mute;
	}

	void FooPlayerManager::random()
	{
		engine->playFile(playlistManager->getNextTrack(FooPlaybackOrder::Random, FooPlayback::random));
	}

	void FooPlayerManager::setVolume(int v)
	{
		m_volume = v;
		engine->setVolume(v);
	}

	void FooPlayerManager::enqueueNextTrack()
	{
		emit enqueueNextTrack(playlistManager->getNextTrack(playbackOrder, FooPlayback::enque));
	}

	void FooPlayerManager::changeCurrentTrackTime(qint64 t)
	{
		currentTrackTime = t;
		emit currentTrackTimeChanged(currentTrackTime, totalTrackTime);
	}

	void FooPlayerManager::changeTotalTrackTime(qint64 t)
	{
		totalTrackTime = t;
		emit currentTrackTimeChanged(currentTrackTime, totalTrackTime);
	}
}
