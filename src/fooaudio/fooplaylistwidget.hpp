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

#ifndef _FOOPLAYLIST_HPP_
#define _FOOPLAYLIST_HPP_

#include <QStringList>
#include <QTreeView>
#include <QString>
#include <QUuid>

class FooPlaylistWidget : public QTreeView
{
	Q_OBJECT

public:
	FooPlaylistWidget (const QString& name, const QUuid& uuid,QWidget *parent = 0);

	void changeToCurrent();

	QString name() const;
	QUuid uuid() const;

	QStringList Filters;

private:
	QString playlistName;
	QUuid playlistUuid;

protected:
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent*);

private slots:
	void contextMenuRequested(const QPoint &position);

	void play();
	void remove();
	void crop();
	void cut();
	void copy();
	void paste();
	void addToPlaybackQueue();
	void removeFromPlaybackQueue();
	void properties();

signals:
	void currentChanged();
	void play(QModelIndex);
	void remove(QModelIndexList);
	void crop(QModelIndexList);
	void cut(QModelIndexList);
	void copy(QModelIndexList);
	void paste(QModelIndexList);
	void addToPlaybackQueue(QModelIndexList);
	void removeFromPlaybackQueue(QModelIndexList);
};

#endif // _FOOPLAYLIST_HPP_
