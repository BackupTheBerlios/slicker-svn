/***************************************************************************
    copyright (C) 2003      Sven Leiber <s.leiber@web.de>
                  2000-2001 Matthias Ettrich <ettrich@kde.org>
                  2000-2001 Matthias Elter   <elter@kde.org>
                  2001      Carsten Pfeiffer <pfeiffer@kde.org>
                  2001      Martijn Klingens <mklingens@yahoo.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TESTCARDAPPLET_H
#define TESTCARDAPPLET_H

#include <carddeskcore/CardApplet.h>

#include "systemtraywidget.h"
#include "contentswidget.h"

class ConfigureImpl;
class KWinModule;

typedef long unsigned int Atom;

using namespace CardDesk;

class Systemtray : public CardApplet
{
    Q_OBJECT
public:
    Systemtray(const QString &configFile);
    ~Systemtray();

    virtual QString defaultCardName( void ) { return QString("Systemtray"); }
    virtual QPixmap *tabIcon( void );
    virtual void addToDefaultContextMenu( class QPopupMenu * );
    virtual void aboutToShowContextMenu( class QPopupMenu * );
    virtual bool allowDragHoverToOpen( void );
    virtual bool allowDropOnTab( void );
    virtual bool canDecodeDrag( class QMimeSource * );
    virtual void tabDragDropEvent( QDropEvent * );
    virtual Tray *createTray( Card * );
    virtual Contents *createContents( Card * );
    virtual void saveSettings( class KConfig * );
    virtual void loadSettings( class KConfig * );
    virtual void cardReady( void );

public slots:
    void slotconfigure();
    void updateTrayWindows();
    void systemTrayWindowAdded( WId w );
    void configChanged();

protected:
    QPixmap _icon;
    ConfigureImpl *config;

private:
    bool testBackgroundConfig( QString str );

    SystemtrayWidget *trayWidget;
    ContentsWidget *contentsWidget;
    Card *_c;

    KWinModule *kwin_module;
    QValueList<WId> systemTrayWindows;

    Atom net_system_tray_selection;
    Atom net_system_tray_opcode;
};

#endif
