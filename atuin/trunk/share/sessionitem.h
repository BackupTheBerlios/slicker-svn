//
// C++ Interface: sessionitem
//
// Description: 
//
//
// Author: Ulrik Mikaelsson <rawler@rsn.bth.se>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
