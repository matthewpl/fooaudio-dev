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

#include "fooplaylist.hpp"

#include <QDebug>
#include <QTime>

namespace Fooaudio
{
	FooPlaylist::FooPlaylist(QString name, QString metaConfig,/* int _metaVersion,*/ QObject *parent) : QObject(parent)
	{
		playlistName = name;
		playlistUuid = QUuid::createUuid();
		//m_columnCount = 0;
		currentTrackIndex = -1;
		shouldCurrentTrackIndex = -1;
		playlistColumnConfigChanged(metaConfig/*, _metaVersion*/);
	}


	QString FooPlaylist::name() const
	{
		return playlistName;
	}

	void FooPlaylist::setName(const QString &name)
	{
		playlistName = name;
	}

	QString FooPlaylist::data(int row, int column)
	{
		//if(metaVersion != playlist.at(row).getMetaVersion())
		{
			QString meta = playlistColumnsConfig;

			meta.replace("%pn", row == currentTrackIndex ? ">" : "");
			meta.replace("%fp", playlist.at(row).file().toString());
			meta.replace("%ti", playlist.at(row).title());
			meta.replace("%al", playlist.at(row).album());
			meta.replace("%tn", playlist.at(row).trackNumber());
			meta.replace("%ar", playlist.at(row).artist());
			meta.replace("%pe", tr("Performer"));
			meta.replace("%cr", tr("Copyright"));
			meta.replace("%li", tr("License"));
			meta.replace("%or", tr("Organization"));
			meta.replace("%ds", tr("Description"));
			meta.replace("%ge", tr("Genre"));
			meta.replace("%dt", playlist.at(row).date());
			meta.replace("%lo", tr("Location"));
			meta.replace("%ct", tr("Contact"));
			meta.replace("%is", tr("ISRC"));
			meta.replace("%cm", tr("Comment"));
			meta.replace("%le", tr("Length"));
			meta.replace("%br", tr("Bitrate"));
			meta.replace("%sr", tr("Sample rate"));
			meta.replace("%ch", tr("Channels"));

			QStringList metaList(meta.split(";"));

			playlist[row].setMeta(metaList);
			playlist[row].setMetaVersion(metaVersion);

			//qDebug() << "meta" << playlist.at(row).getMeta();

			return playlist.at(row).getMeta().at(column);
		}
		/*else
	{
		return playlist.at(row).getMeta().at(column);
	}*/
	}

	QUuid FooPlaylist::uuid() const
	{
		return playlistUuid;
	}

	void FooPlaylist::playlistColumnConfigChanged(QString config/*, int v*/)
	{
		playlistColumnsConfig = config;
		//metaVersion = v;

		config.replace("%pn", tr("Playing now"));
		config.replace("%fp", tr("Path"));
		config.replace("%ti", tr("Title"));
		config.replace("%al", tr("Album"));
		config.replace("%tn", tr("Track"));
		config.replace("%ar", tr("Artist"));
		config.replace("%pe", tr("Performer"));
		config.replace("%cr", tr("Copyright"));
		config.replace("%li", tr("License"));
		config.replace("%or", tr("Organization"));
		config.replace("%ds", tr("Description"));
		config.replace("%ge", tr("Genre"));
		config.replace("%dt", tr("Date"));
		config.replace("%lo", tr("Location"));
		config.replace("%ct", tr("Contact"));
		config.replace("%is", tr("ISRC"));
		config.replace("%cm", tr("Comment"));
		config.replace("%le", tr("Length"));
		config.replace("%br", tr("Bitrate"));
		config.replace("%sr", tr("Sample rate"));
		config.replace("%ch", tr("Channels"));

		playlistColumnsHeaders = config.split(";");
	}

	QString FooPlaylist::header(int column) const
	{
		if (column > playlistColumnsHeaders.size())
		{
			return QString();
		}
		else
		{
			return playlistColumnsHeaders.at(column);
		}
	}

	int FooPlaylist::columnCount() const
	{
		return 4;
	}

	int FooPlaylist::trackCount() const
	{
		return playlist.size();
	}

