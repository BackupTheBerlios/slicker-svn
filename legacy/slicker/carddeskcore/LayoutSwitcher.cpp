/***************************************************************************
                          LayoutSwitcher.cpp  -  description
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

#include "LayoutSwitcher.h"
#include <qwidget.h>
#include <qlayout.h>
#include <qsizepolicy.h>
#include <qlabel.h>
#include <qcstring.h>
#include <kdebug.h>

/////////////////////////////////////////////////////////////////////////////
// LayoutSwitcher
//
//		ScreenEdge _edge;
//		QBoxLayout *_layout;
//		QPtrList<QWidget> _layoutItems;
//		QWidget *_widget;
//		bool _verbose;
//		int _spacing;

namespace CardDesk {

LayoutSwitcher::LayoutSwitcher( QWidget * widget, ScreenEdge edge, bool verbose )
	: _edge(edge), _layout(0L), _contentslayout(0L), _widget(widget), _verbose(verbose), _spacing(-1)
{
	_layoutItems.setAutoDelete(false);
}

QPtrList<QWidget> &LayoutSwitcher::elements()
{
	return _layoutItems;
}

void LayoutSwitcher::setSpacing( int s )
{
	if (s < 0) s = 0;
	_spacing = s;
	relayout();
}

void LayoutSwitcher::addWidget( QWidget *e, bool r, int at)
{
	if (_verbose)
		kdDebug(155003) << "LayoutSwitcher: "  << _widget->name() << "  " <<  e->name() << endl;

	if (e == _widget)
	{
		if (_verbose)
			kdDebug(155003) <<  ("LayoutSwitcher[%s]::addWidget() Attempting to add widget %s to itself!\n",
			       _widget->name(), e->name());

		return;
	}

	if (e->parentWidget() != _widget)
	{
		if (_verbose)
			printf("\te->parentWidget: %s != _widget: %s\treparenting\n",
						 e->parentWidget() ? e->parentWidget()->name() : "0L",
						 _widget->name());

		e->reparent(_widget, QPoint(0, 0), false);
	}

	if (at == -1)
	{
		if (_verbose)
		{
			printf("\tAppending widget to end of stack\n");
		}
		_layoutItems.append(e);
	}
	else
	{
		if (_verbose)
		{
			printf("\tInserting widget to position %d in stack\n", at);
		}
		_layoutItems.insert(at, e);
	}


	e->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

	if (r)
	{
		kdDebug(155003) << "Laying out contents\n";
		relayout();
	}
}

void LayoutSwitcher::removeWidget( QWidget *e)
{
	_layoutItems.remove(e);
	relayout();
}

void LayoutSwitcher::removeAll( bool r )
{
	_layoutItems.clear();
	if (r) relayout();
}

void LayoutSwitcher::setEdge( ScreenEdge edge )
{
	if (_verbose)
	{
		printf("LayoutSwitcher[%s]::setEdge( edge: %s )\n",
					 _widget->name(), EdgeToString(edge).latin1());

	}
	
	_edge = edge;

	if (_layout)
	{
		for (QWidget *i = _layoutItems.first(); i; i = _layoutItems.next())
		{
			_layout->remove(i);
			_contentslayout->remove(i);
		}
		_layout->removeItem(_contentslayout);
		delete _contentslayout;
		_contentslayout = 0L;
		delete _layout;
		_layout = 0L;
	}

	if (_spacing > 0)
	{
		_layout = new QBoxLayout(_widget,QBoxLayout::TopToBottom, _spacing);
		_contentslayout  = new QBoxLayout( QBoxLayout::LeftToRight, _spacing);
	}
	else
	{
		_layout = new QBoxLayout(_widget,QBoxLayout::TopToBottom, -1);
		_contentslayout  = new QBoxLayout( QBoxLayout::LeftToRight, -1);
	}
	
	for (QWidget *i = _layoutItems.first(); i; i = _layoutItems.next())
	{	
		if (i->size().height() > 1 )
		{	
			if (!i->isVisible()) i->show();

			if (i->name() == QCString("Contents"))
			{
				_contentslayout->addWidget(i);
				_layout->addLayout(_contentslayout);
			}
			else if (i->name() == QCString("Resizer"))
			{
				_contentslayout->addWidget(i);
			}
			else _layout->addWidget(i);
		}
		else i->hide();
	}


	switch( _edge )
	{
		case TopEdge:
		{
			_layout->setDirection(QBoxLayout::TopToBottom);
			_contentslayout->setDirection(QBoxLayout::LeftToRight);
			break;
		}

		case BottomEdge:
		{
			_layout->setDirection(QBoxLayout::BottomToTop);
			_contentslayout->setDirection(QBoxLayout::LeftToRight);
			break;
		}

		case RightEdge:
		{
			_layout->setDirection(QBoxLayout::RightToLeft);
			_contentslayout->setDirection(QBoxLayout::TopToBottom);
			break;
		}

		case LeftEdge:
		{
			_layout->setDirection(QBoxLayout::LeftToRight);
			_contentslayout->setDirection(QBoxLayout::TopToBottom);
			break;
		}

		default: break;
	}
}


ScreenEdge LayoutSwitcher::edge( void )
{
	return _edge;
}

void LayoutSwitcher::relayout( void )
{
	setEdge(_edge);
}

}; //namespace
