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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <qvaluelist.h>
#include "../share/plugininfo.h"
#include <qstring.h>
#include <qptrlist.h>

/*!PluginManager is a singleton class which scans for information about available plugins*/

class PluginManager : public QObject
{
	Q_OBJECT

	public:
		/*!Returns the the current instance of pluginManager.  If none exists an instance is created*/
		static PluginManager* pluginManager();
		/*!Returns a QPtrList of all available CardDesk plugins*/
		QPtrList<PluginInfo> cardPlugins();
		/*!This function currently does nothing*/
		QPtrList<PluginInfo> sliderPlugins();
		/*!Returns a PluginInfo given the name of a plugin*/
		PluginInfo *getInfoForName(const QString &name);

	protected:
		PluginManager();
		QPtrList<PluginInfo> plugins(const QStringList& desktopFiles);

	private:
		void scanCardPlugins();
		void scanSliderPlugins();
		QPtrList<PluginInfo> mCardPluginInfoList;
		QPtrList<PluginInfo> mSliderPluginInfoList;
};

#endif
