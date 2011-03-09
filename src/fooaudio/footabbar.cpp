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

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QStyle>
#include <QLineEdit>
#include <QUrl>

#include <QtDebug>

namespace Fooaudio
{
	FooTabBar::FooTabBar(QWidget *parent) : QTabBar(parent), m_showTabBarWhenOneTab(true), editedTabIndex(-1)
	{
		setContextMenuPolicy(Qt::CustomContextMenu);
		setAcceptDrops(true);
		setElideMode(Qt::ElideRight);
		setUsesScrollButtons(true);
		setMovable(true);
		setFocusPolicy(Qt::StrongFocus);
		setExpanding(false);
		setDocumentMode(true);
		setTabsClosable(false);
		setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

		connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuRequested(const QPoint &)));
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

			menu.addAction(tr ("Close &Other Playlists"), this, SLOT (closeOtherTabs()))->setData(index);

			menu.addSeparator();

			menu.addAction(tr ("&Rename Playlist"), this, SLOT(renameTab()))->setData(index);
		}

		menu.exec (QCursor::pos());
	}

	void FooTabBar::cloneTab ()
	{
		if (QAction *action = qobject_cast<QAction*>(sender()))
		{
			int index = action->data().toInt();
			emit cloneTab(index);
		}
	}

	void FooTabBar::closeTab()
	{
		if (QAction *action = qobject_cast<QAction*>(sender()))
		{
			int index = action->data().toInt();
			emit closeTab(index);
		}
	}

	void FooTabBar::closeOtherTabs()
	{
		if (QAction *action = qobject_cast<QAction*>(sender()))
		{
			int index = action->data().toInt();
			emit closeOtherTabs(index);
		}
	}

	void FooTabBar::renameTab(int index)
	{
		QLineEdit* edit = new QLineEdit(tabText(index), this);

		connect(edit, SIGNAL(editingFinished()), this, SLOT(tabNameEditingFinished()));

		QRect rect = tabRect(index);
		editedTabIndex = index;

		edit->move(rect.x(), rect.y());
		edit->setFocus();
		edit->resize(rect.width(), rect.height());
		edit->show();
	}

	void FooTabBar::renameTab()
	{
		if (QAction *action = qobject_cast<QAction*>(sender()))
		{
			int index = action->data().toInt();

			renameTab(index);
		}
	}

	void FooTabBar::tabNameEditingFinished()
	{
		if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender()))
		{
			emit tabNameChange(editedTabIndex, lineEdit->text());

			editedTabIndex = -1;

			lineEdit->deleteLater();
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
				renameTab(i);
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

		QTabBar::mouseMoveEvent(event);
	}
}
