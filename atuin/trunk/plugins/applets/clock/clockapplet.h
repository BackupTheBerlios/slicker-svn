/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@slicker.org>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef CLOCKAPPLET_H
#define CLOCKAPPLET_H

#include <kconfigbase.h>
#include <kgenericfactory.h>
#include "plugin.h"
#include "sessionapplet.h"
#include "sessionappletdef.h"

#include "clock.h" 

#include <qvbox.h>

class ClockAppletDef;

class ClockPlugin : public Plugin
{
	Q_OBJECT
public:
	ClockPlugin(QObject * parent, const char * name, const QStringList &args);
	virtual ~ClockPlugin();
private:
	ClockAppletDef * _appletDef;
};

class ClockApplet : public QObject, public SessionApplet
{
	Q_OBJECT
public:
	ClockApplet(SessionAppletDef * appletDef, const QString & id, ClockPlugin * clockPlugin);
	virtual ~ClockApplet();
	void loadSettings();
private slots:
	void slotSettingsChanged();
private:
	ClockPlugin * _plugin;
	Clock * _clock;
	QVBox * _vbox;
};

class ClockAppletDef : public SessionAppletDef
{
public:
	ClockAppletDef(ClockPlugin * plugin);
	virtual ~ClockAppletDef();
	
	SessionApplet * createSessionApplet();
	SessionApplet * loadSessionApplet(KConfigBase * config);
	
private:
	ClockPlugin * _plugin;
}; 

#endif
