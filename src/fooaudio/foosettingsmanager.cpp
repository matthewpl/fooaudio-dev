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

#include "foosettingsmanager.hpp"
#include <QFileDialog>
#include <QDebug>
#include <QCoreApplication>

namespace Fooaudio
{
	FooSettingsManager::FooSettingsManager()
	{
		settingsManagerWindow = new FooSettingsManagerWindow();
		connect(qApp, SIGNAL(aboutToQuit()), settingsManagerWindow, SLOT(deleteLater()));
	}

	void FooSettingsManager::showWindow()
	{
		settingsManagerWindow->show();
	}

	void FooSettingsManager::saveSettings(QString name, QMap<QString, QVariant> &mapa)
	{
		QSettings settings(QDir::homePath() + "/.fooaudio/" + name, QSettings::IniFormat);
		settings.setIniCodec("UTF-8");

		saveSettings(mapa, settings);

		settings.sync();
	}

	void FooSettingsManager::saveSettings(const QMap<QString, QVariant> &mapa, QSettings &settings)
	{
		QMapIterator<QString, QVariant> i(mapa);
		while (i.hasNext())
		{
			i.next();
			QString key = i.key();
			QVariant value = i.value();

			if (value.type() == QVariant::Map)
			{
				settings.beginGroup(key);
				saveSettings(value.toMap(), settings);
				settings.endGroup();
			}
			else
			{
				settings.setValue(key, value);
			}
		}
	}

	QMap<QString, QVariant> FooSettingsManager::readSettings(QString name)
	{
		QSettings settings(QDir::homePath() + "/.fooaudio/" + name, QSettings::IniFormat);
		settings.setIniCodec("UTF-8");

		QMap<QString, QVariant> mapa;

		readSettings(mapa, settings);

		return mapa;
	}

	void FooSettingsManager::readSettings(QMap<QString, QVariant> &mapa, QSettings &settings)
	{
		QStringList childKeys = settings.childKeys();

		for (int i = 0; i < childKeys.size(); i++)
		{
			mapa.insert(childKeys.at(i), settings.value(childKeys.at(i)));
		}

		QStringList childGroups = settings.childGroups();

		for (int i = 0; i < childGroups.size(); i++)
		{
			settings.beginGroup(childGroups.at(i));

			readSettings(mapa, settings);

			settings.endGroup();
		}
	}
}
