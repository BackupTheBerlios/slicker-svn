/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "sessionapplet.h"
#include "sessionmanager.h"

SessionApplet::SessionApplet(SessionAppletDef *appletDef, const QString & id)
 : Applet(appletDef), SessionItem(id)
{
}

SessionApplet::~SessionApplet()
{
}

void SessionApplet::store(KConfigBase * config)
{
    SessionItem * parent = sessionParent();
    
    if (parent)
        config->writeEntry("parent", parent->id());
}

void SessionApplet::restore(KConfigBase * config)
{
    SessionItem * parentItem = SessionManager::self()->getItem(config->readEntry("parent", "none"));
    AppletHost * parent = dynamic_cast<AppletHost*>(parentItem);
    if (parent)
        transfer(parent);
}

SessionItem * SessionApplet::sessionParent()
{
    return dynamic_cast<SessionItem*>(host());
}
