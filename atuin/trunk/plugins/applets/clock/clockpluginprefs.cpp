/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

 
#include "clockpluginprefs.h"

typedef KGenericFactory <ClockPluginPrefs> ClockPluginPrefsFactory;
K_EXPORT_COMPONENT_FACTORY(kcm_slicker_clock, ClockPluginPrefsFactory("kcm_slicker_clock"))

ClockPluginPrefs::ClockPluginPrefs(QWidget * parent, const char * name, const QStringList &args)
	: PluginPrefsPage(ClockPluginPrefsFactory::instance(), parent, args)
{
}

ClockPluginPrefs::~ClockPluginPrefs()
{
}

void ClockPluginPrefs::load()
{
	emit KCModule::changed(false);
}

void ClockPluginPrefs::save()
{
	emit KCModule::changed(false);
}

#include "clockpluginprefs.moc"
