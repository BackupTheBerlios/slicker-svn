/***************************************************************************
 *   Copyright (C) 2004 by Markus Breitenberger                            *
 *   bre@slicker.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
  : QButton(parent, name, f), _popupMenu(0)
{

}

LauncherButton::~LauncherButton()
{
    delete _popupMenu;
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
    KIconEffect::visualActivate(this, rect());
    QButton::mousePressEvent(e);
}

void LauncherButton::contextMenuEvent (QContextMenuEvent *e)
{
    if (_popupMenu) {
        _popupMenu->popup(e->globalPos());
    }
}

QPopupMenu* LauncherButton::popupMenu()
{    
    if (!_popupMenu) {
        _popupMenu = new QPopupMenu();
    }
    return _popupMenu;
}
 
#include "launcherbutton.moc"
