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

#include "configdialog_impl.h"
#include "configdialog.h"

#include <qwidget.h>
#include <qcheckbox.h>
#include <qdialog.h>

#include <kpushbutton.h>
#include <kconfig.h>

ConfigDialogImpl::ConfigDialogImpl()
{
    readConfig();
}

ConfigDialogImpl::~ConfigDialogImpl()
{}

void ConfigDialogImpl::displayDialog()
{
    ui = new Configdialog(0L, "configure", true);
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(slotCancel()));
    connect(ui->buttonOk, SIGNAL(clicked()), SLOT(slotOk()));
    setConfig();
    ui->show();
}

void ConfigDialogImpl::slotCancel()
{
    delete ui;
}

void ConfigDialogImpl::slotOk()
{
    applyConfig();
    writeConfig();
    emit(newSettings());
    delete ui;
}

void ConfigDialogImpl::readConfig()
{
    KConfig conf( "informationcenter" );
    conf.setGroup( "Baseinfo" );

    _host = conf.readBoolEntry("host", true);
    _user = conf.readBoolEntry("user", true);
    _kde = conf.readBoolEntry("kde", true);
}

void ConfigDialogImpl::setConfig()
{
    ui->host->setChecked(_host);
    ui->user->setChecked(_user);
    ui->kde->setChecked(_kde);
}

void ConfigDialogImpl::applyConfig()
{
    if(ui->host->isChecked())
	_host = true;
    else
	_host = false;

    if(ui->user->isChecked())
	_user = true;
    else
	_user = false;

    if(ui->kde->isChecked())
	_kde = true;
    else
	_kde = false;
}

void ConfigDialogImpl::writeConfig()
{
    KConfig conf( "informationcenter" );
    conf.setGroup( "Baseinfo" );
    
    conf.writeEntry("host", _host);
    conf.writeEntry("user", _user);
    conf.writeEntry("kde", _kde);
}
