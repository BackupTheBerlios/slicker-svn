/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef SESSIONITEM_H
#define SESSIONITEM_H

#include <kconfig.h>
#include <qstring.h>

class SessionItemFactory;

/**
* The SessionItem class should be used for storing/restoring portions of
* Slicker between sessions. The SessionManager calls restore on application
* start and store on application exit.
* @author Ulrik Mikaelsson
*/
class SessionItem{
public:
    SessionItem(const QString &id);
    virtual ~SessionItem();
    
    virtual void restore(KConfigBase * config) = 0;
    virtual void store(KConfigBase * config) = 0;
    
    const QString & id() const;
    SessionItemFactory * factory() const;
    QString factoryName() const;
    static QString factoryName(const QString & id);
    
private:
    QString _id;
    SessionItemFactory * _factory;
};

#endif
