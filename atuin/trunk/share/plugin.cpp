#include "plugin.h"

#include <ksettings/dispatcher.h>

Plugin::Plugin(KInstance *instance, QObject *parent, const char *name)
        : QObject(parent, name)
{

	KSettings::Dispatcher::self()->registerInstance( instance, this, SIGNAL( settingsChanged() ) );
        connect(this, SIGNAL(settingsChanged()), this, SLOT(saveConfig()));	
}

Plugin::~Plugin()
{}

KConfig * Plugin::config()
{
    return _info->config();    
}

KPluginInfo * Plugin::info()
{
    return _info;
}

void Plugin::saveConfig()
{
    _info->config()->sync();
}

#include "plugin.moc"
