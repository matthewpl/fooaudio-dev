#include "fooplayliststorage.hpp"

#include "foopluginmanager.hpp"
#include <QFile>
#include <QTextStream>

namespace Fooaudio
{
	FooPlaylistStorage::FooPlaylistStorage()
	{
		formats = FooPluginManager::instance().getPlaylistFormats();

		for (int i = 0; i < formats.size(); ++i)
		{
			formatNames.append(formats.at(i)->getNameFilter());
		}
	}

	QStringList FooPlaylistStorage::getFormatNames()
	{
		return formatNames;
	}

	void FooPlaylistStorage::save(FooPlaylist *playlist, QString path, QString format)
	{
		for (int i = 0; i < formats.size(); ++i)
		{
			if (formats.at(i)->getNameFilter() == format)
			{
				QString data = formats.at(i)->convert(playlist, path);

				QFile file(path);

				if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					return;
				}

				QTextStream out(&file);
				out << data;
				out.flush();

				file.close();

				break;
			}
		}
	}

	void FooPlaylistStorage::load(QList<FooTrack>& playlist, QString path, QString format)
	{

	}
}
