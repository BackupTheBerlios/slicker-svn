//
// C++ Implementation: sessionappletdef
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
