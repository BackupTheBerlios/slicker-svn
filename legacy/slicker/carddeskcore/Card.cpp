/***************************************************************************
                          Card.cpp  -  description
                             -------------------
    begin                : Mon Dec 23 2002
    copyRightEdge            : (C) 2002 by Shamyl Zakariya
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

#include "Card.h"
#include "CardApplet.h"
#include "Deck.h"
#include "LayoutSwitcher.h"
#include "CardElements.h"
#include "CardManager.h"
#include "Const.h"

#include <qlayout.h>
#include <qfile.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kstandarddirs.h>

#include "../share/pluginloader.h"
#include "../share/plugininfo.h"

/////////////////////////////////////////////////////////////////////////////
// Card
//
//		LayoutSwitcher *_switcher;
//
//		Tab *_tab;
//		Tray *_tray;
//		Separator *_separator;
//		Contents *_contents;
//
//		double _position;
//		ScreenEdge _edge;
//		QString _cardName;
//
//		Deck *_deck;
//		Card *_deckBefore, *_deckAfter;
//		CardApplet *_applet;


namespace CardDesk {

Card::Card( const PluginInfo &info, KConfigGroup *configGroup)
	: QWidget(), _switcher(0L), _tab(0L), _tray(0L), 		_separator(0L),
	 _contents(0L), _resizerRight(0L), _resizerLeft(0L), _deck(0L),
	 _deckBefore(0L), _deckAfter(0L), _cardId(QString::null)
{
	_contentsHeight = -1;
	_info = new PluginInfo(info);
	_edge = TopEdge;
	_position = 0.0;

	_applet = PluginLoader::pluginLoader()->loadApplet(info);

	if (_applet != 0L)
	{
		if (configGroup)
			loadConfig(*configGroup);
		if (_cardName == QString::null)
			_cardName = _applet->defaultCardName();
		_switcher = new LayoutSwitcher(this, _edge, false);  //true means verbose...
		_applet->attachToCard(this);
		createDeck();
		//applet()->setCardName();

		//this needs to be done early on
		Tab *tab = _applet->createTab(this);
		setTab(tab);
		//check to see if applet wants a tray
		Tray *tray = _applet->createTray(this);
		if (tray) setTray(tray);

		Separator *sep = _applet->createSeparator(this);
		Contents * cont = _applet->createContents(this);
		Resizer * resR = _applet->createResizer(this);
		Resizer * resL = _applet->createResizer(this);
		resL->setSide(false); //SET that the resizer is on the left side.

		setSeparator(sep);
		setContents(cont);
		setResizer(resR, resL);

		shut(false); //don't animate.
		applet()->cardReady();
		if (_contentsHeight > -1)
			cont->setPreferredHeight(_contentsHeight);

		setMouseTracking ( true );

		if (_deck)
			_deck->setWindowManager();
	}
}


Card::~Card()
{
	leaveDeck( false );

	Manager::instance()->unregisterCard(this);
	if (_switcher)
		delete _switcher;
	if (_applet)
		delete _applet;
	if (_resizerRight)
		delete _resizerRight;
	if (_resizerLeft)
		delete _resizerLeft;
}

bool Card::isOkay()
{
	return (_applet != 0L);
}

void Card::setCardName( QString name )
{
	emit nameChanged(this, _cardName, name);
	_cardName = name;
	if (_tab) _tab->repaint();
}

QString Card::cardName( void )
{
	return _cardName;
}

void Card::loadConfig(KConfigGroup &configGroup)
{
	_position = configGroup.readDoubleNumEntry("Position",0.0);
	_edge = StringToEdge(configGroup.readEntry("ScreenEdge", "TopEdge"));
	_contentsHeight = configGroup.readNumEntry("ContentsHeight", -1);
	_cardName = configGroup.readEntry("Name", QString::null);
}

void Card::saveConfig(KConfigGroup &configGroup)
{
	//it would probably be better if we wrote these two elsewhere
	configGroup.writeEntry("AppletConfigFile", _info->configFile());
	configGroup.writeEntry("AppletDesktopFile", _info->desktopFile());
	configGroup.writeEntry("Name", cardName());
	configGroup.writeEntry("Position", position());
	configGroup.writeEntry("ScreenEdge", EdgeToString(edge()));
	configGroup.writeEntry("ContentsHeight", _contents->storedHeight());
}

CardApplet *Card::applet( void )
{
	return _applet;
}


/////////////////////////////////////////////////////////////////////////

Deck *Card::deck( void )
{
	return _deck;
}

Card *Card::rootCard( void )
{
	return deck()->root();
}

bool Card::isRootCard( void )
{
	if (deck())	return (rootCard() == this);
	else
	{
		return false;
	}
}

bool Card::inDeck( void )
{
	/*
		all cards are in a (hidden) deck. But this method only returns
		true if we're in a populated deck
	*/

	if (isRootCard() && (deck()->count() == 1))
		return false;

	return true;
}

