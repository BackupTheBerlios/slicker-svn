/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef LAUNCHERPLUGINPREFS_H
#define LAUNCHERPLUGINPREFS_H

#include <kcmodule.h>
#include "pluginprefspage.h"

class QLineEdit;

/**
  *@author Markus Breitenberger
  */
class LauncherPluginPreferences : public PluginPrefsPage
{
   Q_OBJECT
public:

    LauncherPluginPreferences(QWidget *parent = 0, const char* name = 0,
                              const QStringList &args = QStringList());
    ~LauncherPluginPreferences();

    virtual void save();
    virtual void load();

};


#endif  // LAUNCHERPLUGINPREFS_H

