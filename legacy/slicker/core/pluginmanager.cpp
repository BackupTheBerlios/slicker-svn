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

#include <kglobal.h>
#include <kstandarddirs.h>
#include <qdir.h>
#include <kdebug.h>

#include "pluginmanager.h"

typedef QPtrList<PluginInfo> InfoList;

PluginManager* PluginManager::pluginManager()
{
	static PluginManager pluginManager;
	return &pluginManager;
}

PluginManager::PluginManager()
{


}

PluginInfo *PluginManager::getInfoForName(const QString &name)
{
	InfoList list = cardPlugins();
	PluginInfo *info;
	for (info = list.first(); info ; info = list.next())
	{
		if (info->name() == name)
			return info;
	}
	return 0;
}

QPtrList<PluginInfo> PluginManager::cardPlugins()
{
	scanCardPlugins();
	return mCardPluginInfoList;
}


void PluginManager::scanCardPlugins() {
	mCardPluginInfoList.clear();
	mCardPluginInfoList = plugins(KGlobal::dirs()->findAllResources ( "appdata","cardapplets/*.desktop") );
}

QPtrList<PluginInfo> PluginManager::plugins( const QStringList& desktopFiles ) 
{
	kdDebug(155000) << desktopFiles.count() << " desktopFiles found" << endl;
	QPtrList<PluginInfo> plugins;

	for ( QStringList::ConstIterator it = desktopFiles.begin();
		it != desktopFiles.end(); ++it ) 
	{
		kdDebug(155000) << "Found Desktop File: " << *it << endl;
		plugins.append( new PluginInfo(*it) );
	}
	kdDebug(155000) << plugins.count() << " plugins found" << endl;
	//qHeapSort( plugins.begin(), plugins.end() );
	return plugins;
}


#include "pluginmanager.moc"
