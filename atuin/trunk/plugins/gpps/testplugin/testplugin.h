#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "plugin.h"
#include "sessionapplet.h"
#include "sessionappletdef.h"

class TestAppletDef;
class QLabel;

class TestPlugin : public Plugin
{
    Q_OBJECT
public:
    TestPlugin(QObject *parent, const char *name, const QStringList &args);
    virtual ~TestPlugin();
private:
    TestAppletDef * _appletDef;
};

class TestApplet : public QObject, public SessionApplet
{
    Q_OBJECT
public:
    TestApplet(SessionAppletDef * appletDef, const QString & id, TestPlugin * testPlugin);
    virtual ~TestApplet();
private slots:
    void slotSettingsChanged();
private:
    TestPlugin * _plugin;
    QLabel * _label;
};

class TestAppletDef : public SessionAppletDef
{
public:
    TestAppletDef(TestPlugin * plugin);
    virtual ~TestAppletDef();
    
    SessionApplet * loadSessionApplet(KConfigBase * config);
    SessionApplet * createSessionApplet();
private:
    TestPlugin * _plugin;
};

#endif

