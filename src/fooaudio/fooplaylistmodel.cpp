/**********************************************************************
 *
 * fooaudio
 * Copyright (C) 2009-2010  fooaudio team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "fooplaylistmodel.hpp"

#include <QDebug>

FooPlaylistModel::FooPlaylistModel(FooPlaylist *playlist, QObject *parent) : QAbstractItemModel(parent)
{
	this->playlist = playlist;
	m_columnCount = columnCount();
	m_rowCount = rowCount();
}

FooPlaylistModel::~FooPlaylistModel()
{

}

QVariant FooPlaylistModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (role != Qt::DisplayRole && role != Qt::EditRole)
	{
		return QVariant();
	}

	return playlist->data(index.row(), index.column());
}

QVariant FooPlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return playlist->header(section);
	}

	return QVariant();
}

QModelIndex FooPlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QModelIndex FooPlaylistModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

int FooPlaylistModel::rowCount(const QModelIndex &parent) const
{
	return playlist->trackCount();
}

int FooPlaylistModel::columnCount(const QModelIndex &parent) const
{
	return playlist->columnCount();
}

Qt::ItemFlags FooPlaylistModel::flags(const QModelIndex &index) const
{
	if (index.isValid())
	{
		qDebug() << "flagi";
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	return 0;
}

bool FooPlaylistModel::insertColumns(int position, int columns, const QModelIndex &parent )
{
	beginInsertColumns(parent, position, position + columns - 1);
	endInsertColumns();

	return true;
}

bool FooPlaylistModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	beginRemoveColumns(parent, position, position + columns - 1);
	endRemoveColumns();

	return true;
}

bool FooPlaylistModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);
	endInsertRows();

	m_rowCount = rowCount();
	return true;
}

bool FooPlaylistModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);
	endRemoveRows();

	m_rowCount = rowCount();
	return true;
}

void FooPlaylistModel::addTracks(int k)
{
	int row = m_rowCount;
	insertRows(row, k);

	emit dataChanged(index(row, 0), index(row + k, columnCount()));
}

void FooPlaylistModel::removeTrack(int i)
{
	removeRow(i);

	emit dataChanged(index(i, 0), index(rowCount(), columnCount()));
}

void FooPlaylistModel::headersChanged()
{
	removeColumns(0, m_columnCount);
	insertColumns(0, columnCount());

	m_columnCount = columnCount();

	qDebug() << "columnCount" << m_columnCount;
}

void FooPlaylistModel::rowChanged(int row)
{
	qDebug() << "rowChanged" << row << columnCount();

	emit dataChanged(index(row, 0), index(row, columnCount()));
}
