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
#ifndef LAUNCHERAPPLET_H
#define LAUNCHERAPPLET_H

#include <kurl.h>
#include <qobject.h>
#include "plugin.h"
#include "sessionapplet.h"
#include "sessionappletdef.h"

class LauncherButton;
class LauncherPlugin;
class KFileItem;

/**
 *  LauncherApplet
 */
class LauncherApplet : public QObject, public SessionApplet
{
    Q_OBJECT
public:
    LauncherApplet(SessionAppletDef * appletDef, const QString & id, LauncherPlugin *plugin);
    virtual ~LauncherApplet();
    
    virtual void store(KConfigBase * config);
    virtual void restore(KConfigBase * config);
    
    bool init();

private:
    QString newDesktopFile(const KURL& url);
        
private slots:
    void slotSettingsChanged();
    void slotButtonClicked();
    void slotShowPreferences();
    void slotRemove();
    
private:
    LauncherPlugin * _plugin;
    LauncherButton * _button;
    KFileItem      * _fileItem;
    KURL             _url;
};

/**
 *  LauncherAppletDef
 */
class LauncherAppletDef : public SessionAppletDef
{
public:
    LauncherAppletDef(LauncherPlugin* plugin);
    virtual ~LauncherAppletDef();
    
    SessionApplet* loadSessionApplet(KConfigBase* config);
    SessionApplet* createSessionApplet();

private:
    LauncherPlugin * _plugin;
};

#endif // LAUNCHERAPPLET_H
