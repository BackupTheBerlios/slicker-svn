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
#include <qcursor.h>
#include <qpopupmenu.h>



Applet::Applet(AppletDef *appletDef)
: _contextMenu(0)
{
    _host = NULL;
    _appletDef = appletDef;
    _appletDef->registerApplet(this);
    _icon = NULL;
    _content = NULL;
}
    
void Applet::popupContextMenu()
{
    if (!_contextMenu) {
        _contextMenu = new KActionMenu();
    }
    KActionPtrList& actions = contextActions();
    KActionPtrList::Iterator it_action = actions.begin();
    for (; it_action != actions.end(); ++it_action)
    {
        _contextMenu->remove(*it_action);
        _contextMenu->insert(*it_action);
    }
    _contextMenu->popup(QCursor::pos());
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
    
    if (_contextMenu) {
        delete _contextMenu;
	_contextMenu = NULL;
    }
       
    _appletDef->deregisterApplet(this);
}

bool Applet::eventFilter(QObject* o, QEvent* e)
{
    if (o == icon()) {
        switch (e->type()) {
        case QEvent::ContextMenu:
            popupContextMenu();
            break;
        }
    }
    else if (o == content()) {
    }
    
    return FALSE;
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

void Applet::setIcon(QWidget* icon)
{
    _icon = icon;
    if (_icon) {       
        _icon->installEventFilter(this);
    }
}

QWidget * Applet::icon()
{
    return _icon;   
}

void Applet::setContent(QWidget* content)
{
    _content = content;
    if (_content) {       
        _content->installEventFilter(this);
    }
}

QWidget * Applet::content()
{
    return _content;
}


AppletHost * Applet::host()
{
    return _host;
}

KActionPtrList& Applet::contextActions()
{
    return _defaultContextActions;
}

void Applet::addDefaultContextAction(KAction* action)
{
    _defaultContextActions.append(action);
}

void Applet::addContextAction(KAction* action)
{
    contextActions().append(action);
}
