/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef PLUGIN_H
#define PLUGIN_H

#include <kconfig.h>
#include <kinstance.h>
#include <kplugininfo.h>
#include <qobject.h>

/**
* A plugin is a piece of binary code that may get loaded and executed
* dynamically in a parent application.
* 
* This should be subclassed and exported through the use of KGenericFactory<YourPluginClass>
* and K_EXPORT_COMPONENT_FACTORY.
* See the Slicker Applet Developer Guide for more info.
* @author Ulrik Mikaelsson
*/
class Plugin : public QObject
{
    Q_OBJECT
friend class PluginManager;
public:
    Plugin(KInstance *instance, QObject *parent, const char *name);
    virtual ~Plugin();

    KConfig * config();
    KPluginInfo * info();
    
public slots:
    void saveConfig();
signals:
    void settingsChanged();

private:
    KPluginInfo * _info;
};

#endif
