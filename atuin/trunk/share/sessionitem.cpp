/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "sessionitem.h"
#include "sessionmanager.h"

SessionItem::SessionItem(const QString &id)
{
    _id = id;
    _factory = SessionManager::self()->getFactory(factoryName());
    SessionManager::self()->registerItem(this);
}

SessionItem::~SessionItem()
{
    SessionManager::self()->unregisterItem(this);
}
   
const QString & SessionItem::id() const
{
    return _id;
}

QString SessionItem::factoryName() const
{
    return factoryName(_id);
}

QString SessionItem::factoryName(const QString & id)
{
    return id.section('-', 0, -2);
}

SessionItemFactory * SessionItem::factory() const
{
    return _factory;
}
