/***************************************************************************
                          NewCardDialog.h  -  description
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

#ifndef NEWCARDDIALOG_H
#define NEWCARDDIALOG_H

#include <qwidget.h>
#include <qfile.h>
#include <kdialogbase.h>

#include "pluginlistbox.h"
#include "../share/plugininfo.h"

/**
  *@author Shamyl Zakariya
  */

class NewCardDialog : public KDialogBase
{
	Q_OBJECT
	public:
		NewCardDialog(QWidget *parent = 0L,
									const char *name = 0,
									bool modal = false);

		QString pluginDesktopFile( void ) { return _current->desktopFile(); }

		static QString selectPlugin(QWidget *parent = 0L,
		                                             const QString& caption = QString::null);
	protected slots:
		void pluginSelected( PluginInfo * );

	protected:
		class PluginListBox *_plb;
		class QVBoxLayout *_mainLayout;
		PluginInfo *_current;
		class QLabel *_authorLabel, *_versionLabel;

};

#endif
