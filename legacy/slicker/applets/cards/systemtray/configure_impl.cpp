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

#include "configure_impl.h"
#include "configure_ui.h"

#include <qwidget.h>
#include <qcheckbox.h>
#include <qdialog.h>
#include <qradiobutton.h>

#include <kpushbutton.h>
#include <knuminput.h>
#include <kconfig.h>
#include <kwin.h>
#include <kcombobox.h>
#include <klistbox.h>

#include <qxembed.h>

ConfigureImpl::ConfigureImpl()
{
	readConfig();
}

ConfigureImpl::~ConfigureImpl()
{}

void ConfigureImpl::displayDialog(QPtrList<QXEmbed> *tray, QPtrList<QXEmbed> *contents)
{
	_tray = tray;
	_contents = contents;
	ui = new Configure(0L, "configure", true);
	connect(ui->cancel, SIGNAL(clicked()), SLOT(slotCancel()));
	connect(ui->ok, SIGNAL(clicked()), SLOT(slotOk()));
	connect(ui->remove, SIGNAL(clicked()), SLOT(slotRemove()));
	connect(ui->add, SIGNAL(clicked()), SLOT(slotAdd()));
	setConfig();
	ui->show();
}

void ConfigureImpl::slotCancel()
{
	delete ui;
}

void ConfigureImpl::slotOk()
{
	applyConfig();
	writeConfig();
	emit(newConfig());
	delete ui;
}

void ConfigureImpl::slotRemove()
{
	if(ui->contents->currentItem() > -1)
	{
		ui->systemtray->insertItem(*ui->contents->pixmap(ui->contents->currentItem()), ui->contents->text(ui->contents->currentItem()));
		ui->contents->removeItem(ui->contents->currentItem());
	}
}

void ConfigureImpl::slotAdd()
{
	if(ui->systemtray->currentItem() > -1)
	{
		ui->contents->insertItem(*ui->systemtray->pixmap(ui->systemtray->currentItem()), ui->systemtray->text(ui->systemtray->currentItem()));
		ui->systemtray->removeItem(ui->systemtray->currentItem());
	}
}

void ConfigureImpl::readConfig()
{
	KConfig conf( "systemtray" );
	conf.setGroup( "tray" );

	_maxtraylines = conf.readNumEntry("maxtraylines", 1);
	_maxcontentslines = conf.readNumEntry("maxcontentslines", 1);
	_deficon = conf.readBoolEntry("deficonpositiontray", true);

	conf.setGroup( "background" );
	_background = conf.readListEntry("names", ',');
}

void ConfigureImpl::setConfig()
{
	QXEmbed *emb;

	ui->linestray->setValue(_maxtraylines);
	ui->linescontents->setValue(_maxcontentslines);
	if(_deficon)
		ui->conf_tray->setChecked(true);
	else
		ui->conf_contents->setChecked(true);

	// The icons are not the correct ones. They are the programmicons and not
	// the systemtray icons.
	// At the moment I don't know where are the other icons    :)
	// FIXME: Change that later!!
	if(!_deficon)
	{
		emb = _tray->first();
		while ((emb = _tray->current()) != 0L)
		{
			WId wid = emb->embeddedWinId();
			ui->systemtray->insertItem(KWin::icon(wid, 16, 16, true), KWin::info(wid).name);
			_tray->next();
		}
	}
	else
	{
		emb = _contents->first();
		while ((emb = _contents->current()) != 0L)
		{
			WId wid = emb->embeddedWinId();
			ui->systemtray->insertItem(KWin::icon(wid, 16, 16, true), KWin::info(wid).name);
			_contents->next();
		}
	}

	if(!_deficon)
	{
		emb = _contents->first();
		while ((emb = _contents->current()) != 0L)
		{
			WId wid = emb->embeddedWinId();
			ui->contents->insertItem(KWin::icon(wid, 16, 16, true), KWin::info(wid).name);
			_contents->next();
		}
	}
	else
	{
		emb = _tray->first();
		while ((emb = _tray->current()) != 0L)
		{
			WId wid = emb->embeddedWinId();
			ui->contents->insertItem(KWin::icon(wid, 16, 16, true), KWin::info(wid).name);
			_tray->next();
		}
	}
}

void ConfigureImpl::applyConfig()
{
	QStringList background;
	bool exist = false;
	int i = 0;

	_maxtraylines = ui->linestray->value();
	_maxcontentslines = ui->linescontents->value();

	if(ui->conf_tray->isChecked())
		_deficon = true;
	else
		_deficon = false;

	// add icons that are at the moment not in the systemtray.
	for(QStringList::Iterator it = _background.begin(); it != _background.end(); ++it)
	{
		exist = false;
		for(i = 0; i <= (int)ui->systemtray->count()-1; i++ )
			if(ui->systemtray->text(i) == (QString)(*it))
				exist = true;
		for(i = 0; i <= (int)ui->contents->count()-1; i++ )
			if(ui->contents->text(i) == (QString)(*it))
				exist = true;
		if(!exist)
			background << ((QString)(*it));
	}
	// add the selected icons
	for(int i = 0; i <= (int)ui->contents->count()-1; i++ )
		background << ui->contents->text(i);

	_background = background;
}

void ConfigureImpl::writeConfig()
{
	KConfig conf( "systemtray" );

	conf.setGroup( "tray" );
	conf.writeEntry("maxtraylines", _maxtraylines);
	conf.writeEntry("maxcontentslines", _maxcontentslines);
	conf.writeEntry("deficonpositiontray", _deficon);

	conf.setGroup( "background" );
	conf.writeEntry("names", _background, ',');
}
