#ifndef FOOPLAYLISTMODEL_HPP
#define FOOPLAYLISTMODEL_HPP

#include <QAbstractItemModel>
#include "fooplaylist.hpp"

namespace Fooaudio
{
	class FooPlaylistModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		explicit FooPlaylistModel(FooPlaylist *playlist, QObject *parent = 0);
		~FooPlaylistModel();

		QVariant data(const QModelIndex &index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		Qt::ItemFlags flags(const QModelIndex &index) const;

		bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
		bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
		bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
		bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	private:
		FooPlaylist *playlist;
		int m_columnCount;
		int m_rowCount;

	public slots:
		void addTracks(int);
		void removeTrack(int);

		void headersChanged();
		void rowChanged(int);
	};
}

#endif // FOOPLAYLISTMODEL_HPP
