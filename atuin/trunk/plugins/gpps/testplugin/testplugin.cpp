
#include "testplugin.h"
#include <kdebug.h>
#include <kconfigbase.h>
#include <kgenericfactory.h>
#include <qlabel.h>

typedef KGenericFactory<TestPlugin> TestPluginFactory;
K_EXPORT_COMPONENT_FACTORY( slicker_testplugin, TestPluginFactory("slicker_testplugin") );

/****** TestPlugin *****************************/
TestPlugin::TestPlugin(QObject *parent, const char *name, const QStringList &/*args*/) : Plugin(TestPluginFactory::instance(), parent, name)
{
    _appletDef = new TestAppletDef(this);
}

TestPlugin::~TestPlugin()
{
    delete _appletDef;
}

/****** TestApplet *****************************/
TestApplet::TestApplet(SessionAppletDef * appletDef, const QString & id,  TestPlugin * plugin)
 : SessionApplet(appletDef, id)
{
    _label = new QLabel(plugin->config()->readEntry("text", "Testing123"), 0); 
    _icon = _label;
    _plugin = plugin;

    connect(_plugin, SIGNAL(settingsChanged()), this, SLOT(slotSettingsChanged()));
}

TestApplet::~TestApplet()
{
}

void TestApplet::slotSettingsChanged()
{
    _label->setText(_plugin->config()->readEntry("text", "Testing123"));
}

/****** TestAppletDef **************************/

TestAppletDef::TestAppletDef(TestPlugin * plugin)
 : SessionAppletDef("testapplet", "TestApplet", "TestApplet", "Ulrik Mikaelsson")
{
    _plugin = plugin;
}

TestAppletDef::~TestAppletDef()
{
}

SessionApplet * TestAppletDef::createSessionApplet()
{
    return new TestApplet(this, generateItemId(), _plugin);    
}
    
SessionApplet * TestAppletDef::loadSessionApplet(KConfigBase * config)
{
    SessionApplet * retVal = new TestApplet(this, config->group(), _plugin);
    retVal->restore(config);
    return retVal;
}


#include "testplugin.moc"

