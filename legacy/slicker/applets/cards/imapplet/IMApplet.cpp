/***************************************************************************
			  IMApplet.cpp  -  description
			     -------------------
    begin		: Thu Mar 6 2003
    copyright		: (C) 2003 by Erik Mathisen
    email		: erik@mathisen.us
    copyright	   : (C) 2003 by Shamyl Zakariya
    email	       : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#include "IMApplet.h"

#include <qstring.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <carddeskcore/Const.h>
#include <carddeskcore/Card.h>
#include <carddeskcore/CardApplet.h>

using namespace CardDesk;

extern "C" {
  CardApplet *init( const QString &configFile ) {
    return new IMApplet(configFile);
  }
}



IMApplet::IMApplet(const QString &configFile) : CardApplet(configFile)
{
  init();
}

void IMApplet::init()
{
  KIconLoader il;
  _icon = il.loadIcon("kopete", KIcon::Small, 0, KIcon::DefaultState);

  _name = "Instant Message Applet";
}

void IMApplet::addToDefaultContextMenu(QPopupMenu*)
{}

bool IMApplet::allowDropOnTab()
{
  return true;
}

bool IMApplet::canDecodeDrag(QMimeSource *e)
{
  return QUriDrag::canDecode(e);
}

void IMApplet::tabDragDropEvent(QDropEvent*)
{}

Tray *IMApplet::createTray(Card *c)
{
	c = c;
  return NULL;
}

Separator *IMApplet::createSeparator(Card *c)
{
  return new Separator(c, true);
}

Contents *IMApplet::createContents(Card *c)
{
  Contents *contents = new Contents(c);
  _contentsWidget = new QWidget(0, "IMApplet-ContentsWidget");
  QVBoxLayout *layout = new QVBoxLayout(_contentsWidget);

  //kopete_widget = new Kopete;
  //layout->addWidget(kopete_widget->mainWidget());

  contents->setContents(_contentsWidget);
  return contents;
}

QPixmap *IMApplet::tabIcon()
{
  return &_icon;
}

void IMApplet::saveSettings(class KConfig *)
{}

void IMApplet::loadSettings(class KConfig*)
{}

void IMApplet::cardReady()
{}
