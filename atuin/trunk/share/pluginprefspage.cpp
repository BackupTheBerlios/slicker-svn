#include "pluginprefspage.h"

#include <ktrader.h>
#include <kdebug.h>
#include "pluginmanager.h"

PluginPrefsPage::PluginPrefsPage(KInstance *instance, QWidget *parent, const QStringList &args) : KCModule(instance, parent, args)
{
    // UGLY! This shit should not be allowed, but it is.
    QString pluginId = instance->instanceName();
    pluginId.remove("kcm_");
    _info = PluginManager::self()->infoForPluginId(pluginId);
}

PluginPrefsPage::~PluginPrefsPage()
{
}

KPluginInfo * PluginPrefsPage::info()
{
    return _info;
}

KConfig * PluginPrefsPage::config()
{
    return _info->config();    
}

#include "pluginprefspage.moc"
