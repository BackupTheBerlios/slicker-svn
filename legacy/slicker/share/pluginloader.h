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

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <qptrdict.h>

#include "../carddeskcore/Card.h"
#include "plugininfo.h"

/*!This singelton class is used to return an applet given information about the plugin.
 * This is the only place in the architecture that the plugin library is loaded
 */
class PluginLoader : public QObject
{
	Q_OBJECT


	public:
		/*!
		 * PluginLoader is a singleton.  This function returns its instance
		 */
		static PluginLoader* pluginLoader();
		/*! Returns a CardApplet given information about the plugin
		 */
		CardDesk::CardApplet* loadApplet(const PluginInfo &);

		virtual ~PluginLoader();

	protected:
		PluginLoader();

	private:
		QPtrDict<PluginInfo> _dict;

};

#endif
