/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
 
#include "clock.h"

/**** Clock ****************************/

Clock::Clock(QWidget * parent, const char * name)
	: QWidget(parent, name)
{	
	update();
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer->start(500);
}

Clock::~Clock()
{
}

void Clock::update()
{
	_datetime = QDateTime::currentDateTime();
	displayTime();
}

void Clock::displayTime()
{
}

/**** PlainClock *************************/

PlainClock::PlainClock(QWidget * parent, const char * name)
	: Clock(parent, name)
{
	setMinimumSize(32,32);
	_showSecs=false;
	_timelabel = new QLabel("",this);
	
}

PlainClock::~PlainClock()
{
}

void PlainClock::displayTime()
{
	QString timeStr = KGlobal::locale()->formatTime(_datetime.time(), _showSecs);
	_timelabel->setText(timeStr); 

}

#include "clock.moc"
 