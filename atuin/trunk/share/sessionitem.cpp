//
// C++ Implementation: sessionitem
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
