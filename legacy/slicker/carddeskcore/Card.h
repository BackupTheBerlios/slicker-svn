/***************************************************************************
                          Card.h  -  description
                             -------------------
    begin                : Mon Dec 23 2002
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

#ifndef CARD_H
#define CARD_H

#include <qwidget.h>
#include <qframe.h>
#include <qptrlist.h>
#include <qptrvector.h>
#include <qrect.h>

#include <kconfigbase.h>
#include <kconfig.h>

#include "Const.h"

class KConfig;
class PluginInfo;

namespace CardDesk {

class Tab;
class Tray;
class Separator;
class Contents;
class LayoutSwitcher;
class Deck;
class CardApplet;
class Resizer;
/**
  *@author Shamyl Zakariya
  */

class Card : public QWidget
{
	friend class Deck;
	friend class CardElement;

	Q_OBJECT
	public:


		Card(const PluginInfo &, KConfigGroup *configGroup = 0L);
	
		/**
			Deletes this card. If the card is in a deck, the card will leave the deck first.
			Also removes card from Manager.
		*/
		~Card();

		bool isOkay();
		
		/**
			Changes the name of the card.
		*/
		void setCardName( QString name );

		/**
			Returns the name of the card.
		*/
		QString cardName( void );

		/**
			Returns a pointer to this card's applet
		*/
		CardApplet *applet( void );


		/////////////////////////////////////////////////////////////////////////
		// Deck related methods. You probably won't be calling these directly.

		/**
			Returns a pointer to the deck object this card is in.
			NB: *All* cards have a deck. Even lonely cards which aren't decked
			with others. When a card joins another card's deck, it deletes its own
			and becomes a child of that other card's deck.
		*/
		Deck *deck( void );

		/**
			Returns a pointer to the card which is root of this card's deck. If the
			card isn't in a deck (!inDeck() == true) rootCard == this
		*/
		Card * rootCard( void );

		/**
			Returns true if this card is root card of the deck. True for cards which aren't
			in a deck, as a lonely card is root of its own deck.
		*/
		bool isRootCard( void );

		/**
			Returns true if this card's deck has other cards as well. This is
			semantic twiddling, yes.
		*/
		bool inDeck( void );

		/*
			Joins the deck for target card @card, in order after @card.
			returns true on success
		*/
		bool joinDeckAfter( Card * card );

		/*
			Similar to above, joins @card's deck, but directly before @card. If @card
			is root for the deck, this becomes the new root.
			returns true on success
		*/
		bool joinDeckBefore( Card * card );

		/*
			Removes this card from whatever deck its in, including its own
			if it's root in an unpopulated deck. if @becomeFreeFloating is true,
			card will make a new deck for itself, otherwise, it will be reparented to
			0L and hidden. The former case is for leaving a deck to become a new
			root card on a ScreenEdge. The latter is for when a card is about to join a deck.

			returns true on success.
		*/
		bool leaveDeck( bool becomeFreeFloating );

		/*
			::leaveDeck will leave a card with a null deck. If another card attempts
			to become child of that card, it needs to make a new deck.
		*/
		void createDeck( void );

		/**
			Same as above, but puts card at @position, @edge & @distanceFromEdge
		*/
		void createDeck( double position, ScreenEdge edge, int distanceFromEdge );

		/////////////////////////////////////////////////////////////////////////
		// CardElement related methods. Allow for setting up a Tab, Tray Separator
		// and Contents elements.

		/**
			Sets the Tab implementation for this card. If there already is a Tab, it
			is removed and deleted.
		*/
		void setTab( Tab * );
		Tab *getTab( void );

		/**
			Sets the Tray implementation for this card. If there already is on, it
			is removed and deleted.
		*/
		void setTray( Tray * );

		/**
			The Tray is the only optional element. So here's a method for removing it.
		*/
		void removeTray( void );
		Tray *getTray( void );

		/**
			Sets the separator for this card. The separator is the widget *between* the always
			visible top half and the cometimes hidden contents. The separator can be used to
			resize the card in place.
		*/
		void setSeparator( Separator * );
		Separator * getSeparator( void );

		/**
			Sets the contents implementation for this card. If there already is one, it
			is deleted and replaced with the new one.
		*/
		void setContents( Contents * );
		Contents *getContents( void );
		/** 
			Set the resizer mplementation for this card. If there already is one, it
			is deleted and replaced with the new one.
		*/
		void setResizer( Resizer * , Resizer *);
		Resizer *getResizerR( void );
		Resizer *getResizerL( void );
		/**
			Opens the card, by showing the separator & contents.
		*/
		void open( bool animate = true );

		/**
			Shuts the card, by hiding the separator & contents.
		*/
		void shut( bool animate = false );

		/**
			Returns true if the card is open.
		*/
		bool isOpen( void );

		/**
			Returns the width of the card against the edge it is attached.
		*/
		int breadth( void );

		/**
			Returns the length of the card perpendicular to the edge it is attached.
		*/
		int length( void );

