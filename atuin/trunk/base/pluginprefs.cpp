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


