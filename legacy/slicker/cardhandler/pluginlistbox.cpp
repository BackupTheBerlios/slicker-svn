/***************************************************************************
                          PluginListBox.cpp  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Shamyl Zakariya
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

#include "pluginlistbox.h"
#include <kglobalsettings.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qrect.h>
#include <kdebug.h>

#include <pluginmanager.h>

/*
		PluginListBox *_listBox;
		PluginRoster::Info *_info;
		bool _hilight;
*/

PluginListBoxEntry::PluginListBoxEntry(PluginListBox *parent, PluginInfo *info)
	: QLabel(parent->container(), info->name().latin1()),
	_listBox(parent), _info(info), _hilight(false)
{
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setMargin(4);

	QString text;
	text.sprintf("<b>%s</b><br>%s", _info->name().latin1(), _info->description().latin1());
	setText(text);

	hilight(false);

	parent->connect(this, SIGNAL(selected(PluginListBoxEntry *)),
	                SLOT(slotPluginSelected(PluginListBoxEntry *)));
}

void PluginListBoxEntry::hilight( bool h)
{
	_hilight = h;
	if (_hilight)
	{
		setPaletteBackgroundColor(KGlobalSettings::highlightColor());
		setPaletteForegroundColor(KGlobalSettings::highlightedTextColor());
		setFrameStyle(Box | Plain);
		setIndent(0);
	}
	else
	{
		setPaletteBackgroundColor(KGlobalSettings::baseColor());
		setPaletteForegroundColor(KGlobalSettings::textColor());
		setFrameStyle(NoFrame);
		setIndent(1);
	}
}

void PluginListBoxEntry::mousePressEvent(QMouseEvent *)
{
	emit selected(this);
}

void PluginListBoxEntry::paintEvent(QPaintEvent *e)
{
	QLabel::paintEvent(e);
	if (!_hilight)
	{
		bool doIt = false;
		PluginListBoxEntry *next = _listBox->nextFor(this);
		if (!next) doIt = true;
		if (next && !next->_hilight) doIt = true;

		if (doIt)
		{
			QColor lineColor = KGlobalSettings::baseColor().dark();
			QPainter paint(this);
			QRect r(rect());
		
			paint.setPen(lineColor);
			paint.drawLine(r.bottomLeft(), r.bottomRight());
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
/*
		QPtrList<PluginListBoxEntry> _entries;
		PluginListBoxEntry *_current;
*/

PluginListBox::PluginListBox(QWidget *parent, const char *name )
	: QScrollView(parent,name), _current(0L)
{
	viewport()->setPaletteBackgroundColor(KGlobalSettings::baseColor());
	
	_entries.setAutoDelete(false);
	setResizePolicy(AutoOneFit);

	_listWidget = new QWidget(viewport());
	_listLayout = new QVBoxLayout(_listWidget);
	_listLayout->setSpacing(0);
	_listLayout->setMargin(0);
		
	addChild(_listWidget);
}

void PluginListBox::populate()
 {
 	_entries.setAutoDelete(true);
 	_entries.clear();
 	_entries.setAutoDelete(false);
 	

	QPtrList<PluginInfo> list = PluginManager::pluginManager()->cardPlugins();
	PluginInfo *info;
	for (info = list.first(); info !=0 ; info = list.next() ) {
		PluginListBoxEntry *entry = new PluginListBoxEntry(this, info);
		kdDebug(155004) << "populate() name = " << info->name() << endl;
 		_entries.append( entry );
 		_listLayout->addWidget(entry);
	}
	
 	_listLayout->addStretch(1);
 }

PluginListBoxEntry *PluginListBox::nextFor(PluginListBoxEntry *e)
{
	int pCount = _entries.count();
	for (int i = 0; i < pCount; i++)
	{
		PluginListBoxEntry *entry = _entries.at(i);
		if (entry == e) return _entries.at(i + 1);
	}

	return 0L;
}

PluginListBoxEntry *PluginListBox::previousFor(PluginListBoxEntry *e)
{
	int pCount = _entries.count();
	for (int i = 0; i < pCount; i++)
	{
		PluginListBoxEntry *entry = _entries.at(i);
		if (entry == e) return _entries.at(i - 1);
	}

	return 0L;
}


PluginInfo *PluginListBox::current( void )
{
	return _current->info();
}

void PluginListBox::slotPluginSelected( PluginListBoxEntry *e )
{
	int count = _entries.count();
	for (int i = 0; i < count; i++)
	{
		PluginListBoxEntry *entry = _entries.at(i);
		entry->hilight( entry == e );
	}

	_current = e;
	emit pluginSelected( _current->info() );
}

void PluginListBox::slotSelectNextPlugin( void )
{
	PluginListBoxEntry *next = 0L;
	if (_current)
	{
		next = nextFor(_current);
	}
	else
	{
		next = _entries.at(0);
	}

	if (next) slotPluginSelected(next);
}

void PluginListBox::slotSelectPreviousPlugin( void )
{
	PluginListBoxEntry *prev = 0L;
	if (_current)
	{
		prev = previousFor(_current);
	}
	else
	{
		prev = _entries.at(0);
	}

	if (prev) slotPluginSelected(prev);
}

void PluginListBox::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Up)
	{
		slotSelectPreviousPlugin();
	}
	else if (e->key() == Qt::Key_Down)
	{
		slotSelectNextPlugin();
	}

	if (_current)
	{
		int cCount = _entries.count();
		int y = 0;
		for (int i = 0; i < cCount; i++)
		{
			PluginListBoxEntry *entry = _entries.at(i);
			y += entry->size().height();
			if (entry == _current)
			{
				y -= entry->size().height() / 2;
				break;
			}
		}

		ensureVisible(0, y);
	}
	
}
