/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include "pluginprefs.h"
#include <kdebug.h>
#include <klocale.h>
#include <ksettings/dispatcher.h>
#include <qlayout.h>
#include "pluginmanager.h"

PluginPrefs::PluginPrefs()
        : KDialogBase( Plain, i18n( "Configure" ), Default | Cancel | Apply | Ok, Ok, NULL, "PluginPrefs", false, true )
{
    setChanged(false);

    setInitialSize( QSize( 640, 480 ) );

    ( new QVBoxLayout( plainPage(), 0, 0 ) )->setAutoAdd( true );
    _pluginSelector = new KPluginSelector(plainPage());
    setMainWidget(_pluginSelector);

    connect( _pluginSelector, SIGNAL( changed( bool ) ), this, SLOT( setChanged( bool ) ) );
    connect( _pluginSelector, SIGNAL( configCommitted( const QCString & ) ),
             KSettings::Dispatcher::self(), SLOT( reparseConfiguration( const QCString & ) ) );

    _pluginSelector->addPlugins( PluginManager::self()->availablePlugins());
}

PluginPrefs::~PluginPrefs()
{}

KPluginSelector * PluginPrefs::pluginSelector()
{
    return _pluginSelector;
}

void PluginPrefs::setChanged( bool c )
{
    enableButton( Apply, c );
}

void PluginPrefs::slotDefault()
{
    _pluginSelector->defaults();

    setChanged( false );
}

void PluginPrefs::slotApply()
{
    _pluginSelector->save();
    
    setChanged( false );

    // As soon as configCommitted starts getting signaled, we won't need this.
    PluginManager::self()->resyncPlugins();
}

void PluginPrefs::slotOk()
{
    slotApply();
    accept();
}

void PluginPrefs::show()
{
    _pluginSelector->load();
    KDialogBase::show();
}

#include "pluginprefs.moc"


