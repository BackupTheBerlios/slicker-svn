/***************************************************************************
			  IMApplet.h  -  description
			     -------------------
    begin		: Thu Mar 6 2003
    copyright	    : (C) 2003 by Erik Mathisen
    email		: erik@mathisen.us
 ***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#ifndef _IMAPPLET_H_
#define _IMAPPLET_H_

#include <qwidget.h>
#include <qpixmap.h>
#include <carddeskcore/CardApplet.h>
#include <carddeskcore/CardElements.h>

using namespace CardDesk;

  /**
   *@author Erik Mathsien
   */
	class Kopete;

  class IMApplet : public CardApplet {
    Q_OBJECT
   public:
    IMApplet (const QString &configFile);
    virtual QString defaultCardName( void ) { return QString("IM Applet"); }
    virtual void addToDefaultContextMenu (class QPopupMenu * defaultMenu);
    virtual bool allowDropOnTab (void);
    virtual bool canDecodeDrag (class QMimeSource *);
    virtual void tabDragDropEvent (QDropEvent *);
    virtual Tray *createTray (Card *);
    virtual Separator *createSeparator (Card *);
    virtual Contents *createContents (Card *);
    virtual void saveSettings (class KConfig *);
    virtual void loadSettings (class KConfig *);
    virtual void cardReady (void);
    QPixmap* tabIcon();

  protected:

    QWidget * _contentsWidget;
    QPixmap  _icon;
		QString _name;
		//Kopete *kopete_widget;

  private:

    void init();

  }; // End of IMApplet class


#endif
