/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef SESSIONAPPLET_H
#define SESSIONAPPLET_H

#include <applet.h>
#include "sessionitem.h"
#include "sessionappletdef.h"

/**
@author Ulrik Mikaelsson
*/
class SessionApplet : public Applet, public SessionItem
{
public:
    SessionApplet(SessionAppletDef *appletDef, const QString & id);
    virtual ~SessionApplet();

    virtual void store(KConfigBase * config);
    virtual void restore(KConfigBase * config);
    
    SessionItem * sessionParent();
};

#endif
