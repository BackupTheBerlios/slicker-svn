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

#include "informationcenterwidget.h"
#include "baseinfo.h"
//#include "info.h"
#include "diskinfo.h"
#include "configdialog_impl.h"

#include <kurl.h>

#include <qlayout.h>

InformationcenterWidget::InformationcenterWidget(QWidget *parent, const char *name, ConfigDialogImpl *config)
  : QWidget(parent, name)
{
	_config = config;

	QScrollView *m_scroll = new QScrollView(this, "scrollview");
	m_scroll->setResizePolicy( QScrollView::AutoOneFit );

	QWidget *w1 = new QWidget(m_scroll->viewport());
	baselayout = new QVBoxLayout(w1, 0, 0, "Baselayout");
	infowidget = new BaseinfoWidget(w1, "Baseinfo", _config);
//	allinfowidget = new InfoWidget(w1, "Info", _config);
	diskinfowidget = new DiskInfoWidget(w1, "DiskInfo", _config);

	baselayout->addWidget(infowidget);
//	baselayout->addWidget(allinfowidget);
	baselayout->addWidget(diskinfowidget);

	m_scroll->addChild(w1);
	QVBoxLayout* l = new QVBoxLayout(this);
	l->addWidget(m_scroll);
	l->setSpacing(0);
	l->setMargin(0);
}

InformationcenterWidget::~InformationcenterWidget()
{}
