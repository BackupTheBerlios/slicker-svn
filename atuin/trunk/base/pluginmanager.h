/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

#include <qobject.h>
#include <qstring.h>
#include <kplugininfo.h>

#include "plugin.h"

class PluginPrefs;

/**
 * PluginManager
 * Utterly responsible for making sure that the plugins gets loaded, all in a nice fashion.
 * It's also responsible for updating the PluginPrefs, and dispatch signals between PluginPrefs
 * and the plugins.
 *
 * @author Ulrik Mikaelsson
 **/
class PluginManager : public QObject
{
    Q_OBJECT
    friend class Slicker;
private:
    PluginManager();
    virtual ~PluginManager();
public:
    /**
    * Get the single running self of the PluginManager
    */
    static PluginManager *self();

    /**
    * Loads a plugin described by a given pluginId
    */
    Plugin *loadPlugin(const QString &pluginId);

    /**
    * Unloads a plugin by a given pluginId
    */
    void unloadPlugin(const QString &pluginId);

    /**
    * Finds a plugin by a specific name
    */
    KPluginInfo * infoForPluginId( const QString &pluginId ) const;

    PluginPrefs * pluginPrefs() const;

    Plugin * isPluginLoaded(const QString &pluginId);

    /**
    * Recieve a list of the plugins currently found in the plugins directories.
    */
    KPluginInfo::List availablePlugins();

public slots:
    /**
    * Bring up the PluginPrefs dialog and let the user choose plugins.
    */
    void showPluginPrefs();

    /**
    * Will inspect the KConfig and load/unload according to the settings.
    */
    void resyncPlugins();
signals:
    /**
    * Emitted as soon as a plugin is registered
    */
    void pluginRegistered(Plugin *plugin);

    /**
    * Emitted right before a plugin is unregistered
    */
    void pluginUnregistered(Plugin *plugin);

    /**
    * Emitted as soon as a plugin is successfully loaded
    */
    void pluginLoaded(Plugin *plugin);

    /**
    * Emitted as soon as a plugin is unloaded
    */
    void pluginUnloaded(Plugin *plugin);
private:
    static PluginManager * _self;
    QMap<KPluginInfo *, Plugin *> _loadedPlugins;
    KPluginInfo::List _plugins;
    PluginPrefs *_pluginPrefs;
};

#endif
