/***************************************************************************
                          carddesk.h  -  description
                             -------------------
    begin                : Mon Dec 23 19:30:00 EST 2002
    copyright            : (C) 2002 by Shamyl Zakariya
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

#ifndef TESTGUI_H
#define TESTGUI_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <carddeskcore/Const.h>
#include <carddeskcore/Card.h>
#include <kapp.h>
#include <kdialog.h>
#include <qwidget.h>
#include <qptrlist.h>


namespace CardDesk {
class Card;
}
class QListViewItem;

using namespace CardDesk;

class TestGui : public KDialog
{
  Q_OBJECT 
  public:
    TestGui(QWidget* parent=0, const char *name=0);
    ~TestGui();
    
    void rebuildTreeView( void );

  public slots:
		void cardClicked(QListViewItem *cardItem);

		void cardRegistered(QString name, Card *card);
		void cardUnregistered(QString name, Card *card);
		void cardMoved(Card *, double actual, double desired, int edge);
		void cardOpenedOrShut(Card *);
		void cardRenamed(Card *, QString, QString );
		
		void deckingChanged( void );

		void killCard( void );
		void cardSelected(const QString &);

		void positionSlider( int );
		void edgeCB( const QString & );

	protected:
		virtual void closeEvent(QCloseEvent *);
	
		class QSlider *_positionSlider;
		class QComboBox *_edgeCB;
		class KListView *_cardLV;
		class QPushButton *_killPB;

		QString _currentCard;
		bool _sliderActive;

};

#endif
