/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
                  1998 Michael Kropfberger <michael.kropfberger@gmx.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "diskinfo.h"
#include "configdialog_impl.h"
#include "listview.h"

#include <stdlib.h>

#include <klocale.h>
#include <kconfig.h>
#include <kpopupmenu.h>
#include <kapplication.h>
#include <kmessagebox.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qheader.h>

#define TEXTCOLOR black
#define BACKGROUND QColor(211, 214, 223)

#define FULL_PERCENT 95.0

#ifndef GUI_DEFINED
static bool GUI;
#define GUI_DEFINED
#endif

/**************************************************************/

CListViewItem::CListViewItem( KListView * parent, QListViewItem * after )
  :QListViewItem( parent, after )
{}

// 2001-03-10 Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// This method returns for all numeric columns the converted number.
// The conversion is done with
//    1. fixed position of the colon
//    2. a sufficient number of leading blanks
// This ensures that the lexical string comparison gives numerical order.

QString CListViewItem::key ( int column, bool ) const
{
  QString tmp;

  switch (column) {
  case DiskInfoWidget::sizeCol:
    tmp.sprintf("%10d",size);
    break;

  case DiskInfoWidget::freeCol:
    tmp.sprintf("%10d",avail);
    break;

  case DiskInfoWidget::fullCol:
  case DiskInfoWidget::usageCol:
    tmp.sprintf("%7.2f",full);
    break;

  default:
    tmp = text(column);
    break;
  }

  return tmp;
}

void CListViewItem::setKeys (int kb_size, int kb_avail, float percent_full)
{
  size  = kb_size;
  avail = kb_avail;
  full  = (percent_full >= 0.) ? percent_full : 0.;
  return;
}

/**************************************************************/

DiskInfoWidget::DiskInfoWidget(QWidget *parent, const char *name, ConfigDialogImpl *config, bool init)
	: QWidget(parent, name)
{
		_config = config;

		connect(&mDiskList , SIGNAL(readDFDone() ), this, SLOT (updateDFDone()) );
		connect(&mDiskList , SIGNAL(criticallyFull(DiskEntry*)), this, SLOT (criticallyFull(DiskEntry*)) );

		mTabProp.resize(8);
		mTabProp[0] = new CTabEntry( "Icon", i18n("Icon"), false, 0);
		mTabProp[1] = new CTabEntry( "Device", i18n("Device"), false, 0);
		mTabProp[2] = new CTabEntry( "Type", i18n("Type"), false, 0);
		mTabProp[3] = new CTabEntry( "Size", i18n("Size"), false, 0);
		mTabProp[4] = new CTabEntry( "MountPoint", i18n("Mount Point"), true, 90 );
		mTabProp[5] = new CTabEntry( "Free", i18n("Free"), false, 0);
		// xgettext:no-c-format
		mTabProp[6] = new CTabEntry( "Full%", i18n("Full %"), false, 0);
		mTabProp[7] = new CTabEntry( "UsageBar", i18n("Usage"), true, 100);

		GUI = !init;
		if( GUI )
		{
			QVBoxLayout *topLayout = new QVBoxLayout( this, 0, 0 );
			mList = new CListView( this, "list" );
			topLayout->addWidget( mList );

			mList->setAllColumnsShowFocus( true );
			mList->setFrameStyle( QFrame::WinPanel + QFrame::Sunken );
			mList->setShowSortIndicator(true);
			QPalette pal;
			QColorGroup cg;
			cg.setColor( QColorGroup::Button, BACKGROUND );
			cg.setColor( QColorGroup::ButtonText, TEXTCOLOR );
			cg.setColor( QColorGroup::Base, BACKGROUND );
			pal.setActive( cg );
			cg.setColor( QColorGroup::Button, BACKGROUND );
			cg.setColor( QColorGroup::ButtonText, TEXTCOLOR );
			cg.setColor( QColorGroup::Base, BACKGROUND );
			pal.setInactive( cg );
			cg.setColor( QColorGroup::Button, BACKGROUND );
			cg.setColor( QColorGroup::ButtonText, TEXTCOLOR );
			cg.setColor( QColorGroup::Base, BACKGROUND );
			pal.setDisabled( cg );
			mList->setPalette( pal );
//			connect( mList,
//				SIGNAL( pressed( QListViewItem *, const QPoint &, int )),
//				this, SLOT( rightButtonPressed( QListViewItem *, const QPoint &, int )));
//			connect( mList,
//				SIGNAL( clicked( QListViewItem *, const QPoint &, int )),
//				this, SLOT( rightButtonClicked( QListViewItem *, const QPoint &, int )));
			connect( mList->header(), SIGNAL(sizeChange(int, int, int)),
				this, SLOT(columnSizeChanged(int, int, int)) );
			makeColumns();
		}

		updateDF();
		setUpdateFrequency(60);
}

DiskInfoWidget::~DiskInfoWidget()
{
}

void DiskInfoWidget::makeColumns( void )
{
	uint i;

	//
	// 1999-11-29 Espen Sand
	// This smells like a bad hack but I need to remove the headers
	// first. If I don't, the list look like shit afterwards. The iterator
	// is just used to prevent an endless loop. With my Qt (1999-11-10 ?)
	// I only need as many iterations as there are header items but who knows
	// what a new Qt can do!
	//
	for( i=1000; mList->header()->count() > 0 && i>0; i-- )
	{
		mList->header()->removeLabel( 0 );
		mList->header()->update();
	}
	for( i=0; i<mTabProp.size(); mList->removeColumn(i++) );
	mList->clear();

	for( i=0; i<mTabProp.size(); i++ )
	{
		CTabEntry &e = *mTabProp[i];
		if( e.mVisible == true )
			mList->addColumn( e.mName, e.mWidth );
		else
			mList->addColumn( e.mName, 0 ); // zero width makes it invisible
	}
}

