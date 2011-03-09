#include "fooaudioplaylistm3uformat.hpp"

#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QFileInfo>
#include <QDir>

FooaudioPlaylistM3uFormat::FooaudioPlaylistM3uFormat(QObject* parent) : FooPlaylistFormat(parent)
{
}

FooaudioPlaylistM3uFormat::~FooaudioPlaylistM3uFormat()
{
}

QString FooaudioPlaylistM3uFormat::convert(const Fooaudio::FooPlaylist* playlist, QString path) const
{
	QString data;
	QTextStream stream(&data, QIODevice::WriteOnly);

	QFileInfo playlistPath(path);
	QString playlistDirAbsolutePath = playlistPath.absoluteDir().absolutePath();

	if (!playlistDirAbsolutePath.endsWith("/"))
	{
		playlistDirAbsolutePath.append("/");
	}

	stream << "#EXTM3U" << endl;

	int playlistTrackCount = playlist->trackCount();

	for (unsigned int i = 0; i < playlistTrackCount; ++i)
	{
		QString trackPath = playlist->getTrack(i).file().toString();

		if (trackPath.startsWith(playlistDirAbsolutePath))
		{
			trackPath.remove(0, playlistDirAbsolutePath.size());
		}

		stream << endl << "#EXTINF:" << playlist->getTrack(i).lenghtInSeconds() << ","
				<< playlist->getTrack(i).title() << endl << trackPath << endl;
	}

	stream.flush();

	return data;
}

QList<Fooaudio::FooTrack> FooaudioPlaylistM3uFormat::convert(QString data, QString path) const
{
	QTextStream stream(&data, QIODevice::ReadOnly);
	QString line;
	QList<Fooaudio::FooTrack> tracksList;

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

			Fooaudio::FooTrack track(line);
			track.setLenght(lenght.toInt());
			track.setTitle(title);
		}
	}

	return tracksList;
}

QString FooaudioPlaylistM3uFormat::getNameFilter() const
{
	return tr("M3U playlist (*.m3u)");
}
