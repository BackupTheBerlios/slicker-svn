/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef APPLETDEF_H
#define APPLETDEF_H

#include <qobject.h>
#include <qptrlist.h>
#include <kaction.h>
#include <klocale.h>

class AppletHost;
class Applet;

/**
* This class specifies an AppletFactory, with information about the applets
* it spawns. The appletdef fills two purposes, to neatly gather and show information
* of currently available Applet-Types and to keep track of the applets currently loaded
* of it's class.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class AppletDef
{
friend class Applet;
public:
    AppletDef(const QString &name, const QString &description, const QString &author);
    virtual ~AppletDef();

    virtual bool acceptsHost(const AppletHost * host) const;

    const QString & author() const;
    const QString & description() const;
    const QString & name() const ;

    virtual Applet * createApplet() = 0;

protected:
    void setAuthor(const QString value);

    void setDescription(const QString value);

    void setName(const QString value);

    void registerApplet(Applet *applet);
    void deregisterApplet(Applet *applet);
private:
    QPtrList<Applet> _applets;

    QString _author;
    QString _description;
    QString _name;
};

/**
* A KAction to build up the AppletMenu.
* Add this KAction to a menu, and you will get a
* AppletDefsMenu as a submenu for free.
*/
class AppletDefsMenuItem : public KAction
{
    Q_OBJECT
public:
    AppletDefsMenuItem(AppletDef * appletDef, AppletHost * appletHost = 0);
protected slots:
    void slotActivated();
private:
    AppletDef * _appletDef;
    AppletHost * _appletHost;
};

/**
* The appletMenu. Lists available applets and gives the user the
* option to create new ones. This class can be bound to a certain
* AppletHost, and in that case the menu are filtered to only
* show AppletDefs that are compatible with the host.
*/
class AppletDefsMenu : public KActionMenu
{
    Q_OBJECT
public:
    AppletDefsMenu(QObject *parent=0, const char *name=0, AppletHost * appletHost = 0,const QString &text=i18n("Applets"));
public slots:
    virtual void slotAppletDefRegistered(AppletDef * appletDef);
    virtual void slotAppletDefDeregistered(AppletDef * appletDef);
private:
    QMap<AppletDef*, AppletDefsMenuItem*> _appletDefMenuMap;
    AppletHost * _appletHost;
};


#endif
