/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@slicker.org>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

 
#include "clockpluginprefs.h"

typedef KGenericFactory <ClockPluginPrefs> ClockPluginPrefsFactory;
K_EXPORT_COMPONENT_FACTORY(kcm_slicker_clock, ClockPluginPrefsFactory("kcm_slicker_clock"))

ClockPluginPrefs::ClockPluginPrefs(QWidget * parent, const char *, const QStringList &args)
	: PluginPrefsPage(ClockPluginPrefsFactory::instance(), parent, args)
{
	_vbox = new QVBox(this);
	_vbox->setMinimumSize(300,80);
	_vbox->setSpacing(10);
	_hbuttongroup = new QHButtonGroup("Style:",_vbox);
	_hbuttongroup->setInsideSpacing(10);
	_hbuttongroup->setExclusive(true);
	_radioPlain = new QRadioButton(i18n("Plain"),_hbuttongroup);
	_radioAnalog = new QRadioButton(i18n("Analog"),_hbuttongroup);
	_radioSlicker = new QRadioButton(i18n("Slicker"),_hbuttongroup);
	
	_hbox = new QHBox(_vbox);
	_hbox->setSpacing(10);
	_checkShowSecs = new QCheckBox(i18n("Show Seconds"),_hbox);
	_checkShowDate = new QCheckBox(i18n("Show Date"),_hbox);
	
	connect(_radioPlain, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_radioAnalog, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_radioSlicker, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	
	connect(_checkShowSecs, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_checkShowDate, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	
	load();
}

ClockPluginPrefs::~ClockPluginPrefs()
{
}

void ClockPluginPrefs::load()
{
	QString s = config()->readEntry("Style","Slicker");
	if(s == "Slicker")
		_radioSlicker->setChecked(true);
	else if(s == "Plain")
		_radioPlain->setChecked(true);
	else if(s == "Analog")
		_radioAnalog->setChecked(true);
		
	s = config()->readEntry("showSecs","true");
	
	if(s == "true")
		_checkShowSecs->setChecked(true);
	else
		_checkShowSecs->setChecked(false);
		
	s = config()->readEntry("showDate", "true");
	
	if(s == "true")
		_checkShowDate->setChecked(true);
	else
		_checkShowDate->setChecked(false);
	
	emit KCModule::changed(false);
}

void ClockPluginPrefs::save()
{
	if(_radioPlain->isChecked())
		config()->writeEntry("Style","Plain");
	else if(_radioAnalog->isChecked())
		config()->writeEntry("Style","Analog");
	else if(_radioSlicker->isChecked())
		config()->writeEntry("Style","Slicker");
		
	if(_checkShowSecs->isChecked())
		config()->writeEntry("showSecs","true");
	else
		config()->writeEntry("showSecs","false");
		
	if(_checkShowDate->isChecked())
		config()->writeEntry("showDate","true");
	else
		config()->writeEntry("showDate","false");
		
	emit KCModule::changed(false);
}

void ClockPluginPrefs::slotConfigChanged()
{
	emit KCModule::changed(true);
}

#include "clockpluginprefs.moc"
