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

#include <QLabel>
#include <QStringList>
#include <QtGui>
#include <QUrl>
#include <QtDebug>

#include "foomainwindow.hpp"
#include "fooplaylistwidget.hpp"

namespace Fooaudio
{
	FooPlaylistWidget::FooPlaylistWidget(const QString& name, const QUuid& uuid, QWidget *parent) : QTreeView(parent)
	{
		playlistName = name;
		playlistUuid = uuid;

		setSelectionMode(QAbstractItemView::ExtendedSelection);
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setSortingEnabled(false);
		setIndentation(0);
		setAlternatingRowColors(true);
		// For drag and drop files, QAbstractItemView::DragDrop doesn't work (why?)
		setAcceptDrops(true);
		setDragDropMode(QAbstractItemView::InternalMove);
		setDragEnabled(true);
		viewport()->setAcceptDrops(true);
		setDropIndicatorShown(true);
		// Context Menu
		setContextMenuPolicy(Qt::CustomContextMenu);
		setItemsExpandable(false);
		setRootIsDecorated(false);

		connect(this, SIGNAL (customContextMenuRequested (const QPoint &)), this, SLOT (contextMenuRequested (const QPoint &)));

		// 	QStringList l;
		// 	l << tr("File");
		// 	setHeaderLabels(l);
		//
		// 	// TODO Remove and add something normal
		// 	Filters << ".mp3"  << ".wma" << ".mp4" << ".mpg" << ".mpeg" << ".m4a";
		// 	Filters << ".flac" << ".ogg" << ".wav" << ".3gp" << ".ac3" << ".aac";

		// TODO .m3u .m4u
	}

	QString FooPlaylistWidget::name() const
	{
		return playlistName;
	}

	void FooPlaylistWidget::setName(QString newName)
	{
		playlistName = newName;
	}

	QUuid FooPlaylistWidget::uuid() const
	{
		return playlistUuid;
	}

	void FooPlaylistWidget::contextMenuRequested (const QPoint &position)
	{
		/*
	 Play
	 ---
	 Remove (Del)
	 Crop
	 ---
	 Cut (Ctrl+X)
	 Copy (Ctrl+C)
	 Paste (Ctrl+V) -- przed aktualnie zaznaczonym - nie zawsze widoczne
	 ---
	 Add to Playback Queue
	 Remove from Playback Queue -- nie zawsze widoczne
	 Properties (Alt+Enter)
	 */

		QMenu menu;

		QModelIndexList index = selectedIndexes();
		/*	QList<int> l;

	for (int i = 0; i < index.size(); i++)
	{
		QVariant v(index.at(i).row());
		l.append(v);
	}
*/
		if (index.size())
		{
			menu.addAction(tr("Play"), this, SLOT(play()));//->setData(l);

			menu.addSeparator();

			menu.addAction(tr("Remove"), this, SLOT(remove()));//->setData(l);
			menu.addAction(tr("Crop"), this, SLOT(crop()));//->setData(l);

			menu.addSeparator();

			menu.addAction(tr("Cut"), this, SLOT(cut()), QKeySequence::Cut);//->setData(index);
			menu.addAction(tr("Copy"), this, SLOT(copy()), QKeySequence::Copy);//->setData(index);
			menu.addAction(tr("Paste"), this, SLOT(paste()), QKeySequence::Paste);//->setData(index);

			menu.addSeparator();

			menu.addAction(tr("Add to Playback Queue"), this, SLOT(addToPlaybackQueue()));//->setData(index);
			menu.addAction(tr("Remove from Playback Queue"), this, SLOT(removeFromPlaybackQueue()));//->setData(index);
			menu.addAction(tr("Properties"), this, SLOT(properties()), Qt::ALT + Qt::Key_Enter);//->setData(index);
		}

		menu.exec(QCursor::pos());
	}

	void FooPlaylistWidget::play()
	{
		emit play(selectionModel()->selectedRows().at(0));
	}

	void FooPlaylistWidget::remove()
	{
		emit remove(selectionModel()->selectedRows());
	}

	void FooPlaylistWidget::crop()
	{

	}

	void FooPlaylistWidget::cut()
	{

	}

	void FooPlaylistWidget::copy()
	{

	}

	void FooPlaylistWidget::paste()
	{

	}

	void FooPlaylistWidget::addToPlaybackQueue()
	{

	}

	void FooPlaylistWidget::removeFromPlaybackQueue()
	{

	}

	void FooPlaylistWidget::properties()
	{

	}

	void FooPlaylistWidget::dragEnterEvent(QDragEnterEvent * event)
	{
		if (event->mimeData()->hasUrls())
		{
			event->acceptProposedAction();
		}
	}

	void FooPlaylistWidget::dropEvent(QDropEvent * event)
	{
		QList<QUrl> urlList;
		if (event->mimeData()->hasUrls())
		{
			urlList = event->mimeData()->urls();

			// if just text was dropped, urlList is empty (size == 0)
			if ( urlList.size() > 0)
			{
				int index /*= indexOfTopLevelItem(itemAt (event->pos()))*/;
			}
		}

		event->acceptProposedAction();
	}

	void FooPlaylistWidget::changeToCurrent()
	{
		emit currentChanged();
	}
}
