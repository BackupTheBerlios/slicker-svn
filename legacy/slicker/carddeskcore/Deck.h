/***************************************************************************
                          Deck.h  -  description
                             -------------------
    begin                : Fri Dec 27 2002
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

#ifndef _DECK_H
#define _DECK_H
 

#include <qwidget.h>
#include <qframe.h>
#include <qptrlist.h>
#include <qptrvector.h>
#include <qrect.h>
#include <qregion.h>

#include "Const.h"

namespace CardDesk {

class LayoutSwitcher;
class Card;

/**
	Deck is a special class. All cards have a deck, the deck is the top level window
	containing one or more cards.

	When a card is alone, not in a deck, it has its own Deck, and it is the only member
	of that deck.

	When multiple cards are stacked in a deck, the root card (the one against the edge of
	the screen) *owns* the deck object. The other cards get pointers to that deck. All
	are children of that deck.

	YOU SHOULD NEVER CREATE OR DELETE A DECK YOURSELF. Deck creation/deletion and other
	crap are taken care of via Card methods. Card is your API, Deck is simply an
	internal class managed entirely via Card.
*/

class Deck : public QWidget
{
	Q_OBJECT

	friend class Card;

	public:

		/**
			Creates a Deck with @root as the deck's root card, at position
			@position and edge @edge.
		*/
		Deck(Card *root, double position, ScreenEdge edge);

		void addCardAfter(Card *after, Card *joiningCard);
		void addCardBefore(Card *before, Card *joiningCard);

		void removeCard(Card *);

		int count();

		/*
			Returns true if this deck contains only the one free moving card.
			By "empty" we mean this deck exists only to support that one card, so
			for all intents and purposes, it's empty.
		*/
		bool empty( void ) { return (count() == 1); }

		Card *cardAt(int which);

		/**
			Given that we're dealing with a stacked deck of cards, this method returns
			a pointer to the card after which a new card should be inserted given
			the distance from the edge. Uses halfway points in card height.
			 Think of it this way, if there are four cards,
			a, b, c & d, and each is (for example) 30 pixels tall,
			distanceFromEdge =
				0 - 14: returns 0l (distance is in front of all cards)
				15 - 44: returns a
				45 - 59: returns b
				60 - 75: returns c and so on.
				any number beyond midpoint of tail card returns tail()

			This is used for deck reordering.
		*/
		Card *cardForDistanceFromEdge( int distanceFromEdge );

		/**
			Returns index of card passed in.
		*/
		int indexOf(Card *);

		/*
			Returns pointer to head of deck.
		*/
		Card *root( void );

		/*
			Returns pointer to tail of deck. If deck is empty, this
			will equal root.
		*/
		Card *tail( void );



		/*
			Called by Card members of the deck, to notify that for whatever
			reason the card size has changed, and the deck needs to accommodate it.
			May result in Deck moving on screen to keep edges from going offscreen.
		*/
		void cardSizeChanged( bool reposition );

		/////////////////////////////////////////////////////////////////////////

		/**
			Moves deck to @position on edge e. If edge == None, current edge will
			be used. Will negotiate with Manager to snap decks in proximity and
			will also negotiate decking.

			If the passed in position and edge result in a possible decking circumstance,
			setPosition will return a pointer to the deck that should be decked to. Its
			up to the caller to handle actual joining of the deck.
			If no decking should result from move, 0L is returned.

			if @checkCollisions is false, snapping and such won't be calculated. This
			is ideally only used when cards are sliding open and shut.
		*/
		Deck* setPosition(double position, ScreenEdge e, bool checkCollisions, int distanceFromEdge = -1);
		double position( void );

		void setEdge(ScreenEdge ScreenEdge);
		ScreenEdge edge( void );

		ScreenEdge adjacentEdge( void );
		int shapeMask( void );

		int breadth( void );
		int length( void );

		QRect screen( void );

		/**
			Looks at all cards in the deck and resizes deck to the widest, and
			sets all the cards elements to accommodate.
		*/
		void calculateDeckWidth( void );

		/**
			Calculates the width of a card as the widest of the card's elements (Tab, Tray, etc)
		*/
		int calculateCardWidth(Card *);

		/**
			Sets the widths of the elements in @card to @w
		*/
		void setCardWidth(Card *card, int w);

		/**
			Lets the card snap back to its preferred width (as calculated by calculateCardWidth)
		*/
		void restoreCardWidth(Card *);
		
		/**
		 	Sets the window manager up using KWin::setStrut so other apps maximise around cards
		*/
		void setWindowManager();
	protected:

		/**
			Determines the screen resolution. Useful for cards which
			follow screen edges ;)
		*/
		void cacheDesktopSize( void );

		/**
			Determines if this deck is up against a screen edge. E.g., a deck on TopEdge
			with a position of 0.0 is against LeftEdge. This is used to make a mask for
			the deck's pretty curved edges.
			If the edge state is different (or @force == true) than the last time this was
			called, _shapeMask will be set (as an ORing of ShapeCorners enum) and
			applyShapeMask will be called.
		*/
		void checkEdgeState( bool force = false );

		/**
			Uses _shapeMask to make appropriate calls to QWidget::setMask to make curved edges.
		*/
		void applyShapeMask( void );

		/////////////////////////////////////////////////////////////////////////

		LayoutSwitcher *_switcher;
		ScreenEdge _edge, _adjacentEdge;
		int _shapeMask;
		double _position;
		QRect _screen;
		QRegion _currentMask;
		bool _setWindowManager;
};

}; //namespace

#endif

