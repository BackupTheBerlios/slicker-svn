/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "plugin.h"

#include <ksettings/dispatcher.h>

Plugin::Plugin(KInstance *instance, QObject *parent, const char *name)
        : QObject(parent, name)
{

	KSettings::Dispatcher::self()->registerInstance( instance, this, SIGNAL( settingsChanged() ) );
        connect(this, SIGNAL(settingsChanged()), this, SLOT(saveConfig()));	
}

Plugin::~Plugin()
{}

KConfig * Plugin::config()
{
    return _info->config();    
}

KPluginInfo * Plugin::info()
{
    return _info;
}

void Plugin::saveConfig()
{
    _info->config()->sync();
}

#include "plugin.moc"
