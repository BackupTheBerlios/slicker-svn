//
// C++ Interface: sessionapplet
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
