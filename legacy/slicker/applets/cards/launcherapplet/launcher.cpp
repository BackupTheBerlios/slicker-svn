 /*
 * Copyright (C) 1998, 1999 Torben Weis, weis@kde.org
 * Copyright (C) 2002 Mark Westcott, mark@houseoffish.org
 *

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License version 2 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "launcher.h"
#include "kdiconview.h"

#include <kglobal.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <qtimer.h>
#include <qlayout.h>

Launcher::Launcher(QWidget *parent) 
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	mpIconView = new KDIconView( this, 0 );
	layout->add(mpIconView);
	mpIconView->show();
};

Launcher::~Launcher()
{



}

void Launcher::init()
{
	QTimer::singleShot(0, this, SLOT( start() ));
}

void Launcher::start()
{
	KRootWm *wm = new KRootWm( this );
	if (mpIconView)
	{
		mpIconView->initConfig(false);
		mpIconView->start( wm );
	}
}

void Launcher::shutdown()
{
	mpIconView->slotSaveIconPositions();

}

KURL Launcher::url() const
{
	if (mpIconView)
		return mpIconView->url();
	return KURL();
}

void Launcher::setURL(const KURL &url)
{
	kdDebug() << "Launcher::setURL " << url.path() << endl;
	mpIconView->setURL(url);
}
