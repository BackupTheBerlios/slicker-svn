#ifndef APPLETHOST_H
#define APPLETHOST_H

#include <qobject.h>
#include <qptrlist.h>

class AppletDef;
class Applet;

/**
* An AppletHost is a container for applets. It normally allows
* one or many applets to be held within it.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class AppletHost
{
public:
    AppletHost(QString &name);
    AppletHost(const char *name);
    virtual ~AppletHost();

    /**
    * These two functions tell whether this AppletHost
    * would accept an applet of type applet.
    */
    virtual bool acceptsApplet(const AppletDef * applet) const;
    virtual bool acceptsApplet(const Applet * applet) const;

    /**
    * Applet should be moved into this AppletHost.
    * The implementation of this MUST CALL registerApplet!
    */
    virtual bool accept(Applet * applet) = 0;

    /**
    * Applet got detached, might be time for some cleanup.
    * The implementation of this MUST CALL deregisterApplet!
    */
    virtual void detach(Applet * applet) = 0;

    const QString &name() const;

protected:
    void registerApplet(Applet *applet);
    void deregisterApplet(Applet *applet);

private:
    QPtrList<Applet> _applets;
    QString _name;
};

#endif
