/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef LAUNCHERPLUGIN_H
#define LAUNCHERPLUGIN_H

#include "plugin.h"

class LauncherAppletDef;

/**
 * LauncherPlugin
 */
class LauncherPlugin : public Plugin
{
    Q_OBJECT
public:
    LauncherPlugin(QObject *parent, const char *name, const QStringList &args);
    virtual ~LauncherPlugin();
    
private:
    LauncherAppletDef * _appletDef;
};

#endif  // LAUNCHERPLUGIN_H