	void FooPlaylist::remove(QModelIndexList indexList)
	{
		int j = 0;

		for (int i = indexList.size() - 1; i >= 0; --i)
		{
			if (indexList.at(i).row() == currentTrackIndex)
			{
				shouldCurrentTrackIndex = currentTrackIndex;
			}

			if (j > 0 || indexList.at(i).row() <= currentTrackIndex)
			{
				j++;
			}

			playlist.removeAt(indexList.at(i).row());
			emit removedTrack(indexList.at(i).row());
		}

		if (!(shouldCurrentTrackIndex < 0))
		{
			currentTrackIndex = -1;
			shouldCurrentTrackIndex -= j;
		}
		else
		{
			currentTrackIndex -= j;
		}
	}

	void FooPlaylist::currentChanged()
	{
		emit currentChanged(this);
	}

	void FooPlaylist::addTracks(QList<FooTrack> tracks)
	{
		playlist.append(tracks);

		qDebug() << "size" << tracks.size();

		emit addedTracks(tracks.size());
	}

	QUrl FooPlaylist::getRandomTrack()
	{
		QTime midnight(0, 0, 0);
		qsrand(midnight.secsTo(QTime::currentTime()));

		currentTrackIndex = qrand() % playlist.size();

		return playlist.at(currentTrackIndex).file();
	}

	void FooPlaylist::play(QModelIndex model)
	{
		int prevTrackIndex = currentTrackIndex;
		currentTrackIndex = model.row();
		emit play(this, playlist.at(currentTrackIndex).file());

		emit metaChanged(prevTrackIndex);
		emit metaChanged(currentTrackIndex);
	}

	QUrl FooPlaylist::getNextTrack(FooPlaybackOrder::FooPlaybackOrder order, FooPlayback::FooPlayback playback)
	{
		if (shouldCurrentTrackIndex >= 0)
		{
			currentTrackIndex = shouldCurrentTrackIndex;
			shouldCurrentTrackIndex = -1;
		}

		int beforeTrackIndex = currentTrackIndex;

		QUrl newTrack;

		if (playlist.empty())
		{
			currentTrackIndex = -1;
			shouldCurrentTrackIndex = -1;
		}
		else if (order == FooPlaybackOrder::Random || playback == FooPlayback::random)
		{
			newTrack = getRandomTrack();
		}
		else if (currentTrackIndex < 0)
		{
			currentTrackIndex = 0;

			newTrack = playlist.at(currentTrackIndex).file();
		}
		else
		{
			if ((playback == FooPlayback::enque && order == FooPlaybackOrder::Default) || (playback == FooPlayback::next && (order == FooPlaybackOrder::Default || order == FooPlaybackOrder::Repeat_Track)))
			{
				++currentTrackIndex;

				if (currentTrackIndex >= playlist.size())
				{
					currentTrackIndex = -1;
				}
				else
				{
					newTrack = playlist.at(currentTrackIndex).file();
				}
			}
			else if (playback == FooPlayback::prev && (order == FooPlaybackOrder::Default || order == FooPlaybackOrder::Repeat_Track))
			{
				--currentTrackIndex;

				if (currentTrackIndex < 0)
				{
					currentTrackIndex = -1;
				}
				else
				{
					newTrack = playlist.at(currentTrackIndex).file();
				}
			}
			else if (order == FooPlaybackOrder::Repeat_Playlist && (playback == FooPlayback::enque || playback == FooPlayback::next))
			{
				++currentTrackIndex;

				if (currentTrackIndex >= playlist.size())
				{
					currentTrackIndex = 0;
				}

				newTrack = playlist.at(currentTrackIndex).file();
			}
			else if (playback == FooPlayback::prev && order == FooPlaybackOrder::Repeat_Playlist)
			{
				--currentTrackIndex;

				if (currentTrackIndex < 0)
				{
					currentTrackIndex = playlist.size() - 1;
				}

				newTrack = playlist.at(currentTrackIndex).file();
			}
			else if ((playback == FooPlayback::enque && order == FooPlaybackOrder::Repeat_Track) || playback == FooPlayback::play)
			{
				newTrack = playlist.at(currentTrackIndex).file();
			}
		}

		emit metaChanged(beforeTrackIndex);

		if (currentTrackIndex >= 0)
		{
			emit metaChanged(currentTrackIndex);
		}

		return newTrack;
	}

	const FooTrack& FooPlaylist::getTrack(int index) const
	{
		return playlist.at(index);
	}
}
