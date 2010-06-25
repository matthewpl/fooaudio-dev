/**********************************************************************
 *
 * fooaudio
 * Copyright(C) 2009-2010  fooaudio team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
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

#include <QtDebug>
#include <QtGui>
#include <QDir>
#include <QUrl>

#include "fooabout.hpp"
#include "foomainwindow.hpp"
#include "fooplaylistmanager.hpp"
#include "fooplaylistwidget.hpp"
#include "footrack.hpp"
#include "foosettingsmanager.hpp"

FooMainWindow::FooMainWindow(FooPlaylistManager *playlistManager, FooPlayerManager *playerManager) : QMainWindow()
{
	this->playlistManager = playlistManager;
	this->playerManager = playerManager;

	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(saveSettings()));

	qsrand(QTime::currentTime().second());
	fooTabWidget = new FooTabWidget(playlistManager, this);
	setCentralWidget(fooTabWidget);

	this->setWindowIcon(QIcon(":images/icon64.png"));

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	createSystrayIcon();

	setTrayIcon();

	connect(this->volumeSlider, SIGNAL(valueChanged(int)), playerManager, SLOT(setVolume(int)));
	connect(playerManager, SIGNAL(muted(bool)), this, SLOT(mute(bool)));
	connect(playerManager, SIGNAL(currentTrackTimeChanged(qint64,qint64)), trackSlider, SLOT(valuesChanged(qint64,qint64)));
	connect(trackSlider, SIGNAL(valueChanged2(qint64)), playerManager, SIGNAL(seek(qint64)));
	connect(volumeSlider, SIGNAL(valueChanged(int)), playerManager, SLOT(setVolume(int)));
	connect(this, SIGNAL(changePlaybackOrder(FooPlaybackOrder::FooPlaybackOrder)), playerManager, SLOT(changePlaybackOrder(FooPlaybackOrder::FooPlaybackOrder)));

	bool mute = playerManager->isMuted();
	volumeAction->setIcon(QIcon(mute ? ":images/mute.png" : ":images/vol.png"));

	readSettings();
}

FooMainWindow::~FooMainWindow()
{
}

void FooMainWindow::readSettings()
{
	volumeSlider->setValue(playerManager->getVolume());
	playbackOrderChanged(playerManager->getPlaybackOrder());

	QMap<QString, QVariant> settings = FooSettingsManager::instance().readSettings("MainWindow");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void FooMainWindow::playbackOrderChanged(FooPlaybackOrder::FooPlaybackOrder order)
{
	switch (order)
	{
	case FooPlaybackOrder::Default:
		defaultOrderAction->setChecked(true);
		break;

	case FooPlaybackOrder::Repeat_Playlist:
		repeatPlaylistOrderAction->setChecked(true);
		break;

	case FooPlaybackOrder::Repeat_Track:
		repeatTrackOrderAction->setChecked(true);
		break;

	case FooPlaybackOrder::Random:
		randomOrderAction->setChecked(true);
		break;

	default:
		defaultOrderAction->setChecked(true);
		break;
	}
}

void FooMainWindow::saveSettings()
{
	QMap<QString, QVariant> settings;

	settings.insert("geometry", saveGeometry());
	settings.insert("windowState", saveState());

	FooSettingsManager::instance().saveSettings(QString("MainWindow"), settings);
}

void FooMainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));

	fileMenu->addAction(openAction);
	openAction->setEnabled(false);

	fileMenu->addAction(openAudioCDAction);
	openAudioCDAction->setEnabled(false);

	fileMenu->addSeparator();

	fileMenu->addAction(addFilesAction);

	fileMenu->addAction(addFolderAction);

	fileMenu->addAction(addLocationAction);

	fileMenu->addSeparator();

	fileMenu->addAction(newPlaylistAction);

	fileMenu->addAction(loadPlaylistAction);

	fileMenu->addAction(savePlaylistAction);

	fileMenu->addSeparator();

	fileMenu->addAction(preferencesAction);

	fileMenu->addSeparator();

	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));

	editMenu->addAction(undoAction);
	undoAction->setEnabled(false);

	editMenu->addAction(redoAction);
	redoAction->setEnabled(false);

	editMenu->addSeparator();

	editMenu->addAction(removeAction);

	editMenu->addAction(clearAction);

	editMenu->addAction(selectAllAction);

	// Sort SubMenu
	sortMenu = new QMenu(tr("&Sort"), editMenu);
	editMenu->addAction(sortMenu->menuAction());

	sortMenu->addAction(randomizeAction);
	randomizeAction->setEnabled(false);

	sortMenu->addAction(reverseAction);
	reverseAction->setEnabled(false);

	sortMenu->addAction(sortByFilePatchAction);
	sortByFilePatchAction->setEnabled(false);

	sortMenu->addAction(sortByAlbumAction);
	sortByAlbumAction->setEnabled(false);

	sortMenu->addAction(sortByTrackNumberAction);
	sortByTrackNumberAction->setEnabled(false);

	sortMenu->addAction(sortByTitleAction);
	sortByTitleAction->setEnabled(false);

	editMenu->addAction(searchAction);
	searchAction->setEnabled(false);

	editMenu->addAction(removeDuplicatesAction);
	removeDuplicatesAction->setEnabled(false);

	editMenu->addAction(removeDeadItemsAction);
	removeDeadItemsAction->setEnabled(true);

	viewMenu = menuBar()->addMenu(tr("&View"));

	viewMenu->addAction(alwaysOnTopAction);
	alwaysOnTopAction->setEnabled(false);

	visualisationsMenu = new QMenu(tr("&Visualisations"), viewMenu);
	viewMenu->addAction(visualisationsMenu->menuAction());

	viewMenu->addAction(consoleAction);
	consoleAction->setEnabled(false);

	viewMenu->addAction(equalizerAction);
	equalizerAction->setEnabled(false);

	viewMenu->addAction(playlistManagerAction);
	playlistManagerAction->setEnabled(false);

	layoutMenu = new QMenu(tr("&Layout"), viewMenu);
	viewMenu->addAction(layoutMenu->menuAction());

	layoutMenu->addAction(quickSetupAction);
	quickSetupAction->setEnabled(false);

	layoutMenu->addAction(enableLayoutEditionModeAction);
	enableLayoutEditionModeAction->setEnabled(false);

	layoutMenu->addAction(createScratchboxAction);
	createScratchboxAction->setEnabled(false);

	layoutMenu->addSeparator();

	layoutMenu->addAction(cutLayoutAction);
	cutLayoutAction->setEnabled(false);

	layoutMenu->addAction(copyLayoutAction);
	copyLayoutAction->setEnabled(false);

	layoutMenu->addAction(pasteLayoutAction);
	pasteLayoutAction->setEnabled(false);

	playbackMenu = menuBar()->addMenu(tr("&Playback"));

	playbackMenu->addAction(stopAction);

	playbackMenu->addAction(pauseAction);

	playbackMenu->addAction(playAction);

	playbackMenu->addAction(prevAction);

	playbackMenu->addAction(nextAction);

	playbackMenu->addAction(randomAction);

	playbackMenu->addSeparator();

	orderMenu = new QMenu(tr("&Order"), playbackMenu);
	playbackMenu->addAction(orderMenu->menuAction());

	orderMenu->addAction(defaultOrderAction);

	orderMenu->addAction(repeatPlaylistOrderAction);

	orderMenu->addAction(repeatTrackOrderAction);

	orderMenu->addAction(randomOrderAction);

	orderMenu->addAction(shuffleTracksOrderAction);

	orderMenu->addAction(shuffleAlbumsOrderAction);

	orderMenu->addAction(shuffleFoldersOrderAction);

	playbackMenu->addAction(stopAfterCurrentAction);
	stopAfterCurrentAction->setCheckable(true);
	stopAfterCurrentAction->setChecked(false);

	playbackMenu->addAction(playbackFollowsCursorAction);
	playbackFollowsCursorAction->setEnabled(false);

	playbackMenu->addAction(cursorFollowsPlaybackAction);
	cursorFollowsPlaybackAction->setCheckable(true);
	cursorFollowsPlaybackAction->setChecked(false);

	libraryMenu = menuBar()->addMenu(tr("&Library"));

	libraryMenu->addAction(albumListAction);
	albumListAction->setEnabled(false);

	libraryMenu->addAction(searchAlbumAction);
	searchAlbumAction->setEnabled(false);

	libraryMenu->addSeparator();

	libraryMenu->addAction(configureAction);
	configureAction->setEnabled(false);

	helpMenu = menuBar()->addMenu(tr("&Help"));

	helpMenu->addAction(aboutAction);

	helpMenu->addAction(aboutQtAction);
}

void FooMainWindow::createToolBars()
{
	iconSize = new QSize(16, 16);

	trackToolBar = new QToolBar(tr("Track ToolBar"), this);
	trackToolBar->setIconSize(*iconSize);
	trackToolBar->setObjectName("trackToolBar");
	trackToolBar->setFloatable(false);
	trackSlider = new FooSlider(Qt::Horizontal, this);
	trackSlider->setRange(0, 0);
	trackToolBar->addWidget(trackSlider);
	addToolBar(trackToolBar);

	volumeToolBar = new QToolBar(tr("Volume ToolBar"), this);
	volumeToolBar->setIconSize(*iconSize);
	volumeToolBar->setObjectName("volumeToolBar");
	volumeToolBar->setFloatable(false);

	volumeToolBar->addAction(volumeAction);
	volumeSlider = new FooSlider(Qt::Horizontal, this);
	volumeSlider->setRange(0, 100);
	volumeSlider->setTracking(true);
	volumeToolBar->addWidget(volumeSlider);
	addToolBar(volumeToolBar);

	playbackToolBar = new QToolBar(tr("Playback ToolBar"), this);
	playbackToolBar->setIconSize(*iconSize);
	playbackToolBar->setObjectName("playbackToolBar");
	playbackToolBar->setFloatable(false);

	playbackToolBar->addAction(stopAction);

	playbackToolBar->addAction(playAction);

	playbackToolBar->addAction(pauseAction);

	playbackToolBar->addAction(prevAction);

	playbackToolBar->addAction(nextAction);

	playbackToolBar->addAction(randomAction);


	addToolBar(playbackToolBar);
}

void FooMainWindow::createActions()
{
	randomAction = new QAction(QIcon(":images/random.png"), tr("&Random"),this);
	connect(randomAction, SIGNAL(triggered()), playerManager, SLOT(random()));

	nextAction = new QAction(QIcon(":images/next.png"), tr("&Next"),this);
	connect(nextAction, SIGNAL(triggered()), playerManager, SLOT(next()));

	prevAction = new QAction(QIcon(":images/prev.png"), tr("Pr&evious"),this);
	connect(prevAction, SIGNAL(triggered()), playerManager, SLOT(previous()));

	pauseAction = new QAction(QIcon(":images/pause.png"), tr("Pa&use"),this);
	connect(pauseAction, SIGNAL(triggered()), playerManager, SLOT(pause()));

	playAction = new QAction(QIcon(":images/play.png"), tr("&Play"),this);
	connect(playAction, SIGNAL(triggered()), playerManager, SLOT(play()));

	stopAction = new QAction(QIcon(":images/stop.png"), tr("&Stop"),this);
	connect(stopAction, SIGNAL(triggered()), playerManager, SLOT(stop()));

	volumeAction = new QAction(tr("Mute"), this);
	connect(volumeAction, SIGNAL(triggered()), playerManager, SLOT(mute()));

	openAction = new QAction(tr("&Open..."), this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	openAudioCDAction = new QAction(tr("Open Audio &CD..."), this);
	connect(openAudioCDAction, SIGNAL(triggered()), this, SLOT(openAudioCD()));

	addFilesAction = new QAction(tr("&Add Files..."), this);
	connect(addFilesAction, SIGNAL(triggered()), this, SLOT(addFiles()));

	addFolderAction = new QAction(tr("A&dd Folder..."), this);
	connect(addFolderAction, SIGNAL(triggered()), this, SLOT(addFolder()));

	addLocationAction = new QAction(tr("Add Lo&cation..."), this);
	addLocationAction->setShortcut(Qt::CTRL + Qt::Key_U);
	connect(addLocationAction, SIGNAL(triggered()), this, SLOT(addLocation()));

	newPlaylistAction = new QAction(tr("&New playlist"), this);
	newPlaylistAction->setShortcut(QKeySequence::New);
	connect(newPlaylistAction, SIGNAL(triggered()), playlistManager, SLOT(createPlaylist()));

	loadPlaylistAction = new QAction(tr("&Load playlist..."), this);
	connect(loadPlaylistAction, SIGNAL(triggered()), this, SLOT(loadPlaylist()));

	savePlaylistAction = new QAction(tr("&Save playlist..."), this);
	savePlaylistAction->setShortcut(QKeySequence::Save);
	connect(savePlaylistAction, SIGNAL(triggered()), this, SLOT(savePlaylist()));

	preferencesAction = new QAction(tr("&Preferences"), this);
	preferencesAction->setShortcut(Qt::CTRL + Qt::Key_P);
	connect(preferencesAction, SIGNAL(triggered()), &FooSettingsManager::instance(), SLOT(showWindow()));

	exitAction = new QAction(tr("&Exit"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

	undoAction = new QAction(tr("&Undo"), this);
	connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

	redoAction = new QAction(tr("&Redo"), this);
	connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

	removeAction = new QAction(tr("R&emove"), this);
	connect(removeAction, SIGNAL(triggered()), fooTabWidget, SLOT(remove()));

	clearAction = new QAction(tr("&Clear"), this);
	connect(clearAction, SIGNAL(triggered()), fooTabWidget, SLOT(clear()));

	selectAllAction = new QAction(tr("Select &all"), this);
	connect(selectAllAction, SIGNAL(triggered()), fooTabWidget, SLOT(selectAll()));

	randomizeAction = new QAction(tr("&Randomize"), this);
	connect(randomizeAction, SIGNAL(triggered()), this, SLOT(randomize()));

	reverseAction = new QAction(tr("R&everse"), this);
	connect(reverseAction, SIGNAL(triggered()), this, SLOT(reverse()));

	sortByFilePatchAction = new QAction(tr("Sort by file &patch"), this);
	connect(sortByFilePatchAction, SIGNAL(triggered()), this, SLOT(sortByFilePatch()));

	sortByAlbumAction = new QAction(tr("Sort by &album"), this);
	connect(sortByAlbumAction, SIGNAL(triggered()), this, SLOT(sortByAlbum()));

	sortByTrackNumberAction = new QAction(tr("Sort by track &number"), this);
	connect(sortByTrackNumberAction, SIGNAL(triggered()), this, SLOT(sortByTrackNumber()));

	sortByTitleAction = new QAction(tr("Sort by &title"), this);
	connect(sortByTitleAction, SIGNAL(triggered()), this, SLOT(sortByTitle()));

	searchAction = new QAction(tr("Sear&ch"), this);
	searchAction->setShortcut(QKeySequence::Find);
	connect(searchAction, SIGNAL(triggered()), this, SLOT(search()));

	removeDuplicatesAction = new QAction(tr("Remove &duplicates"), this);
	connect(removeDuplicatesAction, SIGNAL(triggered()), this, SLOT(removeDuplicates()));

	removeDeadItemsAction = new QAction(tr("Remove dead &items"), this);
	connect(removeDeadItemsAction, SIGNAL(triggered()), this, SLOT(removeDeadItems()));

	alwaysOnTopAction = new QAction(tr("Always on &top"), this);
	alwaysOnTopAction->setShortcut(Qt::ALT +  Qt::Key_A);
	connect(alwaysOnTopAction, SIGNAL(triggered()), this, SLOT(alwaysOnTop()));

	consoleAction = new QAction(tr("&Console"), this);
	connect(consoleAction, SIGNAL(triggered()), this, SLOT(console()));

	equalizerAction = new QAction(tr("&Equalizer"), this);
	connect(equalizerAction, SIGNAL(triggered()), this, SLOT(equalizer()));

	playlistManagerAction = new QAction(tr("&Playlist Manager"), this);
	connect(playlistManagerAction, SIGNAL(triggered()), this, SLOT(playlistManagerView()));

	quickSetupAction = new QAction(tr("&Quick setup"), this);
	connect(quickSetupAction, SIGNAL(triggered()), this, SLOT(quickSetup()));

	enableLayoutEditionModeAction = new QAction(tr("&Enable layout edition mode"), this);
	connect(enableLayoutEditionModeAction, SIGNAL(triggered()), this, SLOT(enableLayoutEditionMode()));

	createScratchboxAction = new QAction(tr("Create &scratchbox"), this);
	connect(createScratchboxAction, SIGNAL(triggered()), this, SLOT(createScratchbox()));

	cutLayoutAction = new QAction(tr("&Cut"), this);
	connect(cutLayoutAction, SIGNAL(triggered()), this, SLOT(cutLayout()));

	copyLayoutAction = new QAction(tr("C&opy"), this);
	connect(copyLayoutAction, SIGNAL(triggered()), this, SLOT(copyLayout()));

	pasteLayoutAction = new QAction(tr("&Paste"), this);
	connect(pasteLayoutAction, SIGNAL(triggered()), this, SLOT(pasteLayout()));

	defaultOrderAction = new QAction(tr("&Default"), this);
	defaultOrderAction->setCheckable(true);
	connect(defaultOrderAction, SIGNAL(triggered()), this, SLOT(defaultOrder()));

	repeatPlaylistOrderAction = new QAction(tr("Repeat (&playlist)"), this);
	repeatPlaylistOrderAction->setCheckable(true);
	connect(repeatPlaylistOrderAction, SIGNAL(triggered()), this, SLOT(repeatPlaylistOrder()));

	repeatTrackOrderAction = new QAction(tr("Repeat (&track)"), this);
	repeatTrackOrderAction->setCheckable(true);
	connect(repeatTrackOrderAction, SIGNAL(triggered()), this, SLOT(repeatTrackOrder()));

	randomOrderAction = new QAction(tr("Ra&ndom"), this);
	randomOrderAction->setCheckable(true);
	connect(randomOrderAction, SIGNAL(triggered()), this, SLOT(randomOrder()));

	shuffleTracksOrderAction = new QAction(tr("&Shuffle (tracks)"), this);
	shuffleTracksOrderAction->setCheckable(true);
	shuffleTracksOrderAction->setEnabled(false);
	connect(shuffleTracksOrderAction, SIGNAL(triggered()), this, SLOT(shuffleTracksOrder()));

	shuffleAlbumsOrderAction = new QAction(tr("S&huffle (albums)"), this);
	shuffleAlbumsOrderAction->setCheckable(true);
	shuffleAlbumsOrderAction->setEnabled(false);
	connect(shuffleAlbumsOrderAction, SIGNAL(triggered()), this, SLOT(shuffleAlbumsOrder()));

	shuffleFoldersOrderAction = new QAction(tr("Shuffle (&folders)"), this);
	shuffleFoldersOrderAction->setCheckable(true);
	shuffleFoldersOrderAction->setEnabled(false);
	connect(shuffleFoldersOrderAction, SIGNAL(triggered()), this, SLOT(shuffleFoldersOrder()));

	stopAfterCurrentAction = new QAction(tr("S&top after current"), this);
	connect(stopAfterCurrentAction, SIGNAL(triggered()), this, SLOT(stopAfterCurrent()));

	playbackFollowsCursorAction = new QAction(tr("Playback &follows cursor"), this);
	connect(playbackFollowsCursorAction, SIGNAL(triggered()), this, SLOT(playbackFollowsCursor()));

	cursorFollowsPlaybackAction = new QAction(tr("&Cursor follows playback"), this);
	connect(cursorFollowsPlaybackAction, SIGNAL(triggered()), this, SLOT(cursorFollowsPlayback()));

	albumListAction = new QAction(tr("&Album list"), this);
	connect(albumListAction, SIGNAL(triggered()), this, SLOT(albumList()));

	searchAlbumAction = new QAction(tr("&Search"), this);
	connect(searchAlbumAction, SIGNAL(triggered()), this, SLOT(searchAlbum()));

	configureAction = new QAction(tr("&Configure"), this);
	connect(configureAction, SIGNAL(triggered()), this, SLOT(configure()));

	aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void FooMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void FooMainWindow::createSystrayIcon()
{
	if(!QSystemTrayIcon::isSystemTrayAvailable())
		return;

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":images/icon64.png"));

	trayMenu = new QMenu();
	trayMenu->addAction(stopAction);
	trayMenu->addAction(pauseAction);
	trayMenu->addAction(playAction);
	trayMenu->addAction(prevAction);
	trayMenu->addAction(nextAction);
	trayMenu->addAction(randomAction);

	trayMenu->addSeparator();

	trayMenu->addAction(preferencesAction);

	trayMenu->addSeparator();

	trayMenu->addAction(exitAction);

	trayIcon->setContextMenu(trayMenu);

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			  this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void FooMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		setVisible(!isVisible());
		break;
		//case QSystemTrayIcon::MiddleClick:
		//    ;
		//    	break;
		//case QSystemTrayIcon::Context:
		//    	break;
	}
}

bool FooMainWindow::isCursorFollowsPlayback()
{
	return cursorFollowsPlaybackAction->isChecked();
}


void FooMainWindow::closeEvent(QCloseEvent *event)
{
#warning "tray!"
	/*	if(trayIconAction->isChecked())
	{
		hide();
		event->ignore();
	}
	else
	{*/
	event->accept();
	//}
}

