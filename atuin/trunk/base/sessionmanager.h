/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <qobject.h>
#include <qdict.h>

#include "sessionitem.h"
#include "sessionitemfactory.h"

/**
* The SessionManager is what keeps a session going.
* It's main responsibility is to store and restore a session
* on Slicker exit and startup.
* The sessionManager gets bound to a certain sessiondata-file
* and tries to load all the items found in the sessiondata file.
* SessionManager is also capable of on-demand loading of sessionitems,
* that means, that when some class requests a certain item, the
* sessionmanager may search through the sessiondata-file for not-yet-loaded
* sessionitems.
* 
* Slickers concept of sessiondata builds on the Factory-pattern, using
* factories to create small pieces of data, called a "SessionItem". These
* items are given a unique identifier, and can be stored/restored from/to a file.
*
* @author Ulrik Mikaelsson
*/
class SessionManager : public QObject{
    Q_OBJECT
private:
    SessionManager();
    static SessionManager * _self;
public:
    static SessionManager * self();
    ~SessionManager();

    void registerFactory(SessionItemFactory * factory);
    void unregisterFactory(SessionItemFactory * factory);
    
    void registerItem(SessionItem * item);
    void unregisterItem(SessionItem * item);
    
    SessionItem * getItem(const QString &id) const;
    SessionItemFactory * getFactory(const QString &id) const;
    
    SessionItem * loadItem(const QString &id) const;
    
public slots:
    void store(KConfig * config);
    void restore(KConfig * config);
    void store();
    void restore();

private:
    QDict<SessionItem> _items;
    QDict<SessionItemFactory> _itemFactories;
    KConfig * _defaultConfig;
    KConfig * _currentConfig;
};

#endif
