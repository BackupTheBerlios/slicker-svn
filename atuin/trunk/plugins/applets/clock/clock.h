/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
 
#ifndef CLOCK_H
#define CLOCK_H

#include <qwidget.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qlabel.h>
#include <klocale.h>
#include <kglobal.h>

/**
 * Class Clock: Base class for all clocks
 */
 
class Clock : public QWidget
{
	Q_OBJECT
public:
	Clock(QWidget * parent = 0, const char * name = 0);
	~Clock();
	
public slots:
	
	//function to update the displayed time
	void update();
	//empty virtual function to display the updated time, hast to be overwritten by inherited classes 
	virtual void displayTime();
	
protected:
	QDateTime _datetime;

private:
	QTimer * _timer;
	

};

/**
 * Class PlainClock: Plain Clock with a Label showing the time and a label showing the date
 */
 
class PlainClock : public Clock
{
	Q_OBJECT
public:
	PlainClock(QWidget * parent = 0, const char * name = 0);
	~PlainClock();
private:
	void displayTime();
	bool _showSecs;
	QLabel * _timelabel;
	
	
};

#endif
