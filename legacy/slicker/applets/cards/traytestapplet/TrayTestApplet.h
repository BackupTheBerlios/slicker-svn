/***************************************************************************
                          TrayTestApplet.h  -  description
                             -------------------
    begin                : Sat Jan 11 2003
    copyright            : (C) 2003 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRAYTESTAPPLET_H
#define TRAYTESTAPPLET_H

#include <qwidget.h>
#include <qpixmap.h>

#include <carddeskcore/CardApplet.h>
#include <carddeskcore/CardElements.h>

namespace CardDesk {

class IconTrayButton : public TrayWidget
{
	Q_OBJECT
	public:
		IconTrayButton(Tray *, const char *name, QPixmap icon);

		virtual void orientationChanged( bool horizontal );

	signals:
		void pressed(IconTrayButton *src);

	protected:
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
		virtual void paintEvent(QPaintEvent *);

		///////////////////////////////////////////////////////////////

		QPixmap _icon;
		bool _down;
};

/**
  *@author Shamyl Zakariya
  */

class TrayTestApplet : public CardApplet
{
	Q_OBJECT
	public:
		TrayTestApplet(const QString &configFile);

		QString defaultCardName() { return "Tray test"; }

		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );

		virtual bool allowDropOnTab( void );

		virtual bool canDecodeDrag( class QMimeSource * );

		virtual void tabDragDropEvent( QDropEvent * );

		virtual Tray *createTray( Card * );

		virtual Separator *createSeparator( Card * );

		virtual Contents *createContents( Card * );

		virtual void saveSettings( class KConfig * );

		virtual void loadSettings( class KConfig * );

		virtual void cardReady( void );


	protected:
		QWidget *_contentsWidget;
};

}

#endif
