/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@slicker.org>

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
	_timer->stop();
}

void Clock::update()
{
	_datetime = QDateTime::currentDateTime();
	displayTime();
}

void Clock::displayTime()
{
}

void Clock::setShowSecs(bool showSecs)
{
	_showSecs = showSecs;
}

void Clock::setShowDate(bool showDate)
{
	_showDate = showDate;
}

/**** PlainClock *************************/

PlainClock::PlainClock(QWidget * parent, const char * name)
	: Clock(parent, name)
{
	_showSecs=false;
	_grid = new QGridLayout(this, 2,1);
	_timelabel = new QLabel(this);
	_grid->addWidget(_timelabel,0,0);
	
	_datelabel = new QLabel(this);
	_grid->addWidget(_datelabel,1,0);
	
}

PlainClock::~PlainClock()
{
}

void PlainClock::displayTime()
{
	QString timeStr = KGlobal::locale()->formatTime(_datetime.time(), _showSecs);
	_timelabel->setText(timeStr); 
	
	if(_showDate)
	{
		QString dateStr = KGlobal::locale()->formatDate(_datetime.date(), true);
		_datelabel->setText(dateStr);
	}

}

/**** AnalogClock *************************/

AnalogClock::AnalogClock(QWidget * parent, const char * name)
	: Clock(parent, name)
{
	setMinimumSize(32,32);
	
}

AnalogClock::~AnalogClock()
{

}

void AnalogClock::displayTime()
{
	repaint(rect());
}

void AnalogClock::paintEvent(QPaintEvent *)
{
	QTime time = _datetime.time();
	QRect clockRect = rect();
	
	QPixmap * pix = new QPixmap(clockRect.size());
	pix->fill(paletteBackgroundColor());
	
	QPainter p(pix);
	p.setBrush(black);
	
	p.translate(clockRect.right() / 2, rect().bottom() / 2);
	
	for(int i = 0; i < 360; i+=30)
	{
		p.rotate(30);
		p.drawRect(QRect(0,rect().bottom()/(-2),1,2));
	}
	
	p.rotate(90);
	
	int ang = 360 / 12 * time.hour();
	p.rotate(ang);
	p.drawRect(QRect(-1,0,clockRect.bottom()/(-4),3));
	p.rotate(-ang);
	ang = 360 / 60 * time.minute();
	p.rotate(ang);
	p.drawRect(QRect(0,0,clockRect.bottom()/(-2),2));
	p.rotate(-ang);

	if(_showSecs)
	{
		ang = 360 / 60 * time.second();
		p.rotate(ang);
		p.drawRect(QRect(0,0,clockRect.bottom()/(-2),1));
		p.rotate(-ang);
	}
	p.end();
	p.begin(this);
	p.drawPixmap(clockRect.topLeft(),*pix);
	p.end();
	
	delete(pix);
}

/**** SlickerClock *******************/

SlickerClock::SlickerClock(QWidget * parent, const char * name)
	: Clock(parent, name)
{
	_showSecs=true;
	
	_grid = new QGridLayout(this,3,3);
	
	QFont f = font();
	f.setPixelSize(11);
	_timeLabel = new QLabel("",this);
	_timeLabel->setAlignment(AlignCenter);
	_timeLabel->setFont(f);
	

	
	_secWidget = new QWidget(this);
	_secWidget->setMinimumSize(12,12);
	
	_grid->addMultiCellWidget(_timeLabel,0,0,0,1);
	_grid->addWidget(_secWidget,0,2,AlignCenter);
	
	_dayLabel = new QLabel(this);
	_dayLabel->setAlignment(AlignCenter);
	f.setPixelSize(20);
	_dayLabel->setFont(f);
		
	f.setPixelSize(9);
	_monthLabel = new QLabel(this);
	_monthLabel->setAlignment(AlignCenter);
	_monthLabel->setFont(f);
	_yearLabel = new QLabel(this);
	_yearLabel->setAlignment(AlignCenter);
	_yearLabel->setFont(f);
	
	_grid->addMultiCellWidget(_dayLabel,1,2,0,1);
	_grid->addWidget(_monthLabel,1,2);
	_grid->addWidget(_yearLabel,2,2);

}

SlickerClock::~SlickerClock()
{
}

void SlickerClock::displayTime()
{
	QString s = KGlobal::locale()->formatTime(_datetime.time(),false);
	_timeLabel->setText(s);
	
	if(_showDate)
	{
		s.setNum(_datetime.date().day());
		_dayLabel->setText(s);
		s = QDate::shortMonthName(_datetime.date().month());
		_monthLabel->setText(s);
		s.setNum(_datetime.date().year());
		_yearLabel->setText(s);
	}
	
	QRect rect = _secWidget->rect();
	
	QPixmap pix(rect.size());
	pix.fill(paletteBackgroundColor());
	
	QPainter p(&pix);
	p.setBrush(black);
	
	p.drawArc(rect,0,360*16);
	p.drawPie(rect,90*16,(-1)*16*(360 / 60 * _datetime.time().second()));
	p.end();
	
	p.begin(_secWidget);
	p.drawPixmap(rect.topLeft(),pix);
	p.end();

} 

#include "clock.moc"
