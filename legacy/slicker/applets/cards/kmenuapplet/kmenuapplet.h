/***************************************************************************
                          kmenuapplet.h  -  description
                             -------------------
    begin                : Wed Jan 15 2003
    copyright            : (C) 2003 by Kurre Ståhlberg
    email                : kurre@cc.hut.fi
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KMENUAPPLET_H
#define KMENUAPPLET_H

#include "../../../carddeskcore/Const.h"
#include "../../../carddeskcore/CardApplet.h"

class SlickerKMenu;
/**
  *@author Kurre Ståhlberg
  */
using namespace CardDesk;
  
class KMenuApplet : public CardApplet  {
public:
	KMenuApplet(const QString &configFile);
 	~KMenuApplet();
  /**
			Must be implemented -- even if empty as this is pre virtual. Should save
			the settings for this applet. DON'T CHANGE THE GROUP.
		 */
  void saveSettings(class KConfig * );
  /**
			Must be implemented -- even if empty as this is pre virtual. Should load
			the settings for this applet. DON'T CHANGE THE GROUP -- IT'S ALREADY SET.
		 */
  void loadSettings(class KConfig * );
  /**
			All cards have a contents area. You should create a Contents object, add your
			gui to it, (see Contents class doc) and return it here. Like above, this will
			only be called once, when the Card is created.
		 */
  Contents * createContents(Card * );
  /** 
			This is the pure virtual -- override this to return
			a unique plugin name. For example, your fancy kfm browser
			card might return "KFMCardPlugin"

			This will be used to identify your plugin class. E.g., so a user can
			say "Make me a card using KFMCardPlugin"
		 */

  Tab *createTab( Card * );
  
  QString defaultCardName();
  
  QString pluginName();
  /** 
			If you want your tab to draw an icon, return a pointer to it here.
			You will maintain ownership of the icon. It's your responsibility to
			delete it. If you don't want an icon, return NULL (default)

			Tab will auto-resize to accommodate icon size. It can be big, small, etc.
			If icon size changes, emit propertiesChanged.

		 */
  QPixmap * tabIcon();
  /** 
			This will be called when the card is opened. If your applet does heavy things and
			doesn't need them running when shut (for example, and animated display in its
			contents area), you can use this as a trigger. See ::cardShut
			for the complement of this method.
		 */
  void cardOpened();
//   /** No descriptions */
//   void tabMousePressEvent(QMouseEvent * e);

private:
  SlickerKMenu * menu;
  QPixmap icon;
};

#endif
