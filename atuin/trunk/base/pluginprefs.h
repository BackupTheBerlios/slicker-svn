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
