/***************************************************************************
    copyright (C) 2003      Sven Leiber <s.leiber@web.de>
                  2000-2001 Matthias Ettrich <ettrich@kde.org>
                  2000-2001 Matthias Elter   <elter@kde.org>
                  2001      Carsten Pfeiffer <pfeiffer@kde.org>
                  2001      Martijn Klingens <mklingens@yahoo.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONTENTSWIDGET_H
#define CONTENTSWIDGET_H

#include "configure_impl.h"
#include "systemtraywidget.h"

#include <carddeskcore/CardApplet.h>

#include <qwidget.h>
#include <qxembed.h>
#include <qptrlist.h>

class KArrowButton;
class QXEmbed;

typedef long unsigned int Atom;

using namespace CardDesk;

class ContentsWidget : public QWidget
{
	Q_OBJECT
public:
	ContentsWidget(ConfigureImpl *config, int width, int height, SystemtrayWidget *trayWidget, Card *c, Contents *contents);
	~ContentsWidget();

	QPtrList<QXEmbed> *getm_wins();

public slots:
	void layoutTray();

protected slots:
	void leftarrowPressed();
	void rightarrowPressed();

private:
	QPtrList<QXEmbed> m_Wins;
	Atom net_system_tray_selection;
	Atom net_system_tray_opcode;

	ConfigureImpl *_config;
	SystemtrayWidget *_trayWidget;
	Contents *_contents;
	Card *_c;

	KArrowButton *rightarrow;
	KArrowButton *leftarrow;

	bool _horizontal;
	bool arrow;
	int position;
	int _width;
	int _height;
};

#endif
