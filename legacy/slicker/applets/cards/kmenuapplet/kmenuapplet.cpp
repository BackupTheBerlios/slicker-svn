/***************************************************************************
                          kmenuapplet.cpp  -  description
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

#include "kmenuapplet.h"
#include "../../../carddeskcore/CardApplet.h"
#include "slickerkmenu.h"
#include <kdebug.h>
#include <kiconloader.h>
#include "kmenutab.h"


extern "C"
{
	CardApplet *init(const QString& configFile)
	{
		return new KMenuApplet(configFile);
	}

}


KMenuApplet::KMenuApplet( const QString &configFile )
      : CardApplet( configFile )
{
  menu = NULL;
  icon = NULL;
}

KMenuApplet::~KMenuApplet(){
}
/**
			Must be implemented -- even if empty as this is pre virtual. Should load
			the settings for this applet. DON'T CHANGE THE GROUP -- IT'S ALREADY SET.
		 */
void KMenuApplet::loadSettings(class KConfig * ){
}
/** 
			Must be implemented -- even if empty as this is pre virtual. Should save
			the settings for this applet. DON'T CHANGE THE GROUP.
		 */
void KMenuApplet::saveSettings(class KConfig * ){
}

/**
			All cards have a contents area. You should create a Contents object, add your
			gui to it, (see Contents class doc) and return it here. Like above, this will
			only be called once, when the Card is created.
		 */
Contents * KMenuApplet::createContents(Card * c){
  if(!menu) menu = new SlickerKMenu(c);
  menu->getContents();
  return menu->getContents();
}

Tab * KMenuApplet::createTab(Card *c)
{
	kdDebug() << "KMenuApplet::createTab" << endl;
	return new KMenuTab(c,this);
}

QString KMenuApplet::pluginName(){
  return QString("KMenuApplet");
}

QString KMenuApplet::defaultCardName() {
  return QString("KMenu");
}

QPixmap * KMenuApplet::tabIcon(){
	//For some reason carddesk calls this function five times, so makes sense to store icon

	if (icon.isNull())
	  icon = KGlobal::instance()->iconLoader()->loadIcon("kmenu", KIcon::Panel,
                                              0, KIcon::ActiveState, 0L, true);
	if (!icon.isNull()) {
		return &icon;
	} else {
		return NULL;
	}
}
/**
			This will be called when the card is opened. If your applet does heavy things and
			doesn't need them running when shut (for example, and animated display in its
			contents area), you can use this as a trigger. See ::cardShut
			for the complement of this method.
		 */
void KMenuApplet::cardOpened(){
  menu->show();
}

