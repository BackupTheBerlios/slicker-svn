/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "applethost.h"
#include "appletmanager.h"
#include "applet.h"


AppletHost::AppletHost(QString &name)
{
    AppletManager::self()->registerAppletHost(this);
    _name = name;
}

AppletHost::AppletHost(const char *name)
{
    AppletManager::self()->registerAppletHost(this);
    _name = name;
}

AppletHost::~AppletHost()
{
    // TODO: Relocate applets held, but for now just shut them down.
    QPtrListIterator<Applet> it( _applets );
    for (Applet *applet = it.toFirst(); (applet = it.current()); ++it )
        delete applet;

    AppletManager::self()->deregisterAppletHost(this);
}

bool AppletHost::acceptsApplet(const AppletDef * applet) const
{
    applet->name(); // Avoid warnings. If anyone knows a better way, please do tell.
    return true;
}

bool AppletHost::acceptsApplet(const Applet * applet) const
{
    return acceptsApplet(applet->appletDef());
}

void AppletHost::registerApplet(Applet *applet)
{
    _applets.append(applet);
}

void AppletHost::deregisterApplet(Applet *applet)
{
    _applets.remove(applet);
}

const QString &AppletHost::name() const
{
    return _name;
}

void AppletHost::removeAllApplets()
{
	
	QPtrListIterator<Applet> iter(_applets);
	QPtrList <Applet> appletList;
	Applet * applet;
	
	//A copy of the list is needed, because it will be destroyed while deregistering the applets
	for(applet = 0; (applet=iter.current()); ++iter)
		appletList.append(applet);
				
	QPtrListIterator <Applet> iter2(appletList);

	for(applet = 0; (applet = iter2.current()); ++iter2)
	{
		deregisterApplet(applet);
		delete(applet);
	}
}


