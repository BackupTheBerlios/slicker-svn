/***************************************************************************
                          slickerkmenu.h  -  description
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

#ifndef SLICKERKMENU_H
#define SLICKERKMENU_H

#include "service_mnu.h"
#include "../../../carddeskcore/CardElements.h"
#include <qpixmap.h>

using namespace CardDesk;

class CardDesk::Card;

/**
  *@author Kurre Ståhlberg
  */

class SlickerKMenu : public PanelServiceMenu  {
  Q_OBJECT
public:
	SlickerKMenu(Card * c);
	~SlickerKMenu();
  /** No descriptions */
  Contents * getContents();
  /** No descriptions */
  void hideEvent(QHideEvent * e);
  /** No descriptions */
  bool isPopup() const;
  /** No descriptions */
  void show();
public slots:
  /** No descriptions */
  void hide();

private:
  Contents * contents;
  Card * c;
protected slots: // Protected slots
  /** No descriptions */
  virtual void slotExec(int id);


};

#endif