void FooMainWindow::open()
{
}

void FooMainWindow::openAudioCD()
{
}

void FooMainWindow::addFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

	if(files.isEmpty())
		return;

	playlistManager->addFilesToCurrentPlaylist(files);
}

void FooMainWindow::addFolder()
{
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

	if(dirName.isEmpty())
		return;

	QFileInfo info;
	info.setFile(dirName);
	if(!info.isDir())
		return;

	QList<QUrl> urls;
	urls.append(QUrl(dirName));
}

void FooMainWindow::addLocation()
{
	bool ok = false;
	QString locName = QInputDialog::getText(this, tr("Add Location"), tr("Enter adress:"),
														 QLineEdit::Normal, "http://", &ok);
	qDebug() << "Main Window: Add Location:" << locName;
	if(!ok || locName.isEmpty())
		return;

	QUrl adress = QUrl(locName);
	if(!adress.isValid() || adress.host().isEmpty())
		return;
}

void FooMainWindow::loadPlaylist()
{
	QString playlistPath = QFileDialog::getOpenFileName(this, tr("Choose playlist file"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation), "Playlists(*.m3u *.pls)");

	if(!playlistPath.isEmpty())
	{
		QFile file(playlistPath);
		if(file.open(QIODevice::ReadOnly))
		{
			QList<QUrl> urls;
			QTextStream stream(&file);
			QString line;
			while(!stream.atEnd())
			{
				line = stream.readLine();
				if(!line.isEmpty())
				{
				}
			}
			file.close();
			FooPlaylistWidget * wid = static_cast<FooPlaylistWidget *>(fooTabWidget->currentWidget());
			if(!wid)
				return;
		}
	}
}

