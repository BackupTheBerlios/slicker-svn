/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "applet.h"
#include "appletmanager.h"

Applet::Applet(AppletDef *appletDef)
{
    _host = NULL;
    _appletDef = appletDef;
    _appletDef->registerApplet(this);
    _icon = NULL;
    _content = NULL;
}

Applet::~Applet()
{
    if (_host)
        _host->detach(this);
    
    if (_icon) {
        delete _icon;
        _icon = NULL;
    }
        
    if (_content) {
        delete _content;
        _content = NULL;
    }
    
    _appletDef->deregisterApplet(this);
}

AppletDef * Applet::appletDef() const
{
    return _appletDef;
}

const QString & Applet::name()
{
    return appletDef()->name();
}

bool Applet::acceptsHost(const AppletHost * host) const
{
    return appletDef()->acceptsHost(host);
}

bool Applet::transfer(AppletHost * newHost)
{
    // Do not allow to set host to null
    if (!newHost)
        return false;

    // Make sure host accepts the applet and applet accepts the host.
    if (!acceptsHost(newHost) || !newHost->acceptsApplet(this))
        return false;

    // Detach from the old host, if there is one
    if (_host)
        _host->detach(this);

    // Try to attach to newHost
    if (newHost->accept(this)) {
        _host = newHost;
        return true;
    } else {
        _host = NULL;
        return false;
    }
}

QWidget * Applet::icon()
{
    return _icon;   
}

QWidget * Applet::content()
{
    return _content;
}

AppletHost * Applet::host()
{
    return _host;
}
