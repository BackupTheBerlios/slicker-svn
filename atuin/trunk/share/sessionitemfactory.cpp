/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

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

