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

#ifndef FOOSLIDER_HPP
#define FOOSLIDER_HPP

#include <QSlider>

namespace Fooaudio
{
	class FooSlider : public QSlider
	{
		Q_OBJECT
	public:
		explicit FooSlider(QWidget *parent = 0);
		explicit FooSlider(Qt::Orientation orientation, QWidget *parent = 0);

	protected:
		void mousePressEvent(QMouseEvent *ev);
		void mouseReleaseEvent(QMouseEvent *ev);

	private:
		bool isChangeing;

	signals:
		void valueChanged2(qint64);

	public slots:
		void valuesChanged(qint64 position, qint64 total);
	};
}

#endif // FOOSLIDER_HPP