bool Card::joinDeckAfter( Card * card )
{
	if (!leaveDeck(false)) return false;

	if (!card->deck())
		card->createDeck();

	//deck reparents 'this'
	card->deck()->addCardAfter(card, this);
	_deck = card->deck();

	emit deckingChanged( this );
	return true;
}

bool Card::joinDeckBefore( Card * card )
{
	if (!leaveDeck(false)) return false;

	if (!card->deck())
		card->createDeck();

	//deck reparents 'this'
	card->deck()->addCardBefore(card, this);
	_deck = card->deck();

	emit deckingChanged( this );
	return true;
}

bool Card::leaveDeck( bool becomeFreeFloating )
{
	if (!deck())
	{
		return true; //technically this is OK
	}

	if (isRootCard() && (deck()->count() > 1))
	{
		//printf("Card[%s]::leaveDeck()\tRoot card of populate deck leaving\n", cardName().latin1());

		/*
			This should only occur when the root card of a deck is being deleted, or if
			the deck is being re-ordered.
		*/
		_deck->removeCard(this);
		reparent(0L, QPoint(0, 0), false);

		//get rid of my deck pointer.
		_deck = 0L;

		if (becomeFreeFloating)
		{
			createDeck();
		}

		emit deckingChanged( this );
		return true;
	}

	if (isRootCard() && (deck()->count() == 1))
	{
		/*
			hide me, to avoid flickering. Will be shown when reparented to new deck
		*/
		reparent(0L, QPoint(0, 0), false);
		delete _deck;
		_deck = 0L;

		if (becomeFreeFloating)
		{
			//printf("Card[%s]::leaveDeck()\tLogically problematic circumstance of" \
			// root of unpopulated deck leaving its own deck, then making a new one. Trouble?\n",
			//       cardName().latin1();

			createDeck();
		}

		emit deckingChanged( this );
		return true;
	}

	/*
		if we're here, card must be non-root in populated deck. Free to leave.
		if !becomeFreeFloating, we lose our deck here, and are hidden.
	*/

	_deck->removeCard(this);
	reparent(0L, QPoint(0, 0), false);
	_deck = 0L;

	if (becomeFreeFloating) createDeck();

	emit deckingChanged( this );
	return true;
}

void Card::createDeck( void )
{
	_deck = new Deck(this, desiredPosition(), edge()); //will reparent this to itself
	_deck->restoreCardWidth(this);
	setPosition(_position, _edge);
	deck()->show();
	emit deckingChanged( this );
}

void Card::createDeck( double position, ScreenEdge edge, int distanceFromEdge )
{
	_position = position;
	_edge = edge;

	_deck = new Deck(this, _position, _edge); //will reparent this to itself
	_deck->restoreCardWidth(this);
	setPosition(_position, _edge, distanceFromEdge);
	deck()->show();
	emit deckingChanged( this );
}



/////////////////////////////////////////////////////////////////////////


void Card::setTab( Tab *tab )
{
	if (_tab)
	{
		_switcher->removeWidget(_tab);
		delete _tab;
	}

	_tab = tab;
	insertCardElements();
}

Tab *Card::getTab( void )
{
	return _tab;
}

void Card::setTray( Tray *tray )
{
	if (_tray)
	{
		removeTray();
	}

	_tray = tray;
	insertCardElements();
}

void Card::removeTray( void )
{
	_switcher->removeWidget(_tray);
	delete _tray;
}

Tray *Card::getTray( void )
{
	return _tray;
}

