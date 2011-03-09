#include "fooaudioplaylistplsformat.hpp"

#include <QTextStream>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

FooaudioPlaylistPlsFormat::FooaudioPlaylistPlsFormat(QObject* parent) : FooPlaylistFormat(parent)
{
}

FooaudioPlaylistPlsFormat::~FooaudioPlaylistPlsFormat()
{
}

QString FooaudioPlaylistPlsFormat::convert(const Fooaudio::FooPlaylist* playlist, QString path) const
{
	QString data;
	QTextStream stream(&data, QIODevice::WriteOnly);

	QFileInfo playlistPath(path);
	QString playlistDirAbsolutePath = playlistPath.absoluteDir().absolutePath();

	if (!playlistDirAbsolutePath.endsWith("/"))
	{
		playlistDirAbsolutePath.append("/");
	}

	QString number;
	number.setNum(playlist->trackCount());

	stream << "[playlist]" << endl << "NumberOfEntries=" << number << endl;

	int playlistTrackCount = playlist->trackCount();

	for (unsigned int i = 0; i < playlistTrackCount; ++i)
	{
		number.setNum(i);

		QString trackPath = playlist->getTrack(i).file().toString();

		if (trackPath.startsWith(playlistDirAbsolutePath))
		{
			trackPath.remove(0, playlistDirAbsolutePath.size());
		}

		stream << endl << "File" << number << "=" << trackPath << endl
				<< "Title" << number << "=" << playlist->getTrack(i).title() << endl
				<< "Length" << number << "=" << playlist->getTrack(i).lenghtInSeconds() << endl;
	}

	stream << endl << "Version=2";
	stream.flush();

	return data;
}

QList<Fooaudio::FooTrack> FooaudioPlaylistPlsFormat::convert(QString data, QString path) const
{
	QTextStream stream(&data, QIODevice::ReadOnly);
	QString line;
	QList<Fooaudio::FooTrack> tracksList;

	while (!stream.atEnd())
	{
		line = stream.readLine();

		if (!line.isEmpty() && line.startsWith("File"))
		{
			QStringList lineList = line.split("=");
			QString filePath;

			filePath = lineList.at(1);

			QFileInfo trackInfo(filePath);
			QFileInfo playlistInfo(path);

			if (trackInfo.isRelative())
			{
				filePath = playlistInfo.absoluteDir().absolutePath() + "/" + trackInfo.filePath();
			}

			Fooaudio::FooTrack track(filePath);

			line = stream.readLine();
			lineList = line.split("=");

			QString lenght;
			QString title;

			if(lineList.at(0).toUpper().startsWith("Title"))
			{
				title = lineList.at(1);
			}

			line = stream.readLine();
			lineList = line.split("=");

			if(lineList.at(0).toUpper().startsWith("Lenght"))
			{
				lenght = lineList.at(1);
			}

			track.setLenght(lenght.toInt());
			track.setTitle(title);

			tracksList.append(track);
		}
	}

	return tracksList;
}

QString FooaudioPlaylistPlsFormat::getNameFilter() const
{
	return tr("PLS playlist (*.pls)");
}
