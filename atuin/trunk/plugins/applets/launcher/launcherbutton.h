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
#ifndef LAUNCHERBUTTON_H
#define LAUNCHERBUTTON_H

#include <qpopupmenu.h>
#include <qbutton.h>
#include <qstring.h>

class LauncherButton : public QButton {
    Q_OBJECT
    
public:
    LauncherButton(QWidget *parent=0, const char *name=0,  WFlags f=0 );
    virtual ~LauncherButton();
    
    virtual void setTooltip(QString tip);
    virtual QSize sizeHint () const;
    
    void setIcon(QString iconName);

    /// Get a pointer to the button's Popup-Menu    
    virtual QPopupMenu* popupMenu();

protected:
    virtual void drawButton(QPainter *);
    virtual void contextMenuEvent (QContextMenuEvent *);
    virtual void enterEvent (QEvent *);
    virtual void leaveEvent (QEvent *);
    virtual void mousePressEvent (QMouseEvent *);
    
private:
    QPopupMenu * _popupMenu;
    QString      _iconName;
    
    QPixmap      _icon;
    QPixmap      _iconHighlight;
};

#endif // LAUNCHERBUTTON_H
