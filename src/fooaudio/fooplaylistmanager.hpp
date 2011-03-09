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

#ifndef FOOPLAYLISTMANAGER_HPP
#define FOOPLAYLISTMANAGER_HPP

#include "fooaudioengine.hpp"
#include "fooplaylist.hpp"
#include "footrack.hpp"
#include "fooplaybackorder.hpp"

#include <QList>
#include <QObject>
#include <QUrl>

namespace Fooaudio
{
	class FooPlaylistManager : public QObject
	{
		Q_OBJECT

	public:
		FooPlaylistManager(FooAudioEngine *engine, QObject *parent = 0);
		~FooPlaylistManager();

		void addFilesToCurrentPlaylist(QStringList);
		QList<FooPlaylist*>* getPlaylists();
		QUrl getNextTrack(FooPlaybackOrder::FooPlaybackOrder, FooPlayback::FooPlayback playback);

		void setPlaylistColumnConfig(QString newConfig);
		QString getPlaylistColumnConfig();

		void savePlaylistToPls(QString filePath);
		void savePlaylistToM3u(QString filePath);

		void loadPlaylistFromPls(QString filePath);

	private:
		FooAudioEngine *engine;

		QString playlistColumnConfig;

		QList<FooPlaylist*>* playlists;

		FooPlaylist *currentViewingtPlaylist;
		FooPlaylist *currentPlayingPlaylist;

		//int metaVersion;

		int playlistIndex(QString name, QUuid uuid) const;

	signals:
		void newPlaylistCreated(FooPlaylist *);
		void playlistRemoved(QString, QUuid);
		void playlistRenamed(QUuid uuid, QString oldName, QString newName);
		void playlistColumnConfigChanged(QString/*, int*/);

	public slots:
		void createPlaylist();
		void changeCurrentPlaylist(FooPlaylist *);
		void play(FooPlaylist *, QUrl);
		void removePlaylist(QString, QUuid);
		void renamePlaylist(QUuid uuid, QString oldName, QString newName);
	};
}

#endif // FOOPLAYLISTMANAGER_HPP