void FooMainWindow::savePlaylist()
{
	// TODO: fix and use this metod(now the getting selected filter is not working)
	//QString fileName = QFileDialog::getSaveFileName(this, tr("Choose a filename to save playlist"),QDesktopServices::storageLocation(QDesktopServices::MusicLocation)+"playlist.pls", "M3U Playlist(*.m3u);; Pls Playlist(*.pls)",&selFilter);
	QStringList formats;
	formats.append("M3U Playlist (*.m3u)");
	formats.append("PLS Playlist (*.pls)");
	QFileDialog dialog(this, tr("Choose a filename to save playlist"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilters(formats);
	if(dialog.exec() == QDialog::Accepted)
	{
		QString selectedFilter = dialog.selectedNameFilter();
		QString fileName = dialog.selectedFiles()[0];
		if(!fileName.isEmpty())
		{
			fileName.append("."+selectedFilter); // add selected extension to file name
			if(fileName.endsWith("pls"))
			{
				playlistManager->savePlaylistToPls(fileName);
			}
			else
			{
				playlistManager->savePlaylistToM3u(fileName);
			}
		}
	}
}

void FooMainWindow::exit()
{
	qApp->quit();
}

void FooMainWindow::undo()
{
}

void FooMainWindow::redo()
{
}

void FooMainWindow::sortBy()
{
}

void FooMainWindow::randomize()
{
}

void FooMainWindow::reverse()
{
}

void FooMainWindow::sortByFilePatch()
{
}

void FooMainWindow::sortByAlbum()
{
}

void FooMainWindow::sortByTrackNumber()
{
}

void FooMainWindow::sortByTitle()
{
}

void FooMainWindow::search()
{
}

void FooMainWindow::removeDuplicates()
{
	FooPlaylistWidget * wid = static_cast<FooPlaylistWidget *>(fooTabWidget->currentWidget());
	if(!wid) return;
	QVector<QString> files;
	//	foreach(QTreeWidgetItem* item, wid->itemsList())
	{
		// 		if(files.contains(item->text(0)))
		// 		{
		// 			wid->takeTopLevelItem(wid->indexOfTopLevelItem(item));
		// 		}
		// 		else
		// 		{
		// 			files.append(item->text(0));
		// 		}
	}
}

// TODO move this to playlist
void FooMainWindow::removeDeadItems()
{
	FooPlaylistWidget * wid = static_cast<FooPlaylistWidget *>(fooTabWidget->currentWidget());
	if(!wid) return;

	// TODO to check this out

	QFileInfo fileInfo;
	//	foreach(QTreeWidgetItem* item, wid->itemsList())
	//	{
	//		fileInfo.setFile(item->text(0));
	//		if(fileInfo.isFile() && !fileInfo.exists())
	//		{
	// 			wid->takeTopLevelItem(wid->indexOfTopLevelItem(item));
	//		}
	//	}
}

void FooMainWindow::alwaysOnTop()
{
}

void FooMainWindow::console()
{
}

void FooMainWindow::equalizer()
{
}

void FooMainWindow::quickSetup()
{
}

void FooMainWindow::enableLayoutEditionMode()
{
}

void FooMainWindow::createScratchbox()
{
}

void FooMainWindow::mute(bool m)
{
	volumeAction->setIcon(QIcon(m ? ":images/mute.png" : ":images/vol.png"));
}

void FooMainWindow::uncheckAllOrders()
{
	this->defaultOrderAction->setChecked(false);
	this->repeatPlaylistOrderAction->setChecked(false);
	this->repeatTrackOrderAction->setChecked(false);
	this->randomAction->setChecked(false);
	this->shuffleAlbumsOrderAction->setChecked(false);
	this->shuffleFoldersOrderAction->setChecked(false);
	this->shuffleTracksOrderAction->setChecked(false);
}

void FooMainWindow::defaultOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Default);

	this->defaultOrderAction->setChecked(true);
}

