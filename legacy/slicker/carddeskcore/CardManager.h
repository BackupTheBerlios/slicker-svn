/***************************************************************************
                          Manager.h  -  description
                             -------------------
    begin                : Thu Dec 26 2002
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

#ifndef Manager_H
#define Manager_H

#include <qobject.h>

#include "Card.h"


namespace CardDesk {
/*!
	Manager is a singleton managerial class for cards.  SlickerCardManager inherits this function to add plugin functionality and to support the saving of cards between slicker sessions.
*/

class Manager : public QObject
{
	Q_OBJECT
	public:
		~Manager( void );

		/**
			As Manager is a singleton, and the constructor is private, the only
			way to get the Manager is through instance(). The first time ::instance() is
			called, a manager will be created. Subsequent calls will return a pointer to it.
		*/
		static Manager *instance( void );
		static void useImplementation( Manager *);

		/**
			Will reload the cards which were saved in the previous session.
			This is a good thing to put in your main.cpp, or early on in
			your program.
		*/
		virtual void loadCards( void );

		/////////////////////////////////////////////////////////////////////////

		/**
			Returns a CardList (QPtrList<Card>) of all the
			cards for screen @screen on @ScreenEdge.

			If @ScreenEdge is left at None, all cards will be returned.
		*/
		virtual CardList cards( ScreenEdge ScreenEdge = None );

		/**
			Returns a list of all cards which are either
				root card in a deck, or not in a deck. E.g., cards
				which touch the screen ScreenEdge.
		*/
		virtual CardList rootCards( void );

		/**
			Returns a list of cards which aren't in a deck
		*/
		virtual CardList lonerCards( void );

		/**
			Returns card by name or 0L if no card has that name.
		*/
		virtual Card *card(QString name);

		/**
			Returns the number of cards.
		*/
		virtual int countCards( void );

		/**
			Writes cards and their decking info to stdout. Will prepend @message.
		*/
		virtual void enumerateCardsToStdout( QString message = QString::null );

		/**
			Allows applet to clean up any stuff it may have left about, then destroys card
		*/

		virtual void destroyCard(Card *);

		/////////////////////////////////////////////////////////////////////////

		/**
			When a card wants to delete itself it calls Manager::instance()->suicide(this);
			I did this because I'm no certaitn that it's good programming style for a card
			implementation to call

			void MyCard::die()
			{
				delete this;
			}

			Technically, this might work. But it's a bad idea ;)
		*/
		virtual void suicide( Card * );

		/////////////////////////////////////////////////////////////////////////

		/**
			Positions deck at @desiredPos along @desiredEdge using @screenGeo (for screen rez).
			Will not let decks overlap, unless desiredPos of this deck is within a threshold of
			the target deck's position(). In that case, positionDeck() will return a pointer to the deck
			that this deck should deck to. Unless decking threshold is met, this method returns OL.

			In the case of "snapping" to positions, @desiredPos is overwritten.

			if @checkCollision is false, collision checks won't be made. The card will simply be positioned
			wherever desiredPos points to. This may result in overlap. Will always return 0L in this circumstance.
		*/

		virtual Deck *positionDeck(Deck *deck, double &desiredPos, ScreenEdge desiredEdge,
		                   QRect &screenGeo, bool checkCollision, int distanceFromEdge = -1);

		/**
			Takes a known frame @frame and known edge @edge and figures out its position, which
			is returned.
		*/
		virtual double positionForFrame(QRect frame, ScreenEdge edge, QRect &screen);

		/**
			Returns how far apart (int pixels) cards which are adjacent will be spaced
		*/
		virtual int spacing( void );

		/**
			Set the space between two cards
		*/
		virtual void setSpacing( int space );

		/**
			Returns how far apart in pixels the center point of two cards would have to be within
			for those two cards to deck.
		*/
		virtual int deckingThreshold( void );

		/**
			Returns how far apart in pixels a deck would have to be from the deck passed in
			for docking to occur.
		*/
		virtual int deckingThresholdFor( Deck * );

 signals:

		/*
			Emitted when a card is created.
		*/
		void cardRegistered(QString name, Card *card);

		/*
			@card is *about* to die. This is your last chance to do something with it.
		*/
		void cardUnregistered(QString name, Card *card);

		/**
			Emitted when a card is opened.
			@card -- the card which opened.
		*/
		void cardOpened(Card *card);

		/**
			Emitted when a card is closed.
			@card -- the card which closed.
		*/
		void cardClosed(Card *card);

		/**
			Emitted when the decking changes, for any/all card(s).
		*/
		void deckingChanged( void );

		void deckingRulesChanged( void );

		/**
			Emitted when a card is renamed.
			@card -- the card which was renamed.
			@oldName -- the old name of the card (pre-rename)
			@newName -- the card's new name.
		*/
		void cardRenamed(Card *card, QString oldName, QString newName);

		/**
			Emitted whenever a card moves.
			@card -- the moving card.
			@actualPosition -- the position the card moved to.
			@desiredPosition -- the position the card wanted to move to. May be different
					than @actualPosition due to prevention of card overlapping.
			@edge -- the edge the card moved to.
		*/
		void cardMoved( Card *card, double actualPosition, double desiredPosition, int edge );

		/**
			Emitted when the last card has been removed/deleted.
		*/
		void lastCardUnregistered( void );

	public slots:

		/**
			Brings up a dialog to work with user to create a new
			card from available plugins.
			This is attached to the default context menu when right
			clicking on a tab.
		*/
		virtual Card *newCardDialog( void );

		/**
			Brings up a configuration dialog for CardDesk.
			This is attached to the default context menu when right
			clicking on a tab.
		*/
		virtual void configureDialog( void );


		virtual void slotDeckingModified( Card * );
		virtual void slotDeckingRulesModified( Card * );
		virtual void slotCardOpened(Card *);
		virtual void slotCardClosed(Card *);
		virtual void slotCardRenamed(Card *, QString oldName, QString newName);
		virtual void slotCardMoved( Card *, double actualPosition, double desiredPosition, int edge );

		virtual void slotShutdown( void );

	protected slots:

		/**
			The suicide() method doesn't directly delete the card -- instead it pushes
			the card into a stack, and sets a timeout to call this.
		*/
		virtual void deleteMoribundCards( void );

	protected:

		friend class Card;
		/**
			Called in the constructor for Card. Registers it with Manager
		*/
		virtual void registerCard(Card *);

		/**
			Called in the destructor for Card. Unregisters it.
		*/
		virtual void unregisterCard(Card *);

	protected:

		Manager( void );
		CardList _cards;
		CardList _moribundCards;
		int mSpace;

	private:

		static Manager *_instance;
};

}; //namespace

#endif
