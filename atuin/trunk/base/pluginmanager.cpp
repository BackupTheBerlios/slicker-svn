/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "pluginmanager.h"

#include <kdebug.h>
#include <kparts/componentfactory.h>
#include <ksettings/dispatcher.h>
#include <kconfig.h>

#include "pluginprefs.h"
#include "slicker.h"

PluginManager *PluginManager::_self = NULL;

PluginManager::PluginManager()
{
    _self = this;

    KPluginInfo::List plugins = KPluginInfo::fromServices( KTrader::self()->query( QString::fromLatin1( "Slicker/Plugin" ) ));
    KPluginInfo::List::iterator pluginPtr;
    for (pluginPtr = plugins.begin(); pluginPtr != plugins.end(); ++pluginPtr) {
        KPluginInfo *plugin = *pluginPtr;
        QString configFile = Slicker::self()->dataDir() + "/plugins/" + plugin->pluginName() + ".config";
        plugin->setConfig(new KConfig(configFile, false, false, "data"), "default");
        _plugins.append(plugin);
    }
    
    kdDebug(0) << "Found " << _plugins.size() << " plugins." << endl;

    resyncPlugins();
    KSettings::Dispatcher::self()->registerInstance( KGlobal::instance(), this, SLOT(resyncPlugins()));

    _pluginPrefs = new PluginPrefs();
}

PluginManager::~PluginManager()
{
    _self = NULL;
}

PluginManager *PluginManager::self()
{
    if (!_self)
        _self = new PluginManager();

    return _self;
}

Plugin *PluginManager::loadPlugin( const QString &pluginId )
{
    KPluginInfo *info = infoForPluginId( pluginId );
    if ( !info ) {
        kdWarning( 0 ) << k_funcinfo << "Unable to find a plugin named '" << pluginId << "'!" << endl;
        return 0L;
    }

    if ( _loadedPlugins.contains( info ) )
        return _loadedPlugins[ info ];

    int error = 0;
    Plugin *plugin = KParts::ComponentFactory::createInstanceFromService<Plugin>( info->service(), this, 0, QStringList(), &error );

    if ( plugin ) {
        _loadedPlugins.insert( info, plugin );
        info->setPluginEnabled( true );
        plugin->_info = info;

        emit pluginLoaded( plugin );
    } else {
        switch( error ) {
        case KParts::ComponentFactory::ErrNoServiceFound:
            kdDebug( 0 ) << k_funcinfo << "No service implementing the given mimetype "
            << "and fullfilling the given constraint expression can be found." << endl;
            break;

        case KParts::ComponentFactory::ErrServiceProvidesNoLibrary:
            kdDebug( 0 ) << "the specified service provides no shared library." << endl;
            break;

        case KParts::ComponentFactory::ErrNoLibrary:
            kdDebug( 0 ) << "the specified library could not be loaded." << endl;
            break;

        case KParts::ComponentFactory::ErrNoFactory:
            kdDebug( 0 ) << "the library does not export a factory for creating components." << endl;
            break;

        case KParts::ComponentFactory::ErrNoComponent:
            kdDebug( 0 ) << "the factory does not support creating components of the specified type." << endl;
            break;
        }

        kdDebug( 0 ) << k_funcinfo << "Loading plugin '" << pluginId << "' failed, KLibLoader reported error: '" << endl <<
        KLibLoader::self()->lastErrorMessage() << "'" << endl;
    }

    return plugin;
}

void PluginManager::unloadPlugin(const QString &pluginId)
{
    KPluginInfo * info = infoForPluginId( pluginId );
    if (!info)
        return;
    Plugin * plugin = _loadedPlugins[info];
    if (!plugin)
        return;
    emit pluginUnloaded(plugin);
    delete plugin;
    info->setPluginEnabled(false);    
    _loadedPlugins.remove(info);
}

KPluginInfo::List PluginManager::availablePlugins()
{
    return _plugins;
}

KPluginInfo * PluginManager::infoForPluginId( const QString &pluginId ) const
{
    QValueList<KPluginInfo *>::ConstIterator it;
    for ( it = _plugins.begin(); it != _plugins.end(); ++it ) {
        if ( ( *it )->pluginName() == pluginId )
            return *it;
    }

    return 0L;
}

PluginPrefs * PluginManager::pluginPrefs() const
{
    return _pluginPrefs;
}

void PluginManager::showPluginPrefs()
{
    _pluginPrefs->show();
}

Plugin * PluginManager::isPluginLoaded(const QString &pluginId)
{
    KPluginInfo * info = infoForPluginId(pluginId);
    if (!info || !_loadedPlugins.contains(info))
        return NULL;
    return _loadedPlugins[info];
}

void PluginManager::resyncPlugins()
{
    KConfig *config = KGlobal::config();
    QMap<QString, QString> entries = config->entryMap( QString::fromLatin1( "Plugins" ) );
    QMap<QString, QString>::Iterator it;
    for ( it = entries.begin(); it != entries.end(); ++it ) {
        QString key = it.key();
        if ( key.endsWith( QString::fromLatin1( "Enabled" ) ) ) {
            key.setLength( key.length() - 7 );

            if ( it.data() == QString::fromLatin1( "true" ) ) {
                if ( !isPluginLoaded( key ) )
                    loadPlugin( key );
            } else {
                if ( isPluginLoaded( key ) )
                    unloadPlugin( key );
            }
        }
    }
}

#include "pluginmanager.moc"

