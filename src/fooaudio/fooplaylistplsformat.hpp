#ifndef FOOPLAYLISTPLSFORMAT_HPP
#define FOOPLAYLISTPLSFORMAT_HPP

#include "fooplaylistformat.hpp"

class FooPlaylistPlsFormat : public FooPlaylistFormat
{
public:
	FooPlaylistPlsFormat();

	QString convert(const FooPlaylist* playlist) const;
	QList<FooTrack> convert(QString data, QString path = "") const;
};

#endif // FOOPLAYLISTPLSFORMAT_HPP
