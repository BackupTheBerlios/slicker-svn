/***************************************************************************
                          KonsoleApplet.cpp  -  description
                             -------------------
    begin                : Sat Jan 11 2003
    copyright            : (C) 2003 by Fabian Uffer
    email                : fuffer@student.ethz.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "KonsoleApplet.h"

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
#include <kdebug.h>
#include <klibloader.h>
#include <qwidget.h>
#include "../../../carddeskcore/CardManager.h"

KonsoleApplet::KonsoleApplet( const QString &configFile)
	:CardApplet(configFile)
{
	init();
}

void KonsoleApplet::init()
{
	KIconLoader il;
	_icon = il.loadIcon("konsole", KIcon::Small, 0, KIcon::DefaultState);
}

QPixmap *KonsoleApplet::tabIcon( void )
{
	return &_icon;
}

QSize KonsoleApplet::miniDisplaySize( void )
{
	return QSize( 40, 18 );
}

void KonsoleApplet::paintMiniDisplay(QPainter *, int, int)
{}

void KonsoleApplet::addToDefaultContextMenu( QPopupMenu * )
{}

bool KonsoleApplet::allowDropOnTab( void )
{
	return false;
}

bool KonsoleApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void KonsoleApplet::tabDragDropEvent( QDropEvent * )
{}

Separator *KonsoleApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

Contents *KonsoleApplet::createContents( Card *c )
{
	Contents *contents = new Contents(c);
	_contentsWidget = new QWidget(0, "TrayTestApplet-ContentsWidget");
	QVBoxLayout *layout = new QVBoxLayout(_contentsWidget);

	KLibFactory *factory = KLibLoader::self()->factory("libkonsolepart");
	if (!factory) return contents;

    	part = (KParts::ReadOnlyPart *) factory->create(_contentsWidget);
    	if (!part) return contents;
    	if (part->widget()->inherits("QFrame"))
      	((QFrame*)part->widget())->setFrameStyle(QFrame::Panel|QFrame::Sunken);

	layout->addWidget(part->widget());
	contents->setContents(_contentsWidget);
	connect(part, SIGNAL(destroyed()),this, SLOT(destroy()) );
	return contents;
}

void KonsoleApplet::saveSettings( class KConfig * )
{}

void KonsoleApplet::loadSettings( class KConfig * )
{}

void KonsoleApplet::cardReady( void )
{}

void KonsoleApplet::destroy(void){
	Manager::instance()->suicide(card());
}

bool KonsoleApplet::allowHoverToOpen( void ){
	return false;
}

Resizer *KonsoleApplet::createResizer( Card *c)
{
	return new Resizer(c, true);
}
