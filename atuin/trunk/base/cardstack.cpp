/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@rsn.bth.se> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
#include "cardstack.h"
#include <qptrlist.h>

/*** CardStack *********************/

CardStack::CardStack(const QString & id) : EdgeWidget(NULL, "CardStack"), AppletHost("CardStack"), SessionItem(id)
{
	_layout = new EdgeWidgetBoxLayout(this, this, EdgeWidgetBoxLayout::Perpendicular, "CardLayout");
	_layout->setAutoAdd(true);
    _layout->setResizeMode( QLayout::Fixed );
	new CardTab(0L, this);
	show();
}

CardStack::~CardStack()
{
}

bool CardStack::accept(Applet * applet)
{
	//TODO: This should be rewritten to provide some more intelligent filtering of applets
	QWidget *icon = applet->icon();
	if (icon == 0)
		return false;
	
	if ((icon->minimumWidth() > 16) || (icon->minimumHeight() > 16))
		return false;
	
	if ((icon->maximumWidth() < 32) || (icon->maximumHeight() < 32))
		return false;
	
	return false;
    
/*
	icon->reparent(_content, QPoint(0,0), true);
	_content->updateGeometry();
	icon->resize(32, 32);
	
	registerApplet(applet);
	return true;
*/
}

void CardStack::detach(Applet * applet)
{
	if (applet->icon())
		applet->icon()->reparent(NULL, QPoint(0,0), false);
	
	deregisterApplet(applet);
	updateGeometry();
}

void CardStack::edgeChanged(EdgeWidget::ScreenEdge/* oldEdge*/)
{
	_layout->invalidate();
	updateGeometry();
}

void CardStack::restore(KConfigBase * config)
{
	setPosition(config->readNumEntry("location", 0), (ScreenEdge)config->readNumEntry("edge", 0));
}

void CardStack::store(KConfigBase * config)
{
	config->writeEntry("edge", edge());
	config->writeEntry("location", location());
}

/*** CardTab **************************/
CardTab::CardTab(Applet * applet, QWidget * parent) : EdgeWidgetLayoutBox( parent, EdgeWidgetBoxLayout::Colinear, "CardTab" )
{
	_label = 0L;
	_applet = 0L;
	_icon = 0L;
	layout()->setMargin(9);
	setApplet(applet);
}

CardTab::~CardTab()
{
	if (_icon)
		_icon->reparent(this, QPoint(10,10), false);
}

Applet * CardTab::applet() const
{
	return _applet;
}

void CardTab::setApplet(Applet * applet)
{
	if (_icon)
		_icon->reparent(0L, QPoint(10,10), true);
	if (_label)
		delete _label;
		
	_applet = applet;
	
	if (_applet)
	{
		_icon = _applet->icon();
		if (_icon)
			_icon->reparent(this, QPoint(10,10), true);
		
		_label = new QLabel(_applet->name(), this, "CardTabLabel"); 
	}
	else
		_label = new QLabel(i18n("Empty Card"), this, "CardTabLabel"); 
}

/*** CardStackFactory *****************/
CardStackFactory::CardStackFactory() : SessionItemFactory("cardstack")
{
	
}

CardStackFactory::~CardStackFactory()
{
	
}

SessionItem * CardStackFactory::loadItem(KConfigBase * configGroup)
{
	SessionItem * retVal = new CardStack(configGroup->group());
	retVal->restore(configGroup);
	return retVal;
}

SessionItem * CardStackFactory::createItem()
{
	return new CardStack(generateItemId());
}

#include "cardstack.moc"
