/*****************************************************************
Copyright (c) 2003 the slicker authors. See file AUTHORS.
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
******************************************************************/

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

private:
    static Slicker * _self;
    KPopupMenu *_menu;
    KGlobalAccel *_globalAccel;
    KActionMenu * _appletMenu;
    QString _dataDir;
};

#endif
