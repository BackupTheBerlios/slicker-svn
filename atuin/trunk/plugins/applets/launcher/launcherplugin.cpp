/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include <kgenericfactory.h>
#include "launcherplugin.h"
#include "launcherapplet.h"

typedef KGenericFactory<LauncherPlugin> LauncherPluginFactory;
K_EXPORT_COMPONENT_FACTORY( slicker_launcherplugin, LauncherPluginFactory("slicker_launcherplugin") );

////////////////////////////////////////////////////////////////////////////
// LauncherPlugin
////////////////////////////////////////////////////////////////////////////

LauncherPlugin::LauncherPlugin(QObject *parent, const char *name,
                               const QStringList &/*args*/)
    : Plugin(LauncherPluginFactory::instance(), parent, name)
{
    _appletDef = new LauncherAppletDef(this);
}

LauncherPlugin::~LauncherPlugin()
{
    delete _appletDef;
}

#include "launcherplugin.moc"
