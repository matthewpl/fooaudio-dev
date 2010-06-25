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

#ifndef FOOSETTINGSMANAGER_HPP
#define FOOSETTINGSMANAGER_HPP

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QSettings>

#include "foosettingsmanagerwindow.hpp"

class FooSettingsManager : public QObject
{
	Q_OBJECT

public:
	static FooSettingsManager& instance()
	{
		static FooSettingsManager fooSettingsManager;
		return fooSettingsManager;
	}

	void saveSettings(QString, QMap<QString, QVariant> &);
	QMap<QString, QVariant> readSettings (QString);

private:
	FooSettingsManager() {}
	FooSettingsManager(const FooSettingsManager &);
	FooSettingsManager& operator=(const FooSettingsManager&);

	FooSettingsManagerWindow settingsManagerWindow;

	void saveSettings(const QMap<QString, QVariant> &, QSettings &);
	void readSettings(QMap<QString, QVariant> &, QSettings &);

signals:

public slots:
	void showWindow();
};

#endif // FOOSETTINGSMANAGER_HPP
