/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
 
#include "clockapplet.h"
 
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
	_clock = new PlainClock(0,"PlainClock");
	_icon = _clock;
	
	_plugin = plugin;
	connect(_plugin, SIGNAL(settingsChanged()), this, SLOT(slotSettingsChanged()));
	
}

ClockApplet::~ClockApplet()
{

}

void ClockApplet::slotSettingsChanged()
{

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
