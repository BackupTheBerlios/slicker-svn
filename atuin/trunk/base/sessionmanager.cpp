/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "sessionmanager.h"

#include <kdebug.h>
#include <kconfig.h>

#include "slicker.h"

SessionManager * SessionManager::_self = 0L;

SessionManager::SessionManager()
{
    _defaultConfig = new KConfig(Slicker::self()->dataDir() + "/default.session", false, false, "data");
    _currentConfig = 0L;
}

SessionManager * SessionManager::self()
{
    if (!_self)
        _self = new SessionManager();
    
    return _self;
}

SessionManager::~SessionManager()
{
    store();
    delete _defaultConfig;
    _self = 0L;
}

void SessionManager::registerFactory(SessionItemFactory * factory)
{
    _itemFactories.insert(factory->id(), factory);
}

void SessionManager::unregisterFactory(SessionItemFactory * factory)
{
    _itemFactories.remove(factory->id());
}

void SessionManager::registerItem(SessionItem * item)
{
    _items.insert(item->id(), item);
}

void SessionManager::unregisterItem(SessionItem * item)
{
    _items.remove(item->id());
	if (_defaultConfig)
		_defaultConfig->deleteGroup( item->id() );
}

SessionItem * SessionManager::getItem(const QString &id) const
{
    SessionItem * item = _items[id];
    
    if (!item && _currentConfig)
        item = loadItem(id);
    
    return item;
}

SessionItemFactory * SessionManager::getFactory(const QString &id) const
{
    return _itemFactories[id];
}

void SessionManager::store()
{
    store(_defaultConfig);
}

void SessionManager::restore()
{
    restore(_defaultConfig);
}

void SessionManager::store(KConfig * config)
{
    kdDebug() << "Storing session data" << endl;
    QDictIterator<SessionItem> iter(_items);
    
    for ( ; iter.current(); ++iter)
    {
        config->setGroup(iter.currentKey());
        iter.current()->store(config);
    }
    config->sync();
}

void SessionManager::restore(KConfig * config)
{
    QStringList items = config->groupList();
    QStringList::iterator iter;
    
    _currentConfig = config;
    
    for ( iter = items.begin(); iter != items.end(); ++iter )
    {
        loadItem(*iter);
    }
    _currentConfig = 0L;
}

SessionItem * SessionManager::loadItem(const QString & id) const
{
    if (_currentConfig && !_items[id])
    {
        KConfigGroup config(_currentConfig, id);
    
        SessionItemFactory * factory = _itemFactories[SessionItem::factoryName(id)];
        return factory->loadItem(&config);
    }
    return 0L;
}

#include "sessionmanager.moc"
