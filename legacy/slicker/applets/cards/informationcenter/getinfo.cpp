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

/*
 * I seperate this funktion out of the main file because they can be
 * made in other forms on other systems.
 */

#include <config.h>
#include "getinfo.h"

#include <unistd.h>
#include <stdlib.h>

#include <kapplication.h>

QString GetInfo::_host = "";
QString GetInfo::_domain = "";
QString GetInfo::_user = "";
QString GetInfo::_kde = "";

void GetInfo::init()
{
    // Get the hostname
    char buf[128];
    gethostname(buf, 128);
    QString hostname(buf);
    sethost(hostname);
    
    // Get the domainname
    getdomainname(buf, 128);
    QString domainname(buf);
    setdomain(domainname);
    
    // Get the name of the logged in user
    char *user = getlogin();
    if(user == 0)
	user = getenv("LOGNAME");
    QString username(user);
    setuser(username);
    
    // Get KDE Version
    setkde(KDE::versionString());
}
