#ifndef FOOAUDIOPLAYLISTM3UFORMAT_HPP
#define FOOAUDIOPLAYLISTM3UFORMAT_HPP

#include "fooplaylistformat.hpp"

class FooaudioPlaylistM3uFormat : public Fooaudio::FooPlaylistFormat
{
public:
	FooaudioPlaylistM3uFormat(QObject *parent = 0);
	~FooaudioPlaylistM3uFormat();

	QString convert(const Fooaudio::FooPlaylist* playlist, QString path) const;
	QList<Fooaudio::FooTrack> convert(QString data, QString path) const;

	QString getNameFilter() const;
};

#endif // FOOAUDIOPLAYLISTM3UFORMAT_HPP
