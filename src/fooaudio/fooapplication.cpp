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

#include "fooapplication.h"
#include "foomainwindow.hpp"
#include "abstractaudioplugin.h"
#include "abstractaudiointerface.h"

#include <QPluginLoader>
#include <QFileInfo>

FooApplication::FooApplication(QObject *parent) : QObject(parent)
{
}

FooApplication::~FooApplication()
{
	delete qApplication;
}

int FooApplication::start(int argc, char *argv[])
{
	qApplication = new QApplication(argc, argv);

	//QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::homePath() + "/.fooaudio/");


	QObject::connect(qApplication, SIGNAL(aboutToQuit()), this, SLOT(quitApp()));

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	qApplication->setApplicationName("fooaudio");
	qApplication->setQuitOnLastWindowClosed(true);

	QFileInfo pluginPath("../lib/libphononplugin.so");
	QPluginLoader loader(pluginPath.absoluteFilePath());

	QObject *p = loader.instance();
	if(!p)
	{
		qDebug() << loader.errorString();
		QMessageBox msgBox;
		msgBox.setText(tr("Can't find Audio Plugin."));
		return msgBox.exec();
	}
	else
	{
		FooAudio::AbstractAudioInterface *aai = qobject_cast<FooAudio::AbstractAudioInterface*>(p);
		if(!aai)
		{
			QMessageBox msgBox;
			msgBox.setText(tr("Can't cast to AbstractAudioInterface!."));
			return msgBox.exec();
		}

		engine = aai->GetAudioPlugin();
		playlistManager = new FooPlaylistManager(engine);
		playerManager = new FooPlayerManager(playlistManager, engine);
		mainWindow = new FooMainWindow(playlistManager, playerManager);

		mainWindow->show();

		return qApplication->exec();
	}
}

void FooApplication::quitApp()
{
}
