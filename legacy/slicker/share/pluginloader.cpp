/***************************************************************************
         				Copyright (C) 2003 Mark Westcott
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "pluginloader.h"


#include <qfile.h>
#include <klibloader.h>
#include <kconfig.h>
#include <kdebug.h>

#include "../carddeskcore/CardApplet.h"

PluginLoader* PluginLoader::pluginLoader()
{
	    static PluginLoader pluginLoader;
	        return &pluginLoader;
}

PluginLoader::PluginLoader()
{
	    _dict.setAutoDelete( true );
}

PluginLoader::~PluginLoader()
{

}

CardDesk::CardApplet* PluginLoader::loadApplet (const PluginInfo &pluginInfo)
{
	QString pluginLib = pluginInfo.lib();
	kdDebug(155006) << "loading library: " << pluginLib << endl;
	KLibLoader *libloader = KLibLoader::self();
	KLibrary *lib = libloader->library(pluginLib.latin1());
	if (!lib)
	{
		kdWarning() << "cannot open card applet: " << pluginLib
						<< " because of " << libloader->lastErrorMessage() << endl;
		return 0L;
    }

		CardDesk::CardApplet* (*init_ptr)(const QString&);
   	init_ptr = (CardDesk::CardApplet* (*)(const QString&))lib->symbol( "init" );

		if (!init_ptr) {
			kdWarning() << "Intiliazed factory, but can't create applet" << endl;
			libloader->unloadLibrary( pluginLib.latin1() );
			return 0L;
		}

		CardDesk::CardApplet* applet = init_ptr(pluginInfo.configFile());

		if (!applet) {
			return 0L;
		}
		
	return applet;
}

