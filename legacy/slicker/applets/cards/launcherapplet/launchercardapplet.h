/*
 * Copyright (C) 2003 by Shamyl Zakariya, zakariya@earthlink.net
 * Copyright (C) 2003 by Mark Westcott, mark@houseoffish.org
 *

***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LAUNCHERCARDAPPLET_H
#define LAUNCHERCARDAPPLET_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <kconfig.h>

#include <carddeskcore/CardApplet.h>


#include "kdiconview.h"
#include "launcher.h"

using namespace CardDesk;

class LauncherCardApplet : public CardApplet
{
	public:
		LauncherCardApplet(const QString &configFile);

		~LauncherCardApplet();


		virtual QString defaultCardName( void ) { return QString("Launcher"); }

		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );

		virtual void aboutToShowContextMenu( class QPopupMenu *menu );

		virtual bool allowDragHoverToOpen( void );

		virtual bool allowDropOnTab( void );

		virtual bool canDecodeDrag( class QMimeSource * );

		virtual void tabDragDropEvent( QDropEvent * );

		virtual Tray *createTray( Card * );

		virtual Separator *createSeparator( Card * );

		virtual Contents *createContents( Card * );

		virtual void cardReady( void );

		QPixmap* tabIcon();


	public slots:

		void cardOpened( void );
		void cardShut( void );
		bool cardDestroy( void );

	protected:

		QString generateShortcutPath();

	private:
		Launcher *launcher;
		int launcherHeight;
		int launcherWidth;
		QString shortcutURL;
		QPixmap mIcon;

};

#endif
