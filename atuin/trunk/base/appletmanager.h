#ifndef APPLETMANAGER_H
#define APPLETMANAGER_H

#include <qobject.h>
#include <kaction.h>
#include <qmap.h>
#include "applet.h"

/**
Keeps track of all existing AppletHosts in the system, as well
as all existing AppletDefs.
@author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class AppletManager : public QObject
{
    Q_OBJECT
    friend class AppletDef;
    friend class AppletHost;
    friend class Slicker;
private:
    AppletManager();

    ~AppletManager();
public:
    static AppletManager* self();

    /**
    *	Returns a QPtrList with all registered AppletHosts
    */
    const QPtrList<AppletHost> * appletHosts() const;

    /**
    *	Returns a QPtrList with all registered AppletDefs
    */
    const QPtrList<AppletDef> * appletDefs() const;

    /**
    * Finds a suitable host for applet
    */
    AppletHost * dispatch(const AppletDef *applet) const;

    /**
    * Finds a suitable host for applet
    */
    AppletHost * dispatch(const Applet *applet) const;

    /**
    * Finds a suitable host for applet and if possible,
    * move applet there.
    */
    bool dispatchAndTransfer(Applet *applet) const;

signals:
    void appletDefRegistered(AppletDef *);
    void appletDefDeregistered(AppletDef *);

    void appletHostRegistered(AppletHost *);
    void appletHostDeregistered(AppletHost *);

protected:
    /**
    * Registers a new AppletHost
    */
    void registerAppletHost(AppletHost * appletHost);

    /**
    * Unregisters a new AppletHost
    */
    void deregisterAppletHost(AppletHost * appletHost);

    /**
    * Registers a new AppletDef
    */
    void registerAppletDef(AppletDef * appletDef);

    /**
    * Unregisters a new AppletDef
    */
    void deregisterAppletDef(AppletDef * appletDef);

private:
    static AppletManager* _self;
    QPtrList<AppletHost> _appletHosts;
    QPtrList<AppletDef> _appletDefs;
};

#endif