void FooMainWindow::repeatPlaylistOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Repeat_Playlist);

	this->repeatPlaylistOrderAction->setChecked(true);
}

void FooMainWindow::repeatTrackOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Repeat_Track);

	this->repeatTrackOrderAction->setChecked(true);
}

void FooMainWindow::randomOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Random);

	this->randomOrderAction->setChecked(true);
}

void FooMainWindow::shuffleTracksOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Shuffle_Tracks);

	this->shuffleTracksOrderAction->setChecked(true);
}

void FooMainWindow::shuffleAlbumsOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Shuffle_Albums);

	this->shuffleAlbumsOrderAction->setChecked(true);
}

void FooMainWindow::shuffleFoldersOrder()
{
	uncheckAllOrders();

	emit changePlaybackOrder(FooPlaybackOrder::Shuffle_Tracks);

	this->shuffleFoldersOrderAction->setChecked(true);
}

void FooMainWindow::stopAfterCurrent()
{
	if(!stopAfterCurrentAction->isChecked())
	{
		this->stopAfterCurrentAction->setChecked(false);
	}
	else
	{
		this->stopAfterCurrentAction->setChecked(true);
	}
}

void FooMainWindow::playbackFollowsCursor()
{
}

void FooMainWindow::cursorFollowsPlayback()
{
}

