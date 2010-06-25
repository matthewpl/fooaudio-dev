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

#ifndef ABSTRACTAUDIOPLUGIN_HPP
#define ABSTRACTAUDIOPLUGIN_HPP

#include <QObject>
#include <QUrl>
#include <QMultiMap>
#include <QStringList>
#include "footrackinfo.hpp"

namespace FooAudio
{
	class AbstractAudioPlugin : public QObject
	{
		Q_OBJECT

	public:
		AbstractAudioPlugin(QObject *parent = 0);
		virtual ~AbstractAudioPlugin() {}

		virtual bool isPlaying() = 0;
		virtual bool isStopped() = 0;
		virtual bool isPaused() = 0;
		virtual bool isMuted() = 0;
		virtual void setMuted(bool) = 0;
		virtual int getVolume() = 0;

		virtual QMap<FooTrackInfo::FooTrackInfo, QString> metaData(const QUrl) = 0;
		virtual QString metaData(const FooTrackInfo::FooTrackInfo, const QUrl) = 0;
		virtual QStringList mimeTypes() = 0;

	signals:
		void aboutToFinish();
		void progress(const qint64 time);
		void changeTotalTime(qint64 time);
		void willPlayNow(const QUrl file);
		void seek(qint64);
		void metaDataChanged(QMultiMap<FooTrackInfo::FooTrackInfo, QString> newMetaData, QUrl url);

	public slots:
		virtual void stop() = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void clearQueue() = 0;
		virtual void enqueueNextFile(const QUrl file) = 0;
		virtual void playFile(const QUrl file) = 0;
		virtual void setVolume(const int volume) = 0;
		virtual void seekTrack(qint64) = 0;
	};
}

#endif // ABSTRACTAUDIOPLUGIN_HPP
