/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef APPLET_H
#define APPLET_H

#include <qwidget.h>
#include <kaction.h>
#include "appletdef.h"
#include "applethost.h"

/**
* An applet is a small group of two widgets, an icon and a contentwidget.
* An applet can and must always be contained in some AppletHost.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*
*
* Date of changes: 2005-01-05 (%Y-%m-%d)
* Author: Jakob Simon-Gaarde (jakob(at)simon-gaarde.dk)
*
* I have moved the applet event handling to the
* toplevel superclass. I can't imagine an applet with no slots anyway
* and this way we can add general context issues to all applets and
* control drag-and-drop functionality in a more general way by adding
* methods like: 
*    
* acceptDrops(bool)
* acceptDropTypes(const QStringList& mimeTypes)
*
* In the effort of achieving this I have added the following functions:
* 
* public:
* eventFilter()
* setIcon()
* setContent()
* addDefaultContextAction()
*
* public slots:
* popupContextMenu()
* 
* protected:
* addContextAction()
* contextActions()
*/

class Applet : public QObject
{
public:
    Applet(AppletDef *appletDef);
    virtual ~Applet();

    /**
    Eventfilter to catch icon (and later content) events
    */
    virtual bool    eventFilter(QObject* o, QEvent * e );   
       
    /**
    * Returns the parent AppletDef
    */
    AppletDef * appletDef() const;

    /**
    * Allows the applet to reject some AppletHosts
    */
    virtual bool acceptsHost(const AppletHost * host) const;

    /**
    * Transfers the applet from whatever it is connected to now, to a new AppletHost.
    */
    bool transfer(AppletHost * newHost);

    /**
    * This could be subclassed
    */
    virtual void             setIcon(QWidget* icon);
    virtual void             setContent(QWidget* content);
    virtual QWidget *        icon();
    virtual const QString &  name();
    virtual QWidget*         content();
   
    /**
    * Add an action to the default context menu. NB! If contextActions() is
    * overwritten default actions can be eliminated if the KActionPtrList
    * returned is not the default one. This method should be used by classes
    * like Slider, CardDeck and Applet it self.
    */
    virtual void             addDefaultContextAction(KAction* action);
       
    AppletHost *             host();
    
public slots:
    virtual void             popupContextMenu();

protected:
    /**
    * Overwrite this function to override the default actions provided by
    * the applet host or the Applet superclass
    */
    virtual KActionPtrList&  contextActions();
    
    /**
    * Add an action to the actual context menu. This action will be put
    * into the KActionPtrList returned by contextActions(), so if isn't 
    * a naughty applet developer who returns a new KActionPtrList each 
    * time contextActions() is called, the action shold certainly appear 
    * in the context menu.
    */
    virtual void             addContextAction(KAction* action);
           
private:
    QWidget *                _icon;
    QWidget *                _content;
    KActionPtrList           _defaultContextActions;
    KActionMenu*             _contextMenu;
    
    AppletHost *             _host;
    AppletDef *              _appletDef;
    
};

#endif
