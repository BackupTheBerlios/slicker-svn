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

#include "baseinfo.h"
#include "getinfo.h"
#include "configdialog_impl.h"

#include <kurl.h>
#include <klocale.h>
#include <kconfig.h>

#include <qlayout.h>
#include <qlabel.h>


BaseinfoWidget::BaseinfoWidget(QWidget *parent, const char *name, ConfigDialogImpl *config)
	: QWidget(parent, name)
{
		_config = config;
		// load the information to display
		GetInfo::init();

		// the layouts
		baselayout = new QHBoxLayout(this, 5, 5, "Baselayout");
		namelayout = new QVBoxLayout(0, 0, 2, "Namelayout");
		infolayout = new QVBoxLayout(0, 0, 2, "Infolayout");

		// Display the Hostname
		hostlabel = new QLabel(this, "Hostlabel");
		hostlabel->setText(i18n("host:"));
		hostlabel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, 0, 0, true));
		namelayout->addWidget(hostlabel);

		host = new QLabel(this, "Host");
		host->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred, 0, 0, true));
		host->setFrameShape(QLabel::Box);
		host->setText(GetInfo::gethost() + ":" + GetInfo::getdomain());
		infolayout->addWidget(host);

		// Display the user that was logged in
		userlabel = new QLabel(this, "Userlabel");
		userlabel->setText(i18n("user:"));
		userlabel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, 0, 0, true));
		namelayout->addWidget(userlabel);

		user = new QLabel(this, "User");
		user->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred, 0, 0, true));
		user->setFrameShape(QLabel::Box);
		user->setText(GetInfo::getuser());
		infolayout->addWidget(user);

		// Display the ip of the workstation
		kdelabel = new QLabel(this, "KDElabel");
		kdelabel->setText(i18n("KDE:"));
		kdelabel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, 0, 0, true));
		namelayout->addWidget(kdelabel);

		kde = new QLabel(this, "KDE");
		kde->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred, 0, 0, true));
		kde->setFrameShape(QLabel::Box);
		kde->setText(GetInfo::getkde());
		infolayout->addWidget(kde);

		baselayout->addLayout(namelayout);
		baselayout->addLayout(infolayout);

		connect(_config, SIGNAL(newSettings()), SLOT(update()));
		update();
}

BaseinfoWidget::~BaseinfoWidget()
{}

void BaseinfoWidget::update()
{
	if(_config->getHost())
	{
		hostlabel->show();
		host->show();
	}
	else
	{
		hostlabel->hide();
		host->hide();
	}

	if(_config->getUser())
	{
		userlabel->show();
		user->show();
	}
	else
	{
		userlabel->hide();
		user->hide();
	}

	if(_config->getKDE())
	{
		kdelabel->show();
		kde->show();
	}
	else
	{
		kdelabel->hide();
		kde->hide();
	}
}
