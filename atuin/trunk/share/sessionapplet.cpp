//
// C++ Implementation: sessionapplet
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
