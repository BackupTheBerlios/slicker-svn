/***************************************************************************
                          CardElements.cpp  -  description
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

#include "CardElements.h"
#include "Card.h"
#include "Deck.h"
#include "CardManager.h"
#include "CardApplet.h"
#include "LayoutSwitcher.h"

#include <qapplication.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qcursor.h>
#include <qwmatrix.h>
#include <qfontmetrics.h>
#include <qpopupmenu.h>
#include <qiconset.h>
#include <kdebug.h>

#include <kpixmapeffect.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <qinputdialog.h>

/*
		Card *_card;
		int _width, _height, _storedHeight;
		bool _open, _animating;
*/

namespace CardDesk {

CardElement::CardElement(Card *parent)
	:QFrame(0L, 0L, WRepaintNoErase | WResizeNoErase), _card(parent),
	_width(200), _height(30), _storedHeight(30),
	_open(true), _animating(false)
{
	setFrameStyle(NoFrame);
	setPreferredSize(200, 30);
	//QPalette pal(QColor(240,240,248),QColor(211,214,223));
	//setPalette(pal);
}

void CardElement::setPreferredSize(int width, int height)
{
	if (height < 0) height = 0;
	if (width < 0) width = 0;

	_width = width;
	_height = height;
	_storedHeight = _height;
	_storedWidth = _width;
	cardOrientationChanged();
	card()->elementSizeChanged(true);
}

void CardElement::setPreferredHeight( int h )
{
	setPreferredSize(_width, h);
}

void CardElement::setPreferredWidth( int w )
{
	setPreferredSize(w, _height);
}

void CardElement::restorePreferredWidth( bool relayout )
{
	_width = _storedWidth;
	cardOrientationChanged();
	if (relayout) card()->elementSizeChanged(true);
}

int CardElement::preferredWidth( void ) const
{
	return _width;
}

int CardElement::preferredHeight( void ) const
{
	return _open ? _height : 0;
}

int CardElement::height( void )
{
	switch (card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			return size().height();
			break;
		}
		case RightEdge: case LeftEdge:
		{
			return size().width();
			break;
		}
		default: break;
	}

	return -1;
}

int CardElement::width( void )
{
	switch (card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			return size().width();
			break;
		}
		case RightEdge: case LeftEdge:
		{
			return size().height();
			break;
		}
		default: break;
	}

	return -1;
}

int CardElement::storedHeight( void )
{
	return _storedHeight;
}

void CardElement::collapse( bool r )
{
	if (_animating) return;

	_height = _storedHeight;

	if (r)
	{
		//when animate shut finishes, _open will be false
		_open = true;
		animateCollapse();
	}
	else
	{
		//bypass animation, and just collapse the bugger
		_open = false;
		cardOrientationChanged();
	}
}

void CardElement::expand( bool r )
{
	//don't continue if we're currently animating
	if (_animating) return;

	_open = true;

	if (r)
	{
		_height = 0;
		animateExpand();
	}
	else
	{
		_height = _storedHeight;
		cardOrientationChanged();
	}
}

void CardElement::animateCollapse()
{
	_animating = true;
	int remaining = _height;

	if (remaining < 4)
	{
		_height = 0;
		_open = false;
		_animating = false;

		cardOrientationChanged();
		card()->elementSizeChanged(true);
	}
	else
	{
		_height -= _height / 2;

		cardOrientationChanged();
		card()->elementSizeChanged(true);
		QTimer::singleShot(10, this, SLOT(animateCollapse()));
	}
}

void CardElement::animateExpand()
{
	_animating = true;
	int remaining = _storedHeight - _height;
	if ( remaining < 10)
	{
		_height = _storedHeight;
		card()->elementSizeChanged(true);
		cardOrientationChanged();

		_animating = false;
	}
	else
	{
		_height += remaining / 6;

		cardOrientationChanged();
		card()->elementSizeChanged(true);
		QTimer::singleShot(10, this, SLOT(animateExpand()));
	}
}


void CardElement::cardOrientationChanged( void )
{
	QSize size;
	switch(card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			size.setWidth(preferredWidth());
			size.setHeight(preferredHeight());
			break;
		}

		case RightEdge: case LeftEdge:
		{
			size.setWidth(preferredHeight());
			size.setHeight(preferredWidth());
			break;
		}

		default: break;
	}

	resize(size);
	setMinimumSize(size);
}

void CardElement::setDeckOverrideWidth( int w, bool relayout )
{
	_width = w;

	cardOrientationChanged();
	if (relayout)
		card()->elementSizeChanged(true);
}

int CardElement::actualPreferredWidth( void )
{
	return _storedWidth;
}


void CardElement::paintEvent( QPaintEvent *e)
{
	if (!card()->deck())
	{
		/*
			If there's no deck, the card's invisible anyway.
		*/
		return;
	}

	QFrame::paintEvent(e);

	QPainter paint(this);
	QRect r(rect());
	ScreenEdge adjacentTo = card()->deck()->adjacentEdge();

	paint.setPen(Qt::black);
	paint.setBrush(Qt::NoBrush);

	switch( card()->edge() )
	{
		case TopEdge: case BottomEdge:
		{
			if (adjacentTo != LeftEdge) paint.drawLine(r.left()+2, r.top(), r.left()+2, r.bottom());
			else paint.fillRect(0, 0, 1, r.bottom(), backgroundBrush());

			if (adjacentTo != RightEdge) paint.drawLine(r.right()-2, r.top(), r.right()-2, r.bottom());
			else paint.fillRect(r.right() - 1, 0, r.right(), r.bottom(), backgroundBrush());
			break;
		}

		case LeftEdge: case RightEdge:
		{
			if (adjacentTo != TopEdge) paint.drawLine(r.left(), r.top()+2, r.right(), r.top()+2);
			else paint.fillRect(0, 0, r.right(), 1, backgroundBrush());

			if (adjacentTo != BottomEdge) paint.drawLine(r.left(), r.bottom()-2, r.right(), r.bottom()-2);
			else paint.fillRect(0, r.bottom() - 1, r.right(), r.bottom(), backgroundBrush());

			break;
		}

		default: break;
	}
}

QColor CardElement::lightColor( void )
{
	return paletteBackgroundColor().light(175);
}

QColor CardElement::mediumDarkColor( void )
{
	return paletteBackgroundColor().dark(130);
}

