/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@slicker.org>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef CLOCKPLUGINPREFS_H
#define CLOCKPLUGINPREFS_H

#include <kcmodule.h>
#include <kgenericfactory.h>
#include "pluginprefspage.h"

#include <qhbuttongroup.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>

class ClockPluginPrefs : public PluginPrefsPage
{
   Q_OBJECT
public:

	ClockPluginPrefs(QWidget *parent = 0, const char* name = 0,
		const QStringList &args = QStringList());
	~ClockPluginPrefs();

	virtual void save();
	virtual void load();
	
	void updateFontLabel(QLabel * label);
	void setNewFont(QLabel * label);
	void saveFont(QFont font, QString prefix);
	QFont loadFont(QString prefix);
	
private slots:
	void slotConfigChanged();
	void slotChangeTimeFont();
	void slotChangeDateFont();
    
private:
	QHButtonGroup * _hstylebuttongroup;
	QVBoxLayout * _vbox;
	QHButtonGroup * _hshowbuttongroup;
	QRadioButton * _radioPlain;
	QRadioButton * _radioAnalog;
	QRadioButton * _radioSlicker;
	QCheckBox * _checkShowSecs;
	QCheckBox * _checkShowDate;
	QVGroupBox * _timefontbox;
	QVGroupBox * _datefontbox;
	QLabel * _timefont;
	QLabel * _datefont;
	QPushButton * _changetimefont;
	QPushButton * _changedatefont;
	
	QHGroupBox * _slickerfontsizebox;
	QLabel * _slickerdaylabel;
	QLabel * _slickermonthlabel;
	QLineEdit * _slickerday;
	QLineEdit * _slickermonth;
	
};


#endif  
