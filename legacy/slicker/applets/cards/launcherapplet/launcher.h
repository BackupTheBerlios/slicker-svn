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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <qwidget.h>
#include <kurl.h>

class KDIconView;
class KActionCollection;

class Launcher : public QWidget
{
	Q_OBJECT
		public:
			Launcher(QWidget *parent);
			~Launcher();

			KDIconView *iconView() const { return mpIconView; }
			KActionCollection *actionCollection();
			KURL url() const;
			void setURL(const KURL &);
		
		public slots:
			void start();
			void shutdown();

		public:
			KDIconView *mpIconView;
			void init();
};

#endif
