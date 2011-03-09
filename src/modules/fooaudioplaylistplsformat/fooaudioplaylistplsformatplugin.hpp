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

#ifndef FOOAUDIOPLAYLISTPLSFORMATPLUGIN_HPP
#define FOOAUDIOPLAYLISTPLSFORMATPLUGIN_HPP

#include <fooplaylistformatplugin.hpp>
#include <fooplaylistformat.hpp>

#include <QObject>

class FooaudioPlaylistPlsFormatPlugin : public QObject, Fooaudio::FooPlaylistFormatPlugin
{
	Q_OBJECT
	Q_INTERFACES(Fooaudio::FooPlaylistFormatPlugin)

public:
	explicit FooaudioPlaylistPlsFormatPlugin(QObject *parent = 0);
	Fooaudio::FooPlaylistFormat* getPlaylistFormatPlugin();
};

#endif // FOOAUDIOPLAYLISTPLSFORMATPLUGIN_HPP
