/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de>

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef APPLETHOSTMENU_H
#define APPLETHOSTMENU_H

#include "appletdef.h"
#include "applethost.h"

/**
* This is the context menu of applet hosts.
* It is used to add new applets to a host, remove the host or display the main menu.
* The menu layout is on the slicker wiki at  http://wiki.slicker.org/tiki-index.php?page=Menu+layout
* 
*/

class AppletHostMenu : public KActionMenu
{
	Q_OBJECT
public:
	AppletHostMenu(QObject *parent=0, const char *name=0, AppletHost *applethost=0, const QString &text=i18n("Applet Host"));
public slots:
	void slotRemove();
private: 
	AppletDefsMenu * _appletDefsMenu;
	KAction * _menuItemRemove; 

};

#endif
