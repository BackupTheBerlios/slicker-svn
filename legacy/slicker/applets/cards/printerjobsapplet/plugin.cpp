/***************************************************************************
  Copyright (C) 2003 by Jason Kivlighn, mizunoami44@users.sourceforge.net
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

#include "PrinterJobsApplet.h"

using namespace CardDesk;

extern "C"
{
	CardApplet *init( const QString &configFile )
	{
		return new PrinterJobsApplet(configFile);
	}

}