void Card::setSeparator( Separator *sep )
{
	if (_separator)
	{
		_switcher->removeWidget(_separator);
		delete _tab;
	}

	_separator = sep;
	insertCardElements();
}

Separator * Card::getSeparator( void )
{
	return _separator;
}

void Card::setContents( Contents *contents )
{
	if (_contents)
	{
		_switcher->removeWidget(_contents);
		delete _tab;
	}

	_contents = contents;
	insertCardElements();
}

Contents *Card::getContents( void )
{
	return _contents;
}

void Card::setResizer( Resizer *resizerR, Resizer *resizerL )
{
	if (_resizerRight || _resizerLeft)
	{
		_switcher->removeWidget(_resizerRight);
		_switcher->removeWidget(_resizerLeft);
		delete _tab;
	}

	_resizerRight = resizerR;
	_resizerLeft = resizerL;
	insertCardElements();
}

Resizer *Card::getResizerL( void )
{
	return _resizerLeft;
}

Resizer *Card::getResizerR( void )
{
	return _resizerRight;
}

void Card::open( bool animate )
{
	if (!getSeparator() || !getContents()) return;

	if (!isOpen())
	{
		getSeparator()->expand(true);
		getResizerL()->expand(animate);
		getResizerR()->expand(animate);
		getContents()->expand(animate); //layout will occur now
		elementSizeChanged(true);
		wasOpened();
	}
}

void Card::shut( bool animate )
{
	if (!getSeparator() || !getContents()) return;

	if (isOpen())
	{
		getSeparator()->collapse(false);
		getResizerR()->collapse(!animate);
		getResizerL()->collapse(!animate);
		getContents()->collapse(!animate); //layout will occur now
		elementSizeChanged(true);
		wasShut();
	}
}

bool Card::isOpen( void )
{
	return getContents() ? getContents()->isOpen() : false;
}

int Card::breadth( void )
{
	switch (edge())
	{
		case TopEdge: case BottomEdge:
			return size().width();
			break;

		case RightEdge: case LeftEdge:
			return size().height();
			break;

		default: break;
	}

	return -1; //should never occur
}

int Card::length( void )
{
	switch (edge())
	{
		case TopEdge: case BottomEdge:
			return size().height();
			break;

		case RightEdge: case LeftEdge:
			return size().width();
			break;

		default: break;
	}

	return -1; //should never occur
}

void Card::elementSizeChanged( bool doReposition )
{
	_switcher->relayout();
	setMinimumSize(minimumSizeHint());

	if (deck())	deck()->cardSizeChanged(doReposition);
}

void Card::repaintElements( void )
{
	if (_tab) _tab->repaint(false);
	if (_tray) _tray->repaint(true);
	if (_separator) _separator->repaint(true);
	if (_contents) _contents->repaint(true);
	if (_resizerRight) _resizerRight->repaint(true);
	if (_resizerLeft) _resizerLeft->repaint(true);
}

/////////////////////////////////////////////////////////////////////////////

