/***************************************************************************
  Copyright (C) 2003 by Jason Kivlighn, mizunoami44@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "PrinterJobsApplet.h"

#include <kdeprint/kmjobviewer.h>
#include <kdeprint/kmtimer.h>
#include <kdeprint/kmmanager.h>

#include <qhbox.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <kdebug.h>

#include "../../../carddeskcore/CardManager.h"

PrinterJobsApplet::PrinterJobsApplet( const QString &configFile )
	:CardApplet( configFile )
{
	KIconLoader il;
	_icon = il.loadIcon("fileprint", KIcon::Small, 0, KIcon::DefaultState);
	
	_name = "Printer Jobs";
}

QString PrinterJobsApplet::cardName( void )
{
	return _name;
}

void PrinterJobsApplet::setCardName(QString &newName)
{
	_name = newName;
	CardApplet::setCardName();
}

QPixmap *PrinterJobsApplet::tabIcon( void )
{
	return &_icon;
}

void PrinterJobsApplet::addToDefaultContextMenu( QPopupMenu * )
{
}

bool PrinterJobsApplet::allowDropOnTab( void )
{
	return true;
}

Separator *PrinterJobsApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

Contents *PrinterJobsApplet::createContents( Card *c )
{
	Contents *contents = new Contents(c);

	viewer = new KMJobViewer(c);
	connect( viewer, SIGNAL(jobsShown(KMJobViewer*,bool)), SLOT(slotJobsShown(KMJobViewer*,bool)) );
	connect( viewer, SIGNAL(printerChanged(KMJobViewer*,const QString&)), SLOT(slotPrinterChanged(KMJobViewer*,const QString&)) );
	connect( viewer, SIGNAL(refreshClicked()), SLOT(slotTimer()) );
	connect( viewer, SIGNAL(viewerDestroyed(KMJobViewer*)), SLOT(slotViewerDestroyed(KMJobViewer*)) );
	viewer->setPrinter("All Printers");

	contents->setContents(viewer);
	return contents;
}

void PrinterJobsApplet::cardReady( void )
{
	timer = KMTimer::self();
	connect(timer,SIGNAL(timeout()),SLOT(slotTimer()));
	
	//note: possibly allow the delay to be configurable later
	timer->delay(10);
	
	CardApplet::cardReady();
}

bool PrinterJobsApplet::allowHoverToOpen( void )
{
	return false;
}

void PrinterJobsApplet::slotPrinterChanged(KMJobViewer* viewer,const QString& printer_name)
{
	viewer->setPrinter(printer_name);
}

void PrinterJobsApplet::slotTimer()
{
	KMManager::self()->printerList(true);
	
	if (viewer)
		viewer->refresh(true);
	
}

void PrinterJobsApplet::slotViewerDestroyed(KMJobViewer*)
{}

void PrinterJobsApplet::slotJobsShown(KMJobViewer* , bool )
{}

void PrinterJobsApplet::cardOpened( )
{
	//resume updating the list of jobs
	timer->release();
	
	//force an update immediately
	slotTimer();
}

void PrinterJobsApplet::cardShut( )
{
	//stop updating the list of jobs
	timer->hold();
}


