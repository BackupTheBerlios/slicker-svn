/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@rsn.bth.se> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
#ifndef CARDSTACK_H
#define CARDSTACK_H

#include <qlabel.h>

#include "applet.h"
#include "applethost.h"
#include "edgewidget.h"
#include "edgewidgethelpers.h"
#include "sessionitem.h"
#include "sessionitemfactory.h"

class CardTab : EdgeWidgetLayoutBox
{
Q_OBJECT
public:
	CardTab(Applet * applet, QWidget * parent);
	virtual ~CardTab();
	
	Applet * applet() const;
	void setApplet(Applet * applet);
private:
	Applet * _applet;
	QWidget * _icon;
	QLabel * _label;
};

/**
* The basic CardStack class
* @author Ulrik Mikaelsson
*/
class CardStack : public EdgeWidget, public AppletHost, public SessionItem
{
Q_OBJECT
public:
	CardStack(const QString & id);
	virtual ~CardStack();
	
	bool accept(Applet * applet);
	void detach(Applet * applet);
	    
	virtual void restore(KConfigBase * config);
	virtual void store(KConfigBase * config);
	
protected slots:
	virtual void edgeChanged(EdgeWidget::ScreenEdge oldEdge);
private:
	EdgeWidgetBoxLayout * _layout;
};

/**
* The SessionItemFactory for the CardStack.
* @author Ulrik Mikaelsson <rawler@rsn.bth.se>
*/
class CardStackFactory : public SessionItemFactory
{
public:
	CardStackFactory();
	virtual ~CardStackFactory();
	
	virtual SessionItem * loadItem(KConfigBase * configGroup);
	virtual SessionItem * createItem();
};

#endif
