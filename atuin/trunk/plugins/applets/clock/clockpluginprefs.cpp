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

#include <qfontdialog.h>

typedef KGenericFactory <ClockPluginPrefs> ClockPluginPrefsFactory;
K_EXPORT_COMPONENT_FACTORY(kcm_slicker_clock, ClockPluginPrefsFactory("kcm_slicker_clock"))

ClockPluginPrefs::ClockPluginPrefs(QWidget * parent, const char *, const QStringList &args)
	: PluginPrefsPage(ClockPluginPrefsFactory::instance(), parent, args)
{
	_vbox = new QVBoxLayout(this);
	_vbox->setSpacing(10);
	_hstylebuttongroup = new QHButtonGroup("Style:",this);
	_hstylebuttongroup->setInsideSpacing(10);
	_hstylebuttongroup->setExclusive(true);
	_vbox->addWidget(_hstylebuttongroup);
	_radioPlain = new QRadioButton(i18n("Plain"),_hstylebuttongroup);
	_radioAnalog = new QRadioButton(i18n("Analog"),_hstylebuttongroup);
	_radioSlicker = new QRadioButton(i18n("Slicker"),_hstylebuttongroup);
	
	_showgroupbox = new QVGroupBox("Show:",this);
	_showgroupbox->setInsideSpacing(10);
	_vbox->addWidget(_showgroupbox);
	_showbox = new QHBox(_showgroupbox);
	_showbox->setSpacing(10);
	_checkShowSecs = new QCheckBox(i18n("Show Seconds"),_showbox);
	_checkShowDate = new QCheckBox(i18n("Show Date"),_showbox);
	_refreshbox = new QHBox(_showgroupbox);
	_refreshbox->setSpacing(5);
	_refreshlabel = new QLabel(i18n("Refresh clock every"),_refreshbox);
	_refresh = new QLineEdit("1000",_refreshbox);
	_refresh->setInputMask("D000");
	_refreshsecondslabel = new QLabel(i18n("seconds"),_refreshbox);
	
	_timefontbox = new QVGroupBox(i18n("Time:"),this);
	_vbox->addWidget(_timefontbox);
	_timefont = new QLabel(_timefontbox);
	_changetimefont = new QPushButton(i18n("Change"),_timefontbox);
	
	_datefontbox = new QVGroupBox(i18n("Date:"),this);
	_vbox->addWidget(_datefontbox);
	_datefont = new QLabel(_datefontbox);
	_changedatefont = new QPushButton(i18n("Change"),_datefontbox);
	
	_slickerfontsizebox = new QHGroupBox(i18n("Font sizes for Slicker style"), _datefontbox);
	_slickerdaylabel = new QLabel(i18n("Day:"),_slickerfontsizebox);
	_slickerday = new QLineEdit(_slickerfontsizebox);
	_slickerday->setInputMask("D00");
	_slickermonthlabel = new QLabel(i18n("Month/Year:"),_slickerfontsizebox);
	_slickermonth = new QLineEdit(_slickerfontsizebox); 
	_slickermonth->setInputMask("D00");
	
	
	connect(_radioPlain, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_radioAnalog, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_radioSlicker, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	
	connect(_checkShowSecs, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_checkShowDate, SIGNAL(toggled(bool)), this, SLOT(slotConfigChanged()));
	connect(_refresh, SIGNAL(textChanged(const QString&)), this, SLOT(slotConfigChanged()));
	
	connect(_changetimefont, SIGNAL(clicked()), this, SLOT(slotChangeTimeFont()));
	connect(_changedatefont, SIGNAL(clicked()), this, SLOT(slotChangeDateFont()));
	
	connect(_slickerday, SIGNAL(textChanged(const QString&)), this, SLOT(slotConfigChanged()));
	connect(_slickermonth, SIGNAL(textChanged(const QString&)), this, SLOT(slotConfigChanged()));
	
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
		
	_timefont->setFont(loadFont("Time"));
	updateFontLabel(_timefont);
	_datefont->setFont(loadFont("Date"));
	updateFontLabel(_datefont);
	
	_slickerday->setText(config()->readEntry("SlickerDaySize","20"));
	_slickermonth->setText(config()->readEntry("SlickerMonthSize","8"));
	
	_refresh->setText(config()->readEntry("Refresh","1"));
	
	
	
	emit KCModule::changed(false);
}

QFont ClockPluginPrefs::loadFont(QString prefix)
{
	prefix+="Font_";
	QFont font;
	QString s;
	
	s=config()->readEntry(prefix+"family","default");
	if(s != "default")
		font.setFamily(s);
	else
		font.setFamily(font.defaultFamily());
		
	s=config()->readEntry(prefix+"size","12");
	font.setPointSize(s.toInt());
	
	s=config()->readEntry(prefix+"bold","false");
	if(s == "true")
		font.setBold(true);
	else
		font.setBold(false);
		
	s=config()->readEntry(prefix+"italic","false");
	if(s == "true")
		font.setItalic(true);
	else
		font.setItalic(false);
		
	return font;
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
		
	saveFont(_timefont->font(),"Time");
	saveFont(_datefont->font(),"Date");
	
	config()->writeEntry("SlickerDaySize", _slickerday->text());
	config()->writeEntry("SlickerMonthSize",_slickermonth->text());
	
	config()->writeEntry("Refresh",_refresh->text());
		
	emit KCModule::changed(false);
}

void ClockPluginPrefs::saveFont(QFont font, QString prefix)
{
	QString s;
	prefix+="Font_";
	config()->writeEntry(prefix+"family",font.family());
	s.setNum(font.pointSize());
	config()->writeEntry(prefix+"size",s);
	
	if(font.bold())
		config()->writeEntry(prefix+"bold","true");
	else
		config()->writeEntry(prefix+"bold","false");
		
	if(font.italic())
		config()->writeEntry(prefix+"italic","true");
	else
		config()->writeEntry(prefix+"italic","false");
		
}

void ClockPluginPrefs::slotConfigChanged()
{
	emit KCModule::changed(true);
}

void ClockPluginPrefs::updateFontLabel(QLabel * label)
{
	QString name = label->font().family();
	QString size;
	size.setNum(label->font().pointSize());
	label->setText(name+" "+size);
}

void ClockPluginPrefs::setNewFont(QLabel * label)
{
	bool ok;
	label->setFont(QFontDialog::getFont(&ok,label->font(),label));
	if(ok)
	{
		updateFontLabel(label);
		emit KCModule::changed(true);
	}
}

void ClockPluginPrefs::slotChangeTimeFont()
{
	setNewFont(_timefont);
}

void ClockPluginPrefs::slotChangeDateFont()
{
	setNewFont(_datefont);
}


#include "clockpluginprefs.moc"
