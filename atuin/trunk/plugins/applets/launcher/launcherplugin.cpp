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