void Card::setPosition(double pos, ScreenEdge e, int distanceFromEdge)
{
	if (pos < 0.0) pos = 0;
	if (pos > 1.0) pos = 1.0;

	_deckBefore = 0L;
	_deckAfter = 0L;

	_position = pos;
	if (e == None) e = edge();

	if (isRootCard())
	{
		//set position, and check for collisions
		Deck *deckTo = deck()->setPosition(_position, e, true, distanceFromEdge);
		if (deck()->empty())
		{
			if (deckTo)
			{
				if (distanceFromEdge == -1 || (deck() && (deck()->count() > 1))  || (!deck()))
				{
					/*
						we're a loner card and have been told we can deck
						distanceFromEdge == -1 if card is being positioned programmatically
						rather than via drag.
					*/
					joinDeckAfter(deckTo->tail());
				}
				else
				{
					Card *c = deckTo->cardForDistanceFromEdge(distanceFromEdge);
					if (c)
					{
						//printf("Would deck after card %s\n", c->cardName().latin1());
						_deckAfter = c;
						_deckBefore = 0L;
					}
					else
					{
						//printf("Would become root, decking before %s\n", deckTo->root()->cardName().latin1());
						_deckBefore = deckTo->root();
						_deckAfter = 0L;
					}
				}
			}
		}
		else
		{
			//nothing -- root of populated deck can't deck to another deck
		}
	}
	else
	{

		/*
			Non root card making a move. We must determine if the move passes decking threshold
			to leave the deck. Otherwise, stay put. First check is easy, switching edges:
		*/

		bool leave = false;

		if (e != edge())
		{
			//printf("Card[%s]::setPosition()\tNon-root deck member, switching edges. leave = true\n",
			//       cardName().latin1());

			_edge = e;
			leave = true;
		}

		if (!leave)
		{
			/*
				Otherwise, we have to calculate what Manager::deckingThreshold
				equates to in our positioning units.
			*/

			QRect screen = deck()->screen();
			double threshold = 0;
			switch( edge() )
			{
				case TopEdge: case BottomEdge:
				{
					threshold = (double) Manager::instance()->deckingThresholdFor(deck()) / (double) (screen.width() - deck()->breadth());
					break;
				}

				case RightEdge: case LeftEdge:
				{
					threshold = (double) Manager::instance()->deckingThresholdFor(deck()) / (double) (screen.height() - deck()->breadth());
					break;
				}

				default: break;
			}

			double deckPos = deck()->position();
			if ( ((_position - deckPos) > threshold) || ((deckPos - _position) > threshold) )
			{
				//printf("Card[%s]::setPosition()\tNon-root deck mamber, moving > threshold. leave = true\n",
				//			 cardName().latin1());
				leave = true;
			}
		}

		if (leave)
		{
			leaveDeck(true); //will automatically call setPosition() on new position
			return; //leave now, we don't want to send multiple positionChanged signals.
		}
	}

	deck()->checkEdgeState();
	emit positionChanged(this, position(), _position, (int)edge());
}

void Card::draggingStart( void )
{
	_deckBefore = 0L;
	_deckAfter = 0L;
}

void Card::draggingDone( void )
{
	if (_deckBefore)
	{
		joinDeckBefore(_deckBefore);
	}

	if (_deckAfter)
	{
		joinDeckAfter(_deckAfter);
	}

	_deckBefore = 0L;
	_deckAfter = 0L;
}


double Card::position( void )
{
	if (deck()) return deck()->position();

	return desiredPosition();
}

double Card::desiredPosition( void )
{
	return _position;
}

void Card::setEdge( ScreenEdge e )
{
	ScreenEdge oldEdge = _edge;

	_edge = e;

	//notify all the elements on the card that our ScreenEdge has changed.
	QPtrList<QWidget> elementList = _switcher->elements();
	for (CardElement *i = (CardElement *) elementList.first(); i; i = (CardElement *) elementList.next())
		i->cardOrientationChanged();

	_switcher->setEdge(edge());
	setMinimumSize(minimumSizeHint());

	if (e != oldEdge)
		emit edgeChanged( this );
}

ScreenEdge Card::edge( void ) const
{
	return _edge;
}

void Card::appletPropertiesChanged( void )
{
	if (getTab())
	{
		getTab()->delegatePropertiesChanged();
	}
}

void Card::followDeck( double position, ScreenEdge e)
{
	_position = position;
	_edge = e;
	emit positionChanged(this, _position, position, e);
}


void Card::insertCardElements( void )
{
	_switcher->removeAll(false);
	if (_tray) _switcher->addWidget(_tray, false);
	if (_resizerLeft) _switcher->addWidget(_resizerLeft,false);
	if (_contents) _switcher->addWidget(_contents, false);
	if (_resizerRight) _switcher->addWidget(_resizerRight,false);
	if (_separator) _switcher->addWidget(_separator, false);
	if (_tab) _switcher->addWidget(_tab, false);

	//this makes sure everything is rotated & sized correctly
	setEdge(edge());
}

void Card::wasShut( void )
{
	emit closed(this);
}

void Card::wasOpened( void )
{
	emit opened(this);
}

bool Card::destroy( void )
{
	if (applet()->cardDestroy())
	{
		//delete config file
		if (!(_info->configFile().isEmpty() || _info->unique()))
		{
			if( QFile::exists( locate( "config", _info->configFile() ) ) )
				QFile::remove( locate( "config", _info->configFile() ) );
		}
		return true;
	}
	return false;
}

}; //namespace
