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

CardStack::CardStack(const QString & id) : EdgeWidget(NULL, "CardStack"), AppletHost("CardStack"), SessionItem(id)
{
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

/*** SliderFactory *****************/
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
