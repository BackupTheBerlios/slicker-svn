/***************************************************************************
    copyright (C) 2003      Sven Leiber <s.leiber@web.de>
                  2000-2001 Matthias Ettrich <ettrich@kde.org>
                  2000-2001 Matthias Elter   <elter@kde.org>
                  2001      Carsten Pfeiffer <pfeiffer@kde.org>
                  2001      Martijn Klingens <mklingens@yahoo.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "systemtraywidget.h"

#include <kwinmodule.h>
#include <kdebug.h>
#include <karrowbutton.h>

#include <qstringlist.h>

#include <X11/Xlib.h>

SystemtrayWidget::SystemtrayWidget(Tray *tray, const char *name, int breadth, int length, ConfigureImpl *config, Card *c)
	: TrayWidget(tray, name, breadth, length)
{
	position = 0;
	arrow = false;
	_config = config;
	_c = c;

	rightarrow = new KArrowButton(this, Qt::RightArrow);
	leftarrow = new KArrowButton(this, Qt::LeftArrow);
	rightarrow->setFlat(true);
	rightarrow->setAutoMask(true);
	rightarrow->hide();
	leftarrow->setFlat(true);
	leftarrow->setAutoMask(true);
	leftarrow->hide();

	connect(rightarrow, SIGNAL(clicked()), this, SLOT(rightarrowPressed()));
	connect(leftarrow, SIGNAL(clicked()), this, SLOT(leftarrowPressed()));

	m_Wins.setAutoDelete(true);
	this->setBackgroundMode(X11ParentRelative);
}

SystemtrayWidget::~SystemtrayWidget()
{
	m_Wins.clear();
}

QPtrList<QXEmbed> *SystemtrayWidget::getm_wins()
{
	return &m_Wins;
}

void SystemtrayWidget::layoutTray()
{
	if (m_Wins.count() == 0)
	{
		setDimensions(0, 0);
		return;
	}

	int i = 0, a = 0;

	QXEmbed* emb;
	int x = 0;
	int count = 0;

// Make this later!   :)
// We can make it so, that the systemtray have everytime the smalest
// size and we can configurate the bigest size.
// At the moment we use that size as permanent size.   :)
//	if((m_Wins.count() * 24) > this->height() &&
//		 (m_Wins.count() * 24) <= (this->height() * 2)  &&
//		  _config->getMaxtraylines() >= 2)
//	{
//		_parent->setDimensions(_c->breadth(), 48);
//		kdDebug() << "Systemtray: make 2 lines" << endl;
//	}
	int aa = length() / 24;
	kdDebug() << "Systemtray: make 2 lines" << length() << endl;
	if(aa < 1)
	{
	  /* The place is to small to display a icon we make than one line with
	     icons that we display at the top */
	  aa = 1;
	}
	/* 8 is at the moment the hardcoded max icons/line */
	if(m_Wins.count()*24 <= 24*8)
		setDimensions(m_Wins.count()*24, 24);
	else
		setDimensions(24*8, 24);
	if(((unsigned int)((breadth()/24)*aa)) >=  m_Wins.count()) /* The maxdisplayable icons are higher as the icons that must be displayed in the tray */
	{
		// If the buttons are displayed hide them
		if(arrow)
		{
			leftarrow->hide();
			rightarrow->hide();
			position = 0;
			arrow = false;
		}
	}
	else
	{
		// If the arrows are not displayed, display them
		if(!arrow)
		{
			leftarrow->show();
			rightarrow->show();
			arrow = true;
		}
		// Change the position of the buttons and change the arrows
		if(_horizontal)
		{
			rightarrow->move(breadth()-11, 0);
			rightarrow->resize(10, length());
			rightarrow->setArrowType(Qt::RightArrow);
			leftarrow->move(1, 0);
			leftarrow->resize(10, length());
			leftarrow->setArrowType(Qt::LeftArrow);
		}
		else
		{
			rightarrow->move(0, breadth()-11);
			rightarrow->resize(length(), 10);
			rightarrow->setArrowType(Qt::DownArrow);
			leftarrow->move(1, 0);
			leftarrow->resize(length(), 10);
			leftarrow->setArrowType(Qt::UpArrow);
		}
		// If we display the last icon on the right side we don't change the position of the icons.
		if(((unsigned int)(((breadth()-47/*-47 is the size of the arrowbuttons*/)/24)*aa)) >=  (m_Wins.count()-(position*aa)))
			position--;
		// It we are at the last left icon we don't change the position of the icons
		if(position < 0)
			position = 0;
	}
	for (emb = m_Wins.first(); emb != 0L; emb = m_Wins.next()) {
		if(2+i*24 >= breadth()-47 && arrow /* All not displayed icons */ || !(count >= position*aa) /* icons that are at the beginning and outside the displayed icons */)
		{
			// we must change x and y for the side of the desktop
			if(_horizontal)
				emb->move(breadth()+10, a*24); /* Set the icons that are not displayed outside the displayed area */
			else
				emb->move(a*24, breadth()+10); /* Set the icons that are not displayed outside the displayed area */
		}
		else
		{
			x = 2+i*24;
			if(arrow)
				x += 12/*+12 for the size of the left button*/;
			// we must change x and y for the side of the desktop
			if(_horizontal)
				emb->move(x, a*24);
			else
				emb->move(a*24, x);
			a++;
			if(a+1 > aa)
			{
				a = 0;
				i++;
			}
    		}
		count++;
	}
}

void SystemtrayWidget::orientationChanged( bool horizontal )
{
	if(horizontal != _horizontal)
	{
		_horizontal = horizontal;
		TrayWidget::orientationChanged(horizontal);
		layoutTray();
	}
}

void SystemtrayWidget::leftarrowPressed()
{
	position--;
	layoutTray();
}

void SystemtrayWidget::rightarrowPressed()
{
	position++;
	layoutTray();
}
