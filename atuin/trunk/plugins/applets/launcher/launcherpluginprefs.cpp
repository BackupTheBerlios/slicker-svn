/***************************************************************************
 *   Copyright (C) 2004 by Markus Breitenberger                            *
 *   bre@slicker.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
