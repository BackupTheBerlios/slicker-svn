/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GETINFO_H
#define GETINFO_H

#include <qstring.h>

class GetInfo
{
public:
  static void init();

  static QString gethost() { return _host; }
  static QString getdomain() { return _domain; }
  static QString getuser() { return _user; }
  static QString getkde() { return _kde; }
  
  static void sethost(const QString &host) { _host = host; }
  static void setdomain(const QString &domain) { _domain = domain; }
  static void setuser(const QString &user) { _user = user; }
  static void setkde(const QString &kde) { _kde = kde; }
  
private:
  static QString _host;
  static QString _domain;
  static QString _user;
  static QString _kde;
};

#endif
