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

#include <QDebug>

#include "footrack.hpp"

FooTrack::FooTrack()
{
	metaVersion = -1;
}

FooTrack::FooTrack(QUrl file) : m_path(file)
{
	metaVersion = -1;
}

QUrl FooTrack::file() const
{
	return m_path;
}

void FooTrack::setFile(QUrl path)
{
	m_path = path;
}

void FooTrack::setFile(QString path)
{
	m_path.setPath(path);
}

QString FooTrack::title() const
{
	return m_title;
}

void FooTrack::setTitle(QString title)
{
	m_title = title;
}

QString FooTrack::version() const
{
	return m_version;
}

void FooTrack::version(QString version)
{
	m_version = version;
}

QString FooTrack::album() const
{
	return m_album;
}

void FooTrack::setAlbum(QString album)
{
	m_album = album;
}

QString FooTrack::trackNumber() const
{
	return m_trackNumber;
}

void FooTrack::setTrackNumber(QString trackNumber)
{
	m_trackNumber = trackNumber;
}

QString FooTrack::artist() const
{
	return m_artist;
}

void FooTrack::setArtist(QString artist)
{
	m_artist = artist;
}

QString FooTrack::performer() const
{
	return m_performer;
}

void FooTrack::setPerformer(QString performer)
{
	m_performer = performer;
}

QString FooTrack::copyright() const
{
	return m_copyright;
}

void FooTrack::setCopyright(QString copyright)
{
	m_copyright = copyright;
}

QString FooTrack::license() const
{
	return m_license;
}

void FooTrack::setLicense(QString license)
{
	m_license = license;
}

QString FooTrack::organization() const
{
	return m_organization;
}

void FooTrack::setOrganization(QString organization)
{
	m_organization = organization;
}

QString FooTrack::description() const
{
	return m_description;
}

void FooTrack::setDescription(QString description)
{
	m_description = description;
}

QString FooTrack::genre() const
{
	return m_genre;
}

void FooTrack::setGenre(QString genre)
{
	m_genre = genre;
}

QString FooTrack::date() const
{
	return m_date;
}

void FooTrack::setDate(QString date)
{
	m_date = date;
}

QString FooTrack::location() const
{
	return m_location;
}

void FooTrack::setLocation(QString location)
{
	m_location = location;
}

QString FooTrack::contact() const
{
	return m_contact;
}

void FooTrack::setContact(QString contact)
{
	m_contact = contact;
}

QString FooTrack::isrc() const
{
	return m_isrc;
}

void FooTrack::setIsrc(QString isrc)
{
	m_contact = isrc;
}

bool FooTrack::operator== (const FooTrack &compare) const
{
	return m_path == compare.m_path;
}

bool FooTrack::operator< (const FooTrack &compare) const
{
	return m_path < compare.m_path;
}

bool FooTrack::operator!= (const FooTrack &compare) const
{
	return m_path != compare.m_path;
}

int FooTrack::getMetaVersion() const
{
	return metaVersion;
}

void FooTrack::setMetaVersion(int i)
{
	metaVersion = i;
}

QStringList FooTrack::getMeta() const
{
	return meta;
}

void FooTrack::setMeta(QStringList _meta)
{
	meta = _meta;
}

int FooTrack::lenght() const
{
	return m_lenght;
}

void FooTrack::setLenght(int lenght)
{
	m_lenght = lenght;
}

QString FooTrack::lenghtInSeconds() const
{
	QString lis;

	return lis.setNum(m_lenght);
}

QString FooTrack::lenghtInMinutes() const
{
	return "0:00";
}
