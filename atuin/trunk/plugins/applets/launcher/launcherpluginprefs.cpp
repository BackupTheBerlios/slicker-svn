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
#include <kfileitem.h>
#include "launcherpluginprefs.h"

typedef KGenericFactory<LauncherPluginPreferences> LauncherPluginPreferencesFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_slicker_launcherplugin, LauncherPluginPreferencesFactory( "kcm_slicker_launcherplugin" )  )

LauncherPluginPreferences::LauncherPluginPreferences(QWidget *parent,
                                                     const char* /*name*/,
                                                     const QStringList &args)
   : PluginPrefsPage(LauncherPluginPreferencesFactory::instance(), parent, args)
{
}

LauncherPluginPreferences::~LauncherPluginPreferences()
{
}

void LauncherPluginPreferences::load()
{
}

void LauncherPluginPreferences::save()
{
    emit KCModule::changed(false);
}

#include "launcherpluginprefs.moc"
