#ifndef FOOPLAYLISTFORMAT_HPP
#define FOOPLAYLISTFORMAT_HPP

#include <QObject>
#include <QString>
#include <QList>

#include "fooplaylist.hpp"
#include "footrack.hpp"

namespace Fooaudio
{
	class FooPlaylistFormat : public QObject
	{
		Q_OBJECT

	public:
		FooPlaylistFormat(QObject *parent = 0) {}
		virtual ~FooPlaylistFormat() {}

		virtual QString convert(const FooPlaylist* playlist, QString path) const = 0;
		virtual QList<FooTrack> convert(QString data, QString path) const = 0;

		virtual QString getNameFilter() const = 0;
	};
}

#endif // FOOPLAYLISTFORMAT_HPP