QColor CardElement::darkColor( void )
{
	return paletteBackgroundColor().dark(170);
}

/////////////////////////////////////////////////////////////////////////////
//		Tab *_tab;

TabDelegate::TabDelegate( void )
	: _tab(0L)
{}

QPixmap *TabDelegate::tabIcon( void )
{
	return 0L;
}

QSize TabDelegate::miniDisplaySize( void )
{
	return QSize(0, 0);
}

void TabDelegate::paintMiniDisplay(QPainter *, int, int)
{}

void TabDelegate::addToDefaultContextMenu( QPopupMenu * )
{}

void TabDelegate::aboutToShowContextMenu( QPopupMenu * )
{}

bool TabDelegate::allowDragHoverToOpen( void )
{
	return false;
}

bool TabDelegate::allowHoverToOpen( void )
{
	return false;
}

bool TabDelegate::allowDropOnTab( void )
{
	return false;
}

bool TabDelegate::canDecodeDrag( QMimeSource * )
{
	return false;
}

void TabDelegate::tabDragDropEvent( QDropEvent * )
{}

void TabDelegate::repaintTab( void )
{
	if (_tab)
		_tab->repaint(false);
}

void TabDelegate::attachToTab( Tab *tab )
{
	_tab = tab;
}


/////////////////////////////////////////////////////////////////////////////
//	Tab
//
//		QPixmap _px;
//		KPixmap _shadowPx;
//		DragLogic _dragger;
//		QTimer _mouseTrackerTimer;
//		TabDelegate *_delegate;
//		QPopupMenu *_contextMenu;
//		bool _stillHovering, _hoverOpened;


Tab::Tab(Card *card, TabDelegate *del)
	:CardElement(card), _dragger(card), _delegate(0L), _contextMenu(0L),
	_stillHovering(false), _hoverOpened(false)
{
	setName("Tab");
	setDelegate(del);
	connect(&_mouseTrackerTimer, SIGNAL(timeout()), SLOT(mouseTrackerTimeout()));
	setMouseTracking (true);
}

void Tab::setDelegate( TabDelegate *d )
{
	if (_delegate) delete _delegate;
	if (!d) d = new TabDelegate();

	_delegate = d;
	_delegate->attachToTab(this);
	calculatePreferredSize();
	delegatePropertiesChanged();
}

TabDelegate *Tab::delegate( void )
{
	return _delegate;
}


