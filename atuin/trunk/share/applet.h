#ifndef APPLET_H
#define APPLET_H

#include <qwidget.h>
#include "appletdef.h"
#include "applethost.h"

/**
* An applet is a small group of two widgets, an icon and a contentwidget.
* An applet can and must always be contained in some AppletHost.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class Applet
{
public:
    Applet(AppletDef *appletDef);
    virtual ~Applet();

    /**
    * Returns the parent AppletDef
    */
    AppletDef * appletDef() const;

    /**
    * Allows the applet to reject some AppletHosts
    */
    virtual bool acceptsHost(const AppletHost * host) const;

    /**
    * Transfers the applet from whatever it is connected to now, to a new AppletHost.
    */
    bool transfer(AppletHost * newHost);

    /**
    * This could be subclassed
    */
    virtual QWidget * icon();
    virtual const QString & name();
    virtual QWidget * content();
    
    AppletHost * host();
    
protected:
    QWidget * _icon;
    QWidget * _content;
    
private:
    AppletHost *_host;
    AppletDef *_appletDef;
};

#endif
