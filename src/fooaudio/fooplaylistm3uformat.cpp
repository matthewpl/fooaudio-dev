#include "fooplaylistm3uformat.hpp"

#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QFileInfo>
#include <QDir>

FooPlaylistM3uFormat::FooPlaylistM3uFormat()
{
}

QString FooPlaylistM3uFormat::convert(const FooPlaylist* playlist) const
{
	QString data;

	data.append("#EXTM3U\n");

	for (unsigned int i = 0; playlist->trackCount(); ++i)
	{
		data.append("\n#EXTINF:" + playlist->getTrack(i).lenghtInSeconds() + ","
						+ playlist->getTrack(i).title() + "\n" + playlist->getTrack(i).file().toString()
						+ "\n");
	}

	return data;
}

QList<FooTrack> FooPlaylistM3uFormat::convert(QString data, QString path) const
{
	QTextStream stream(&data, QIODevice::ReadOnly);
	QString line;
	QList<FooTrack> tracksList;

	while (!stream.atEnd())
	{
		line = stream.readLine();

		if (!line.isEmpty() && line.toUpper().startsWith("#EXTINF:"))
		{
			line.remove(0,line.indexOf(':')+1);

			QStringList lineList = line.split(",");
			QString lenght = lineList.at(0);
			QString title = lineList.at(1);

			line = stream.readLine();

			QFileInfo trackInfo(line);
			QFileInfo playlistInfo(path);

			if (trackInfo.isRelative())
			{
				line = playlistInfo.absoluteDir().absolutePath() + trackInfo.filePath();
			}

			FooTrack track(line);
			track.setLenght(lenght.toInt());
			track.setTitle(title);
		}
	}

	return tracksList;
}
