/***************************************************************************
                          CardApplet.cpp  -  description
                             -------------------
    begin                : Sun Jan 5 2003
    copyright            : (C) 2003 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "CardApplet.h"
#include "Card.h"
#include "CardManager.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <kdebug.h>

#define DEBUG_SESSION 0

namespace CardDesk {


/*
		Card *_card;
*/

CardApplet::CardApplet(const QString &configFile)
	: QObject(), TabDelegate(), _card(0L)
{
	kdDebug(155003) << "CardApplet::CardApplet(" << configFile << ")" << endl;
	_config = new KConfig(configFile,false);
}


QPixmap *CardApplet::tabIcon( void )
{
	return TabDelegate::tabIcon();
}

QSize CardApplet::miniDisplaySize( void )
{
	return TabDelegate::miniDisplaySize();
}

void CardApplet::paintMiniDisplay(QPainter *p, int width, int height)
{
	return TabDelegate::paintMiniDisplay(p, width, height);
}

void CardApplet::addToDefaultContextMenu( QPopupMenu *defaultMenu )
{
	return TabDelegate::addToDefaultContextMenu(defaultMenu);
}

void CardApplet::aboutToShowContextMenu( QPopupMenu *menu )
{
	return TabDelegate::aboutToShowContextMenu(menu);
}

bool CardApplet::allowDragHoverToOpen( void )
{
	return TabDelegate::allowDragHoverToOpen();
}

bool CardApplet::allowHoverToOpen( void )
{
	return TabDelegate::allowHoverToOpen();
}

bool CardApplet::allowDropOnTab( void )
{
	return TabDelegate::allowDropOnTab();
}

bool CardApplet::allowRenameCard( void )
{
	return true;
}

bool CardApplet::canDecodeDrag( QMimeSource *ms )
{
	return TabDelegate::canDecodeDrag(ms);
}

void CardApplet::tabDragDropEvent( QDropEvent *e )
{
	return TabDelegate::tabDragDropEvent(e);
}

void CardApplet::setCardName( const QString &newName )
{
	if (newName != QString::null)
		_name = newName;
	_card->setCardName(cardName());
}

QString CardApplet::defaultCardName( void )
{
	return "Slicker Card";
}

QString CardApplet::cardName( void )
{
	return (_name==QString::null) ? defaultCardName() : _name;
}

Tray *CardApplet::createTray( Card * )
{
	return 0L;
}

Separator *CardApplet::createSeparator( Card *c )
{
	return new Separator(c, false);
}

Contents *CardApplet::createContents( Card *c )
{
	return new Contents(c);
}

Resizer *CardApplet::createResizer( Card *c )
{
	return new Resizer(c, false);
}

Tab *CardApplet::createTab( Card *c)
{
	return new Tab(c, this);
}

Card *CardApplet::card( void )
{
	return _card;
}

void CardApplet::cardReady( void )
{
	_card->getTab()->connect(this, SIGNAL(dropProcessed()), SLOT(dropProcessed()));
}

void CardApplet::cardOpened( void )
{}

void CardApplet::cardShut( void )
{}

bool CardApplet::cardDestroy( void )
{
return true;
}

void CardApplet::environmentChanged( void )
{
	emit propertiesChanged();
}

void CardApplet::attachToCard( Card *c)
{
	_card = c;

	connect(_card, SIGNAL(opened(Card *)), this, SLOT(cardOpened(void)));
	connect(_card, SIGNAL(closed(Card *)), this, SLOT(cardShut(void)));

	connect(_card, SIGNAL(edgeChanged(Card *)), this, SLOT(environmentChanged()));
	connect(_card, SIGNAL(deckingChanged(Card *)), this, SLOT(environmentChanged()));

	_card->connect(this, SIGNAL(propertiesChanged()), SLOT(appletPropertiesChanged()));
}

}; //namespace
