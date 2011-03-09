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

#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "fooplaylistmanager.hpp"
#include "fooplayback.hpp"
#include "fooaudioengine.hpp"

namespace Fooaudio
{
	FooPlaylistManager::FooPlaylistManager(FooAudioEngine *engine, QObject *parent) : QObject(parent)
	{
		this->engine = engine;
		playlists = new QList<FooPlaylist*>();
		//	metaVersion = 1;

		setPlaylistColumnConfig("%pn;%ti;%al;%ar");

		if (playlists->size() < 1)
		{
			createPlaylist();
			currentPlayingPlaylist = NULL;
		}
	}

	FooPlaylistManager::~FooPlaylistManager()
	{
		delete playlists;
	}

	void FooPlaylistManager::play(FooPlaylist *playlist, QUrl url)
	{
		currentPlayingPlaylist = playlist;

		engine->playFile(url);
	}

	void FooPlaylistManager::setPlaylistColumnConfig(QString newConfig)
	{
		playlistColumnConfig = newConfig;
		//	++metaVersion;

		emit playlistColumnConfigChanged(newConfig/*, metaVersion*/);
	}

	QString FooPlaylistManager::getPlaylistColumnConfig()
	{
		return playlistColumnConfig;
	}

	int FooPlaylistManager::playlistIndex(QString name, QUuid uuid) const
	{
		int i;

		for (i = 0; i < playlists->size(); ++i)
		{
			if (playlists->at(i)->name() == name && playlists->at(i)->uuid() == uuid)
			{
				return i;
			}
		}

		return -1;
	}

	void FooPlaylistManager::removePlaylist(QString name, QUuid uuid)
	{
		int i = playlistIndex(name, uuid);

		if (i >= 0)
		{
			FooPlaylist* const p = playlists->at(i);

			if (currentPlayingPlaylist == p)
			{
				currentPlayingPlaylist = NULL;
			}

			delete p;

			playlists->removeAt(i);

			emit playlistRemoved(name, uuid);

			if (playlists->size() == 0)
			{
				createPlaylist();
			}
		}
	}

	void FooPlaylistManager::renamePlaylist(QUuid uuid, QString oldName, QString newName)
	{
		qDebug() << "oldName" << oldName;

		int i = playlistIndex(oldName, uuid);

		playlists->at(i)->setName(newName);

		emit playlistRenamed(uuid, oldName, newName);
	}

	void FooPlaylistManager::addFilesToCurrentPlaylist(QStringList files)
	{
		QList<FooTrack> tracks;

		for (int i = 0; i < files.size(); i++)
		{
			QString file = files.at(i);

			FooTrack track(file);

			engine->metaData(track);

			tracks.append(track);
		}

		currentViewingtPlaylist->addTracks(tracks);
	}

	void FooPlaylistManager::createPlaylist()
	{
		FooPlaylist *playlist = new FooPlaylist("New playlist", playlistColumnConfig, /*metaVersion,*/ this);

		connect(playlist, SIGNAL(currentChanged(FooPlaylist *)), this, SLOT(changeCurrentPlaylist(FooPlaylist*)));
		connect(playlist, SIGNAL(play(FooPlaylist*,QUrl)), this, SLOT(play(FooPlaylist*,QUrl)));
		connect(this, SIGNAL(playlistColumnConfigChanged(QString/*, int*/)), playlist, SLOT(playlistColumnConfigChanged(QString/*, int*/)));

		playlists->append(playlist);

		emit newPlaylistCreated(playlist);
	}

	QList<FooPlaylist*>* FooPlaylistManager::getPlaylists()
	{
		return playlists;
	}

	void FooPlaylistManager::changeCurrentPlaylist(FooPlaylist *p)
	{
		currentViewingtPlaylist = p;

		qDebug() << currentViewingtPlaylist;
	}

	QUrl FooPlaylistManager::getNextTrack(FooPlaybackOrder::FooPlaybackOrder playbackOrder, FooPlayback::FooPlayback playback)
	{
		if (currentPlayingPlaylist == NULL)
		{
			if (currentViewingtPlaylist == NULL)
			{
				return QUrl();
			}
			else
			{
				currentPlayingPlaylist = currentViewingtPlaylist;
			}
		}

		return currentPlayingPlaylist->getNextTrack(playbackOrder, playback);
	}

	void FooPlaylistManager::savePlaylistToPls(QString filePath)
	{
/*		FooPlaylistPlsFormat plsPlaylist;

		QString data = plsPlaylist.convert(currentViewingtPlaylist, filePath);

		qDebug() << "przed zapisem" << endl;

		qDebug() << data << endl;

		QFile f(filePath);
		f.open(QIODevice::WriteOnly);

		QTextStream out(&f);

		out << data;

		out.flush();
		f.flush();

		f.close();

		qDebug() << "już po zapisie" << endl;*/
	}

	void FooPlaylistManager::loadPlaylistFromPls(QString filePath)
	{
/*		qDebug() << "Plik źródłowy" << filePath;

		QFile f(filePath);
		f.open(QIODevice::ReadOnly);

		QTextStream in(&f);
		QString data;

		data = in.readAll();

		f.close();

		FooPlaylistPlsFormat plsPlaylist;

		QList<FooTrack> trackList = plsPlaylist.convert(data, filePath);

		for (int i = 0; i < trackList.size(); ++i)
		{
			engine->metaData(trackList[i]);
		}

		createPlaylist();

		playlists->at(playlists->size() - 1)->addTracks(trackList);*/
	}

	void FooPlaylistManager::savePlaylistToM3u(QString filePath)
	{
	}
}