void Tab::paintTabBase(QPainter *paint)
{
	if (!card()->deck()) return;

	QRect pxRect(_px.rect());
	int r = pxRect.width(), b = pxRect.height();

	QWMatrix flipMatrix;
	flipMatrix.rotate(180);

	ScreenEdge adjacentTo = card()->deck()->adjacentEdge();

	paint->setClipRegion(tabOuterRegion());

	bool hasParent = false, hasChild = false, childIsOpen = false;
	if (card()->inDeck())
	{
		hasParent = (card()->deck()->root() != card());
		hasChild = (card()->deck()->tail() != card());
	}

	if (hasChild)
	{
		childIsOpen = card()->deck()->cardAt( card()->deck()->indexOf(card()) + 1 )->isOpen();
	}

	////////////////////////
	// Now, start drawing the tab outer region

	if (card()->edge() == TopEdge)
	{
		if (hasChild && !childIsOpen)
		{
			paint->drawPixmap(0, pxRect.bottom() - 3, _shadowPx.xForm(flipMatrix));
		}
	}
	else
	{
		if (hasChild && !childIsOpen)
		{
			paint->drawPixmap(0, 0, _shadowPx, 0, 3);
		}
	}
	
	paint->setBrush(Qt::NoBrush);
	paint->setPen(Qt::black);
	switch (card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			if (adjacentTo != LeftEdge)
				paint->drawLine(pxRect.topLeft(), pxRect.bottomLeft());

			if (adjacentTo != RightEdge)
				paint->drawLine(pxRect.topRight(), pxRect.bottomRight());

			break;
		}

		case RightEdge:
		{
			if (adjacentTo != BottomEdge)
				paint->drawLine(pxRect.topLeft(), pxRect.bottomLeft());

			if (adjacentTo != TopEdge)
				paint->drawLine(pxRect.topRight(), pxRect.bottomRight());

			break;
		}

		case LeftEdge:
		{
			if (adjacentTo != TopEdge)
				paint->drawLine(pxRect.topLeft(), pxRect.bottomLeft());

			if (adjacentTo != BottomEdge)
				paint->drawLine(pxRect.topRight(), pxRect.bottomRight());

			break;
		}

		default: break;
	}

	///////////////////////////////////////////////////////////////////////////
	//now, we're going to mask off the curved tab area and clip the painter to it

	paint->setClipRegion(tabInnerRegion());

	//now fill the tab with BG
	paint->setBrush(backgroundBrush());
	paint->setPen(Qt::NoPen);

	//if there is a background pixmap, we rotate it.
	if (backgroundBrush().pixmap() && (card()->edge() == RightEdge || card()->edge() == LeftEdge))
	{
		QWMatrix m; m.rotate(90);
		QPixmap rPx = backgroundBrush().pixmap()->xForm(m);
		QBrush br(backgroundBrush());
		br.setPixmap(rPx);
		paint->setBrush(br);
	}

	paint->drawRect(pxRect);

	paint->setBrush(Qt::NoBrush);
	paint->setPen(Qt::black);

	int shapeMask = card()->deck()->shapeMask();
	switch( card()->edge() )
	{

		case TopEdge: case BottomEdge:
		{
			if (card()->edge() == TopEdge)
			{
				/* make the line longer if we are docked to the side */
				if (adjacentTo == LeftEdge)
					paint->drawLine( pxRect.left(), pxRect.bottom()-2, pxRect.right()-7, pxRect.bottom()-2);
				else if (adjacentTo == RightEdge)
					paint->drawLine( pxRect.left()+7, pxRect.bottom()-2, pxRect.right(), pxRect.bottom()-2);
				else
					paint->drawLine( pxRect.left()+7, pxRect.bottom()-2, pxRect.right()-7, pxRect.bottom()-2);

				if (hasParent && !card()->isOpen() && !card()->getTray())
				{
					paint->drawPixmap(0, 0, _shadowPx.xForm(flipMatrix), 0, 4);
				}

			}
			else
			{
				/* make the line longer if we are docked to the side */
				if (adjacentTo == LeftEdge)
					paint->drawLine(pxRect.left(), pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);
				else if (adjacentTo == RightEdge)
					paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right(), pxRect.top()+2);
				else
					paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);

				if (hasParent && !card()->isOpen() && !card()->getTray())
				{
					paint->drawPixmap(0, pxRect.bottom() - 2, _shadowPx);
				}
			}

			if (adjacentTo != LeftEdge)
				paint->drawLine(pxRect.left()+2, pxRect.top(), pxRect.left()+2, pxRect.bottom());

			if (adjacentTo != RightEdge)
				paint->drawLine(pxRect.right()-2, pxRect.top(), pxRect.right()-2, pxRect.bottom());

			//draw upper left curve
			if (shapeMask & TopLeft )
			{
				paint->drawPoint(3+2, 1+2);
				paint->drawPoint(4+2, 1+2);
				paint->drawPoint(2+2, 2+2);
				paint->drawPoint(1+2, 3+2);
				paint->drawPoint(1+2, 4+2);
			}

			//draw upper right curve
			if (shapeMask & TopRight)
			{
				paint->drawPoint(r - 5-2, 1+2);
				paint->drawPoint(r - 4-2, 1+2);
				paint->drawPoint(r - 3-2, 2+2);
				paint->drawPoint(r - 2-2, 3+2);
				paint->drawPoint(r - 2-2, 4+2);
			}

			//draw lower left curve
			if (shapeMask & BottomLeft)
			{
				paint->drawPoint(1+2, b - 5-2);
				paint->drawPoint(1+2, b - 4-2);
				paint->drawPoint(2+2, b - 3-2);
				paint->drawPoint(3+2, b - 2-2);
				paint->drawPoint(4+2, b - 2-2);
			}

			//draw lower right curve
			if (shapeMask & BottomRight)
			{
				paint->drawPoint(r - 2-2, b - 5-2);
				paint->drawPoint(r - 2-2, b - 4-2);
				paint->drawPoint(r - 3-2, b - 3-2);
				paint->drawPoint(r - 4-2, b - 2-2);
				paint->drawPoint(r - 5-2, b - 2-2);
			}

			break;
		}

		case RightEdge:
		{
			/* make the line longer if we are docked to the side */
			if (adjacentTo == BottomEdge)
				paint->drawLine(pxRect.left(), pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);
			else if (adjacentTo == TopEdge)
				paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right(), pxRect.top()+2);
			else
				paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);

			if (hasParent && !card()->isOpen() && !card()->getTray())
			{
				paint->drawPixmap(0, pxRect.bottom() - 2, _shadowPx);
			}

			if (adjacentTo != BottomEdge)
				paint->drawLine(pxRect.left()+2, pxRect.top(), pxRect.left()+2, pxRect.bottom());

			if (adjacentTo != TopEdge)
				paint->drawLine(pxRect.right()-2, pxRect.top(), pxRect.right()-2, pxRect.bottom());


			if (shapeMask & TopLeft)
			{
				paint->drawPoint(r - 5-2, 1+2);
				paint->drawPoint(r - 4-2, 1+2);
				paint->drawPoint(r - 3-2, 2+2);
				paint->drawPoint(r - 2-2, 3+2);
				paint->drawPoint(r - 2-2, 4+2);
			}

			if (shapeMask & BottomLeft)
			{
				paint->drawPoint(3+2, 1+2);
				paint->drawPoint(4+2, 1+2);
				paint->drawPoint(2+2, 2+2);
				paint->drawPoint(1+2, 3+2);
				paint->drawPoint(1+2, 4+2);
			}

			break;
		}

		case LeftEdge:
		{
			/* make the line longer if we are docked to the side */
			if (adjacentTo == BottomEdge)
				paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right(), pxRect.top()+2);
			else if (adjacentTo == TopEdge)
				paint->drawLine(pxRect.left(), pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);
			else
				paint->drawLine(pxRect.left()+7, pxRect.top()+2, pxRect.right()-7, pxRect.top()+2);

			if (hasParent && !card()->isOpen() && !card()->getTray())
			{
				paint->drawPixmap(0, pxRect.bottom() - 2, _shadowPx);
			}

			if (adjacentTo != TopEdge)
				paint->drawLine(pxRect.left()+2, pxRect.top(), pxRect.left()+2, pxRect.bottom());

			if (adjacentTo != BottomEdge)
				paint->drawLine(pxRect.right()-2, pxRect.top(), pxRect.right()-2, pxRect.bottom());

			if (shapeMask & TopRight)
			{
				paint->drawPoint(3+2, 1+2);
				paint->drawPoint(4+2, 1+2);
				paint->drawPoint(2+2, 2+2);
				paint->drawPoint(1+2, 3+2);
				paint->drawPoint(1+2, 4+2);
			}

			if (shapeMask & BottomRight)
			{
				paint->drawPoint(r - 5-2, 1+2);
				paint->drawPoint(r - 4-2, 1+2);
				paint->drawPoint(r - 3-2, 2+2);
				paint->drawPoint(r - 2-2, 3+2);
				paint->drawPoint(r - 2-2, 4+2);
			}
			break;
		}

		default: break;
	}

	paint->setClipping(false);
}

void Tab::paintTabContents(QPainter *paint)
{
	int xOffset = contentsSpacing();
	QRect pxRect = _px.rect();

	paint->save();

	//draw the icon, if there is an icon
	if (_delegate->tabIcon())
	{
		paint->translate(xOffset, 0);
		paintTabIcon(paint);
		/* Draw the pixmap 4 px away from the side, so the layout was better  */
		xOffset += _delegate->tabIcon()->size().width() + 4;
	}

	xOffset += contentsSpacing();

	paint->setPen(KGlobalSettings::textColor());
	paint->setFont(font()); //not redundant -- because we're drawing into a pixmap

	QFontMetrics fm = paint->fontMetrics();
	int sHeight = fm.ascent();
	paint->drawText( xOffset, pxRect.bottom() / 2 + sHeight / 2, card()->cardName());

	paint->restore();
	if (_delegate->miniDisplaySize().width() > 0) paintTabMiniDisplay(paint);
}