void DiskInfoWidget::updateDFDone( void )
{
	if (mPopup) //The popup menu is ont he screen... Don't touch the list view...
		return;

	mList->clear();

	int i=0;
	CListViewItem *item = 0;
	for( DiskEntry *disk=mDiskList.first(); disk!=0; disk=mDiskList.next() )
	{
		i++;
		QString size, percent;
		if( disk->kBSize() > 0 )
		{
			percent = KGlobal::locale()->formatNumber(disk->percentFull(), 1) + '%';
			size = disk->prettyKBSize();
		}
		else
		{
			percent = i18n("N/A");
			size    = i18n("N/A");
		}

		int k=0;
		item = new CListViewItem( mList, item );
		bool root = disk->mountOptions().find("user",0,false)==-1 ? true : false;
		item->setPixmap( k++, mList->icon( disk->iconName(), root ) );
		item->setText( k++, disk->deviceName() );
		item->setText( k++, disk->fsType() );
		item->setText( k++, size );
		item->setText( k++, disk->mountPoint() );
		item->setText( k++, disk->prettyKBAvail() );
		item->setText( k++, percent );
		item->setKeys( disk->kBSize(), disk->kBAvail(), disk->percentFull());
	}
	readingDF = false;
	updateDiskBarPixmaps();

	mList->sort();
}

void DiskInfoWidget::setUpdateFrequency( int frequency )
{
	//
	// Kill current timer and restart it if the frequency is
	// larger than zero.
	//
	killTimers();
	if( frequency > 0 )
	{
		startTimer( frequency * 1000 );
	}
}

void DiskInfoWidget::timerEvent(QTimerEvent *)
{
  updateDF();
}

void DiskInfoWidget::resizeEvent( QResizeEvent * )
{
   updateDiskBarPixmaps();
}

void DiskInfoWidget::updateDF( void )
{
  //
  // We can only do this if the popupmenu is not present
  //
  if( mPopup == 0 )
  {
    readingDF = TRUE;
    mDiskList.readFSTAB();
    mDiskList.readDF();
  }
}

void DiskInfoWidget::updateDiskBarPixmaps( void )
{
	if (mTabProp[usageCol]->mVisible != true)
		return;

	int size=0, w=0;

	// FIXME: Not the right width!!
/*	for(uint i=0; i<mTabProp.size()-1; i++ )
		size += mList->columnWidth(i);
	w=mList->width() - size - 4;
	if (w<0)
		w=0;
	mList->setColumnWidth(usageCol, w );*/

	int h = mList->fontMetrics().lineSpacing()-2;
	if( h <= 0 )
		return;

	int i=0;
	for(QListViewItem *it=mList->firstChild(); it!=0;it=it->nextSibling(),i++ )
	{
		// I can't get find() to work. The Disks::compareItems(..) is
		// never called.
		//
		//int pos=mDiskList->find(disk);

		DiskEntry dummy(it->text(deviceCol));
		dummy.setMountPoint(it->text(mntCol));
		int pos = -1;
		for( u_int i=0; i<mDiskList.count(); i++ )
		{
			DiskEntry *item = mDiskList.at(i);
			int res = dummy.deviceName().compare( item->deviceName() );
			if( res == 0 )
			{
				res = dummy.mountPoint().compare( item->mountPoint() );
			}
			if( res == 0 )
			{
				pos = i;
				break;
			}
		}

		DiskEntry *disk = mDiskList.at(pos);
		if( disk == 0 ) { continue; }

		if( disk->mounted() == true && disk->percentFull() != -1 )
		{
			int w = mList->columnWidth(usageCol)-2;
			if( w <= 0 ) { continue; }

			QPixmap *pix = new QPixmap( w, h );
			if( pix == 0 ) { continue; }

			pix->fill(white);
			QPainter p(pix);
			p.setPen(black);
			p.drawRect(0,0,w,h);
			QColor c;
			if ( (disk->iconName().find("cdrom") != -1)
					|| (disk->iconName().find("writer") != -1) )
				c = gray;
			else
				c = disk->percentFull() > FULL_PERCENT ? red : darkGreen;
			p.setBrush(c );
			p.setPen(white);
			p.drawRect(1,1,(int)(((float)pix->width()-2)*(disk->percentFull()/100)),
			pix->height()-2);
			it->setPixmap ( usageCol, *pix );
			p.end();
			delete pix;
		}
	}
}

void DiskInfoWidget::criticallyFull( DiskEntry *disk )
{
//  if( mStd.popupIfFull() == true )
//  {
    QString msg = i18n("Device [%1] on [%2] is getting critically full!").
      arg(disk->deviceName()).arg(disk->mountPoint());
    KMessageBox::sorry( this, msg, i18n("Warning"));
//  }
}

void DiskInfoWidget::columnSizeChanged( int, int, int )
{
	if( mTimer == 0 )
	{
		mTimer = new QTimer( this );
		connect( mTimer, SIGNAL(timeout()), this, SLOT(updateDiskBarPixmaps()) );
	}
	else if( mTimer->isActive() == true )
	{
		mTimer->stop();
	}

	mTimer->start( 10, true );
}
