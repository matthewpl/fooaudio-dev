#include "fooplaylistplsformat.hpp"

#include <QTextStream>
#include <QFileInfo>
#include <QDir>

FooPlaylistPlsFormat::FooPlaylistPlsFormat()
{
}

QString FooPlaylistPlsFormat::convert(const FooPlaylist* playlist) const
{
	QString data;
	QTextStream stream(&data, QIODevice::WriteOnly);

	QString number;
	number.setNum(playlist->trackCount());

	stream << "[playlist]" << endl << "NumberOfEntries=" << number << endl;

	for (unsigned int i = 0; playlist->trackCount(); ++i)
	{
		number.setNum(i);

		stream << endl << "File" << number << "=" << playlist->getTrack(i).file().toString() << endl
				<< "Title" << number << "=" << playlist->getTrack(i).title() << endl
				<< "Length" << number << "=" << playlist->getTrack(i).lenghtInSeconds() << endl;
	}

	return data;
}

QList<FooTrack> FooPlaylistPlsFormat::convert(QString data, QString path) const
{
	QTextStream stream(&data, QIODevice::ReadOnly);
	QString line;
	QList<FooTrack> tracksList;

	while (!stream.atEnd())
	{
		line = stream.readLine();

		if (!line.isEmpty())
		{
			QStringList lineList = line.split('=');
			QString filePath;

			if(lineList.at(0).toUpper().startsWith("FILE"))
			{
				filePath = lineList.at(1);
			}

			QFileInfo trackInfo(line);
			QFileInfo playlistInfo(path);

			if (trackInfo.isRelative())
			{
				filePath = playlistInfo.absoluteDir().absolutePath() + trackInfo.filePath();
			}

			FooTrack track(filePath);

			line = stream.readLine();
			lineList = line.split('=');

			QString lenght;
			QString title;

			if(lineList.at(0).toUpper().startsWith("TITLE"))
			{
				title = lineList.at(1);
			}

			line = stream.readLine();
			lineList = line.split('=');

			if(lineList.at(0).toUpper().startsWith("LENGTH"))
			{
				lenght = lineList.at(1);
			}

			track.setLenght(lenght.toInt());
			track.setTitle(title);
		}
	}

	return tracksList;
}
