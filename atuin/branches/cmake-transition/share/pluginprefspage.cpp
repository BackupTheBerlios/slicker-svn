/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "pluginprefspage.h"

#include <ktrader.h>
#include <kdebug.h>
#include "pluginmanager.h"

PluginPrefsPage::PluginPrefsPage(KInstance *instance, QWidget *parent, const QStringList &args) : KCModule(instance, parent, args)
{
    // UGLY! This shit should not be allowed, but it is.
    QString pluginId = instance->instanceName();
    pluginId.remove("kcm_");
    _info = PluginManager::self()->infoForPluginId(pluginId);
}

PluginPrefsPage::~PluginPrefsPage()
{
}

KPluginInfo * PluginPrefsPage::info()
{
    return _info;
}

KConfig * PluginPrefsPage::config()
{
    return _info->config();    
}

#include "pluginprefspage.moc"