void Tab::paintTabIcon(QPainter *paint)
{
	/* Draw the pixmap 4 px away from the side, so the layout was better  */
	switch (card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			paint->drawPixmap(4, height() / 2 - _delegate->tabIcon()->height() / 2, *_delegate->tabIcon());
			break;
		}

		case LeftEdge:
		{
			QWMatrix m;
			m.rotate(270);

			paint->drawPixmap(4, height() / 2 - _delegate->tabIcon()->height() / 2, _delegate->tabIcon()->xForm(m));
			break;
		}

		case RightEdge:
		{
			QWMatrix m;
			m.rotate(90);

			paint->drawPixmap(4, height() / 2 - _delegate->tabIcon()->height() / 2, _delegate->tabIcon()->xForm(m));
			break;
		}

		default: break;
	}

}

void Tab::paintTabMiniDisplay(QPainter *paint)
{
	if (!card()->deck()) return;
	if (_delegate->miniDisplaySize().width() == 0) return;

	QSize mdSize = _delegate->miniDisplaySize();
	if ( (mdSize.height() + (2 * contentsSpacing()) ) > height() )
	{
		mdSize.setHeight( height() - 2 * contentsSpacing() );
	}

	QRect pxRect = _px.rect();
	QRect clipRect(0, 0, mdSize.width(), mdSize.height());

	//determine if clip rect needs to be cropped ... not done yet.
	int dx = pxRect.right() - contentsSpacing() - mdSize.width();
	int dy = (pxRect.height() / 2) - (mdSize.height() / 2);

	/* We must a extra spacing if the card have the curve on the side with the minidisplay */
	ScreenEdge adjacentTo = card()->deck()->adjacentEdge();
	if (adjacentTo != RightEdge && adjacentTo != BottomEdge)
		dx -= 3;

	//clipping is non translated, in physical device coordinates.
	paint->setClipRect(dx, dy, clipRect.width(), clipRect.height());
	paint->translate(dx, dy);

	switch(card()->edge())
	{
		case RightEdge:
		{
			paint->rotate(90);
			paint->translate(0, -clipRect.width() + 1);
			_delegate->paintMiniDisplay(paint, clipRect.height(), clipRect.width());
			break;
		}

		case LeftEdge:
		{
			paint->rotate(270);
			paint->translate(-clipRect.height() + 1, 0);
			_delegate->paintMiniDisplay(paint, clipRect.height(), clipRect.width());
			break;
		}

		default:
			_delegate->paintMiniDisplay(paint, clipRect.width(), clipRect.height());

	}
}

void Tab::delegatePropertiesChanged( void )
{
	setAcceptDrops( _delegate->allowDragHoverToOpen() || _delegate->allowDropOnTab() );
	calculatePreferredSize();
	repaint(false);
}

void Tab::dropProcessed( void )
{
	//cancel a pending opening!
	_stillHovering = false;

	//if the card is open because of a hover-open
	if (_hoverOpened && card()->isOpen())
	{
		card()->shut();
	}
}


void Tab::mouseTrackerTimeout()
{
	mouseMove(mapFromGlobal(QCursor::pos()));
}

void Tab::slotCMOpen( void )
{
	card()->open();
}

void Tab::slotCMClose( void )
{
	card()->shut();
}

void Tab::slotCMDestroy( void )
{
	Manager::instance()->destroyCard(card());
}

void Tab::slotCMRename( void )
{
	bool ok;
	QString name = QInputDialog::getText("Rename", "Rename card: ", QLineEdit::Normal, card()->cardName(), &ok);
	if (ok)
		card()->applet()->setCardName(name);
}

void Tab::showContextMenu( void )
{
	if (!_contextMenu)
	{
		createContextMenu();
	}

	_contextMenu->setItemEnabled(ID_Open, !card()->isOpen());
	_contextMenu->setItemEnabled(ID_Close, card()->isOpen());

	_delegate->aboutToShowContextMenu(_contextMenu);
	_contextMenu->popup(QCursor::pos());
}

void Tab::dragHoverTimeout()
{
	if (_stillHovering && hasMouse() && !card()->isOpen() && _delegate->allowHoverToOpen() && !_dragger.tracking())
	{
		_hoverOpened = true;
		_stillHovering = false;
		card()->open();
	}

	if (_hoverOpened && !hasMouse() && !card()->hasMouse())
	{
		card()->shut();
	}


	if(_hoverOpened && card()->isOpen() && !_dragger.tracking())
		QTimer::singleShot(1000, this, SLOT(dragHoverTimeout()));

	_stillHovering=false;
}

void Tab::createContextMenu( void )
{
	KIconLoader il;
	_contextMenu = new QPopupMenu(this);

	unsigned int cBefore = _contextMenu->count();
	_delegate->addToDefaultContextMenu(_contextMenu);

	if (_contextMenu->count() > cBefore) _contextMenu->insertSeparator();

	QIconSet killIcon(SmallIconSet("remove"));
	QIconSet configureIcon(SmallIconSet("configure"));
	QIconSet newIcon(SmallIconSet("tab_new"));
	QIconSet exitIcon(SmallIconSet("exit"));

	_contextMenu->insertItem("&Open", this, SLOT( slotCMOpen() ), 0, ID_Open);
	_contextMenu->insertItem("&Close", this, SLOT( slotCMClose() ), 0, ID_Close);
	if (card()->applet()->allowRenameCard())
		_contextMenu->insertItem("&Rename", this, SLOT ( slotCMRename() ), 0, ID_Rename);
	_contextMenu->insertItem(killIcon, "&Remove", this, SLOT( slotCMDestroy() ), 0, ID_Kill);
	_contextMenu->insertSeparator();
	_contextMenu->insertItem(newIcon, "&New Card...", Manager::instance(), SLOT(newCardDialog()), 0, ID_New);
	_contextMenu->insertItem(configureIcon, "&Configure CardDesk...", Manager::instance(), SLOT(configureDialog()), 0, ID_Configure);
	_contextMenu->insertItem(exitIcon, "&E&xit", qApp, SLOT(quit()), 0, ID_Exit);
}

void Tab::calculatePreferredSize( void )
{
	int spacing = contentsSpacing();
	int iconHeight = _delegate->tabIcon() ? _delegate->tabIcon()->size().height() + 2 * spacing : 0;
	int iconWidth  = _delegate->tabIcon() ? _delegate->tabIcon()->size().width() + 2 * spacing : 0;
	int fontHeight = card()->fontMetrics().height() + 2* spacing;
	int mdHeight = _delegate->miniDisplaySize().height();
	int mdWidth = _delegate->miniDisplaySize().width() ? _delegate->miniDisplaySize().width() + 2* spacing : 0;
	int textWidth = card()->fontMetrics().width( card()->cardName() );

	int fullWidth = iconWidth + textWidth + mdWidth;
	int extraHeight = iconHeight > mdHeight ? iconHeight : mdHeight;
	extraHeight = extraHeight > fontHeight ? extraHeight : fontHeight;
	extraHeight += spacing;

	QSize size(fullWidth > 200 ? fullWidth : 200, extraHeight);
	setPreferredSize(size.width(), size.height());
}


