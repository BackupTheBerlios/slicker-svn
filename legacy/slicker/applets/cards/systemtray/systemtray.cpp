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

#include "systemtray.h"
#include "configure_impl.h"

#include <carddeskcore/Card.h>
#include <carddeskcore/CardManager.h>

#include <kiconloader.h>
#include <klocale.h>
#include <kwinmodule.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kwin.h>

#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qstringlist.h>

#include <X11/Xlib.h>

using namespace CardDesk;

Systemtray::Systemtray(const QString &configFile)
	: CardApplet(configFile)
{
	KIconLoader il;
	_icon = il.loadIcon("ksysguard", KIcon::Small, 0, KIcon::DefaultState);
	config = new ConfigureImpl();
	connect(config, SIGNAL(newConfig()), SLOT(configChanged()));
}


Systemtray::~Systemtray()
{
	delete trayWidget;
}

QPixmap *Systemtray::tabIcon( void )
{
	return &_icon;
}

void Systemtray::addToDefaultContextMenu( QPopupMenu *menu )
{
	menu->insertItem(SmallIcon("configure"), i18n("Configure"), this, SLOT(slotconfigure()));
}

void Systemtray::slotconfigure()
{
    config->displayDialog(trayWidget->getm_wins(), contentsWidget->getm_wins());
}

void Systemtray::aboutToShowContextMenu( class QPopupMenu * )
{}

bool Systemtray::allowDragHoverToOpen( void )
{
	return true;
}

bool Systemtray::allowDropOnTab( void )
{
	return true;
}

bool Systemtray::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void Systemtray::tabDragDropEvent( QDropEvent * )
{}

Tray *Systemtray::createTray( Card *c )
{
	Tray *tray = new Tray(c);
	_c = c;

	trayWidget = new SystemtrayWidget(tray, "TrayWidget", c->breadth(), 24 * config->getMaxtraylines(), config, c);

	return tray;
}

Contents *Systemtray::createContents( Card *c )
{
	Contents *contents = new Contents(c);

	contentsWidget = new ContentsWidget(config, c->breadth(), 24 * config->getMaxcontentslines(), trayWidget, c, contents);

	contents->setContents(contentsWidget);
	contents->setPreferredSize(c->breadth(), 24 * config->getMaxcontentslines());
	connect(this, SIGNAL(propertiesChanged()), contentsWidget, SLOT(layoutTray()));

	return contents;
}

void Systemtray::saveSettings( KConfig * )
{}

void Systemtray::loadSettings( KConfig * )
{}

void Systemtray::cardReady( void )
{
		bool existing = false;
		bool content = false;

		kwin_module = new KWinModule();
		systemTrayWindows = kwin_module->systemTrayWindows();
		for (QValueList<WId>::ConstIterator it = systemTrayWindows.begin(); it!=systemTrayWindows.end(); it++)
		{
			QXEmbed *emb;
			KWin::Info info = KWin::info(*it);
			content = testBackgroundConfig( info.name );
			if(content)
				if(!config->getDefIcon())
					emb = new QXEmbed(contentsWidget);
				else
					emb = new QXEmbed(trayWidget);
			else
				if(!config->getDefIcon())
					emb = new QXEmbed(trayWidget);
				else
					emb = new QXEmbed(contentsWidget);
			emb->setAutoDelete(false);
			emb->setBackgroundMode(X11ParentRelative);
			connect(emb, SIGNAL(embeddedWindowDestroyed()), SLOT(updateTrayWindows()));
			if(content)
				if(!config->getDefIcon())
					contentsWidget->getm_wins()->append(emb);
				else
					trayWidget->getm_wins()->append(emb);
			else
				if(!config->getDefIcon())
					trayWidget->getm_wins()->append(emb);
				else
					contentsWidget->getm_wins()->append(emb);

			emb->embed(*it);
			emb->resize(24, 24);
			emb->show();
			existing = true;
		}

		if (existing)
			updateTrayWindows();

		connect(kwin_module, SIGNAL(systemTrayWindowAdded(WId)), SLOT(systemTrayWindowAdded(WId)));
		connect(kwin_module, SIGNAL(systemTrayWindowRemoved(WId)), SLOT(updateTrayWindows()));

		QCString screenstr;
		screenstr.setNum(qt_xscreen());
		QCString trayatom = "_NET_SYSTEM_TRAY_S" + screenstr;

		Display *display = qt_xdisplay();
		net_system_tray_selection = XInternAtom( display, trayatom, false );
		net_system_tray_opcode = XInternAtom( display, "_NET_SYSTEM_TRAY_OPCODE", false );

		// Acquire system tray
		XSetSelectionOwner( display,
			net_system_tray_selection,
			trayWidget->winId(),
			CurrentTime );

		WId root = qt_xrootwin();

		if (XGetSelectionOwner(display, net_system_tray_selection) == trayWidget->winId())
		{
			XClientMessageEvent xev;

			xev.type = ClientMessage;
			xev.window = root;

			xev.message_type = XInternAtom(display, "MANAGER", false);
			xev.format = 32;

			xev.data.l[0] = CurrentTime;
			xev.data.l[1] = net_system_tray_selection;
			xev.data.l[2] = trayWidget->winId();
			xev.data.l[3] = 0;       /* Manager specific data */
			xev.data.l[4] = 0;       /* Manager specific data */

			XSendEvent( display, root, false, StructureNotifyMask, (XEvent *)&xev );
		}
}

