/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
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

protected:
    virtual void drawButton(QPainter *);
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
