/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

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
