/***************************************************************************
                          DragLogic.cpp  -  description
                             -------------------
    begin                : Tue Dec 31 2002
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

#include "Card.h"
#include "Deck.h"
#include "CardElements.h"
#include "Const.h"

#include <qapplication.h>
#include <qwidget.h>
#include <qcursor.h>
#include <kglobalsettings.h>
#include <kdebug.h>
#include <stdlib.h>

/*
		QPoint _mouseDown;
		QRect _screen;
		Card *_card;
		bool _tracking, _cardWasShutOnDrag;
*/

namespace CardDesk {

Tab::DragLogic::DragLogic(Card *card)
	:_card(card), _tracking(false), _cardWasShutOnDrag(false)
{}

void Tab::DragLogic::mousePress(QPoint point)
{
	_screen = _card->deck()->screen();
	_mouseDown = point;
	_mouseDownCentered = false;
	_tracking = false;
	_cardWasShutOnDrag = false;
	_card->draggingStart();
}

void Tab::DragLogic::mouseMove(QPoint point)
{

	//if we're not the root card, don't do anything when dragging about inside tab. probably a quicker way of doing this..
	if ( (point.x() > 0)  && (point.x() < _card->width()) && (point.y() > 0) &&
			(point.y() < _card->height()) && _card->inDeck() && !_card->isRootCard())
	{
	return;
	}

	if (!tracking())
	{
		if (abs(_mouseDown.x() - point.x()) > KGlobalSettings::dndEventDelay() ||
				abs(_mouseDown.y() - point.y()) > KGlobalSettings::dndEventDelay())
		{
			_tracking = true;
			if (_card->inDeck() && !_card->isRootCard())
			{
				_card->leaveDeck(false);
				if (_card->isOpen())
				{
					_card->shut();
					_cardWasShutOnDrag = true;
				}

				QPoint p(QCursor::pos());
				ScreenEdge newEdge = _card->edge();
				double position;
				int distanceFromEdge;
				positionForMouse(p, _card->edge(), position, newEdge, distanceFromEdge);

				_card->createDeck(position, newEdge, distanceFromEdge);
			}

		}
	}

	if (tracking())
	{
			QPoint p(QCursor::pos());
			ScreenEdge newEdge = _card->edge();
			double position;
			int distanceFromEdge;
			positionForMouse(p, _card->edge(), position, newEdge, distanceFromEdge);

			_card->setPosition(position, newEdge, distanceFromEdge);
	}
}

void Tab::DragLogic::done( void )
{
	_tracking = false;
	_card->draggingDone();

	if (_cardWasShutOnDrag)
	{
		_card->open();
	}
}

bool Tab::DragLogic::tracking( void )
{
	return _tracking;
}

void Tab::DragLogic::positionForMouse(QPoint &mouse, ScreenEdge starterEdge,
                                      double &position, ScreenEdge &newEdge,
                                      int &distanceFromEdge)
{
	Deck *deck = _card->deck();
	if (!deck) return;

	int edgeSnapThreshold = 10;
	int sHeight = _screen.bottom();
	int sWidth = _screen.right();

	distanceFromEdge = -1;
	switch( starterEdge )
	{	
		case TopEdge:
		{
			if (mouse.y() > edgeSnapThreshold)
			{
				if (mouse.x() >= sWidth - 1) //snap to right edge
				{
					centerMouseDown();
					newEdge = RightEdge;
					position = rightEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.x() <= 1) //snap to left edge
				{
					centerMouseDown();
					newEdge = LeftEdge;
					position = leftEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.y() >= sHeight - 1) //snap to bottom edge
				{
					centerMouseDown();
					newEdge = BottomEdge;
					position = bottomEdgeDrag(mouse, distanceFromEdge);
					break;
				}
			}			

			newEdge = TopEdge;
			position = topEdgeDrag(mouse, distanceFromEdge);
			
			break;
		}

		case RightEdge:
		{
			if (mouse.x() < sWidth - edgeSnapThreshold)
			{
				if (mouse.y() <= 1)
				{
					centerMouseDown();
					newEdge = TopEdge;
					position = topEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.y() >= sHeight - 1)
				{
					centerMouseDown();
					newEdge = BottomEdge;
					position = bottomEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.x() <= 1)
				{
					centerMouseDown();
					newEdge = LeftEdge;
					position = leftEdgeDrag(mouse, distanceFromEdge);
					break;
				}
			}

			newEdge = RightEdge;
			position = rightEdgeDrag(mouse, distanceFromEdge);
			

			break;
		}

		case BottomEdge:
		{
			if (mouse.y() < sHeight - edgeSnapThreshold)
			{
				if (mouse.x() >= sWidth - 1)
				{
					centerMouseDown();
					newEdge = RightEdge;
					position = rightEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.x() <= 1)
				{
					centerMouseDown();
					newEdge = LeftEdge;
					position = leftEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.y() <= 1)
				{
					centerMouseDown();
					newEdge = TopEdge;
					position = topEdgeDrag(mouse, distanceFromEdge);
					break;
				}
			}

			newEdge = BottomEdge;
			position = bottomEdgeDrag(mouse, distanceFromEdge);
			

			break;
		}

		case LeftEdge:
		{
			if (mouse.x() > edgeSnapThreshold)
			{
				if (mouse.y() <= 1)
				{
					centerMouseDown();
					newEdge = TopEdge;
					position = topEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.y() >= sHeight -1)
				{
					centerMouseDown();
					newEdge = BottomEdge;
					position = bottomEdgeDrag(mouse, distanceFromEdge);
					break;
				}
				else if (mouse.x() >= sWidth - 1)
				{
					centerMouseDown();
					newEdge = RightEdge;
					position = rightEdgeDrag(mouse, distanceFromEdge);
					break;
				}
			}

			newEdge = LeftEdge;
			position = leftEdgeDrag(mouse, distanceFromEdge);
			
			break;
		}

		default: break;
	}

	if (position < 0) position = 0;
	if (position > 1.0) position = 1.0;
}

double Tab::DragLogic::topEdgeDrag(QPoint &mouse, int &distanceFromEdge)
{
	Deck *deck = _card->deck();
	int deckWidth = deck->frameGeometry().width();
	int sWidth = _screen.right();
	int mdX = _mouseDownCentered ? deckWidth / 2 : _mouseDown.x();

	double positionOffset = (double) mdX / (double )(sWidth - deckWidth);	
	double position = (double) mouse.x() / (double) (sWidth - deckWidth);
	position -= positionOffset;

	distanceFromEdge = mouse.y();
	return position;
}

double Tab::DragLogic::rightEdgeDrag(QPoint &mouse, int &distanceFromEdge)
{
	Deck *deck = _card->deck();
	int deckHeight = deck->frameGeometry().height();
	int sHeight = _screen.bottom();
	int sWidth = _screen.right();
	int mdY = _mouseDownCentered ? deckHeight / 2 : _mouseDown.y();

	double positionOffset = (double) mdY / (double )(sHeight - deckHeight);
	double position = (double) mouse.y() / (double) (sHeight - deckHeight);
	position -= positionOffset;

	distanceFromEdge = sWidth - mouse.x();
	return position;
}

double Tab::DragLogic::bottomEdgeDrag(QPoint &mouse, int &distanceFromEdge)
{
	Deck *deck = _card->deck();
	int deckWidth = deck->frameGeometry().width();
	int sHeight = _screen.bottom();
	int sWidth = _screen.right();
	int mdX = _mouseDownCentered ? deckWidth / 2 : _mouseDown.x();

	double positionOffset = (double) mdX / (double )(sWidth - deckWidth);
	double position = (1.0 - ((double) mouse.x() / (double) (sWidth - deckWidth)));
	position += positionOffset;

	distanceFromEdge = sHeight - mouse.y();
	return position;
}

double Tab::DragLogic::leftEdgeDrag(QPoint &mouse, int &distanceFromEdge)
{
	Deck *deck = _card->deck();
	int deckHeight = deck->frameGeometry().height();
	int sHeight = _screen.bottom();
	int mdY = _mouseDownCentered ? deckHeight / 2 : _mouseDown.y();

	double positionOffset = (double) mdY / (double )(sHeight - deckHeight);
	double position = (1.0 - ((double) mouse.y() / (double) (sHeight - deckHeight)));
	position += positionOffset;

	distanceFromEdge = mouse.x();
	return position;
}

void Tab::DragLogic::centerMouseDown( void )
{
	_mouseDownCentered = true;
}

} //namespace
