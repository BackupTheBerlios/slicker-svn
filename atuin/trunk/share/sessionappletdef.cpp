/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "sessionappletdef.h"
#include "sessionapplet.h"

SessionAppletDef::SessionAppletDef(const QString& id, const QString &name, const QString &description, const QString &author)
 : AppletDef(name, description, author), SessionItemFactory(id)
{
}


SessionAppletDef::~SessionAppletDef()
{
}

SessionItem * SessionAppletDef::loadItem(KConfigBase * configGroup)
{
    return loadSessionApplet(configGroup);
}

SessionItem * SessionAppletDef::createItem()
{
    return createSessionApplet();
}

Applet * SessionAppletDef::createApplet()
{
    return createSessionApplet();
}
