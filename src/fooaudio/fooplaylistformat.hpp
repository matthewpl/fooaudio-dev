#ifndef FOOPLAYLISTFORMAT_HPP
#define FOOPLAYLISTFORMAT_HPP

#include <QString>
#include <QList>

#include "fooplaylist.hpp"
#include "footrack.hpp"

class FooPlaylistFormatException
{
};

class FooPlaylistFormat
{
public:
	 virtual QString convert(const FooPlaylist* playlist) const = 0;
	 virtual QList<FooTrack> convert(QString data, QString path = "") const = 0;
};

#endif // FOOPLAYLISTFORMAT_HPP
