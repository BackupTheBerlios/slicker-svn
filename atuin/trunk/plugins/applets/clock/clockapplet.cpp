/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@slicker.org>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
 
#include "clockapplet.h"

#include <iostream>
using namespace std;
 
typedef KGenericFactory <ClockPlugin> ClockPluginFactory;
K_EXPORT_COMPONENT_FACTORY(slicker_clock, ClockPluginFactory("slicker_clock"))
 
/***** ClockPlugin *******************/
 
ClockPlugin::ClockPlugin(QObject * parent, const char * name, const QStringList &)
	: Plugin(ClockPluginFactory::instance(),parent,name)
{
	_appletDef = new ClockAppletDef(this);
}

ClockPlugin::~ClockPlugin()
{
	delete(_appletDef);
}

/***** ClockApplet *******************/

ClockApplet::ClockApplet(SessionAppletDef * appletDef, const QString &id, ClockPlugin * plugin)
	: SessionApplet(appletDef, id)
{
	_plugin = plugin;
	_vbox = new QVBox();  
	_icon = _vbox; 		//Hack so that we can recreate the clock if the config is changed 
	loadSettings();
	

	connect(_plugin, SIGNAL(settingsChanged()), this, SLOT(slotSettingsChanged()));
	
}

ClockApplet::~ClockApplet()
{
}

void ClockApplet::slotSettingsChanged()
{
	if(_clock != NULL) delete(_clock);
	loadSettings();

}

void ClockApplet::loadSettings()
{
	QString s = _plugin->config()->readEntry("Style","Slicker");
	if(s == "Plain")
		_clock = new PlainClock(_vbox);
	else if(s == "Analog")
		_clock = new AnalogClock(_vbox);
	else if(s == "Slicker")
		_clock = new SlickerClock(_vbox);
			
	s = _plugin->config()->readEntry("showSecs","true");
	if(s == "true")
		_clock->setShowSecs(true);
	else
		_clock->setShowSecs(false);
		
	s = _plugin->config()->readEntry("showDate","true");
	if(s == "true")
		_clock->setShowDate(true);
	else
		_clock->setShowDate(false);
	
	_clock->show();
}
 
/***** ClockAppletDef *******************/ 

ClockAppletDef::ClockAppletDef(ClockPlugin * plugin)
	: SessionAppletDef("Clock","ClockApplet","ClockApplet","Manuel Uphoff")
{
	_plugin = plugin;
}

ClockAppletDef::~ClockAppletDef()
{
}

SessionApplet * ClockAppletDef::createSessionApplet()
{
	return new ClockApplet(this, generateItemId(), _plugin);
}

SessionApplet * ClockAppletDef::loadSessionApplet(KConfigBase * config)
{
	SessionApplet * retVal = new ClockApplet(this, config->group(), _plugin);
	retVal->restore(config);
	return retVal;
};
	

#include "clockapplet.moc"
