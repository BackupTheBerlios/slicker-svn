/***************************************************************************
                          LayoutSwitcher.h  -  description
                             -------------------
    begin                : Wed Dec 25 2002
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

#ifndef LAYOUTSWITCHER_H
#define LAYOUTSWITCHER_H

#include <qptrlist.h>
#include "Card.h"

class QWidget;
class QBoxLayout;

/////////////////////////////////////////////////////////////////////////////
// LayoutSwitcher -- we need to be able to switch between horizontal and
// and vertical layouts dynamically. This is an internal class used by Deck
// and Card and Tray.

namespace CardDesk {

class LayoutSwitcher
{
	public:
		LayoutSwitcher( QWidget *managed, ScreenEdge edge, bool verbose = false );

		/**
			LayoutSwitcher will default to zero spacing between layed out elements.
			But you can override this by setting @spacing to > 0.
			To remove spacing, set @spacing to zero.
		*/
		void setSpacing( int spacing );
		int getSpacing() { return _spacing; }
		
		void addWidget( QWidget *, bool relayout = true, int at = -1);
		void removeWidget ( QWidget *);
		void removeAll( bool relayout );

		QPtrList<QWidget> &elements();

		void setEdge( ScreenEdge edge );
		ScreenEdge edge( void );

		void relayout( void );

	protected:

		ScreenEdge _edge;
		QBoxLayout *_layout;
		QBoxLayout *_contentslayout;
		QPtrList<QWidget> _layoutItems;
		QWidget *_widget;
		bool _verbose;
		int _spacing;
};

}; //namespace

#endif
