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

#ifndef INFORMATIONCENTERAPPLET_H
#define INFORMATIONCENTERAPPLET_H

#include <qwidget.h>
#include <qpixmap.h>

#include <carddeskcore/CardApplet.h>
#include <carddeskcore/CardElements.h>

class ConfigDialogImpl;

using namespace CardDesk;

class InformationcenterApplet : public CardApplet
{
    Q_OBJECT
public:
    InformationcenterApplet(const QString& configFile);
    virtual QString defaultCardName( void ) { return QString("Information Center"); }
    virtual QPixmap *tabIcon( void );
    virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );
    virtual bool allowDropOnTab( void );
    virtual bool canDecodeDrag( class QMimeSource * );
    virtual void tabDragDropEvent( QDropEvent * );
    virtual Separator *createSeparator( Card * );
    virtual Contents *createContents( Card * );
    virtual void saveSettings( class KConfig * );
    virtual void loadSettings( class KConfig * );
    virtual void cardReady( void );

public slots:
    void slotrun();
    void slotconfigure();

protected:
    QWidget *_contentsWidget;
    QPixmap _icon;

    ConfigDialogImpl *config;

private:
    void init();
};

#endif
