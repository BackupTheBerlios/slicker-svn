/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
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

#include "informationcenterapplet.h"

using namespace CardDesk;

extern "C"
{
	CardApplet *init( const QString &configFile)
	{
		return new InformationcenterApplet(configFile);
	}

}
