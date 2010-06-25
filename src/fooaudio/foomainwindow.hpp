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

#ifndef _FOOMAINWINDOW_HPP_
#define _FOOMAINWINDOW_HPP_

#include <QtGui>
#include <QUrl>
#include "footabwidget.hpp"
#include "fooplaylistmanager.hpp"
#include "fooplayermanager.hpp"
#include "foosettingsmanager.hpp"
#include "fooslider.hpp"
#include "fooplaybackorder.hpp"

class	QSystemTrayIcon;

class	FooPhononAudioEngine;

class	FooMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	FooMainWindow(FooPlaylistManager *playlistManager, FooPlayerManager *playerManager);
	~FooMainWindow();

	int getMaxProgress();

	bool isCursorFollowsPlayback();

	void readSettings();

signals:
	void changePlaybackOrder(FooPlaybackOrder::FooPlaybackOrder);

private slots:
	void saveSettings();
	void mute(bool m);

	void open();
	void openAudioCD();
	void addFiles();
	void addFolder();
	void addLocation();
	void loadPlaylist();
	void savePlaylist();
	void exit();

	void undo();
	void redo();
	void sortBy();
	void randomize();
	void reverse();
	void sortByFilePatch();
	void sortByAlbum();
	void sortByTrackNumber();
	void sortByTitle();
	void search();
	void removeDuplicates();
	void removeDeadItems();

	void alwaysOnTop();
	void console();
	void equalizer();
	void playlistManagerView();
	void quickSetup();
	void enableLayoutEditionMode();
	void createScratchbox();

	void defaultOrder();
	void repeatPlaylistOrder();
	void repeatTrackOrder();
	void randomOrder();
	void shuffleTracksOrder();
	void shuffleAlbumsOrder();
	void shuffleFoldersOrder();
	void stopAfterCurrent();
	void playbackFollowsCursor();
	void cursorFollowsPlayback();

	void albumList();
	void searchAlbum();
	void configure();
	void uncheckAllOrders();
	void about();

	void cutLayout();
	void copyLayout();
	void pasteLayout();

	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void setTrayIcon();

	// update window title according to new meta data
	void updateWindowTitle(QMultiMap<QString, QString> newMetaData);

private:
	FooPlaylistManager *playlistManager;
	FooPlayerManager *playerManager;

	QSize *iconSize;

	FooTabWidget *fooTabWidget;

	FooSlider *trackSlider;
	FooSlider *volumeSlider;

	QSystemTrayIcon *trayIcon;

	QMenu *fileMenu;
	QAction *openAction;
	QAction *openAudioCDAction;
	QAction *addFilesAction;
	QAction *addFolderAction;
	QAction *addLocationAction;
	QAction *newPlaylistAction;
	QAction *loadPlaylistAction;
	QAction *savePlaylistAction;
	QAction *preferencesAction;
	QAction *exitAction;

	QMenu *editMenu;
	QAction *undoAction;
	QAction *redoAction;
	QAction *clearAction;
	QAction *selectAllAction;
	QMenu *selectionMenu;
	QAction *removeAction;
	QAction *cropAction;
	QMenu *sortMenu;
	QAction *sortByAction;
	QAction *randomizeAction;
	QAction *reverseAction;
	QAction *sortByFilePatchAction;
	QAction *sortByArtisAction;
	QAction *sortByAlbumAction;
	QAction *sortByTrackNumberAction;
	QAction *sortByTitleAction;
	QAction *searchAction;
	QAction *removeDuplicatesAction;
	QAction *removeDeadItemsAction;

	QMenu *viewMenu;
	QAction *alwaysOnTopAction;
	QMenu *visualisationsMenu;
	QAction *consoleAction;
	QAction *equalizerAction;
	QAction *playlistManagerAction;
	QMenu *layoutMenu;
	QAction *quickSetupAction;
	QAction *enableLayoutEditionModeAction;
	QAction *createScratchboxAction;
	QAction *cutLayoutAction;
	QAction *copyLayoutAction;
	QAction *pasteLayoutAction;

	QMenu *playbackMenu;
	QAction *stopAction;
	QAction *pauseAction;
	QAction *playAction;
	QAction *prevAction;
	QAction *nextAction;
	QAction *randomAction;
	QMenu *orderMenu;
	QAction *defaultOrderAction;
	QAction *repeatPlaylistOrderAction;
	QAction *repeatTrackOrderAction;
	QAction *randomOrderAction;
	QAction *shuffleTracksOrderAction;
	QAction *shuffleAlbumsOrderAction;
	QAction *shuffleFoldersOrderAction;
	QAction *stopAfterCurrentAction;
	QAction *playbackFollowsCursorAction;
	QAction *cursorFollowsPlaybackAction;

	QMenu *libraryMenu;
	QAction *albumListAction;
	QAction *searchAlbumAction;
	QAction *configureAction;

//	QMenu *settingsMenu;
//	QAction *trayIconAction;

	QMenu *helpMenu;
	QAction *aboutAction;
	QAction *aboutQtAction;

	QMenu *trayMenu;

	/*QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QMenu *queueMenu;
	QAction *addToQueueAction;
	QAction *removeFromQueueAction;
	QAction *clearQueueAction;*/

	QAction *volumeAction;

	QToolBar *trackToolBar;
	QToolBar *playbackToolBar;
	QToolBar *volumeToolBar;

	void createMenus();
	void createToolBars();
	void createActions();
	void createStatusBar();
	void createSystrayIcon();

	void playbackOrderChanged(FooPlaybackOrder::FooPlaybackOrder);

protected:
	void closeEvent(QCloseEvent *);
};

#endif // _FOOMAINWINDOW_HPP_

