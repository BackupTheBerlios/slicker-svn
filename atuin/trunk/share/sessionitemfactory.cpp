//
// C++ Implementation: sessionitemfactory
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sessionitemfactory.h"

#include "sessionmanager.h"

SessionItemFactory::SessionItemFactory(const QString &id)
{
    _id = id;
    SessionManager::self()->registerFactory(this);
}

SessionItemFactory::~SessionItemFactory()
{
    SessionManager::self()->unregisterFactory(this);
}

const QString & SessionItemFactory::id()
{
    return _id;
}

QString SessionItemFactory::generateItemId() const
{
    SessionManager * sm = SessionManager::self();
    int idNo = 1;
    QString itemId = _id + "-%1";
    
    while (sm->getItem(itemId.arg(idNo)))
        idNo++;
        
    return itemId.arg(idNo);
}

