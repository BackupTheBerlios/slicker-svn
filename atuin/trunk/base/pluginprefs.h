/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef PLUGINPREFS_H
#define PLUGINPREFS_H

#include <kdialogbase.h>
#include <kpluginselector.h>

/**
* The PluginPrefs class is the graphical dialog that let's the user choose and configure his/her
* plugins. Due to some very annoying reasons it is also responsible for saving the active plugins
* configuration. Don't blame the author, if you feel this behaviour is incorrect, send a mail to
* Matthias Kretz <mkretz@kde.org>
* @author Ulrik Mikaelsson
*/
class PluginPrefs : public KDialogBase
{
    Q_OBJECT
public:
    PluginPrefs();
    ~PluginPrefs();

    KPluginSelector * pluginSelector();
public slots:
    virtual void show();
    
protected slots:
    void setChanged( bool c );

    void slotDefault();
    void slotApply();
    void slotOk();
private:
    KPluginSelector * _pluginSelector;
};

#endif
