/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include <kglobal.h>
#include <kiconloader.h>
#include <kiconeffect.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qtooltip.h>
#include "launcherbutton.h"
 
LauncherButton::LauncherButton(QWidget *parent, const char *name, WFlags f)
  : QButton(parent, name, f)
{

}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::setIcon(QString iconName)
{
    if (_iconName != iconName) {
        _iconName = iconName;
    
        KIconLoader *iconLdr = KGlobal::iconLoader();

        _icon = iconLdr->loadIcon(_iconName, KIcon::Panel, 0,
                                  KIcon::DefaultState);
        _iconHighlight = iconLdr->loadIcon(_iconName, KIcon::Panel, 0,
                                  KIcon::ActiveState);
                                     
        setPixmap(_icon);
    }
}
 
void LauncherButton::drawButton(QPainter *p)
{
    const QPixmap *pm = pixmap();
    if (pm) {
        p->drawPixmap(0, 0, *pm);
    } else {
        p->fillRect(rect(), QColor(100,0,0));
    }
}

QSize LauncherButton::sizeHint () const
{
    const QPixmap *pm = pixmap();
    
    if (pm) return pm->size();
    return QSize(64,64);
}

void LauncherButton::setTooltip(QString tip)
{
    QToolTip::add(this, tip);
}

void LauncherButton::enterEvent(QEvent *e)
{
    setPixmap(_iconHighlight);
    repaint(false);
    
    QButton::enterEvent(e);
}

void LauncherButton::leaveEvent(QEvent *e)
{
    setPixmap(_icon);
    repaint(false);
    
    QButton::leaveEvent(e);
}

void LauncherButton::mousePressEvent (QMouseEvent *e)
{
    if (e->button() == LeftButton) {
        KIconEffect::visualActivate(this, rect());
    }
    QButton::mousePressEvent(e);
}

#include "launcherbutton.moc"
