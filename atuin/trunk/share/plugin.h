#ifndef PLUGIN_H
#define PLUGIN_H

#include <kconfig.h>
#include <kinstance.h>
#include <kplugininfo.h>
#include <qobject.h>

/**
* A plugin is a piece of binary code that may get loaded and executed
* dynamically in a parent application.
* 
* This should be subclassed and exported through the use of KGenericFactory<YourPluginClass>
* and K_EXPORT_COMPONENT_FACTORY.
* See the Slicker Applet Developer Guide for more info.
* @author Ulrik Mikaelsson
*/
class Plugin : public QObject
{
    Q_OBJECT
friend class PluginManager;
public:
    Plugin(KInstance *instance, QObject *parent, const char *name);
    virtual ~Plugin();

    KConfig * config();
    KPluginInfo * info();
    
public slots:
    void saveConfig();
signals:
    void settingsChanged();

private:
    KPluginInfo * _info;
};

#endif