QRegion Tab::tabInnerRegion( void )
{
	QRect pxRect(_px.rect());
	int r = pxRect.width(), b = pxRect.height();
	QRegion mask(0, 0, r, b);

	int shapeMask = card()->deck()->shapeMask();
	switch( card()->edge() )
	{
		case TopEdge:
		{
			// Remove bottom-left corner.
			if (shapeMask & BottomLeft)
			{
				mask -= QRegion(0, b - 5, 1, 3);
				mask -= QRegion(0, b - 3, 2, 1);
				mask -= QRegion(0, b - 2, 3, 1);
				mask -= QRegion(0, b - 1, 5, 1);
			}

			// Remove bottom-right corner.
			if (shapeMask & BottomRight)
			{
				mask -= QRegion(r - 5, b - 1, 5, 1);
				mask -= QRegion(r - 3, b - 2, 3, 1);
				mask -= QRegion(r - 2, b - 3, 2, 1);
				mask -= QRegion(r - 1, b - 5, 1, 2);
			}

			break;
		}

		case BottomEdge:
		{
			// Remove top-left corner.
			if (shapeMask & TopLeft)
			{
				mask -= QRegion(0, 0, 5, 1);
				mask -= QRegion(0, 1, 3, 1);
				mask -= QRegion(0, 2, 2, 1);
				mask -= QRegion(0, 3, 1, 2);
			}

			// Remove top-right corner.
			if (shapeMask & TopRight)
			{
				mask -= QRegion(r - 5, 0, 5, 1);
				mask -= QRegion(r - 3, 1, 3, 1);
				mask -= QRegion(r - 2, 2, 2, 1);
				mask -= QRegion(r - 1, 3, 1, 2);
			}
			break;
		}

		case RightEdge:
		{
			//remove top-left corner (rotated)
			if (shapeMask & TopLeft)
			{
				mask -= QRegion(r - 5, 0, 5, 1);
				mask -= QRegion(r - 3, 1, 3, 1);
				mask -= QRegion(r - 2, 2, 2, 1);
				mask -= QRegion(r - 1, 3, 1, 2);
			}

			//remove bottom-left, rotated
			if (shapeMask & BottomLeft)
			{
				mask -= QRegion(0, 0, 5, 1);
				mask -= QRegion(0, 1, 3, 1);
				mask -= QRegion(0, 2, 2, 1);
				mask -= QRegion(0, 3, 1, 2);
			}

			break;
		}

		case LeftEdge:
		{
			//remove top-right (rotated)
			if (shapeMask & TopRight)
			{
				mask -= QRegion(0, 0, 5, 1);
				mask -= QRegion(0, 1, 3, 1);
				mask -= QRegion(0, 2, 2, 1);
				mask -= QRegion(0, 3, 1, 2);
			}

			//remove bottom-right (rotated)
			if (shapeMask & BottomRight)
			{
				mask -= QRegion(r - 5, 0, 5, 1);
				mask -= QRegion(r - 3, 1, 3, 1);
				mask -= QRegion(r - 2, 2, 2, 1);
				mask -= QRegion(r - 1, 3, 1, 2);
			}
			break;
		}

		default: break;
	}

	return mask;
}

QRegion Tab::tabOuterRegion( void )
{
	QRegion r(_px.rect());
	QRegion t(tabInnerRegion());
	return (r.subtract(t));
}


void Tab::mousePressEvent(QMouseEvent *e)
{
	//printf("Tab[%s]::mousePressEvent()\n", card()->cardName().latin1());

	qApp->setGlobalMouseTracking(true);
	setMouseTracking(true);

	if (e->button() == Qt::LeftButton)
	{
		_mouseTrackerTimer.start( 30 );
		_dragger.mousePress(e->pos());
	}

	if (e->button() == Qt::RightButton)
	{
		showContextMenu();
	}

}

void Tab::mouseReleaseEvent(QMouseEvent *e)
{
	//printf("Tab[%s]::mouseReleaseEvent()\n", card()->cardName().latin1());

	qApp->setGlobalMouseTracking(false);

	_mouseTrackerTimer.stop();

	if (e->button() == Qt::LeftButton && !_dragger.tracking())
	{
		if (card()->isOpen())
		{
			card()->shut();
		}
		else
		{
			card()->open();
		}
	}

	_dragger.done();
}

void Tab::resizeEvent(QResizeEvent *)
{
	int pxWidth = preferredWidth(), pxHeight = preferredHeight();
	switch(card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			pxWidth = size().width();
			pxHeight = size().height();
			break;
		}

		case LeftEdge: case RightEdge:
		{
			pxWidth = size().height();
			pxHeight = size().width();
			break;
		}

		default: break;
	}

	_px.resize(pxWidth, pxHeight);

	QColor bg = paletteBackgroundColor();
	QColor dark = bg.dark(180);
	_shadowPx.resize(pxWidth-2, 6);

	KPixmapEffect::gradient(_shadowPx, bg, dark, KPixmapEffect::VerticalGradient);
}

void Tab::paintEvent(QPaintEvent *)
{
	QPainter paint(&_px);
	QPainter display(this);

	paintTabBase(&paint);
	paintTabContents(&paint);

	switch(card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			display.drawPixmap(0, 0, _px);
			break;
		}

		case RightEdge:
		{
			QWMatrix m;
			m.rotate(270);

			display.drawPixmap(0, 0, _px.xForm(m));
			break;
		}

		case LeftEdge:
		{
			QWMatrix m;
			m.rotate(90);
			display.drawPixmap(0, 0, _px.xForm(m));
			break;
		}

		default: break;
	}
}

void Tab::dragEnterEvent( QDragEnterEvent *e )
{
	if (_delegate->canDecodeDrag( e ))
	{
		_stillHovering = true;
		QTimer::singleShot(1000, this, SLOT(dragHoverTimeout()));
	}
}

