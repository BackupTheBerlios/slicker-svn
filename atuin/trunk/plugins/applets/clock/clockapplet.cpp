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
	{
		SlickerClock * clock = new SlickerClock(_vbox);
		int daysize = _plugin->config()->readEntry("SlickerDaySize","20").toInt();
		int monthsize = _plugin->config()->readEntry("SlickerMonthSize","8").toInt();
		clock->setDateFontSizes(daysize,monthsize);
		_clock = clock;
	}
			
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
		
	_clock->setTimeFont(loadFont("Time"));
	_clock->setDateFont(loadFont("Date"));
	
	_clock->show();
}

QFont ClockApplet::loadFont(QString prefix)
{
	prefix+="Font_";
	QFont font;
	QString s;

	KConfig * config = _plugin->config();
	
	s=config->readEntry(prefix+"family","default");
	if(s != "default")
		font.setFamily(s);
	else
		font.setFamily(font.defaultFamily());
		
	s=config->readEntry(prefix+"size","12");
	font.setPointSize(s.toInt());
	
	s=config->readEntry(prefix+"bold","false");
	if(s == "true")
		font.setBold(true);
	else
		font.setBold(false);
		
	s=config->readEntry(prefix+"italic","false");
	if(s == "true")
		font.setItalic(true);
	else
		font.setItalic(false);
		
	return font;
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
