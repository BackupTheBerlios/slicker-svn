/***************************************************************************
                          TestCardApplet.h  -  description
                             -------------------
    begin                : Sun Jan 5 2003
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

#ifndef TESTCARDAPPLET_H
#define TESTCARDAPPLET_H

#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <carddeskcore/CardApplet.h>

/**
  *@author Shamyl Zakariya
  */

namespace CardDesk {

class TestCardApplet : public CardApplet
{
	Q_OBJECT
	public:
		TestCardApplet(const QString &configFile);

		~TestCardApplet();

		void setCardName(QString newName);

		virtual QString pluginName( void ) { return QString("TestCardApplet"); }
		virtual QString defaultCardName( void ) { return QString("Test Card"); }
		virtual QPixmap *tabIcon( void );

		virtual QSize miniDisplaySize( void );

		virtual void paintMiniDisplay(QPainter *, int width, int height);

		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );

		virtual void aboutToShowContextMenu( class QPopupMenu *menu );

		virtual bool allowDragHoverToOpen( void );
		
		virtual bool allowHoverToOpen( void );

		virtual bool allowDropOnTab( void );

		virtual bool canDecodeDrag( class QMimeSource * );

		virtual void tabDragDropEvent( QDropEvent * );

		virtual QString cardName( void );

		virtual Tray *createTray( Card * );

		virtual Separator *createSeparator( Card * );

		virtual Contents *createContents( Card * );

		virtual void saveSettings( class KConfig * );

		virtual void loadSettings( class KConfig * );

		virtual void cardReady( void );

	public slots:

		void cardOpened( void );
		void cardShut( void );

		void menuOne( void );
		void menuTwo( void );

		void nameChanged( void );

		void toggleResizeGrip( bool yesno );
		void toggleMiniDisplay(bool yesno);
		void toggleIcon( bool yesno );		

	protected:
		QPixmap _icon;
		QString _name;
		QWidget *_contentsWidget;
		class QLineEdit *_nameLE;
		class DropTestArea *_dtArea;

		bool _showIcon, _showMiniDisplay, _showResizeGrip;
		
};

class DropTestArea : public QLabel
{
	friend class TestCardApplet;

	Q_OBJECT
	public:
		DropTestArea(QWidget *parent, const char *name);

	signals:
		void dropProcessed();

	protected:
		virtual void dragEnterEvent( QDragEnterEvent * );
		virtual void dragMoveEvent( QDragMoveEvent * );
		virtual void dragLeaveEvent( QDragLeaveEvent * );
		virtual void dropEvent( QDropEvent * );

};

}

#endif
