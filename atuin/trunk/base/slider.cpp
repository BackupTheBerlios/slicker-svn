/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "slider.h"

#include <kaction.h>
#include <kapplication.h>
#include <kdebug.h>
#include <qvaluelist.h>

#include "applet.h"

/*** Slider **********************************************************/
Slider::Slider(const QString & id)
        : EdgeWidget(NULL, "Slider"), AppletHost("Slider"), SessionItem(id)
{
    _mainLayout = new EdgeWidgetBoxLayout(this, this, EdgeWidgetBoxLayout::Colinear, "SliderLayout");
    _mainLayout->setAutoAdd( true );
    _mainLayout->setResizeMode( QLayout::Fixed );

    _tray1 = new SliderTray(this);
    _content = new EdgeWidgetLayoutBox(this, this, EdgeWidgetBoxLayout::Colinear, "SliderContentLayout");
    _tray2 = new SliderTray(this);

/*    _content->setMaximumHeight(32);*/

    _hostMenu = new AppletHostMenu(this, "SliderAppletMenu", this, i18n("Slider Menu"));
	_appletMenu = 0L;
	_removeAppletAction = new KAction(i18n("Remove applet"), KShortcut(), this, SLOT(slotRemoveCurrentApplet()), this, "RemoveApplet");

    show();
}

Slider::~Slider()
{
    removeAllApplets();
}


bool Slider::accept(Applet * applet)
{
    //TODO: This should be rewritten to provide some more intelligent filtering of applets
    QWidget *icon = applet->icon();
    if (icon == 0)
        return false;

    if ((icon->minimumWidth() > 32) || (icon->minimumHeight() > 32))
        return false;

    if ((icon->maximumWidth() < 32) || (icon->maximumHeight() < 32))
        return false;

    icon->reparent(_content, QPoint(0,0), true);
    _content->updateGeometry();
    icon->resize(32, 32);
    
    registerApplet(applet);
    return true;
}

void Slider::detach(Applet * applet)
{
    if (applet->icon())
        applet->icon()->reparent(NULL, QPoint(0,0), false);

    deregisterApplet(applet);
    updateGeometry();
}

void Slider::mousePressEvent ( QMouseEvent * e )
{
	if ((e->button() == LeftButton) && !appletAt(e->pos()))
        beginPosDrag(e->pos());

    e->accept();
}

void Slider::mouseReleaseEvent(QMouseEvent *e)
{
	Applet * pressedApplet = appletAt(e->pos());
    
	if (e->button() == LeftButton)
	{
		QWidget * contentWidget = pressedApplet ? pressedApplet->content() : 0L;
		if (contentWidget)
		{
			if (contentWidget->isVisible())
				contentWidget->hide();
			else
				contentWidget->show();
		}
		else
		endPosDrag();
	}

	e->accept();
}

Applet * Slider::appletAt(const QPoint & point)
{
	QWidget * childWidget = directChildAt(point);
	return childWidget ? findApplet(childWidget) : 0L;
}

QWidget * Slider::directChildAt(const QPoint & point)
{
	QWidget * childWidget = childAt(point);
	while (childWidget && (childWidget->parentWidget() != _content))
		childWidget = childWidget->parentWidget();
	return childWidget;
}

void Slider::contextMenuEvent(QContextMenuEvent *e)
{
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
}

void Slider::slotRemoveCurrentApplet()
{
	if (_currentApplet)
		delete _currentApplet;
}

void Slider::edgeChanged(EdgeWidget::ScreenEdge oldEdge)
{
    EdgeWidgetTransformHelper h(this, oldEdge, edge());

    _content->relayout();

    h.transform(*_tray1);
    h.transform(*_tray2);
    h.transform(*_content);

    updateGeometry();
}

void Slider::restore(KConfigBase * config)
{
    setPosition(config->readNumEntry("location", 0), (ScreenEdge)config->readNumEntry("edge", 0));
}

void Slider::store(KConfigBase * config)
{
    config->writeEntry("edge", edge());
    config->writeEntry("location", location());
}

/*** SliderTray *****************/
SliderTray::SliderTray(QWidget * parent)
        : QWidget(parent, "SliderTray")
{
    setMinimumSize(20, 36);
    setMaximumSize(20, 100);
}

/*** SliderFactory *****************/
SliderFactory::SliderFactory() : SessionItemFactory("slider")
{

}

SliderFactory::~SliderFactory()
{

}

SessionItem * SliderFactory::loadItem(KConfigBase * configGroup)
{
    SessionItem * retVal = new Slider(configGroup->group());
    retVal->restore(configGroup);
    return retVal;
}

SessionItem * SliderFactory::createItem()
{
    return new Slider(generateItemId());
}

#include "slider.moc"
