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

#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>

#include "footabbar.hpp"
#include "fooplaylistwidget.hpp"
#include "fooplaylistmanager.hpp"

class FooTabBar;

class FooTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	FooTabWidget (FooPlaylistManager *playlistManager, QWidget *parent = 0);

	void setCurrentPlaylist(int index);
	int getCurrentPlaylistIndex();
	void setCurrentItem(int index);
	int getCurrentItemIndex();

private:
	FooPlaylistManager *playlistManager;

	FooTabBar *tabBar;

	int playlistWidgetIndex(QString name, QUuid uuid) const;

public slots:
	void cloneTab (int index = -1);
	void closeTab (int index = -1);
	void closeOtherTabs (int index);
	void nextTab ();
	void previousTab ();

	//void itemClicked(QTreeWidgetItem *, int);

	void cut (bool remove = true);
	void remove ();
	void copy ();
	void paste ();
	void clear ();
	void selectAll ();

	void createNewPlaylistWidget(FooPlaylist *);
	void removePlaylistWidget(QString, QUuid);
	void currentChanged(int);

signals:
	void newTab();
	void removePlaylist(QString, QUuid);

	// tabwidget signals
	void tabsChanged();
	void lastTabClosed();

	//void itemDoubleClickedSignal(QTreeWidgetItem *item, int column);
};

#endif // _FOOTABWIDGET_HPP_

