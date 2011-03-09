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

#ifndef FOOAUDIOPLAYLISTM3UFORMATPLUGIN_HPP
#define FOOAUDIOPLAYLISTM3UFORMATPLUGIN_HPP

#include <QObject>

#include "fooplaylistformatplugin.hpp"
#include "fooplaylistformat.hpp"

class FooaudioPlaylistM3uFormatPlugin : public QObject, public Fooaudio::FooPlaylistFormatPlugin
{
	Q_OBJECT
	Q_INTERFACES(Fooaudio::FooPlaylistFormatPlugin)

public:
	explicit FooaudioPlaylistM3uFormatPlugin(QObject *parent = 0);
	Fooaudio::FooPlaylistFormat* getPlaylistFormatPlugin();
};

#endif // FOOAUDIOPLAYLISTM3UFORMATPLUGIN_HPP
