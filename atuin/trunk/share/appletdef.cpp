/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "appletdef.h"
#include "appletmanager.h"
#include "applet.h"

#include <kdebug.h>
#include <qptrlist.h>

/*** AppletDef *******************************************************/
AppletDef::AppletDef(const QString &name, const QString &description, const QString &author)
{
    _name = name;
    _description = description;
    _author = author;
    AppletManager::self()->registerAppletDef(this);
}

AppletDef::~AppletDef()
{
    // First we destroy all applets belonging to this AppletDef.
    QPtrListIterator<Applet> it( _applets );
    for (Applet *applet = it.toFirst(); (applet = it.current()); ) // Iteration will happen automatically, when applets gets deregistered
        delete applet;

    AppletManager::self()->deregisterAppletDef(this);
}

bool AppletDef::acceptsHost(const AppletHost * /*host*/) const
{
    return true;
}

const QString & AppletDef::author() const
{
    return _author;
}

const QString & AppletDef::description() const
{
    return _description;
}

const QString & AppletDef::name() const
{
    return _name;
}

void AppletDef::registerApplet(Applet *applet)
{
    _applets.append(applet);
}

void AppletDef::deregisterApplet(Applet *applet)
{
    _applets.remove(applet);
}

/*** AppletDefMenuItem ***********************************************/
AppletDefsMenuItem::AppletDefsMenuItem(AppletDef * appletDef, AppletHost * appletHost)
        : KAction(appletDef->name(), KShortcut(), NULL, "AppletDefMenuItem")
{
    _appletDef = appletDef;
    _appletHost = appletHost;
}

void AppletDefsMenuItem::slotActivated()
{
    Applet * applet = _appletDef->createApplet();
    if (applet) {
        AppletHost * appletHost = _appletHost;

        if (!appletHost)
            appletHost = AppletManager::self()->dispatch(applet);

        if ((!appletHost) || (!applet->transfer(appletHost))) {
            kdDebug(0) << "Could not dispatch applet " << applet->name() << endl;
            delete applet;
        }
    } else
        kdDebug(0) << "Could not create applet of type " << _appletDef->name() << endl;
}

/*** AppletMenu ******************************************************/
AppletDefsMenu::AppletDefsMenu(QObject *parent, const char *name, AppletHost * appletHost, const QString &text)
        : KActionMenu(text, parent, name)
{
    _appletHost = appletHost;

    AppletManager * appletManager = AppletManager::self();
    QPtrListIterator<AppletDef> iter(*appletManager->appletDefs());

    for (AppletDef *appletDef=0; (appletDef = iter.current()); ++iter)
        slotAppletDefRegistered(appletDef);

    connect(appletManager, SIGNAL(appletDefRegistered(AppletDef* )), this, SLOT(slotAppletDefRegistered(AppletDef* )));
    connect(appletManager, SIGNAL(appletDefDeregistered(AppletDef* )), this, SLOT(slotAppletDefDeregistered(AppletDef* )));
}

void AppletDefsMenu::slotAppletDefRegistered(AppletDef * appletDef)
{
    if (_appletHost && (!_appletHost->acceptsApplet(appletDef) || !appletDef->acceptsHost(_appletHost)))
        return; // AppletDef and AppletHost aren't compatible, so let's filter it all out.

    AppletDefsMenuItem * menuItem = new AppletDefsMenuItem(appletDef, _appletHost);
    _appletDefMenuMap[appletDef] = menuItem;
    insert(menuItem);
}

void AppletDefsMenu::slotAppletDefDeregistered(AppletDef * appletDef)
{
    AppletDefsMenuItem * menuItem = _appletDefMenuMap[appletDef];
    if (menuItem) {
        remove
            (menuItem);
        delete menuItem;
    }
}

#include "appletdef.moc"





