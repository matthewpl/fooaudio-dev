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

#include "fooslider.hpp"

#include <QMouseEvent>
#include <QDebug>

FooSlider::FooSlider(QWidget *parent) : QSlider(parent)
{
	isChangeing = true;
	setTracking(false);
}

FooSlider::FooSlider(Qt::Orientation orientation, QWidget *parent): QSlider(orientation, parent)
{
	isChangeing = true;
	setTracking(false);
}

void FooSlider::mousePressEvent(QMouseEvent *event)
{
	isChangeing = false;

	if (event->button() == Qt::LeftButton)
	{
		if (orientation() == Qt::Vertical)
		{
			setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height());
		}
		else
		{
			setValue(minimum() + ((maximum()-minimum()) * event->x()) / width());
		}

		event->accept();
	}

	QSlider::mousePressEvent(event);
}

void FooSlider::mouseReleaseEvent(QMouseEvent *event)
{
	isChangeing = true;
	if (orientation() == Qt::Vertical)
	{
		emit valueChanged2(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height());
	}
	else
	{
		emit valueChanged2(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;
	}
	QSlider::mouseReleaseEvent(event);
}

void FooSlider::valuesChanged(qint64 position, qint64 total)
{
	if (isChangeing)
	{
		if (maximum() != total)
		{
			setMaximum(total);
		}

		setValue(position);
	}
}