bool Systemtray::testBackgroundConfig( QString str )
{
	bool exist = false;
	QStringList background = config->getBackgroundnames();

	for( QStringList::Iterator it = background.begin(); it != background.end(); ++it )
	{
		if(str == (QString)(*it))
		{
			    exist = true;
			    break;
		}
	}

	return exist;
}

void Systemtray::updateTrayWindows( void )
{
	QXEmbed *emb;

	emb = trayWidget->getm_wins()->first();
	while ((emb = trayWidget->getm_wins()->current()) != 0L)
	{
		WId wid = emb->embeddedWinId();
		if ((wid == 0) || !kwin_module->systemTrayWindows().contains(wid) )
			trayWidget->getm_wins()->remove(emb);
		else
			trayWidget->getm_wins()->next();
	}
	emb = contentsWidget->getm_wins()->first();
	while ((emb = contentsWidget->getm_wins()->current()) != 0L)
	{
		WId wid = emb->embeddedWinId();
		if ((wid == 0) || !kwin_module->systemTrayWindows().contains(wid) )
			contentsWidget->getm_wins()->remove(emb);
		else
			contentsWidget->getm_wins()->next();
	}

	trayWidget->layoutTray();
	contentsWidget->layoutTray();
}

void Systemtray::systemTrayWindowAdded( WId w )
{
	bool content = false;
	QXEmbed *emb;

	KWin::Info info = KWin::info(w);
	content = testBackgroundConfig( info.name );

	if(content)
		if(!config->getDefIcon())
			emb = new QXEmbed(contentsWidget);
		else
			emb = new QXEmbed(trayWidget);
	else
		if(!config->getDefIcon())
			emb = new QXEmbed(trayWidget);
		else
			emb = new QXEmbed(contentsWidget);
	emb->setAutoDelete(false);
	emb->setBackgroundMode(X11ParentRelative);
	connect(emb, SIGNAL(embeddedWindowDestroyed()), SLOT(updateTrayWindows()));
	if(content)
		if(!config->getDefIcon())
			contentsWidget->getm_wins()->append(emb);
		else
			trayWidget->getm_wins()->append(emb);
	else
		if(!config->getDefIcon())
			trayWidget->getm_wins()->append(emb);
		else
			contentsWidget->getm_wins()->append(emb);

	emb->embed(w);
	emb->resize(24, 24);
	emb->show();

	trayWidget->layoutTray();
	contentsWidget->layoutTray();
}

void Systemtray::configChanged()
{
	QValueList<WId> ids;
	QXEmbed *emb;
	int i;

	emb = trayWidget->getm_wins()->first();
	while ((emb = trayWidget->getm_wins()->current()) != 0L)
	{
		WId wid = emb->embeddedWinId();
		ids.append(wid);
		trayWidget->getm_wins()->next();
	}
	emb = contentsWidget->getm_wins()->first();
	while ((emb = contentsWidget->getm_wins()->current()) != 0L)
	{
		WId wid = emb->embeddedWinId();
		ids.append(wid);
		contentsWidget->getm_wins()->next();
	}
	emb = trayWidget->getm_wins()->first();
	while ((emb = trayWidget->getm_wins()->current()) != 0L)
	{
		trayWidget->getm_wins()->remove(emb);
		trayWidget->getm_wins()->first();
	}
	emb = contentsWidget->getm_wins()->first();
	while ((emb = contentsWidget->getm_wins()->current()) != 0L)
	{
		contentsWidget->getm_wins()->remove(emb);
		contentsWidget->getm_wins()->first();
	}
	for(i = 0; i <= (int)ids.count()-1; i++)
		systemTrayWindowAdded(*ids.at(i));
}
