/***************************************************************************
                          KonsoleApplet.h  -  description
                             -------------------
    begin                : Sat Jan 11 2003
    copyright            : (C) 2003 by Fabian Uffer
    email                : fuffer@student.ethz.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KONSOLEAPPLET_H
#define KONSOLEAPPLET_H

#include <qwidget.h>
#include <qpixmap.h>

#include "../../../carddeskcore/CardApplet.h"
#include "../../../carddeskcore/CardElements.h"
#include <kparts/part.h>

using namespace CardDesk;

class KonsoleApplet : public CardApplet
{
	Q_OBJECT
	public:
		KonsoleApplet(const QString &configFile);

		virtual QString defaultCardName( void ) { return QString("Konsole"); }
		virtual QPixmap *tabIcon( void );
		virtual QSize miniDisplaySize( void );
		virtual void paintMiniDisplay( QPainter *, int, int );
		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );
		virtual bool allowDropOnTab( void );
		virtual bool canDecodeDrag( class QMimeSource * );
		virtual void tabDragDropEvent( QDropEvent * );
		virtual Separator *createSeparator( Card * );
		virtual Contents *createContents( Card * );
		virtual Resizer *createResizer( Card *);
		virtual void saveSettings( class KConfig * );
		virtual void loadSettings( class KConfig * );
		virtual void cardReady( void );
		virtual bool allowHoverToOpen( void );

	public slots:
		void destroy( void );

	protected:
		QWidget *_contentsWidget;
		QPixmap _icon;
		KParts::ReadOnlyPart *part;

	private:
		void init();
};

#endif
