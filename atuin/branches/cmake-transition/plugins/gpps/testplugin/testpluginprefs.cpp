#include "testpluginprefs.h"

//#include <kparts/componentfactory.h>
#include <kgenericfactory.h>

typedef KGenericFactory<TestpluginPreferences> TestpluginPreferencesFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_slicker_testplugin, TestpluginPreferencesFactory( "kcm_slicker_testplugin" )  )

TestpluginPreferences::TestpluginPreferences(QWidget *parent, const char* /*name*/, const QStringList &args)
                                                        : PluginPrefsPage(TestpluginPreferencesFactory::instance(), parent, args)
{
    _lineEdit = new QLineEdit(this, "Text");
    connect(_lineEdit, SIGNAL(textChanged(const QString& )), this, SLOT(textChanged()));
    
    load();
}

TestpluginPreferences::~TestpluginPreferences()
{
}

void TestpluginPreferences::textChanged()
{
    emit KCModule::changed(true);   
}

void TestpluginPreferences::load()
{
    _lineEdit->setText(config()->readEntry("text", "Testing123"));

    emit KCModule::changed(false);
}

void TestpluginPreferences::save()
{
    config()->writeEntry("text", _lineEdit->text());

    emit KCModule::changed(false);
}

#include "testpluginprefs.moc"

