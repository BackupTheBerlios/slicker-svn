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

#include <carddeskcore/Const.h>
#include <carddeskcore/Card.h>
#include <carddeskcore/CardApplet.h>

using namespace CardDesk;

#include "TrayTestApplet.h"

extern "C"
{
	CardApplet *init(const QString &configFile)
	{
		return new TrayTestApplet(configFile);
	}

}