		/*
			This is to be called by Tab, Tray, Separator or Contents
			classes to notify that their size has changed (for whatever reason).
			Triggers a relayout of the card, and, if @doRepositioning, the deck
			will do positioning calculations to make sure everything fits and that
			the deck is against a screen edge. Normally, you want to set
			@doRepositioning to true, but in the case of animated resizing, leave
			it off (for speed) and call this once when done with it true to force a
			proper position.
		*/
		void elementSizeChanged( bool doRepositioning );

		/*
			This is called by Deck to notify that the elements should be
			repainted. This occurs generally after the deck's shape masking has changed.
		*/
		void repaintElements( void );

		/////////////////////////////////////////////////////////////////////////

		/**
			Moves card to @position on @edge.
			distanceFromEdge is used for in-deck reordering,and is handled internally
			by Tab::DragLogic class. It is sufficient for general use to ignore
			distanceFromEdge and just call
			myCard->setPosition(position, edge);

			If @edge == None (default) card will be positioned on its current edge.
		*/
		void setPosition(double position, ScreenEdge ede = None, int distanceFromEdge = -1);

		/**
			By and large, card positioning is done through the bove method, setPosition. But
			live dragging adds some complications, so when mouse dragging starts, draggingStart
			is called by Tab::DragLogic. When done, draggingDone() is called. This is used
			primarily to handle deck reordering via drag.
		*/
		virtual void draggingStart( void );
		virtual void draggingDone( void );

		/**
			Returns the actual position of the card (as on screen, taking into acount
			snapping and spacing and whatnot).
			position units are from 0.0 to 1.0:
			TopEdge: left side to right side
			RightEdge: top to bottom
			BottomEdge right to left
			LeftEdge bottom to top.
		*/
		double position( void );

		/**
			Returns the position the card intended to be in. May or may not be same as position()
			The reason for a difference is that the card may not have been allowed to slide to
			the desired position do to a collision with another card.
		*/
		double desiredPosition( void );

		/**
			Moves the card from its current wdge to the new @edge. position is left the same.
		*/
		void setEdge( ScreenEdge edge );

		/**
			Returns the current edge the card is on.
		*/
		ScreenEdge edge( void ) const;

		void loadConfig( KConfigGroup &);
		void saveConfig( KConfigGroup &);

		QString cardId() const { return _cardId;  }
		void setCardId(const QString &id) { _cardId = id; }

		PluginInfo *pluginInfo() const { return _info; }
	public slots:

		/**
			Signals that the applet changed some properties. In this case, it only
			handles Tab::delegatePropertiesChanged.
		*/
		virtual void appletPropertiesChanged( void );


		/**
			Called when a card is about to be destroyed.  Retruns true to carry on with destruction, false to halt it
		*/

		bool destroy( void );

	signals:

		/**
			Emitted whenever a card moves.
			Captured and forwarded by Manager::instance(). If you're interested in this signal, listen to
			Manager::positionChanged.
		*/
		void positionChanged( Card *, double actualPosition, double desiredPosition, int edge );

		/**
			Emitted when a card joins a deck, leaves deck, or changes position in a deck.
			Captured and forwarded by Manager::instance(). If you're interested in this signal,
			listen for Manager::deckingChanged
		*/
		void deckingChanged( Card * );

		/**
			Emitted when the edge of a card is changed, allowing listeners to adjust accordingly, such
			as relayout their components.
		*/
		void edgeChanged( Card * );

		/**
			Someday cards might have the option of not being friendly to decking. If a card desides
			it doesn't want to be in a deck, it will leave and emit this signal.
			Captured by Manager::instance()
		*/
		void deckingRulesChanged( Card * );

		/**
			Emitted when this card opens.
		*/
		void opened( Card * );

		/**
			Emitted when card is shut.
		*/
		void closed( Card * );

		/**
			Emitted when the name of the card changes.
		*/
		void nameChanged(Card *, QString oldName, QString newName );


	protected:

		/*
			followDeck is called by the card's deck if and only if:
				-this card is not root
				-the deck has moved.

			This is done to make sure this card's position is kept in sync with its
			fellows in the deck. This will be called by the root card's deck, and
			will emit positionChanged signal.
		*/
		void followDeck( double position, ScreenEdge e);


		/**
			INTERNAL!
			Inserts your tab, tray, separator and contents, in order, if available.
			Don't attempt to call this. Used as broker to LayoutSwitcher.
		*/
		void insertCardElements( void );

		/**
			Called when card has just been shut. INTERNAL. Don't call it.
		*/
		void wasShut( void );

		/**
			Called when card has just been opened. INTERNAL. Don't call it.
		*/
		void wasOpened( void );

		/////////////////////////////////////////////////////////////////////////


		PluginInfo *_info;
		LayoutSwitcher *_switcher;

		Tab *_tab;
		Tray *_tray;
		Separator *_separator;
		Contents *_contents;
		Resizer *_resizerRight;
		Resizer *_resizerLeft;
				
		double _position;
		ScreenEdge _edge;
		QString _cardName;
		QString _cardId;

		Deck *_deck;
		Card *_deckBefore, *_deckAfter;

		CardApplet *_applet;
		
		
		int _contentsHeight;
};

typedef QPtrList<Card> CardList;
typedef QPtrListIterator<Card> CardListIterator;
typedef QPtrVector<Card> CardVector;

}; //namespace

#endif


