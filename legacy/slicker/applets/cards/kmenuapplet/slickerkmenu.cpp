/***************************************************************************
                          slickerkmenu.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
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

#include "slickerkmenu.h"
#include "kmenuapplet.h"
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include "kmenutab.h"
SlickerKMenu::SlickerKMenu(Card * c) :
  PanelServiceMenu(QString::null, QString::null, c,"tear off menu"){

  setFrameStyle(0);
  this->c = c;
  contents = NULL;
  initialize();
}
SlickerKMenu::~SlickerKMenu(){
}
/** No descriptions */
Contents * SlickerKMenu::getContents(){
  if(!contents) {
    contents = new Contents(c);
    contents->setContents(this);
    setWFlags(WType_Popup);
    contents->setPreferredSize(sizeHint().width(), sizeHint().height());
  }

  return contents;
}
/** No descriptions */
void SlickerKMenu::slotExec(int id){
  kdDebug() << "SlickerKMenu::SlotExec" << endl;
  PanelServiceMenu::slotExec(id);
}
/** No descriptions */
void SlickerKMenu::hide(){
  kdDebug() << "SlickerKMenu::hide" << endl;
  PanelServiceMenu::hide();
  KMenuTab::setDontOpenCard(true);
}
/** No descriptions */
void SlickerKMenu::hideEvent(QHideEvent * e){
  c->shut(true);
  kdDebug() << "SlickerKMenu::hideEvent" << endl;
}
/** No descriptions */
bool SlickerKMenu::isPopup() const{
  return true;
}
/** No descriptions */
void SlickerKMenu::show(){
  kdDebug() << "SlickerKMenu::show" << endl;
  PanelServiceMenu::show();
}
