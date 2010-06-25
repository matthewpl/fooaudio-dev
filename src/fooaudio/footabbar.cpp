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

#include "footabbar.hpp"
#include "foochangename.hpp"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QStyle>
#include <QUrl>

#include <QtDebug>

FooTabBar::FooTabBar (QWidget *parent) : QTabBar (parent), m_showTabBarWhenOneTab(true)
{
	setContextMenuPolicy (Qt::CustomContextMenu);
	setAcceptDrops (true);
	setElideMode (Qt::ElideRight);
	setUsesScrollButtons (true);

	connect(this, SIGNAL (customContextMenuRequested (const QPoint &)), this, SLOT (contextMenuRequested (const QPoint &)));

	setMovable(true);
}

void FooTabBar::contextMenuRequested (const QPoint &position)
{
	/*
	 Renane Playlist
	 Remove Playlist
	 Add New Playlist
	 ---
	 Move Left
	 Move Right
	 ---
	 Save All Playlists...
	 Save Playlist...
	 Load Playlist...
	 ---
	 "<playlist-name>" Contents > -- odpuścić na razie
	 */
	QMenu menu;
	menu.addAction(tr("New Playlist..."), this, SIGNAL(newTab()), QKeySequence::New);

	int index = tabAt(position);

	if (-1 != index)
	{
		menu.addAction(tr("&Duplicate Playlist"), this, SLOT(cloneTab()))->setData(index);

		menu.addSeparator();

		menu.addAction(tr("&Close Playlist"), this, SLOT(closeTab()), QKeySequence::Close)->setData(index);

		menu.addSeparator();

		menu.addAction (tr ("Close &Other Playlists"), this, SLOT (closeOtherTabs()))->setData(index);

		menu.addSeparator();

		menu.addAction (tr ("&Rename Playlist"), this, SLOT(renameTab()))->setData(index);
	}

	menu.exec (QCursor::pos());
}

void FooTabBar::cloneTab ()
{
	if (QAction *action = qobject_cast<QAction *> (sender ()))
	{
		int index = action->data().toInt();
		emit cloneTab(index);
	}
}

void FooTabBar::closeTab()
{
	if (QAction *action = qobject_cast<QAction *>(sender ()))
	{
		int index = action->data().toInt();
		emit closeTab(index);
	}
}

void FooTabBar::closeOtherTabs()
{
	if (QAction *action = qobject_cast<QAction*> (sender()))
	{
		int index = action->data().toInt();
		emit closeOtherTabs(index);
	}
}

void FooTabBar::renameTab ()
{
	if (QAction *action = qobject_cast<QAction *>(sender ()))
	{
		int index = action->data().toInt();
		FooChangeName *fooChangeName = new FooChangeName(index, this);
		fooChangeName->show();
	}
}

void FooTabBar::mouseDoubleClickEvent (QMouseEvent *event)
{
	// Remove the line beloe when QTabWidget does not have a one pixel frame
	//  && event->pos().y() < (y() + height()))
	if (!childAt(event->pos ()))
	{
		int i = tabAt(event->pos());
		if (i < 0)
		{
			emit newTab();
		}
		else
		{
			FooChangeName *fooChangeName = new FooChangeName(i, this);
		}

		return;
	}

	QTabBar::mouseDoubleClickEvent (event);
}

void FooTabBar::mousePressEvent (QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragStartPos = event->pos();
	}

	QTabBar::mousePressEvent (event);
}

void FooTabBar::mouseMoveEvent (QMouseEvent *event)
{
	if (event->buttons () == Qt::LeftButton)
	{
		int diffX = event->pos().x() - m_dragStartPos.x();
		int diffY = event->pos().y() - m_dragStartPos.y();

		if ((event->pos () - m_dragStartPos).manhattanLength () > QApplication::startDragDistance () && diffX < 3 && diffX > -3 && diffY < -10)
		{
			QDrag *drag = new QDrag (this);
			QMimeData *mimeData = new QMimeData;
			int index = tabAt (event->pos ());
			mimeData->setText(tabText (index));
			mimeData->setData(QLatin1String("action"), "tab-reordering");
			drag->setMimeData(mimeData);
			drag->exec();
		}
	}

	QTabBar::mouseMoveEvent (event);
}

QSize FooTabBar::tabSizeHint (int index) const
{
	QSize sizeHint=QTabBar::tabSizeHint(index);
	QFontMetrics fm = fontMetrics ();

	return sizeHint.boundedTo(QSize(fm.width(QLatin1Char('M')) * 18, sizeHint.height()));
}

void FooTabBar::tabInserted (int position)
{
	Q_UNUSED(position);
	updateVisibility();
}

void FooTabBar::tabRemoved (int position)
{
	Q_UNUSED(position);
	updateVisibility();
}

void FooTabBar::updateVisibility()
{
	//   setVisible(true);
	//   m_viewTabBarAction->setEnabled(count() == 1);
	//   updateViewToolAction();
}
