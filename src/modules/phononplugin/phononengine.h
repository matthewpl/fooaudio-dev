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

#ifndef PHONONENGINE_H__
#define PHONONENGINE_H__

#include <abstractaudioplugin.h>

#include "footrackinfo.hpp"
#include <QObject>
#include <QUrl>

namespace FooAudio
{
	class PhononEngine : public AbstractAudioPlugin
	{
		Q_OBJECT

	public:
		PhononEngine(QObject *parent = 0);
		~PhononEngine();

		bool isPlaying();
		bool isStopped();
		bool isPaused();
		bool isMuted();
		void setMuted(bool);
		int getVolume();

		QMap<FooTrackInfo::FooTrackInfo, QString> metaData(const QUrl url);
		QString metaData(const FooTrackInfo::FooTrackInfo info, const QUrl url);
		QStringList mimeTypes();

	signals:
		void aboutToFinish();
		void progress(const qint64 time);
		void changeTotalTime(qint64 time);
		void willPlayNow(const QUrl file);
		void metaDataChanged(QMultiMap<FooTrackInfo::FooTrackInfo, QString> newMetaData, QUrl url);

	public slots:
		void stop();
		void play();
		void pause();
		void clearQueue();
		void enqueueNextFile(const QUrl file);
		void playFile(const QUrl file);
		void setVolume(const int volume);
		void seekTrack(qint64);

	private slots:
		void newMetaData();

	private:
		class PhononEnginePrivate;
		PhononEnginePrivate * d;
	};
}

#endif // PHONONENGINE_H__
