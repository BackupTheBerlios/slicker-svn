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

#ifndef DISKINFOWIDGET_H
#define DISKINFOWIDGET_H

#include <qwidget.h>
#include <qlistview.h>

#include "disks.h"
#include "disklist.h"
//#include "stdoption.h"

class QLabel;
class CListView;
class KPopupMenu;
class QTimer;
class ConfigDialogImpl;

/**************************************************************/

class CTabEntry
{
  public:
    CTabEntry( const QString &res, const QString &name, bool visible,
	       uint width )
    {
      mRes     = res;
      mName    = name;
      mVisible = visible;
      mWidth   = width;
    };
    CTabEntry( void ) { }
    ~CTabEntry( void ) { }


    QString mRes;
    QString mName;
    bool mVisible;
    uint mWidth;
};

/**************************************************************/
//
// 2001-03-10 Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
// The purpose of this class is:
// 1) overload the key() method to provide sorting keys so that the
//    columns with a numeric value are sorted by numerical value.
//

class CListViewItem : public QListViewItem
{
  public:
    CListViewItem ( KListView * parent, QListViewItem * after );

    virtual QString key ( int column, bool ascending) const;

    void    setKeys ( int kb_size, int kb_avail, float percent_full ) ;

  private:
    int     size;
    int     avail;
    float   full;
};

/**************************************************************/

class DiskInfoWidget : public QWidget
{
	Q_OBJECT

public:
	DiskInfoWidget(QWidget* parent, const char* name=0, ConfigDialogImpl *config=0L, bool init=false);
	~DiskInfoWidget();

	enum ColId
	{
		iconCol   = 0,
		deviceCol = 1,
		typeCol   = 2,
		sizeCol   = 3,
		mntCol    = 4,
		freeCol   = 5,
		fullCol   = 6,
		usageCol  = 7
	};

public slots:
	void updateDF( void );
	void updateDFDone( void );
	void criticallyFull( DiskEntry *disk );

private slots:
	void updateDiskBarPixmaps( void );
	void columnSizeChanged( int column, int, int newSize );
	void setUpdateFrequency( int frequency );

protected:
	void timerEvent( QTimerEvent * );
	void resizeEvent( QResizeEvent * );

	ConfigDialogImpl *_config;

private:
	void makeColumns( void );

	bool readingDF;
	QMemArray<CTabEntry*> mTabProp;
	QTimer        *mTimer;
	CListView     *mList;
	DiskList   mDiskList;
	KPopupMenu    *mPopup;
//	CStdOption mStd;
};

#endif
