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

#ifndef SYSTEMTRAYWIDGET_H
#define SYSTEMTRAYWIDGET_H

#include "configure_impl.h"

#include <carddeskcore/CardApplet.h>

#include <qwidget.h>
#include <qxembed.h>
#include <qptrlist.h>

class KArrowButton;
class QXEmbed;

typedef long unsigned int Atom;

using namespace CardDesk;

class SystemtrayWidget : public TrayWidget
{
	Q_OBJECT
public:
	SystemtrayWidget(Tray *tray, const char *name, int breadth, int length, ConfigureImpl *config, Card *c);
	~SystemtrayWidget();

	virtual void orientationChanged(bool horizontal);

	bool getHorizontal() { return _horizontal; }

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
	Card *_c;

	KArrowButton *rightarrow;
	KArrowButton *leftarrow;

	bool arrow;
	bool _horizontal;
	int position;
};

#endif
