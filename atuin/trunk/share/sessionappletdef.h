//
// C++ Interface: sessionappletdef
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SESSIONAPPLETDEF_H
#define SESSIONAPPLETDEF_H

#include <appletdef.h>
#include "sessionitemfactory.h"

class SessionApplet;

/**
* A special type of AppletDef. Used for applets that should respawn on Slicker restart.
* @see SessionApplet
* @author Ulrik Mikaelsson
*/
class SessionAppletDef : public AppletDef, public SessionItemFactory
{
public:
    SessionAppletDef(const QString& id, const QString &name, const QString &description, const QString &author);
    virtual ~SessionAppletDef();

    virtual SessionApplet * loadSessionApplet(KConfigBase * configGroup) = 0;
    virtual SessionApplet * createSessionApplet() = 0;
    
    SessionItem * loadItem(KConfigBase * configGroup);
    SessionItem * createItem();
    Applet * createApplet();
};

#endif