void FooMainWindow::albumList()
{
}

void FooMainWindow::searchAlbum()
{
}

void FooMainWindow::configure()
{
}

void FooMainWindow::setTrayIcon()
{
	//	trayIcon->setVisible(trayIconAction->isChecked());
}

void FooMainWindow::updateWindowTitle(QMultiMap<QString, QString> newMetaData)
{
	/*
	metaData("ARTIST"));
	metaData("ALBUM"));
	metaData("TITLE"));
	metaData("DATE"));
	metaData("GENRE"));
	metaData("TRACKNUMBER"));
	metaData("DESCRIPTION"));
*/
	// TODO: make it configurable
	// TODO: add more radio support here(radio - artist - title)
	QString newTitle = QString("%1 - %2").arg(
			newMetaData.take("ARTIST"),
			newMetaData.take("TITLE")
			);

	QString appName = qApp->applicationName();
	appName.append(" ");
	appName.append(newTitle);

	setWindowTitle(appName);

	if(!QSystemTrayIcon::isSystemTrayAvailable())
		return;

	trayIcon->setToolTip(newTitle);
}

void FooMainWindow::cutLayout()
{
}

void FooMainWindow::copyLayout()
{
}

void FooMainWindow::pasteLayout()
{
}

void FooMainWindow::about()
{
	(new FooAbout(this))->show();
}

void FooMainWindow::playlistManagerView()
{
}
