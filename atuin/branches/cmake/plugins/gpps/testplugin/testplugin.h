/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "plugin.h"
#include "sessionapplet.h"
#include "sessionappletdef.h"

class TestAppletDef;
class QLabel;

class TestPlugin : public Plugin
{
    Q_OBJECT
public:
    TestPlugin(QObject *parent, const char *name, const QStringList &args);
    virtual ~TestPlugin();
private:
    TestAppletDef * _appletDef;
};

class TestApplet : public QObject, public SessionApplet
{
    Q_OBJECT
public:
    TestApplet(SessionAppletDef * appletDef, const QString & id, TestPlugin * testPlugin);
    virtual ~TestApplet();
private slots:
    void slotSettingsChanged();
private:
    TestPlugin * _plugin;
    QLabel * _label;
};

class TestAppletDef : public SessionAppletDef
{
public:
    TestAppletDef(TestPlugin * plugin);
    virtual ~TestAppletDef();
    
    SessionApplet * loadSessionApplet(KConfigBase * config);
    SessionApplet * createSessionApplet();
private:
    TestPlugin * _plugin;
};

#endif

