/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "informationcenterapplet.h"

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>

#include <kdebug.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kprocess.h>
#include <kstandarddirs.h>

#include "informationcenterwidget.h"
#include "configdialog_impl.h"

InformationcenterApplet::InformationcenterApplet(const QString& configFile)
	:CardApplet(configFile)
{
    init();
}

void InformationcenterApplet::init()
{
    KIconLoader il;
    _icon = il.loadIcon("ksysguard", KIcon::Small, 0, KIcon::DefaultState);
    config = new ConfigDialogImpl();
}

QPixmap *InformationcenterApplet::tabIcon( void )
{
	return &_icon;
}

void InformationcenterApplet::addToDefaultContextMenu( class QPopupMenu *menu )
{
    menu->insertItem(SmallIcon("ksysguard"), i18n("KDE System Guard"), this, SLOT(slotrun()));
    menu->insertSeparator();
    menu->insertItem(SmallIcon("configure"), i18n("Configure Information Center"), this, SLOT(slotconfigure()));
}

void InformationcenterApplet::slotrun()
{
    KProcess proc;

    proc << locate("exe", "ksysguard");
    proc.start(KProcess::DontCare);
}

void InformationcenterApplet::slotconfigure()
{
    config->displayDialog();
}

bool InformationcenterApplet::allowDropOnTab( void )
{
	return false;
}

bool InformationcenterApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

Separator *InformationcenterApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

Contents *InformationcenterApplet::createContents( Card *c )
{
	Contents *contents = new Contents(c);

	_contentsWidget = new InformationcenterWidget(0, "InformationcenterWidget-ContentsWidget", config);

	contents->setContents(_contentsWidget);

	return contents;
}

void InformationcenterApplet::tabDragDropEvent( QDropEvent * )
{}

void InformationcenterApplet::saveSettings( class KConfig * )
{}

void InformationcenterApplet::loadSettings( class KConfig * )
{}

void InformationcenterApplet::cardReady( void )
{}
