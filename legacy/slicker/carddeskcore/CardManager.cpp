/***************************************************************************
                          Manager.cpp  -  description
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

#include "CardManager.h"

#include "Deck.h"
#include "CardApplet.h"
#include "Const.h"

#include <qtimer.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>

double dAbs(double a)
{
	if (a < 0) return -a;
	return a;
}


/*
		static Manager *_instance;
		CardVector _cards;
*/

namespace CardDesk {

Manager *Manager::_instance = 0L;



Manager::Manager( void )
	:QObject(0L, "Manager")
{
	_cards.setAutoDelete(false);
	connect(qApp, SIGNAL(aboutToQuit()), SLOT(slotShutdown()));
}

Manager::~Manager( void )
{
	//this destructor wont get called
}

void Manager::useImplementation(Manager *manager)
{
	_instance = manager;
}

Manager *Manager::instance( void )
{
	if (!_instance) _instance = new Manager();
	return _instance;
}


/////////////////////////////////////////////////////////////////////////////

CardList Manager::cards( ScreenEdge e )
{
	if (e == None)
		return _cards;
	else
	{
		CardList list;
		CardListIterator it(_cards);
		Card *c;
		while ( (c = it.current()) != 0)
		{
			++it;
			if (c->edge() == e || None == e)
			{
				list.append(c);
			}

		}

		return list;
	}
}

CardList Manager::rootCards( void )
{
	CardList list;
	CardListIterator it(_cards);
	Card *c;
	while ( (c = it.current()) != 0)
	{
		++it;
		if (!c->inDeck() || c->isRootCard())
		{
			list.append(c);
		}
	}
	return list;
}


CardList Manager::lonerCards( void )
{
	CardList list;
	CardListIterator it(_cards);
	Card *c;
	while ( (c = it.current()) != 0)
	{
		++it;
		if (!c->inDeck())
		{
			list.append(c);
		}
	}
	return list;
}

Card *Manager::card(QString name)
{
	CardList list;
	CardListIterator it(_cards);
	Card *c;
	while ( (c = it.current()) != 0)
	{
		++it;
		if (c->cardName() == name)
		{
			return c;
		}
	}

	return 0L;
}

int Manager::countCards(){
	return _cards.count();
}


void Manager::registerCard(Card *card)
{
 	kdDebug(155003) << "Manager::registerCard()" << endl;
	/*
		we're not concerned with performance when adding/removing
		cards, since it won't happen but a handful of times while
		the program is running. So we'll resize the deck for each
		insertion/deletion
	*/

	_cards.append(card);

	connect(card, SIGNAL(deckingChanged(Card *)), SLOT(slotDeckingModified(Card *)));
	connect(card, SIGNAL(deckingRulesChanged(Card *)), SLOT(slotDeckingRulesModified(Card *)));
	connect(card, SIGNAL(opened(Card *)), SLOT(slotCardOpened(Card *)));
	connect(card, SIGNAL(closed(Card *)), SLOT(slotCardClosed(Card *)));
	connect(card, SIGNAL(nameChanged(Card *, QString, QString)), SLOT(slotCardRenamed(Card *, QString, QString)));
	connect(card, SIGNAL(positionChanged(Card *, double, double, int)),
	        SLOT(slotCardMoved(Card *, double, double, int)));

	emit cardRegistered(QString(card->name()), card);
}

void Manager::unregisterCard(Card *card)
{
	//hehe, MUCH nicer implementation than before
	bool a = _cards.remove(card);
	emit cardUnregistered(QString(card->name()), card);
	if (_cards.count() == 0) emit lastCardUnregistered();
}

void Manager::enumerateCardsToStdout( QString message )
{
	//FIXME
// 	QString msg = message == QString::null ? QString("enumerateCardsToStdout()") : message;
// 	QString str;
// 	CardVector roots = rootCards();
// 	for (int i = 0; i < (int) roots.count(); i++)
// 	{
// 		Card *root = roots[i];
// 		str += "\t";
//
// 		for (int i = 0; i < root->deck()->count(); i++)
// 		{
// 			str += QString("[") + root->deck()->cardAt(i)->cardName() + "] ";
// 		}
//
// 		str += "\n";
// 	}
//
// 	printf("%s\n%s\n\n", msg.latin1(), str.latin1());
}

/////////////////////////////////////////////////////////////////////////////

void Manager::suicide( Card *woeIsMe )
{
	_moribundCards.append(woeIsMe);
	QTimer::singleShot(500, this, SLOT(deleteMoribundCards()));
}

void Manager::destroyCard( Card *woeIsMe )
{
	if (woeIsMe->destroy())
	{
		_moribundCards.append(woeIsMe);
		QTimer::singleShot(500, this, SLOT(deleteMoribundCards()));
	}
}

/////////////////////////////////////////////////////////////////////////////


Deck *Manager::positionDeck(Deck *deck, double &desiredPos, ScreenEdge desiredEdge,
                            QRect &screenGeo, bool checkCollision, int distanceFromEdge)
{
	QRect deckFrame = deck->frameGeometry();

	double widthFactor = (double) (screenGeo.width() - deckFrame.width()) * desiredPos;
	double heightFactor = (double) (screenGeo.height() - deckFrame.height()) * desiredPos;

	switch( desiredEdge )
	{
		case TopEdge:
		{
			deckFrame.moveTop( screenGeo.top() );
			deckFrame.moveLeft( (int) (screenGeo.left() + widthFactor) );
			break;
		}

		case RightEdge:
		{
			deckFrame.moveRight( screenGeo.right() );
			deckFrame.moveTop( (int) (screenGeo.top() + heightFactor) );
			break;
		}

		case BottomEdge:
		{
			deckFrame.moveBottom( screenGeo.bottom() );
			deckFrame.moveLeft( (int) (screenGeo.width() + screenGeo.left() - deckFrame.width() - widthFactor) );
			break;
		}

		case LeftEdge:
		{
			deckFrame.moveLeft( screenGeo.left() );
			deckFrame.moveTop( (int) (screenGeo.height() + screenGeo.top() - deckFrame.height() - heightFactor) );
			break;
		}

		default: break;
	}

	//now, determine if there would be any collisions for this position
	Deck *possiblyDeckTo = 0L;
	if (checkCollision)
	{

		CardList cards = rootCards();
		CardListIterator it(cards);
		Card *c;
		while ( (c = it.current()) != 0)
		{
			++it;
			Deck *targetDeck = c->deck();
			if (!targetDeck) continue;
			if (targetDeck == deck) continue;

			QRect targetFrame = targetDeck->frameGeometry();
			//now grow this frame by spacing
			targetFrame.setTop(targetFrame.top() - spacing());
			targetFrame.setRight(targetFrame.right() + spacing());
			targetFrame.setBottom(targetFrame.bottom() + spacing());
			targetFrame.setLeft(targetFrame.left() - spacing());


			if (targetFrame.intersects(deckFrame))
			{
				if (targetDeck->edge() == deck->edge())
				{
					switch (desiredEdge)
					{
						case TopEdge: case BottomEdge:
						{
							double deckingThresh = (double)deckingThresholdFor(targetDeck) / (double)screenGeo.width();
							if ( dAbs(desiredPos - targetDeck->position()) < deckingThresh )
							{
								possiblyDeckTo = targetDeck;
							}
							break;
						}

						case RightEdge: case LeftEdge:
						{
							double deckingThresh = (double)deckingThresholdFor(targetDeck) / (double)screenGeo.height();
							if ( dAbs(desiredPos - targetDeck->position()) < deckingThresh )
							{
								possiblyDeckTo = targetDeck;
							}

							break;
						}

						default: break;
					}

				}


				switch (desiredEdge)
				{
					case TopEdge: case BottomEdge:
					{
						if (targetFrame.center().x() < deckFrame.center().x())
						{
							deckFrame.moveLeft(targetFrame.right());
						}
						else
						{
							deckFrame.moveRight(targetFrame.left());
						}

						//check to see if we were pushed offscreen
						if (deckFrame.left() < 0)
						{
							deckFrame.moveLeft(targetFrame.right());
						}

						if (deckFrame.right() > screenGeo.right())
						{
							deckFrame.moveRight(targetFrame.left());
						}

						break;
					}

					case RightEdge: case LeftEdge:
					{
						if (targetFrame.center().y() < deckFrame.center().y())
						{
							deckFrame.moveTop(targetFrame.bottom());
						}
						else
						{
							deckFrame.moveBottom(targetFrame.top());
						}

						//check to see if we were pushed offscreen
						if (deckFrame.top() < 0)
						{
							deckFrame.moveTop(targetFrame.bottom());
						}

						if (deckFrame.bottom() > screenGeo.bottom())
						{
							deckFrame.moveBottom(targetFrame.top());
						}

						break;
					}

					default: break;
				} //switch

			} //if intsersects
		} //for

		//reset position to accommodate adjustments
		desiredPos = positionForFrame(deckFrame, desiredEdge, screenGeo);
	} //if (collisionCheck)

	if (possiblyDeckTo && (deck->count() == 1) && (distanceFromEdge > -1))
	{
		//printf("Manager::positionDeck()\tDeck %s is over deck %s and being positioned %d from edge.\n",
		//       deck->root()->cardName().latin1(), possiblyDeckTo->root()->cardName().latin1(), distanceFromEdge);
		/*
			in this case, we're moving a one-card deck *away* from the edge for
			the purpose of re-ordering.
			We need to move the deck to possiblyDeckTo's position, and
			then away from the edge by distanceFromEdge
		*/

		QRect deckToGeo(possiblyDeckTo->frameGeometry());

		deckFrame.moveCenter(deckToGeo.center());
		switch (desiredEdge)
		{
			case TopEdge:
			{
				if (distanceFromEdge > deckToGeo.height()) distanceFromEdge = deckToGeo.height();
				deckFrame.moveTop(distanceFromEdge);
				break;
			}
			case RightEdge:
			{
				if (distanceFromEdge > deckToGeo.width()) distanceFromEdge = deckToGeo.width();
				deckFrame.moveRight(screenGeo.right() - distanceFromEdge);
				break;
			}
			case BottomEdge:
			{
				if (distanceFromEdge > deckToGeo.height()) distanceFromEdge = deckToGeo.height();
				deckFrame.moveBottom(screenGeo.height() - distanceFromEdge);
				break;
			}
			case LeftEdge:
			{
				if (distanceFromEdge > deckToGeo.width()) distanceFromEdge = deckToGeo.width();
				deckFrame.moveLeft(distanceFromEdge);
				break;
			}
			default: break;
		}
	}

	deck->move( deckFrame.topLeft() );
	return possiblyDeckTo;
}

double Manager::positionForFrame(QRect frame, ScreenEdge edge, QRect &screen)
{
	double availableWidth = screen.width() - frame.width();
	double availableHeight = screen.height() - frame.height();
	double position = -1;

	switch (edge)
	{
		case TopEdge:
		{
			position = (double)(frame.left() - screen.left()) / availableWidth;
			break;
		}

		case RightEdge:
		{
			position = (double) (frame.top() - screen.top()) / availableHeight;
			break;
		}

		case BottomEdge:
		{
			position = 1.0 - ((double) (frame.left() - screen.left()) / availableWidth);
			break;
		}

		case LeftEdge:
		{
			position = 1.0 - ((double) (frame.top() - screen.top()) / availableHeight);
			break;
		}

		default: break;
	}

	return position;
}

int Manager::spacing( void )
{
	return mSpace;
}

void Manager::setSpacing( int space )
{
	mSpace = space;
}

int Manager::deckingThreshold( void )
{
	return 160;
}

int Manager::deckingThresholdFor( Deck *d )
{
	return (d->breadth() * 3) / 4;
}


/////////////////////////////////////////////////////////////////////////////
// public slots

Card *Manager::newCardDialog( void )
{
	return 0L;
}

void Manager::configureDialog( void )
{
}

void Manager::slotDeckingModified( Card * )
{
	emit deckingChanged();
}

void Manager::slotDeckingRulesModified( Card * )
{
	//don't understand this function
	CardList loners = lonerCards();
	CardListIterator it(loners);
	Card *c;
	while ( (c = it.current()) != 0)
	{
		++it;
		c->setPosition(c->position());
	}

	emit deckingRulesChanged();
}

void Manager::slotCardOpened(Card *c)
{
	emit cardOpened(c);
}

void Manager::slotCardClosed(Card *c)
{
	emit cardClosed(c);
}

void Manager::slotCardRenamed(Card *c, QString oldName, QString newName)
{
	emit cardRenamed(c, oldName, newName);
}

void Manager::slotCardMoved( Card *c, double actualPosition, double desiredPosition, int edge )
{
	emit cardMoved(c, actualPosition, desiredPosition, edge );
}

void Manager::slotShutdown( void )
{

	//call shutdown routines.
	_cards.setAutoDelete(true);
	_cards.clear();
	_cards.setAutoDelete(false);

	delete this;
}

void Manager::deleteMoribundCards( void )
{
	_moribundCards.setAutoDelete(true);
	_moribundCards.clear(); //will delete contents
}

void Manager::loadCards( void )
{
}

}; //namespace
