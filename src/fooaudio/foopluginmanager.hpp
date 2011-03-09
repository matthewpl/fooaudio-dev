#ifndef FOOPLUGINMANAGER_HPP
#define FOOPLUGINMANAGER_HPP

#include <QObject>
#include <QString>
#include <QList>

#include "fooaudioengine.hpp"
#include "fooplaylistformat.hpp"

namespace Fooaudio
{
	class FooPluginManager : public QObject
	{
		Q_OBJECT

	public:
		static FooPluginManager& instance()
		{
			static FooPluginManager pluginManager;
			return pluginManager;
		}

		void findPlugins(QString path = "../lib/");

		FooAudioEngine* getEngine();

		QList<FooPlaylistFormat*> getPlaylistFormats();

	private:
		FooPluginManager() {}
		FooPluginManager(const FooPluginManager&);
		FooPluginManager& operator=(const FooPluginManager&);

		QList<FooAudioEngine*> engines;
		QList<FooPlaylistFormat*> playlistFormats;

	signals:

	public slots:

	};
}

#endif // FOOPLUGINMANAGER_HPP
