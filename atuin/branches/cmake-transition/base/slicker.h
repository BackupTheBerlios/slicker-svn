/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef SLICKER_H
#define SLICKER_H

#include <kglobalaccel.h>
#include <kpopupmenu.h>
#include <kuniqueapplication.h>
#include <qpoint.h>
#include <qstring.h>

#include "edgewidgetmanager.h"
#include "appletdef.h"

class Slider;
class CardStack;

/**
* The Main Slicker-Application class.
* The Slicker class is responsible for holding a unique copy of slicker in memory,
* and take care of initialization/deinitialization of slicker.
*
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class Slicker : public KUniqueApplication
{
    Q_OBJECT
public:
    Slicker();
    virtual ~Slicker();

    static Slicker * self();
        
    /**
     * Receives a handle to the global KPopupMenu
     */
    KPopupMenu *menu();

    /**
     * Receives a handle to the list of global accelerators
     */
    KGlobalAccel *globalAccel();

    /**
     * Returns the path to the slicker data-config.
     */
    const QString &dataDir() const;

public slots:
    void showMenu();
    void showMenu(const QPoint &origin);

    Slider * newSlider();
    CardStack * newCardStack();

private:
    static Slicker * _self;
    KPopupMenu *_menu;
    KGlobalAccel *_globalAccel;
    KActionMenu * _appletMenu;
    QString _dataDir;
};

#endif
