/***************************************************************************
                          Deck.cpp  -  description
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

#include "Deck.h"
#include "Card.h"
#include "LayoutSwitcher.h"
#include "CardManager.h"
#include "CardElements.h"

#include <qdesktopwidget.h>
#include <qregion.h>
#include <kwin.h>
#include <kdebug.h>

static const int DeckFlags = Qt::WStyle_Customize | Qt::WDestructiveClose
                           | Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder
                           | Qt::WRepaintNoErase;

static const int TestingDeckFlags = Qt::WDestructiveClose | Qt::WStyle_StaysOnTop;


namespace CardDesk {
/////////////////////////////////////////////////////////////////////////////
// class Deck
//		LayoutSwitcher *_switcher;
//		ScreenEdge _edge, _adjacentEdge;
//		double _position;
//		int _shapeMask;
//		QRect _screen;

Deck::Deck(Card *rootCard, double position, ScreenEdge edge)
	: QWidget(0L, "VanillaDeck", DeckFlags),
	_switcher(0L), _edge(edge), _adjacentEdge(None),
	_shapeMask(0), _position(position)
{
	//basic WM setup -- self explanatory.
	KWin::setOnAllDesktops(winId(), true);
	KWin::setState(winId(), NET::StaysOnTop | NET::SkipTaskbar | NET::SkipPager);

	_setWindowManager = false;


	//store screen rez
	cacheDesktopSize();
	setCaption(rootCard->name());

	rootCard->reparent(this, QPoint(0, 0));

	_switcher = new LayoutSwitcher(this, edge, false);
	_switcher->addWidget(rootCard, true);
}

void Deck::addCardAfter(Card *after, Card *card)
{
	if (card->parentWidget() != this)
	{
		card->reparent(this, QPoint(0, 0));
	}

	int afterIndex = _switcher->elements().find(after);
	if (afterIndex == -1)
	{
		_switcher->elements().append(card);
	}
	else
	{
		_switcher->elements().insert(++afterIndex, card);
	}

	calculateDeckWidth();

	_switcher->relayout();
	setPosition(_position, _edge, true);
	checkEdgeState(true);
	setWindowManager();
}

void Deck::addCardBefore(Card *before, Card *card)
{
	if (card->parentWidget() != this)
	{
		card->reparent(this, QPoint(0, 0));
	}

	int beforeIndex = _switcher->elements().find(before);
	if (beforeIndex == -1)
	{
		_switcher->elements().insert(0, card);
	}
	else
	{
		_switcher->elements().insert(beforeIndex, card);
	}

	calculateDeckWidth();

	_switcher->relayout();
	setPosition(_position, _edge, true);
	checkEdgeState(true);
	setWindowManager();
}

void Deck::removeCard(Card *card)
{
	_switcher->removeWidget(card);

	calculateDeckWidth();
	setPosition(_position, _edge, true);
	checkEdgeState(true);
	setWindowManager();
}

int Deck::count()
{
	return _switcher->elements().count();
}

Card *Deck::cardAt(int which)
{
	//safe, as we're dealing with a homogenous group
	return (Card *) _switcher->elements().at(which);
}

Card *Deck::cardForDistanceFromEdge( int distanceFromEdge )
{
	if (distanceFromEdge <= root()->length() / 2) return 0L;
	if (distanceFromEdge >= length() - (tail()->length() / 2)) return tail();

	Card *target = 0L;
	int cCount = count();
	int previousCenter = root()->length() / 2, currentCenter = 0;
	for (int i = 1; i < cCount; i++)
	{
		Card *card = cardAt(i);
		int cardLength = card->length();
		currentCenter = previousCenter + (cardAt( i - 1)->length() / 2) + (cardLength / 2);

		if (distanceFromEdge >= previousCenter && distanceFromEdge < currentCenter)
		{
			target = cardAt( i - 1);
			break;
		}

		previousCenter = currentCenter;
	}

	return target;
}

int Deck::indexOf(Card *c)
{
	int cCount = count();
	for (int i = 0; i < cCount; i++)
	{
		if (cardAt(i) == c)
		{
			return i;
		}
	}

	return -1;
}


Card *Deck::root( void )
{
	return (Card *) _switcher->elements().at(0);
}

Card *Deck::tail( void )
{
	return cardAt(count() - 1);
}

void Deck::cardSizeChanged( bool reposition )
{
	calculateDeckWidth();
	_switcher->relayout();
	setFixedSize(minimumSizeHint());

	if (reposition)
	{
		//adjust position to accommodate, but don't do collision checking
		setPosition(position(), edge(), false);
	}

	checkEdgeState();
}

/////////////////////////////////////////////////////////////////////////////

Deck* Deck::setPosition(double pos, ScreenEdge e, bool checkCollisions, int distanceFromEdge)
{
	if (pos < 0) pos = 0;
	else if (pos > 1.0) pos = 1.0;

	_position = pos;

	if (e != None)
	{
		setEdge(e);
	}

	Deck *deckTo =  Manager::instance()->positionDeck(this, _position, _edge,
	                                                  _screen, checkCollisions,
	                                                  distanceFromEdge);

	//now, propagate movement to all cards in this deck (except root, since it already knows)
	int nCards = count();
	for (int i = 1; i < nCards; i++)
	{
		Card *card = cardAt(i);
		if (card) card->followDeck(_position, _edge);
	}

	return deckTo;
}

double Deck::position( void )
{
	return _position;
}

void Deck::setEdge(ScreenEdge e)
{
	bool edgeChanged = false;
	if (_edge != e)
		edgeChanged = true;

	_edge = e;

	//iterate through all cards in this deck and set their edge
	QPtrList<QWidget> elementList = _switcher->elements();
	for (Card *i = (Card *) elementList.first(); i; i = (Card *) elementList.next())
		i->setEdge( edge() );

	_switcher->setEdge(edge());
	setFixedSize(minimumSizeHint());
	checkEdgeState(true);

	if (edgeChanged)
		setWindowManager();
}

ScreenEdge Deck::edge( void )
{
	return _edge;
}

ScreenEdge Deck::adjacentEdge( void )
{
	return _adjacentEdge;
}

int Deck::shapeMask( void )
{
	return _shapeMask;
}

int Deck::breadth( void )
{
	switch (edge())
	{
		case TopEdge: case BottomEdge:
			return frameGeometry().width();
			break;

		case RightEdge: case LeftEdge:
			return frameGeometry().height();
			break;

		default: break;
	}

	return -1; //should never occur
}

int Deck::length( void )
{
	switch (edge())
	{
		case TopEdge: case BottomEdge:
			return frameGeometry().height();
			break;

		case RightEdge: case LeftEdge:
			return frameGeometry().width();
			break;

		default: break;
	}

	return -1; //should never occur
}

QRect Deck::screen( void )
{
	return _screen;
}


void Deck::cacheDesktopSize()
{
	QDesktopWidget dw;
	if (_setWindowManager)
		_screen = dw.screenGeometry();
	else
		_screen = dw.availableGeometry();

}

void Deck::checkEdgeState( bool force )
{
	int shapeMask = 0;

	switch (edge())
	{
		case TopEdge:
		{
			if (_position == 0.0)
			{
				shapeMask = BottomRight;
				_adjacentEdge = LeftEdge;
			}
			else if (_position == 1.0)
			{
				shapeMask = BottomLeft;
				_adjacentEdge = RightEdge;
			}
			else
			{
				shapeMask = BottomLeft | BottomRight;
				_adjacentEdge = None;
			}
			break;
		}

		case RightEdge:
		{
			if (_position == 0.0)
			{
				shapeMask = BottomLeft;
				_adjacentEdge = TopEdge;
			}
			else if (_position == 1.0)
			{
				shapeMask = TopLeft;
				_adjacentEdge = BottomEdge;
			}
			else
			{
				shapeMask = BottomLeft | TopLeft;
				_adjacentEdge = None;
			}
			break;
		}

		case BottomEdge:
		{
			if (_position == 0.0)
			{
				shapeMask = TopLeft;
				_adjacentEdge = RightEdge;
			}
			else if (_position == 1.0)
			{
				shapeMask = TopRight;
				_adjacentEdge = LeftEdge;
			}
			else
			{
				shapeMask = TopLeft | TopRight;
				_adjacentEdge = None;
			}
			break;
		}

		case LeftEdge:
		{
			if (_position == 0.0)
			{
				shapeMask = TopRight;
				_adjacentEdge = BottomEdge;
			}
			else if (_position == 1.0)
			{
				shapeMask = BottomRight;
				_adjacentEdge = TopEdge;
			}
			else
			{
				shapeMask = TopRight | BottomRight;
				_adjacentEdge = None;
			}
			break;
		}

		default: break;
	}

	if (shapeMask != _shapeMask || force)
	{
		_shapeMask = shapeMask;
		applyShapeMask();
	}
}

void Deck::applyShapeMask( void )
{
	QRegion mask(0, 0, width(), height());

	int r(width());
	int b(height());

	// Remove top-left corner.
	if (_shapeMask & TopLeft)
	{
		mask -= QRegion(0, 0, 5, 1);
		mask -= QRegion(0, 1, 3, 1);
		mask -= QRegion(0, 2, 2, 1);
		mask -= QRegion(0, 3, 1, 2);
	}

	// Remove top-right corner.
	if (_shapeMask & TopRight)
	{
		mask -= QRegion(r - 5, 0, 5, 1);
		mask -= QRegion(r - 3, 1, 3, 1);
		mask -= QRegion(r - 2, 2, 2, 1);
		mask -= QRegion(r - 1, 3, 1, 2);
	}

	// Remove bottom-left corner.
	if (_shapeMask & BottomLeft)
	{
		mask -= QRegion(0, b - 5, 1, 3);
		mask -= QRegion(0, b - 3, 2, 1);
		mask -= QRegion(0, b - 2, 3, 1);
		mask -= QRegion(0, b - 1, 5, 1);
	}

	// Remove bottom-right corner.
	if (_shapeMask & BottomRight)
	{
		mask -= QRegion(r - 5, b - 1, 5, 1);
		mask -= QRegion(r - 3, b - 2, 3, 1);
		mask -= QRegion(r - 2, b - 3, 2, 1);
		mask -= QRegion(r - 1, b - 5, 1, 2);
	}

	setMask(mask);

		/*
			now that the edge state has changed, make all cards in this
			deck repaint. They'll read the adjacentEdge() property to
			figure this stuff out.
		*/

	if (mask != _currentMask)
	{
		int cCount = count();
		for (int i = 0; i < cCount; i++)
		{
			Card *c = cardAt(i);
			if (c) c->repaintElements();
		}
	}

	_currentMask = mask;

}

