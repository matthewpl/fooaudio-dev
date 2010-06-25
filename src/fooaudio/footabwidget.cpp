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

#include "footabwidget.hpp"
#include "footabbar.hpp"
#include "fooplaylistmanager.hpp"
#include "fooplaylistwidget.hpp"
#include "fooplaylistmodel.hpp"

#include <QCompleter>
#include <QEvent>
#include <QMenu>

#include <QtDebug>

FooTabWidget::FooTabWidget (FooPlaylistManager *playlistManager, QWidget *parent) : QTabWidget (parent)
{
	this->playlistManager = playlistManager;

	setElideMode(Qt::ElideRight);

	tabBar = new FooTabBar(this);

	setTabBar(tabBar);

	connect(tabBar, SIGNAL(newTab()), playlistManager, SLOT(createPlaylist()));
	connect(playlistManager, SIGNAL(newPlaylistCreated(FooPlaylist *)), this, SLOT(createNewPlaylistWidget(FooPlaylist *)));
	connect(playlistManager, SIGNAL(playlistRemoved(QString,QUuid)), this, SLOT(removePlaylistWidget(QString, QUuid)));
	connect(this, SIGNAL(removePlaylist(QString,QUuid)), playlistManager, SLOT(removePlaylist(QString,QUuid)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

	connect(tabBar, SIGNAL(cloneTab(int)), this, SLOT(cloneTab(int)));
	connect(tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
	connect(tabBar, SIGNAL(closeOtherTabs(int)), this, SLOT(closeOtherTabs(int)));

	setDocumentMode(true);

	tabBar->setTabsClosable(false);
	tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

	for(int i = 0; i < playlistManager->getPlaylists()->size(); i++)
	{
		createNewPlaylistWidget(playlistManager->getPlaylists()->at(i));
	}
}

void FooTabWidget::cloneTab(int index)
{
	QString newName = QString(tr("Copy of %1")).arg(tabText(index));
	//FooPlaylistWidget *fpw = qobject_cast<FooPlaylistWidget *> (widget(index));
//	FooPlaylistWidget *fpwCopy = new FooPlaylistWidget ();
//	addTab (fpwCopy, newName);

//	connect(fpwCopy, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}

void FooTabWidget::closeTab(int index)
{
	FooPlaylistWidget* playlist = static_cast<FooPlaylistWidget*>(widget(index));

	QString name = playlist->name();
	QUuid uuid = playlist->uuid();

	emit removePlaylist(name, uuid);
}

int FooTabWidget::playlistWidgetIndex(QString name, QUuid uuid) const
{
	int i;

	for (i = 0; i < count(); ++i)
	{
		FooPlaylistWidget* playlist = static_cast<FooPlaylistWidget*>(widget(i));

		if (playlist->name() == name && playlist->uuid() == uuid)
		{
			return i;
		}
	}

	return -1;
}

void FooTabWidget::removePlaylistWidget(QString name, QUuid uuid)
{
	int i = playlistWidgetIndex(name, uuid);

	if (i >= 0)
	{
		FooPlaylistWidget* playlist = static_cast<FooPlaylistWidget*>(widget(i));

		delete playlist;
	}
}

void FooTabWidget::closeOtherTabs(int index)
{
	if (-1 == index)
	{
		return;
	}

	for (int i = count() - 1; i > index; --i)
	{
		closeTab(i);
	}
	for (int i = index - 1; i >= 0; --i)
	{
		closeTab(i);
	}
}

void FooTabWidget::nextTab()
{
	int next = currentIndex() + 1;
	if (next == count())
	{
		next = 0;
	}
	setCurrentIndex(next);
}

void FooTabWidget::previousTab()
{
	int next = currentIndex() - 1;
	if (next < 0)
	{
		next = count() - 1;
	}
	setCurrentIndex(next);
}

//
void FooTabWidget::cut (bool remove)
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

//	buffer.clear();

// 	foreach (QTreeWidgetItem * item, foo->selectedItems())
// 	{
// 		if (item)
// 		{
// 			qDebug() << "TabWidget: bufor2 :" << item->text(0);
// 			buffer.append(item->clone());
//
// 			if (remove)
// 				delete item;
// 		}
// 	}
}

void FooTabWidget::remove ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

// 	foreach (QTreeWidgetItem * item, foo->selectedItems())
// 	{
// 		if (item)
// 		{
// 			delete item;
// 		}
// 	}
}

void FooTabWidget::copy ()
{
	cut(false);
}

void FooTabWidget::paste ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	//qDebug() << "TabWidget: count: " << buffer.count();
	//qDebug() << "TabWidget: size: " << buffer.size();

/*	int line = foo->indexOfTopLevelItem(foo->currentItem ());
	qDebug() << "TabWidget: line :" << line;
	if (line < 0)
		line = foo->plistCount();
	else
		line++;

	foreach (QTreeWidgetItem * item, buffer)
	{
		if (item)
		{
			qDebug() << "TabWidget: bufor2 :" << item->text(0);
			// if line not selected
			foo->insertTopLevelItem(line, item->clone());
			line++;
		}
	}*/
}

void FooTabWidget::clear ()
{
	FooPlaylistWidget* foo = static_cast<FooPlaylistWidget*> (currentWidget());
	if (!foo)
		return;

// 	foo->clear ();
}

void FooTabWidget::selectAll ()
{
	FooPlaylistWidget* foo = static_cast<FooPlaylistWidget*> (currentWidget());
	if (!foo)
		return;

// 	foo->selectAll ();
}

void FooTabWidget::createNewPlaylistWidget(FooPlaylist *playlist)
{
	FooPlaylistWidget *playlistWidget = new FooPlaylistWidget(playlist->name(), playlist->uuid(), this);

	connect(playlistWidget, SIGNAL(doubleClicked(QModelIndex)), playlist, SLOT(play(QModelIndex)));
	connect(playlistWidget, SIGNAL(play(QModelIndex)), playlist, SLOT(play(QModelIndex)));
	connect(playlistWidget, SIGNAL(remove(QModelIndexList)), playlist, SLOT(remove(QModelIndexList)));
	connect(playlistWidget, SIGNAL(currentChanged()), playlist, SLOT(currentChanged()));

	FooPlaylistModel *model = new FooPlaylistModel(playlist, this);

	connect(playlist, SIGNAL(addedTracks(int)), model, SLOT(addTracks(int)));
	connect(playlist, SIGNAL(removedTrack(int)), model, SLOT(removeTrack(int)));
	connect(playlist, SIGNAL(headersChanged()), model, SLOT(headersChanged()));
	connect(playlist, SIGNAL(metaChanged(int)), model, SLOT(rowChanged(int)));

	playlistWidget->setModel(model);

	addTab(playlistWidget, playlist->name());
}

void FooTabWidget::currentChanged(int index)
{
	if (FooPlaylistWidget* playlistWidget = (FooPlaylistWidget*)widget(index))
	{
		playlistWidget->changeToCurrent();
	}
}
