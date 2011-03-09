#ifndef FOOPLAYLISTSTORAGE_HPP
#define FOOPLAYLISTSTORAGE_HPP

#include <QObject>
#include <QStringList>

#include "fooplaylistformat.hpp"

namespace Fooaudio
{
	class FooPlaylistStorage : public QObject
	{
		Q_OBJECT

	public:
		static FooPlaylistStorage& instance()
		{
			static FooPlaylistStorage playlistStorage;
			return playlistStorage;
		}

		QStringList getFormatNames();
		void save(FooPlaylist *playlist, QString path, QString format);
		void load(QList<FooTrack>& playlist, QString path, QString format);

	private:
		FooPlaylistStorage();
		FooPlaylistStorage(const FooPlaylistStorage&);
		FooPlaylistStorage& operator=(const FooPlaylistStorage&);

		QList<FooPlaylistFormat*> formats;
		QStringList formatNames;

	signals:

	public slots:

	};
}

#endif // FOOPLAYLISTSTORAGE_HPP
