/*****************************************************************
 
Copyright (c) 2003 the slicker authors. See file AUTHORS.
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
******************************************************************/

#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kconfig.h>
#include <kcrash.h>
#include <kdebug.h>
#include <klocale.h>
#include <qtimer.h>
#include <qvaluelist.h>
#include <stdlib.h>

#include "slicker.h"
#include "pluginmanager.h"
#include "sessionmanager.h"
#include "slider.h"
#include "appletmanager.h"

Slicker * Slicker::_self = NULL;

Slicker::Slicker()
        : KUniqueApplication(true, true, false)
{
    _self = this;
    KGlobal::config()->setName("slickerrc");
    _dataDir = "slicker";
    PluginManager::self(); // Make sure all the manager classes are created
    AppletManager::self();
    
    new SliderFactory();
    SessionManager::self()->restore();

    _menu = new KPopupMenu( NULL, "Slicker Menu");
    _globalAccel = new KGlobalAccel(this, "Global Accelerators");
    _appletMenu = new AppletDefsMenu(_menu, "Applet Menu");

    _menu->insertItem( "Create Slider", this, SLOT(newSlider()));
    _menu->insertItem( "Plugin Preferences", PluginManager::self(), SLOT(showPluginPrefs()));
    _appletMenu->plug(_menu);
    _menu->insertSeparator();
    _menu->insertItem( "Quit", this, SLOT(quit()));

    _globalAccel->insert( "Show Slickermenu", i18n("Show Slickermenu"),
                          i18n("This action opens up the main Slickermenu"),
                          ALT+CTRL+Key_O, 0, this, SLOT(showMenu()), true, true );
    _globalAccel->updateConnections();
}

Slicker::~Slicker()
{
    delete _menu;
    delete SessionManager::self();
    delete PluginManager::self();
    delete AppletManager::self();
}

Slicker * Slicker::self()
{
    return _self;   
}

KPopupMenu *Slicker::menu()
{
    return _menu;
}

void Slicker::showMenu()
{
    menu()->show();
}

void Slicker::showMenu(const QPoint &origin)
{
    //TODO: Implement this correctly.
    origin.isNull();
    showMenu();
}

const QString &Slicker::dataDir() const
{
    return _dataDir;        
}

Slider * Slicker::newSlider()
{
    SessionItemFactory * factory = SessionManager::self()->getFactory("slider");
    if (factory)
        return (Slider*)factory->createItem();
    else
        return 0L;
}

int main( int argc, char *argv[] )
{
    KAboutData aboutData("slicker", I18N_NOOP("Slicker")
                         , "0.1"
                         , I18N_NOOP("A replacement panel for KDE")
                         , KAboutData::License_GPL
                         , I18N_NOOP("(c) 2002-2004, The Slicker Team"));

    KCmdLineArgs::init( argc, argv, &aboutData );
    Slicker::addCmdLineOptions();

    if (!Slicker::start()) {
        fprintf(stderr, "slicker is already running!\n");
        exit(0);
    }

    KCrash::setCrashHandler();
    Slicker slicker;
    return slicker.exec();
}

#include "slicker.moc"

