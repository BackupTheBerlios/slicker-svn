/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BASEINFOWIDGET_H
#define BASEINFOWIDGET_H

#include <qwidget.h>
#include <qstring.h>
#include <qlayout.h>
#include <qframe.h>
#include <kparts/part.h>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class ConfigDialogImpl;

class BaseinfoWidget : public QWidget
{
	Q_OBJECT

public:
	BaseinfoWidget(QWidget* parent, const char* name=0, ConfigDialogImpl *config=0L);
	~BaseinfoWidget();

protected:
	QHBoxLayout *baselayout;
	QVBoxLayout *namelayout;
	QVBoxLayout *infolayout;

	QLabel *hostlabel;
	QLabel *host;
	QLabel *userlabel;
	QLabel *user;
	QLabel *kdelabel;
	QLabel *kde;

	ConfigDialogImpl *_config;

protected slots:
  void update();
};

#endif
