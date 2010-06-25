#ifndef FOOPLAYLISTM3UFORMAT_HPP
#define FOOPLAYLISTM3UFORMAT_HPP

#include "fooplaylistformat.hpp"

class FooPlaylistM3uFormat : public FooPlaylistFormat
{
public:
	FooPlaylistM3uFormat();

	QString convert(const FooPlaylist* playlist) const;
	QList<FooTrack> convert(QString data, QString path = "") const;
};

#endif // FOOPLAYLISTM3UFORMAT_HPP
