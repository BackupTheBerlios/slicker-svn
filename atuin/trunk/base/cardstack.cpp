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
#include <qobjectlist.h>
#include <kdebug.h>

/*** CardStack *********************/

CardStack::CardStack(const QString & id) : EdgeWidget(NULL, "CardStack"), AppletHost("CardStack"), SessionItem(id)
{
	_layout = new EdgeWidgetBoxLayout(this, this, EdgeWidgetBoxLayout::Perpendicular, "CardLayout");
	_layout->setAutoAdd(true);
	_layout->setResizeMode( QLayout::Fixed );
	_emptyStackTab = new CardTab(0L, this);
	show();
}

CardStack::~CardStack()
{
	removeAllApplets();
}

bool CardStack::accept(Applet * applet)
{
	//TODO: This should be rewritten to provide some more intelligent filtering of applets
	QWidget * icon = applet->icon();
	QWidget * content = applet->content();
	
	if (!content)
		return false;
	
	if (icon)
	{
		if ((icon->minimumWidth() > 16) || (icon->minimumHeight() > 16))
			return false;
	
		if ((icon->maximumWidth() < 32) || (icon->maximumHeight() < 32))
			return false;
	}

	if (applets().count() == 0)
		_emptyStackTab->hide();
	CardTab * tab = new CardTab(applet, this);
	tab->show();
	content->reparent(this, QPoint(0, 0), true);
	
	_layout->invalidate();
	
	registerApplet(applet);
	return true;
}

void CardStack::detach(Applet * applet)
{
	if (applet->icon())
		applet->icon()->reparent(NULL, QPoint(0,0), false);
	applet->content()->reparent(NULL, QPoint(0,0), false);
	
	deregisterApplet(applet);
	
	if (applets().count() == 0)
		_emptyStackTab->show();
	
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
CardTab::CardTab(Applet * applet, CardStack * parent) : EdgeWidgetLayoutBox( parent, EdgeWidgetBoxLayout::Colinear, "CardTab" )
{
	_label = 0L;
	_applet = 0L;
	_icon = 0L;
	_stack = parent;
	layout()->setMargin(2);
	setApplet(applet);
	setBackgroundColor(QColor(0,0,0));
}

CardTab::~CardTab()
{
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
	{
		removeChild(_label);
		delete _label;
		_label = 0L;
	}
		
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
	relayout();
}

void CardTab::mousePressEvent ( QMouseEvent * e )
{
	if ((e->button() == LeftButton))
		_stack->beginPosDrag(e->pos());

	e->accept();
}

void CardTab::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == LeftButton)
		_stack->endPosDrag();

	e->accept();
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
