/***************************************************************************
                          const.h  -  description
                             -------------------
    begin                : Thu Dec 26 2002
    copyright            : (C) 2002 by Shamyl Zakariya
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

#ifndef __CONST_H__
#define __CONST_H__

#include <qstring.h>
#include <qpoint.h>

namespace CardDesk {

/**
	ScreenEdge represents the edge of the screen that a card is attached to. So
	if card->edge() returns RightEdge, the card is attached to the right edge
	of the screen.
*/
enum ScreenEdge { TopEdge, RightEdge, BottomEdge, LeftEdge, None };

/**
	This enum is used more or less internally to the Deck class to represent
	which edges of a card should be "rounded". A card on the left edge would
	have a shape of TopRight | BottomRight, for example.
*/
enum ShapeCorners { TopLeft = 1, TopRight = 2, BottomRight = 4, BottomLeft = 8 };

/**
	Describes the type of plugin.
	AppletOnly - plugin provides just an applet
	CustomCardAndApplet - plugin provides an applet and custom Card
	CustomCardOnly - plugin provides a custom card which doesn't need an applet.
	None - plugin is invalid.
*/
enum PluginType { AppletOnly, CustomCardAndApplet, CustomCardOnly, NoPlugin };

 //namespace

/*
	This is weird, but these global function names seem to conflict with
	existing functions. It doesn't make sense but it messes up some
	stdc++ and Qt stl functions. Doesn't matter now they're in the CardDesk namespace
*/

QString EdgeToString(ScreenEdge ScreenEdge);
ScreenEdge StringToEdge(const QString &e);

QString PluginTypeToString( PluginType type );
double distance(QPoint a, QPoint b);

}

#endif
