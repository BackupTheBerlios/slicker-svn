/***************************************************************************
         Copyright: 2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Manuel Uphoff <uppi@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "applethostmenu.h"
#include "kmessagebox.h"

AppletHostMenu::AppletHostMenu(QObject *parent, const char *name, AppletHost *appletHost, const QString &text)
	: KActionMenu(text, parent, name)
{
	_appletDefsMenu = new AppletDefsMenu(this, "AddApplet", appletHost, i18n("Add Applet"));
	insert(_appletDefsMenu);
	
	//TODO: Insert Main Menu here
	
	_menuItemRemove = new KAction(i18n("Remove"),KShortcut(),this,SLOT(slotRemove()),this,"RemoveAppletHost");
	insert(_menuItemRemove);
}

void AppletHostMenu::slotRemove()
{
	int ret = KMessageBox::warningYesNo
		(0,i18n("Are you sure you wish to remove this container? This will remove all its applets and cannot be undone"),
		i18n("Remove Container"),
		KStdGuiItem::yes(),
		KStdGuiItem::no(),
		"DoNotAskRemoveContainer",
		KMessageBox::Notify);
		
	if( ret == KMessageBox::Yes) 
	{
		delete(parent());
	}
}



#include "applethostmenu.moc"

