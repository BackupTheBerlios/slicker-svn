/*
 * Copyright (C) 2003 by Shamyl Zakariya, zakariya@earthlink.net
 * Copyright (C) 2003 by Mark Westcott, mark@houseoffish.org
 *

***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <carddeskcore/Card.h>

#include <kconfig.h>
#include <kiconloader.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qdir.h>
#include <qtimer.h>
#include <konq_iconviewwidget.h>
#include "launchercardapplet.h"
#include <kdebug.h>
#include <kstandarddirs.h>
#include <krandomsequence.h>
#include "kdiconview.h"
#include "launcher.h"

#include <stdlib.h>
#include <kmessagebox.h>

#include <kio/netaccess.h>
using namespace CardDesk;

LauncherCardApplet::LauncherCardApplet(const QString &configFile)
	: CardApplet(configFile)
{
	launcherWidth = config()->readNumEntry("LauncherWidth",200);
	launcherHeight = config()->readNumEntry("LauncherHeight",200);
	shortcutURL = config()->readEntry("ShortcutURL", generateShortcutPath());
	KIconLoader il;
	mIcon = il.loadIcon("run", KIcon::Small, 0, KIcon::DefaultState);
}

LauncherCardApplet::~LauncherCardApplet()
{
	config()->writeEntry("LauncherWidth",launcher->mpIconView->contentsWidth());
	config()->writeEntry("LauncherHeight",launcher->mpIconView->contentsHeight());
	config()->writeEntry("ShortcutURL", launcher->url().path());
	config()->sync();
	launcher->shutdown();
}

void LauncherCardApplet::addToDefaultContextMenu( QPopupMenu *defaultMenu )
{}

void LauncherCardApplet::aboutToShowContextMenu( class QPopupMenu * )
{

}

bool LauncherCardApplet::allowDragHoverToOpen( void )
{
	return true;
}

bool LauncherCardApplet::allowDropOnTab( void )
{
	return false;
}

bool LauncherCardApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void LauncherCardApplet::tabDragDropEvent( QDropEvent *e )
{}

Tray *LauncherCardApplet::createTray( Card * )
{
	return NULL;
}

Separator *LauncherCardApplet::createSeparator( Card *c )
{
	Separator *sep = new Separator(c, true);
	sep->setGripper(true);
	return sep;
}

Contents *LauncherCardApplet::createContents( Card *c )
{
	kdDebug() << "LauncherCardApplet::createContents" << endl;
	QWidget *_contentsWidget = new QWidget();
	Contents *contents = new Contents(c);
	launcher = new Launcher(c);
	launcher->setURL(shortcutURL);
	launcher->resize(launcherWidth,launcherHeight);
	contents->setContents(launcher);
	contents->setPreferredHeight(200);
	return contents;
}


void LauncherCardApplet::cardReady( void )
{
	launcher->start();
	CardApplet::cardReady();
	emit propertiesChanged();
}

void LauncherCardApplet::cardOpened( void )
{

}

void LauncherCardApplet::cardShut( void )
{
}

QPixmap *LauncherCardApplet::tabIcon()
{
	return &mIcon;
}

bool LauncherCardApplet::cardDestroy( void )
{
	kdDebug() << "LauncherCardApplet::cardDestroyed()" << endl;
	//ask luser if they really want to delete contents, if so delete it :)
	int answer = KMessageBox::questionYesNo(0,
		"This will delete the launcher card and all its contents.  Continue?","Delete launcher?");
	if (answer==KMessageBox::Yes) {
		KIO::NetAccess::del(shortcutURL);
		return true;
	} else {
	return false;
	}
}


QString LauncherCardApplet::generateShortcutPath()
{
	QString path;
	KRandomSequence sequence;
	QString random;
	random.setNum(sequence.getLong(1000));
	path = KGlobal::dirs()->saveLocation("data","slicker/launcher/" + random, true);
	return path;
}

