/***************************************************************************
 *   Copyright (C) 2004 by Markus Breitenberger                            *
 *   bre@slicker.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
