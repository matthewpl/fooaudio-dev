#include "foopluginmanager.hpp"

#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>

#include "fooaudioengineplugin.hpp"
#include "fooplaylistformatplugin.hpp"
#include "nopluginexception.hpp"

namespace Fooaudio
{
	void FooPluginManager::findPlugins(QString path)
	{
		if (!path.endsWith("/"))
		{
			path.append("/");
		}

		QDir pluginsDir(path);

		if (pluginsDir.isRelative())
		{
			pluginsDir.setPath(qApp->applicationDirPath() + "/" + path);
		}

		//foreach (QString fileName, pluginsDir.entryList(QDir::Files))
		QStringList fileNameList = pluginsDir.entryList(QDir::Files);
		for (int i = 0; i < fileNameList.size(); ++i)
		{
			QString fileName = fileNameList.at(i);
			qDebug() << "file plugin name:" << fileName << flush;

			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

			QObject *plugin = loader.instance();

			if (plugin)
			{
				qDebug() << "tylek" << flush;
				if (FooAudioEnginePlugin* aep = qobject_cast<FooAudioEnginePlugin*>(plugin))
				{
					qDebug() << "dupa" << flush;
					engines.append(aep->getAudioEnginePlugin());
				}
				if (FooPlaylistFormatPlugin* pfp = qobject_cast<FooPlaylistFormatPlugin*>(plugin))
				{
					qDebug() << "dupa2" << flush;
					playlistFormats.append(pfp->getPlaylistFormatPlugin());
				}
			}
		}

		if (engines.isEmpty())
		{
			throw NoPluginException("engine");
		}
		if (playlistFormats.isEmpty())
		{
			throw NoPluginException("playlist format");
		}
	}

	FooAudioEngine* FooPluginManager::getEngine()
	{
		return engines.at(0);
	}

	QList<FooPlaylistFormat*> FooPluginManager::getPlaylistFormats()
	{
		return playlistFormats;
	}
}
