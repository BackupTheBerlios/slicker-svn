/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef SESSIONITEMFACTORY_H
#define SESSIONITEMFACTORY_H

#include <kconfig.h>
#include <qstring.h>

class SessionItem;

/**
* This class, as the name implies, is responsible for creating new instances
* of SessionData. This is required in order for the SessionManager to know
* how to instantiate SessionData-objects of a certain type.
* @author Ulrik Mikaelsson
*/
class SessionItemFactory
{
public:
    SessionItemFactory(const QString &id);
    virtual ~SessionItemFactory();
    
    /**
    * The type of SessionItem. May be things like "slider" etc.
    */
    const QString & id();

    virtual SessionItem * loadItem(KConfigBase * configGroup) = 0;
    virtual SessionItem * createItem() = 0;

protected:
    QString generateItemId() const;
private:
    QString _id;
};

#endif