void Deck::calculateDeckWidth( void )
{
	int maxWidth = -1;
	int cCount = count();
	for (int i = 0; i < cCount; i++)
	{
		int cW = calculateCardWidth(cardAt(i));
		if (cW > maxWidth) maxWidth = cW;
	}

	//now, resize all cards to this width
	for (int i = 0; i < cCount; i++)
	{
		setCardWidth(cardAt(i), maxWidth);
	}
}

int Deck::calculateCardWidth(Card *c)
{
	int cWidth = -1;
	if (c->getTab()) cWidth = c->getTab()->actualPreferredWidth();
	if (c->getTray() && c->getTray()->actualPreferredWidth() > cWidth) cWidth = c->getTray()->actualPreferredWidth();
	if (c->getSeparator() && c->getSeparator()->actualPreferredWidth() > cWidth) cWidth = c->getSeparator()->actualPreferredWidth();
	if (c->getContents() && c->getContents()->actualPreferredWidth() > cWidth) cWidth = c->getContents()->actualPreferredWidth();

	return cWidth;
}

void Deck::setCardWidth(Card *c, int width)
{
	if (c->getTab()) c->getTab()->setDeckOverrideWidth(width, false);
	if (c->getTray()) c->getTray()->setDeckOverrideWidth(width, false);
	if (c->getSeparator()) c->getSeparator()->setDeckOverrideWidth(width, false);
	if (c->getContents()) c->getContents()->setDeckOverrideWidth(width, false);
}

void Deck::restoreCardWidth(Card *c)
{
	//figure out the widest element in card
	int w = calculateCardWidth(c);
	setCardWidth(c, w);
}

void Deck::setWindowManager()
{
		if (_setWindowManager)
		{
		  kdDebug(155003) << "Deck::setWindowManager" << endl;
        NETStrut strut;
        kdDebug(155003) << "Strut setting: length() = " << length() << endl;
        kdDebug(155003) << "Strut setting: mapToGlobal() = " << mapToGlobal(QPoint(0,0)).y() << endl;
        kdDebug(155003) << "Strut setting: top = " << length() + mapToGlobal(QPoint(0,0)).y() << endl;
        if (_edge == TopEdge) {
                strut.top = length() + mapToGlobal(QPoint(0,0)).y();
        }
        else if (_edge == BottomEdge) {
                strut.bottom = length();
        }
        else if (_edge == LeftEdge) {
                strut.left = width() + mapToGlobal(QPoint(0,0)).x();
        }
        else if (_edge == RightEdge) {
                strut.right = width();
        }
        KWin::setStrut( winId(), strut.left, strut.right,  strut.top, strut.bottom );
		}


}

}; //namespace




