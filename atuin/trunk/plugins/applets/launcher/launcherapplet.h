/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
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
class LauncherApplet : public SessionApplet
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
