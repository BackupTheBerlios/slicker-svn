/***************************************************************************
  Copyright (C) 2003 by Jason Kivlighn, mizunoami44@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRINTERJOBSAPPLET_H
#define PRINTERJOBSAPPLET_H

#include <qwidget.h>
#include <qpixmap.h>

#include "../../../carddeskcore/CardApplet.h"
#include "../../../carddeskcore/CardElements.h"

using namespace CardDesk;

class KMTimer;
class KMJobViewer;

class PrinterJobsApplet : public CardApplet
{
	Q_OBJECT
	public:
		PrinterJobsApplet( const QString &configFile );

	protected:
		virtual QString defaultCardName( void ) { return QString("Printer Jobs"); }
		virtual QString cardName( void );
		void setCardName(QString &newName);
		virtual QPixmap *tabIcon( void );
		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );
		virtual bool allowDropOnTab( void );
		virtual Separator *createSeparator( Card * );
		virtual Contents *createContents( Card * );
		virtual void cardReady( void );
		virtual bool allowHoverToOpen( void );
		virtual void cardOpened( void );
		virtual void cardShut( void );

		QPixmap _icon;
		QString _name;
		KMJobViewer *viewer;
		KMTimer *timer;

	private slots:
		void slotJobsShown(KMJobViewer*,bool);
		void slotPrinterChanged(KMJobViewer*,const QString&);
		void slotTimer();
		void slotViewerDestroyed(KMJobViewer*);
};

#endif

