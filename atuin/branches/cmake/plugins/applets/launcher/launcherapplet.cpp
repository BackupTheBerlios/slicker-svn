/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Markus Breitenberger <bre@slicker.org> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include <kpropertiesdialog.h>
#include <kdesktopfile.h>
#include <kfiledialog.h>
#include <kfileitem.h>
#include <kio/netaccess.h>
#include <qregexp.h>
#include <qfile.h>
#include <kstandarddirs.h>
#include "launcherapplet.h"
#include "launcherplugin.h"
#include "launcherbutton.h"

////////////////////////////////////////////////////////////////////////////
// LauncherApplet
////////////////////////////////////////////////////////////////////////////

LauncherApplet::LauncherApplet(SessionAppletDef * appletDef,
                               const QString & id,  LauncherPlugin * plugin)
    : SessionApplet(appletDef, id),
      _plugin(plugin), _fileItem(0)
{
    _button = new LauncherButton();
    _button->popupMenu()->insertItem( i18n("&Preferences"), this, SLOT(slotShowPreferences()));
    _button->popupMenu()->insertItem( i18n("&Remove"), this, SLOT(slotRemove()));

    connect(_plugin, SIGNAL(settingsChanged()), this, SLOT(slotSettingsChanged()));
    connect(_button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    
    _icon = _button;
    
    slotSettingsChanged();
}

LauncherApplet::~LauncherApplet()
{
    delete _fileItem;
}

void LauncherApplet::slotSettingsChanged()
{
    delete _fileItem;
    _fileItem = new KFileItem( KFileItem::Unknown, KFileItem::Unknown, _url.url());

    _button->setIcon(_fileItem->iconName());
       
    if (KDesktopFile::isDesktopFile(_url.path())) {
        KDesktopFile df(_url.path());   
        QString comment = df.readComment();
        QString name = df.readName();
        if (comment.isEmpty()) {
            _button->setTooltip(name);
        } else {
            _button->setTooltip(name + " - " + comment);
        }
    } else {
        _button->setTooltip(_fileItem->text());
    }
}

/**
 *  Initialize the applet.
 *
 *  Only called when we create a new applet. Asks the user for an URL and
 *  creates a .desktop file...
 */
bool LauncherApplet::init()
{
    //    
    // Get an url...
    //
    KFileDialog fileDlg(0, 0, 0, 0, true);
    fileDlg.exec();
    _url = fileDlg.selectedURL();
    
    if (!_url.isValid()) {
        return false;
    }
    
    //
    // Create a .desktop file
    //
    QString file = newDesktopFile(_url);
    
    if (KDesktopFile::isDesktopFile(_url.path())) {
        // Copy existing file...
        KURL dest;
        dest.setPath(file);
        KIO::NetAccess::upload(_url.path(), dest, 0);
    } else {
        // Create new file...
        KDesktopFile df(file);
        
        df.writeEntry("Encoding", "UTF-8");
        df.writeEntry("Type","Link");
        df.writeEntry("Name", _url.prettyURL());
        
        if (_url.isLocalFile()) {
            KFileItem item( KFileItem::Unknown, KFileItem::Unknown, _url );
            df.writeEntry("Icon", item.iconName() );
        } else {
            df.writeEntry("Icon", KMimeType::favIconForURL(_url));
        }
        df.writeEntry("URL", _url.url());
        df.sync();
    }
    
    // Use the .desktop file as URL
    _url = KURL();
    _url.setPath(file);
    
    // Update everything...
    slotSettingsChanged();
    
    return true;
}

void LauncherApplet::slotButtonClicked()
{
    if (_fileItem) {
        _fileItem->run();
    }
}

void LauncherApplet::slotShowPreferences()
{
    // Will delete itself...
    KPropertiesDialog *pDlg = new KPropertiesDialog(_fileItem, 0L, 0L, false, false);
    pDlg->setFileNameReadOnly(true);
    connect(pDlg, SIGNAL(applied()), SLOT(slotSettingsChanged()));
    pDlg->show();
}

void LauncherApplet::slotRemove()
{
    host()->detach(this);
    
    QFile desktopFile(_url.path());
    desktopFile.remove();
    
    delete this;
}

void LauncherApplet::store(KConfigBase * config)
{
    SessionApplet::store(config);
    
    config->writeEntry("URL", _url.url());
}

void LauncherApplet::restore(KConfigBase * config)
{    
    SessionApplet::restore(config);
    
    _url = KURL(config->readEntry("URL"));
    
    slotSettingsChanged();
}

QString LauncherApplet::newDesktopFile(const KURL& url)
{
   QString base = url.fileName();
   if (base.endsWith(".desktop"))
      base.truncate(base.length()-8);
   QRegExp r("(.*)(?=-\\d+)");
   if (r.search(base) > -1)
      base = r.cap(1);

   QString file = base + ".desktop";

   for(int n = 1; ++n; )
   {
      QString path = locate("appdata", file);
      if (path.isEmpty())
         break;

      file = QString("%2-%1.desktop").arg(n).arg(base);
   }
   file = locateLocal("appdata", file);
   return file;
}
    
////////////////////////////////////////////////////////////////////////////
// LauncherAppletDef
////////////////////////////////////////////////////////////////////////////

LauncherAppletDef::LauncherAppletDef(LauncherPlugin * plugin)
 : SessionAppletDef("launcherapplet", "LauncherApplet", "LauncherApplet", "Markus Breitenberger"),
   _plugin(plugin)
{
}

LauncherAppletDef::~LauncherAppletDef()
{
}

SessionApplet * LauncherAppletDef::createSessionApplet()
{
    LauncherApplet *applet = new LauncherApplet(this, generateItemId(), _plugin);
    if (!applet->init()) {
        delete applet;
        applet = 0;
    }
    return applet;
}
    
SessionApplet * LauncherAppletDef::loadSessionApplet(KConfigBase * config)
{
    SessionApplet * retVal = new LauncherApplet(this, config->group(), _plugin);
    retVal->restore(config);
    return retVal;
}

#include "launcherapplet.moc"