void Tab::mouseMoveEvent( QMouseEvent *e )
{
	if(!_stillHovering)
	{
		_stillHovering = true;
		QTimer::singleShot(1000, this, SLOT(dragHoverTimeout()));
	}
	e->ignore();
}

void Tab::dragMoveEvent( QDragMoveEvent *e )
{
	if (_delegate->canDecodeDrag(e) )
	{
		e->accept();
		//presumably we should make some sort of visual response.
	}
	else
		e->ignore();
}

void Tab::dragLeaveEvent( QDragLeaveEvent * )
{
	_stillHovering = false;
}

void Tab::dropEvent( QDropEvent *e )
{
	if (_delegate->allowDropOnTab())
	{
		_stillHovering = false;
		_delegate->tabDragDropEvent( e );
	}
}

void Tab::mouseMove(QPoint pos)
{
	_dragger.mouseMove(pos);
}


/////////////////////////////////////////////////////////////////////////////

/*
				int _breadth, _length;
				Tray *_tray;
*/

TrayWidget::TrayWidget(Tray *tray, const char *name, int breadth, int length)
	:QWidget(tray, name), _breadth(breadth), _length(length ? length : breadth),
	_tray(tray)
{
	_tray->addTrayWidget(this);
}

void TrayWidget::setDimensions(int breadth, int length)
{
	_breadth = breadth;
	_length = length > 0 ? length : breadth;

	if (_tray) _tray->fitTrayToContents();
}

void TrayWidget::orientationChanged( bool horizontal )
{
	kdDebug(155003) << "TRAYWIDGET:orientationChanged" << endl;
	if (horizontal)
	{
		resize(_breadth, _length);
		setFixedSize(_breadth, _length);
	}
	else
	{
		resize(_length, _breadth);
		setFixedSize(_length, _breadth);
	}
}

/*
		KPixmap _shadowPx;
		LayoutSwitcher *_switcher;
*/

Tray::Tray(Card *c)
	:CardElement(c)
{
	//switcher must be created before call to setPreferredSize, as that
	//triggers a relayout, which requires switcher to be allocated.
	_switcher = new LayoutSwitcher(this, card()->edge(), false); //false means not verbose..
	_switcher->setSpacing(4);

	setPreferredSize(200, 20);
	setName("Tray");
}

Tray::~Tray( void )
{
	delete _switcher;
}

void Tray::addTrayWidget( TrayWidget *w)
{
	_switcher->addWidget(w);
	fitTrayToContents();
}

void Tray::removeTrayWidget( TrayWidget *w )
{
	_switcher->removeWidget(w);
	fitTrayToContents();
}

int Tray::countTrayWidgets( void )
{
	return _switcher->elements().count();
}

TrayWidget *Tray::getTrayWidget( int at )
{
	return (TrayWidget *)_switcher->elements().at(at);
}

void Tray::fitTrayToContents( void )
{
	int totalBreadth = 0, maxLength = 0;
	int wCount = countTrayWidgets();
	if (wCount == 0) return;

	for (int i = 0; i < wCount; i++)
	{
		TrayWidget *tw = getTrayWidget(i);
		if (tw->length() > maxLength) maxLength = tw->length() + 6; /* I don't know where it komes from but it fixes a size Problem at the moment */
		totalBreadth += _switcher->getSpacing() + tw->breadth(); //a little spacing
	}

	setPreferredSize(totalBreadth > 200 ? totalBreadth : 200, maxLength);
	_switcher->relayout();
	//kdDebug() << maxLength << endl; //random numbers in debug output doesn't help much
	//kdDebug() << totalBreadth << endl; //random numbers in debug output doesn't help much
}

void Tray::cardOrientationChanged(void)
{
	kdDebug(155003) << "Tray::cardOrientationChanged()"  << endl;
	CardElement::cardOrientationChanged();
	int pxWidth = preferredWidth(), pxHeight = preferredHeight();
	ScreenEdge edge = card()->edge();

	switch(edge)
	{
		case TopEdge: case BottomEdge:
		{
			pxWidth = size().width();
			pxHeight = size().height();
			break;
		}

		case LeftEdge: case RightEdge:
		{
			pxWidth = size().height();
			pxHeight = size().width();
			break;
		}

		default: break;
	}

	QColor bg = paletteBackgroundColor();
	QColor dark = bg.dark(180);
	_shadowPx.resize(pxWidth, 6);

	KPixmapEffect::gradient(_shadowPx, bg, dark, KPixmapEffect::VerticalGradient);

	switch (edge)
	{
		case TopEdge:
		{
			QWMatrix m;
			m.rotate(180);
			_shadowPx = _shadowPx.xForm(m);
			break;
		}

		case RightEdge:
		{
			QWMatrix m;
			m.rotate(270);
			_shadowPx = _shadowPx.xForm(m);
			break;
		}

		case LeftEdge:
		{
			QWMatrix m;
			m.rotate(90);
			_shadowPx = _shadowPx.xForm(m);
			break;
		}

		default: break; //we don't worry about bottom because it's already set up
	}

	int eCount = countTrayWidgets();
	for (int i = 0; i < eCount; i++)
	{
		TrayWidget *tw = getTrayWidget(i);
		kdDebug(155003) << "Change orientation for all traywidgets" << endl;
		tw->orientationChanged( (edge == TopEdge) || (edge == BottomEdge) );
	}

	_switcher->setEdge(edge);
}

