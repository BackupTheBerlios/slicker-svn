/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "appletmanager.h"

/*** AppletManager ***************************************************/
AppletManager *AppletManager::_self = NULL;

AppletManager::AppletManager()
        : QObject(NULL, "AppletManager")
{
    _self = this; // AppletMenu needs AppletManager to be instantiated.
}

AppletManager::~AppletManager()
{
    // Cleanup AppletDef:s
    QPtrListIterator<AppletDef> itD( _appletDefs );
    for (AppletDef *appletDef = itD.toFirst(); (appletDef = itD.current()); ++itD )
        delete appletDef;

    // Cleanup AppletHost:s
    QPtrListIterator<AppletHost> itH( _appletHosts );
    for (AppletHost *AppletHost = itH.toFirst(); (AppletHost = itH.current()); ++itH )
        delete AppletHost;
}

AppletManager * AppletManager::self()
{
    if (!_self)
        _self = new AppletManager();

    return _self;
}

const QPtrList<AppletHost> * AppletManager::appletHosts() const
{
    return &_appletHosts;
}

const QPtrList<AppletDef> * AppletManager::appletDefs() const
{
    return &_appletDefs;
}

AppletHost * AppletManager::dispatch(const AppletDef *applet) const
{
    QPtrListIterator<AppletHost> it( *appletHosts() );
    AppletHost * host;
    for ( it.toFirst(); (host = it.current()); ++it ) {
        if (host->acceptsApplet(applet) && applet->acceptsHost(host) )
            return host;
    }

    return NULL;
}

AppletHost * AppletManager::dispatch(const Applet *applet) const
{
    QPtrListIterator<AppletHost> it( *appletHosts() );
    AppletHost *host;
    for ( it.toFirst(); (host = it.current()); ++it ) {
        if (host->acceptsApplet(applet) && applet->acceptsHost(host) )
            return host;
    }

    return NULL;
}

bool AppletManager::dispatchAndTransfer(Applet *applet) const
{
    AppletHost * candidate = dispatch(applet);

    if (!candidate)
        return FALSE;

    return applet->transfer(candidate);
}

void AppletManager::registerAppletHost(AppletHost * appletHost)
{
    _appletHosts.append(appletHost);
    emit appletHostRegistered(appletHost);
}

void AppletManager::deregisterAppletHost(AppletHost * appletHost)
{
    _appletHosts.remove(appletHost);
    emit appletHostDeregistered(appletHost);
}

void AppletManager::registerAppletDef(AppletDef * appletDef)
{
    _appletDefs.append(appletDef);
    emit appletDefRegistered(appletDef);
}

void AppletManager::deregisterAppletDef(AppletDef * appletDef)
{
    _appletDefs.remove(appletDef);
    emit appletDefDeregistered(appletDef);
}

#include "appletmanager.moc"






