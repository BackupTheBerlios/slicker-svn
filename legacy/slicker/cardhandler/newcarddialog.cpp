/***************************************************************************
                          NewCardDialog.cpp  -  description
                             -------------------
    begin                : Sat Jan 11 2003
    copyright            : (C) 2003 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "newcarddialog.h"
#include "pluginlistbox.h"
#include "CardManager.h"

#include <qlayout.h>
#include <qframe.h>

#include <klocale.h>
#include <kiconloader.h>


/*
		class PluginListBox *_plb;
		class QVBoxLayout *_mainLayout;
		PluginRoster::Info *_current;
		class QLabel *_authorLabel, *_versionLabel;
*/

NewCardDialog::NewCardDialog(QWidget *parent, const char *name, bool modal )
: KDialogBase(parent,name, modal, i18n("Select a Card Type"), Ok|Cancel),
	_current(0L)
{
        setIcon(KGlobal::iconLoader()->loadIcon("package_games_card",KIcon::Small));
	QFrame *page = makeMainWidget();
	_mainLayout = new QVBoxLayout(page, 0, spacingHint());

	_plb = new PluginListBox(page, "PluginListBox");
	//_plb->populate(Manager::instance()->pluginRoster());
	_plb->populate();
	_mainLayout->addWidget(_plb);

	QGridLayout *metaLayout = new QGridLayout(_mainLayout, 2, 2, 4);
	metaLayout->setColStretch(1, 2.0);
	QLabel *l = new QLabel("Author", page);
	metaLayout->addWidget(l, 0, 0);

	l = new QLabel("Version", page);
	metaLayout->addWidget(l, 1, 0);

	_authorLabel = new QLabel(page);
	_authorLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	metaLayout->addWidget(_authorLabel, 0, 1);

	_versionLabel = new QLabel(page);
	_versionLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	metaLayout->addWidget(_versionLabel, 1, 1);

	connect(_plb, SIGNAL(pluginSelected(PluginInfo *)),
	        SLOT(pluginSelected(PluginInfo *)));

	setInitialSize(QSize(400, minimumSizeHint().height()));
	enableButtonOK(false);
}

QString NewCardDialog::selectPlugin(QWidget *parent, const QString& caption)
{
	NewCardDialog dlg(parent, "NewCardDialog", true);

	if ( !caption.isNull() )
		dlg.setCaption( caption );

	if ( dlg.exec() == QDialog::Accepted )
		return dlg.pluginDesktopFile();
	else
		return QString::null;
}

void NewCardDialog::pluginSelected( PluginInfo *info )
{
	_current = info;
	if (_current)
	{
		_authorLabel->setText(_current->author());
		_versionLabel->setText(_current->version());
		enableButtonOK(true);
	}
	else enableButtonOK(false);
}