void Tray::paintEvent(QPaintEvent *e)
{
	CardElement::paintEvent(e);
	if (!card()->deck()) return;

	QPainter paint(this);

	/*
		We only paint the shadow if we
			- have a parent card
			- are closed.
	*/
	bool paintShadow = false;
	if (card()->deck()->root() != card() && !card()->isOpen())
		paintShadow = true;

	ScreenEdge adjacentTo = card()->deck()->adjacentEdge();
	if (paintShadow)
	{
		switch ( card()->edge() )
		{
			case TopEdge:
			{
				if (adjacentTo == None)
					paint.drawPixmap(1, 0, _shadowPx, 2, 4);

				else if (adjacentTo == LeftEdge)
					paint.drawPixmap(0, 0, _shadowPx, 1, 4);

				else if (adjacentTo == RightEdge)
					paint.drawPixmap(1, 0, _shadowPx, 1, 4);

				break;
			}

			case RightEdge:
			{
				if (adjacentTo == None)
					paint.drawPixmap(width() - 3, 1, _shadowPx, 0, 2);

				else if (adjacentTo == TopEdge)
					paint.drawPixmap(width() - 3, 0, _shadowPx, 0, 1);

				else if (adjacentTo == BottomEdge)
					paint.drawPixmap(width() - 3, 1, _shadowPx, 0, 1);

				break;
			}

			case BottomEdge:
			{
				if (adjacentTo == None)
					paint.drawPixmap(1, height() - 3, _shadowPx, 2);

				else if (adjacentTo == LeftEdge)
					paint.drawPixmap(0, height() - 3, _shadowPx, 1);

				else if (adjacentTo == RightEdge)
					paint.drawPixmap(1, height() - 3, _shadowPx, 1);

				break;
			}

			case LeftEdge:
			{
				if (adjacentTo == None)
					paint.drawPixmap(0, 1, _shadowPx, 3, 2);

				else if (adjacentTo == TopEdge)
					paint.drawPixmap(0, 0, _shadowPx, 3, 1);

				else if (adjacentTo == BottomEdge)
					paint.drawPixmap(0, 1, _shadowPx, 3, 1);


				break;
			}

			default: break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
//	Separator
//
//		QPoint _mouseDownPos;
//		bool _dragging, _gripperActive;
//		int _originalContentsHeight, _newContentsHeight;

Separator::Separator(Card *card, bool gripper)
	: CardElement(card), _gripperActive(gripper)
{
	setFrameStyle(NoFrame);
	setGripper(gripper);
	doCursor();
	collapse(false);
	setName("Gripper");
}

void Separator::setGripper( bool t )
{
	_gripperActive = t;
	if (_gripperActive)
	{
		setPreferredHeight( 5 );
	}
	else
	{
		setPreferredHeight( 2 );
	}
	repaint();
}

bool Separator::gripper( void )
{
	return _gripperActive;
}

void Separator::cardOrientationChanged( void )
{
	CardElement::cardOrientationChanged();
	doCursor();
}

void Separator::resizeTimeout( void )
{
	//this hack isn't necessary for cards on top or left edge.
	if (card()->edge() == TopEdge || card()->edge() == LeftEdge) return;

	if (_dragging)
	{
		if (_newContentsHeight > -1)
		{
			card()->getContents()->setPreferredHeight(_newContentsHeight);
			card()->elementSizeChanged(false);
		}

		QTimer::singleShot(200, this, SLOT(resizeTimeout()));
		return;
	}
}

void Separator::doCursor( void )
{
	if (!card()) unsetCursor();

	switch(card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			if (_gripperActive) setCursor(QCursor(Qt::SizeVerCursor));
			else unsetCursor();

			break;
		}

		case RightEdge: case LeftEdge:
		{
			if (_gripperActive) setCursor(QCursor(Qt::SizeHorCursor));
			else unsetCursor();

			break;
		}

		default: unsetCursor();
	}
}

void Separator::paintEvent(QPaintEvent *e)
{
	CardElement::paintEvent(e);

	QPainter paint(this);
	paint.setPen(darkColor());

	if (_gripperActive)
	{

		switch( card()->edge() )
		{
			case TopEdge: case BottomEdge:
			{
				paint.setPen(lightColor());
				paint.drawLine( 3, 1, rect().right() - 3, 1);
				paint.drawLine( 3, 1, 3, rect().bottom() - 1);

				paint.setPen(mediumDarkColor());
				paint.drawLine( rect().right() - 3, 1, rect().right() - 3, rect().bottom() - 1);
				paint.drawLine( 3, rect().bottom() - 1, rect().right() - 3, rect().bottom() - 1);

				break;
			}

			case RightEdge: case LeftEdge:
			{
				paint.setPen(lightColor());
				paint.drawLine( 1, 3, 1, rect().bottom() - 3);
				paint.drawLine( 1, 3, rect().right() - 1, 3);

				paint.setPen(mediumDarkColor());
				paint.drawLine( rect().right() - 1, 3, rect().right() - 1, rect().bottom() - 3);
				paint.drawLine( 1, rect().bottom() - 3, rect().right() - 1, rect().bottom() - 3);

				break;
			}

			default: break;
		}
	}
	else
	{
		switch(card()->edge())
		{
			case TopEdge: case BottomEdge:
			{
				paint.setPen(mediumDarkColor());
				paint.drawLine( 3, 0, rect().right() - 3, 0);

				break;
			}

			case RightEdge: case LeftEdge:
			{
				paint.setPen(mediumDarkColor());
				paint.drawLine( 0, 3, 0, rect().bottom() - 3);
				break;
			}

			default: break;
		}
	}
}

void Separator::mousePressEvent(QMouseEvent *e)
{
	if (_gripperActive)
	{
		_mouseDownPos = e->globalPos();
		_deckULPos = card()->deck()->frameGeometry().topLeft();

		_dragging = true;
		_originalContentsHeight = _card->getContents()->preferredHeight();
		_newContentsHeight = -1;

		resizeTimeout(); //will stop when _dragging goes false
	}
}

void Separator::mouseMoveEvent(QMouseEvent *e)
{
	if (!_dragging || !_gripperActive) return;

	QPoint mPos(e->globalPos());
	Contents *cardContents = card()->getContents();

	switch (card()->edge())
	{
		case TopEdge:
		{
			int dy = mPos.y() - _mouseDownPos.y();
			cardContents->setPreferredHeight(_originalContentsHeight + dy);
			card()->elementSizeChanged(true);
			break;
		}

		case RightEdge:
		{
			int dx = mPos.x() - _mouseDownPos.x();

			_newContentsHeight = _originalContentsHeight - dx;
			if (_newContentsHeight < 0)
			{
				_newContentsHeight = 0;
				dx = _originalContentsHeight;
			}

			card()->deck()->move(_deckULPos.x() + dx, _deckULPos.y());
			break;
		}

		case BottomEdge:
		{
			int dy = mPos.y() - _mouseDownPos.y();

			_newContentsHeight = _originalContentsHeight - dy;
			if (_newContentsHeight < 0)
			{
				_newContentsHeight = 0;
				dy = _originalContentsHeight;
			}

			card()->deck()->move(_deckULPos.x(), _deckULPos.y() + dy);
			break;
		}

		case LeftEdge:
		{
			int dx = mPos.x() - _mouseDownPos.x();
			cardContents->setPreferredHeight(_originalContentsHeight + dx);
			card()->elementSizeChanged(true);
			break;
		}

		default: break;
	}

}

void Separator::mouseReleaseEvent(QMouseEvent *)
{
	if (_gripperActive)
	{
		_dragging = false;
		if (_newContentsHeight > -1)
		{
			card()->getContents()->setPreferredHeight(_newContentsHeight);
			card()->elementSizeChanged(true);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//		QWidget *_contents;

Contents::Contents(Card *card)
	:CardElement(card), _contents(0L)
{
	setName("Contents");
	setPreferredSize(200, 100);
	collapse(false);
}

QWidget* Contents::setContents(QWidget *c)
{
	QWidget *oldContents = 0L;
	if (_contents)
	{
		oldContents = _contents;
		oldContents->reparent(0L, QPoint(0, 0), false);
	}

	_contents = c;

	if (_contents)
	{
		_contents->reparent(this, QPoint(1, 1), true);
		relayoutContents();
	}

	return oldContents;
}

QWidget *Contents::getContents( void )
{
	return _contents;
}

void Contents::cardOrientationChanged( void )
{
	CardElement::cardOrientationChanged();
	relayoutContents();
}

void Contents::resizeEvent(QResizeEvent *e)
{
	CardElement::resizeEvent(e);
	relayoutContents();
}

void Contents::relayoutContents( void )
{
	if (!_contents) return;
	QRect r(rect());
	QSize ms(_contents->sizeHint());

	/*
		We check on relayout to see if we're trying to layout a widget
		shorter/narrower than its sizeHint(). If that's the case, we just
		translate it, instead of resizing. This is to work around a bug (?)
		in QGridLayout which crashes if height/width is zero.
	*/

	switch(card()->edge())
	{
		case TopEdge:
		{
			if (r.height() < ms.height())
				_contents->move(1, r.height() - ms.height());
			else
				_contents->setGeometry(1, 0, r.width() - 2, r.height());
			break;
		}

		case RightEdge:
		{
			if (r.width() < ms.width())
				_contents->move(0, 1);
			else
				_contents->setGeometry(0, 1, r.width(), r.height() - 2);
			break;
		}

		case BottomEdge:
		{
			if (r.height() < ms.height())
				_contents->move(1, 0);
			else
				_contents->setGeometry(1, 0, r.width() - 2, r.height());
			break;
		}

		case LeftEdge:
		{
			if (r.width() < ms.width())
				_contents->move(r.width() - ms.width(), 1);
			else
				_contents->setGeometry(0, 1, r.width(), r.height() - 2);
			break;
		}

		default: break;
	}
}

Resizer::Resizer(Card *card, bool resizer)
	: CardElement(card), _resizerActive(resizer)
{
	setFrameStyle(QFrame::Box);
	setResizer(resizer);
	doCursor();
	setName("Resizer");
	collapse(false);
}

void Resizer::setResizer( bool t )
{
	_resizerActive = t;
	_RightSide = t;
	if (_resizerActive)
	{
		setPreferredSize(3,100);

	}
	else
	{
		setPreferredSize(0,0 );
	}
	repaint();
}

bool Resizer::resizer( void )
{
	return _resizerActive;
}

void Resizer::doCursor( void )
{
	if (!card()) unsetCursor();

	switch(card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			if (_resizerActive) setCursor(QCursor(Qt::SizeHorCursor));
			else unsetCursor();

			break;
		}

		case RightEdge: case LeftEdge:
		{
			if (_resizerActive) setCursor(QCursor(Qt::SizeVerCursor));
			else unsetCursor();

			break;
		}

		default: unsetCursor();
	}
}
void Resizer::cardOrientationChanged( void )
{
	CardElement::cardOrientationChanged();
	doCursor();
}

void Resizer::paintEvent(QPaintEvent *e)
{
	CardElement::paintEvent(e);
	QPainter paint(this);

	if (_RightSide)
	{
		paint.setPen(Qt::red);
		paint.setBackgroundMode( Qt::OpaqueMode );
		//paint.drawLine(0,0,100,100);
	}
}

void Resizer::mousePressEvent(QMouseEvent *e)
{
	if (_resizerActive)
	{
		_mouseDownPos = e->globalPos();
		_deckULPos = card()->deck()->frameGeometry().topLeft();

		_dragging = true;
		_originalContentsWidth = _card->getContents()->preferredWidth();
		_newContentsWidth = -1;

//		resizeTimeout(); //will stop when _dragging goes false
	}
}

void Resizer::mouseMoveEvent(QMouseEvent *e)
{
	if (!_dragging || !_resizerActive) return;

	QPoint mPos(e->globalPos());
	Contents *cardContents = card()->getContents();


	switch (card()->edge())
	{
		case TopEdge: case BottomEdge:
		{
			if (_RightSide)
			{
				int dx = mPos.x() - _mouseDownPos.x();
				cardContents->setPreferredWidth(_originalContentsWidth + dx);
			}
			else
			{
				int dx = mPos.x() - _mouseDownPos.x();
				cardContents->setPreferredWidth(_originalContentsWidth - dx);
			}
			break;
		}

		case RightEdge: case LeftEdge:
		{
			if (_RightSide)
			{
				int dy = mPos.y() - _mouseDownPos.y();
				cardContents->setPreferredWidth(_originalContentsWidth + dy);
			}
			else
			{
				int dy = mPos.y() - _mouseDownPos.y();
				cardContents->setPreferredWidth(_originalContentsWidth - dy);
			}
			break;
		}
			/*case BottomEdge:
			{
				int dx = mPos.x() - _mouseDownPos.x();
				cardContents->setPreferredWidth(_originalContentsWidth + dx);
				//card()->elementSizeChanged(true);
				break;
			}

			case LeftEdge:
			{
				int dy = mPos.y() - _mouseDownPos.y();
				cardContents->setPreferredWidth(_originalContentsWidth + dy);
				//card()->elementSizeChanged(true);
				break;
			}*/

			default: break;
	}
	card()->elementSizeChanged(true);
}

void Resizer::mouseReleaseEvent(QMouseEvent *)
{
	if (_resizerActive)
	{
		_dragging = false;
		if (_newContentsWidth > -1)
		{
			card()->getContents()->setPreferredWidth(_newContentsWidth);
			card()->elementSizeChanged(true);
		}
	}
}

void Resizer::setSide(bool RightSide )
{
	_RightSide = RightSide;
}



}; //namespace
