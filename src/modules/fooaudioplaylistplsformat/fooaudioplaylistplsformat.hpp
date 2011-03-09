#ifndef FOOAUDIOPLAYLISTPLSFORMAT_HPP
#define FOOAUDIOPLAYLISTPLSFORMAT_HPP

#include "fooplaylistformat.hpp"

class FooaudioPlaylistPlsFormat : public Fooaudio::FooPlaylistFormat
{
public:
	FooaudioPlaylistPlsFormat(QObject *parent = 0);
	~FooaudioPlaylistPlsFormat();

	QString convert(const Fooaudio::FooPlaylist* playlist, QString path) const;
	QList<Fooaudio::FooTrack> convert(QString data, QString path) const;

	QString getNameFilter() const;
};

#endif // FOOAUDIOPLAYLISTPLSFORMAT_HPP
