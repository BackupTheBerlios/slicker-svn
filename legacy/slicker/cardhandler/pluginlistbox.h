/***************************************************************************
                          PluginListBox.h  -  description
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

#ifndef PLUGINLISTBOX_H
#define PLUGINLISTBOX_H

#include <qwidget.h>
#include <qlabel.h>
#include <qptrlist.h>
#include <qscrollview.h>

#include <plugininfo.h>

/**
  *@author Shamyl Zakariya
  */

class PluginListBox;
  
class PluginListBoxEntry : public QLabel
{
	Q_OBJECT
	public:
		PluginListBoxEntry(PluginListBox *parent, PluginInfo *info);

		void hilight( bool );
		PluginInfo *info( void ) { return _info; }

	signals:
		void selected(PluginListBoxEntry *);

	protected:
		virtual void mousePressEvent(QMouseEvent *);
		virtual void paintEvent(QPaintEvent *);

		PluginListBox *_listBox;
		PluginInfo *_info;
		bool _hilight;
};
  
class PluginListBox : public QScrollView
{
	Q_OBJECT
	public: 
		PluginListBox(QWidget *parent=0, const char *name=0);

		QWidget *container( void ) { return _listWidget; }

		//void populate(PluginRoster *);
		void populate();
		PluginInfo *current( void );

		PluginListBoxEntry *nextFor(PluginListBoxEntry *);
		PluginListBoxEntry *previousFor(PluginListBoxEntry *);

	signals:
		void pluginSelected( PluginInfo * );

	public slots:
		void slotPluginSelected( PluginListBoxEntry * );
		void slotSelectNextPlugin( void );
		void slotSelectPreviousPlugin( void );
		
	protected:
		virtual void keyPressEvent(QKeyEvent *);
	
		QPtrList<PluginListBoxEntry> _entries;
		PluginListBoxEntry *_current;

		QWidget *_listWidget;
		class QVBoxLayout *_listLayout;	
};

#endif
