/***************************************************************************
                          TrayTestApplet.cpp  -  description
                             -------------------
    begin                : Sat Jan 11 2003
    copyright            : (C) 2003 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "TrayTestApplet.h"

#include <kconfig.h>
#include <kiconloader.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>

/*
		class QPixmap _icon;
		bool _down;
*/

using namespace CardDesk;

IconTrayButton::IconTrayButton(Tray *tray, const char *name, QPixmap icon)
	:TrayWidget(tray, name, icon.size().width() + 2, icon.size().height() + 2),
	_icon(icon), _down(false)
{}

void IconTrayButton::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		_down = true;
		repaint();
	}
}

void IconTrayButton::mouseReleaseEvent(QMouseEvent *e)
{
	_down = false;
	if (e->button() == Qt::LeftButton)
	{
		emit pressed(this);
	}

	repaint();
}

void IconTrayButton::paintEvent(QPaintEvent *)
{
	QPainter paint(this);
	if (_down)
		paint.drawPixmap(1, 1, _icon);
	else
		paint.drawPixmap(0, 0, _icon);
}

void IconTrayButton::orientationChanged( bool horizontal )
{}

/*
		QWidget *_contentsWidget;
*/

TrayTestApplet::TrayTestApplet(const QString &configFile)
	:CardApplet(configFile)
{}

void TrayTestApplet::addToDefaultContextMenu( QPopupMenu * )
{}

bool TrayTestApplet::allowDropOnTab( void )
{
	return true;
}

bool TrayTestApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void TrayTestApplet::tabDragDropEvent( QDropEvent * )
{

}

Tray *TrayTestApplet::createTray( Card *c )
{
	Tray *tray = new Tray(c);

	KIconLoader il;
	new IconTrayButton(tray, "folder", il.loadIcon("folder", KIcon::Toolbar, 0, KIcon::DefaultState));
	new IconTrayButton(tray, "exec", il.loadIcon("exec", KIcon::Toolbar, 0, KIcon::DefaultState));
	new IconTrayButton(tray, "folder_home", il.loadIcon("folder_home", KIcon::Toolbar, 0, KIcon::DefaultState));

	return tray;
}

Separator *TrayTestApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

Contents *TrayTestApplet::createContents( Card *c )
{
	Contents *contents = new Contents(c);
	_contentsWidget = new QWidget(0, "TrayTestApplet-ContentsWidget");
	QVBoxLayout *layout = new QVBoxLayout(_contentsWidget);

	QLabel *l = new QLabel("<b>TrayTestApplet</b>", _contentsWidget);
	layout->addWidget(l);

	contents->setContents(_contentsWidget);
	return contents;
}

void TrayTestApplet::saveSettings( class KConfig * )
{

}

void TrayTestApplet::loadSettings( class KConfig * )
{

}

void TrayTestApplet::cardReady( void )
{

}
