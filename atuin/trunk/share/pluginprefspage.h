/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef PLUGINCONFIGPAGE_H
#define PLUGINCONFIGPAGE_H

#include <kcmodule.h>
#include <kplugininfo.h>
#include <kconfig.h>
#include <kinstance.h>

/**
* This is a helper class to be used by all PluginPrefs modules.
* It helps keeping a reference to the appropriate KConfig for the Plugin.
* @author Ulrik Mikaelsson
*/
class PluginPrefsPage : public KCModule
{
Q_OBJECT
public:
    PluginPrefsPage(KInstance *instance, QWidget *parent=0, const QStringList &args=QStringList());
    virtual ~PluginPrefsPage();
public:
    KPluginInfo * info();
    KConfig * config();
    
private:
    KPluginInfo * _info;
};

#endif
