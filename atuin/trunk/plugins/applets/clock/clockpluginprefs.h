/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef CLOCKPLUGINPREFS_H
#define CLOCKPLUGINPREFS_H

#include <kcmodule.h>
#include <kgenericfactory.h>
#include "pluginprefspage.h"

class ClockPluginPrefs : public PluginPrefsPage
{
   Q_OBJECT
public:

    ClockPluginPrefs(QWidget *parent = 0, const char* name = 0,
                              const QStringList &args = QStringList());
    ~ClockPluginPrefs();

    virtual void save();
    virtual void load();

};


#endif  
