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

#ifndef FOOPLAYLIST_HPP
#define FOOPLAYLIST_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QModelIndex>
#include <QUrl>
#include <QUuid>

#include "fooplaybackorder.hpp"
#include "fooplayback.hpp"
#include "footrack.hpp"
#include "footrackinfo.hpp"

namespace Fooaudio
{
	class FooPlaylist : public QObject
	{
		Q_OBJECT

	public:
		explicit FooPlaylist(QString playlistName, QString metaConfig, /*int _metaVersion,*/ QObject *parent = 0);

		QString name() const;
		void setName(const QString &name);

		QUuid uuid() const;

		QString data(int row, int column);
		QString header(int column) const;

		QStringList headers() const;

		int trackCount() const;
		int columnCount() const;

		void addTracks(QList<FooTrack>);
		QUrl getNextTrack(FooPlaybackOrder::FooPlaybackOrder, FooPlayback::FooPlayback);

		const FooTrack& getTrack(int index) const;

	private:
		QString playlistName;
		QUuid playlistUuid;

		QStringList playlistColumnsHeaders;
		QString playlistColumnsConfig;
		//int m_columnCount;

		QList<FooTrack> playlist;
		int currentTrackIndex;
		int shouldCurrentTrackIndex;

		int metaVersion;

		QUrl getRandomTrack();

	signals:
		void play(FooPlaylist*, QUrl);
		void currentChanged(FooPlaylist*);
		void addedTracks(int);
		void removedTrack(int);
		void headersChanged();
		void metaChanged(int);

	public slots:
		void play(QModelIndex);
		void remove(QModelIndexList);
		void currentChanged();
		void playlistColumnConfigChanged(QString config/*, int v*/);
	};
}

#endif // FOOPLAYLIST_HPP
