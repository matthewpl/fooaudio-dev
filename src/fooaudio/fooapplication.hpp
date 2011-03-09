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

#ifndef FOOAPPLICATION_HPP
#define FOOAPPLICATION_HPP

#include <QApplication>

#include "fooaudioengine.hpp"
#include "fooplaylistmanager.hpp"
#include "fooplayermanager.hpp"
#include "foomainwindow.hpp"

namespace Fooaudio
{
	class FooApplication : public QObject
	{
		Q_OBJECT
	public:
		FooApplication(QObject *parent = 0);
		~FooApplication();
		int start(int argc, char *argv[]);

	private:
		QApplication *qApplication;

		FooAudioEngine *engine;
		FooPlaylistManager *playlistManager;
		FooPlayerManager *playerManager;
		FooMainWindow *mainWindow;

	public slots:
		void quitApp();
	};
}

#endif // FOOAPPLICATION_HPP
