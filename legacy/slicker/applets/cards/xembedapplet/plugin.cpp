/***************************************************************************
                          plugin.cpp  -  description
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

#include <qstring.h>
#include <kconfig.h>
#include <kdebug.h>


#include <carddeskcore/Const.h>
#include <carddeskcore/Card.h>
#include <carddeskcore/CardApplet.h>

#include "XEmbedCardApplet.h"

/**
	NOTE:
	The factory functions are *always* named
		CardApplet *init(const QString&)

*/

using namespace CardDesk;

extern "C"
{
	CardApplet *init(const QString &configFile)
	{
		XEmbedCardApplet *tca = new XEmbedCardApplet(configFile);
		return tca;
	}

}

