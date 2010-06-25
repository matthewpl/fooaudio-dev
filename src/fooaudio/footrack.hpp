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

#ifndef FOOTRACK_HPP
#define FOOTRACK_HPP

#include <QString>
#include <QStringList>
#include <QUrl>

#include "footrackinfo.hpp"

class FooTrack
{
public:
	FooTrack();
	FooTrack(QUrl path);

	QUrl file() const;
	void setFile(QUrl path);
	void setFile(QString path);

	QString title() const;
	void setTitle(QString title);

	QString version() const;
	void version(QString version);

	QString album() const;
	void setAlbum(QString album);

	QString trackNumber() const;
	void setTrackNumber(QString trackNumber);

	QString artist() const;
	void setArtist(QString artist);

	QString performer() const;
	void setPerformer(QString performer);

	QString copyright() const;
	void setCopyright(QString copyright);

	QString license() const;
	void setLicense(QString license);

	QString organization() const;
	void setOrganization(QString organization);

	QString description() const;
	void setDescription(QString description);

	QString genre() const;
	void setGenre(QString genre);

	QString date() const;
	void setDate(QString date);

	QString location() const;
	void setLocation(QString location);

	QString contact() const;
	void setContact(QString contact);

	QString isrc() const;
	void setIsrc(QString isrc);

	int lenght() const;
	void setLenght(int lenght);

	QString lenghtInSeconds() const;
	QString lenghtInMinutes() const;

	bool operator== (const FooTrack &compare) const;
	bool operator< (const FooTrack &compare) const;
	bool operator!= (const FooTrack &compare) const;

	int getMetaVersion() const;
	void setMetaVersion(int i);

	QStringList getMeta() const;
	void setMeta(QStringList meta);

private:
	QUrl m_path;

	QString m_title;
	QString m_version;
	QString m_album;
	QString m_trackNumber;
	QString m_artist;
	QString m_performer;
	QString m_copyright;
	QString m_license;
	QString m_organization;
	QString m_description;
	QString m_genre;
	QString m_date;
	QString m_location;
	QString m_contact;
	QString m_isrc;

	int m_lenght;

	QStringList meta;

	int metaVersion;
};

#endif
