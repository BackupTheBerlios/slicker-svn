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

#include <kapplication.h>
#include <kdebug.h>

#include "applet.h"

/*** Slider **********************************************************/
Slider::Slider(const QString &id)
        : EdgeWidget(NULL, "Slider"), AppletHost("Slider"), SessionItem(id)
{
    _mainLayout = new EdgeWidgetBoxLayout(this, this, EdgeWidgetBoxLayout::Colinear, "SliderLayout");
    _mainLayout->setAutoAdd( true );
    _mainLayout->setResizeMode( QLayout::Fixed );

    _tray1 = new SliderTray(this);
    _content = new EdgeWidgetLayoutBox(this, this, EdgeWidgetBoxLayout::Colinear, "SliderContentLayout");
    _tray2 = new SliderTray(this);

/*    _content->setMaximumHeight(32);*/

    _appletMenu = new AppletHostMenu(this, "SliderAppletMenu", this, i18n("Slider Menu"));

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
    if (e->button() == LeftButton)
        beginPosDrag(e->pos());

    e->accept();
}

void Slider::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == LeftButton)
        endPosDrag();

    e->accept();
}

void Slider::contextMenuEvent(QContextMenuEvent *e)
{
    _appletMenu->popup(e->globalPos());
    e->accept();
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

/*** SliderCreator *****************/
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
