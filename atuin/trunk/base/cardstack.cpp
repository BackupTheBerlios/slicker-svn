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

#include <kaction.h>
#include <kapplication.h>
#include <qvaluelist.h>
#include "applet.h"

/*** CardStack *********************/

CardStack::CardStack(const QString & id) : EdgeWidget(NULL, "CardStack"), AppletHost("CardStack"), SessionItem(id)
{
	_layout = new EdgeWidgetBoxLayout(this, this, EdgeWidgetBoxLayout::Perpendicular, "CardLayout");
	_layout->setAutoAdd(true);
	_layout->setResizeMode( QLayout::Fixed );
	_emptyStackTab = new CardTab(0L, this);
	
	_hostMenu = new AppletHostMenu(this, "CardAppletMenu", this, i18n("Slider Menu"));
	_appletMenu = 0L;
	_removeAppletAction = new KAction(i18n("Remove applet"), KShortcut(), this, SLOT(slotRemoveCurrentApplet()), this, "RemoveApplet");
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
	
	applet->addDefaultContextAction(_removeAppletAction);
	
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

void CardStack::contextMenuEvent(QContextMenuEvent *e)
{
/*
	QWidget * childWidget = directChildAt(e->pos());
	_currentApplet = childWidget ? findApplet(childWidget) : 0L;
	
	if (_currentApplet)
	{
		if (_appletMenu)
			delete _appletMenu;
		
		_appletMenu = new KActionMenu(_currentApplet->name(), this, "AppletMenu");
		
		KActionPtrList actionList = _currentApplet->contextActions(childWidget, e->pos());
		KActionPtrList::Iterator iter;
		
		for (iter = actionList.begin(); iter != actionList.end(); iter++)
			_appletMenu->insert(*iter);
		
		_appletMenu->insert(_removeAppletAction);
		_appletMenu->popup(e->globalPos());
	}
	else
		_hostMenu->popup(e->globalPos());
    e->accept();
*/
	QWidget * childWidget = directChildAt(e->pos());
	_currentApplet = childWidget ? findApplet(childWidget) : 0L;
	
	if (!_currentApplet)
	{
		_hostMenu->popup(e->globalPos());
	}
	e->accept();	
}

QWidget * CardStack::directChildAt(const QPoint & point)
{
	QWidget * childWidget = childAt(point);
	//while (childWidget && (childWidget->parentWidget() != _content))
	//		childWidget = childWidget->parentWidget();
	return childWidget;
}

void CardStack::slotRemoveCurrentApplet()
{
	if (_currentApplet)
		delete _currentApplet;
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
	

	show();
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
	if (e->button() == RightButton) 
	{
		//do some stuff in here
	}
	e->accept();
}

void CardTab::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == LeftButton)
		_stack->endPosDrag();

	if (e->button() == RightButton) 
	{
		//lets see if we can change the label...
		_label = new QLabel(i18n("Moved"), this, "CardTabLabel");
		relayout();
		
	}
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
