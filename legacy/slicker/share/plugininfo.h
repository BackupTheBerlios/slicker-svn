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


#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <qobject.h>
#include <qdict.h>
#include <qstring.h>

//this is probably redundant
enum SlickerPluginType { CardDeskAppletOnly, CardDeskCustomCardAndApplet, CardDeskCustomCardOnly, NoPlugin };

//!  Contains information about a Slicker plugin

class PluginInfo
{
	public:


	//! PluginInfo constructor
	/*!
	Construct a PluginInfo from paths to a desktop file and a config file.
	\param desktopFile Path to the desktop file containing information about the applet
	\param configFile Path to the config file for the applet
	If no path to a config file is passed, a suitable path is generated.
	*/

		PluginInfo(const QString &desktopFile = QString::null, const QString &configFile = QString::null);

		/*!Returns applet's name*/
		QString name() const { return mName; }

		/*!Returns applet's description*/
		QString description() const { return mDescription; }

		/*!Returns applet's author*/
		QString author() const { return mAuthor; }

		/*!Returns applet's version number*/
		QString version() const { return mVersion; }

		/*!Returns applet's library path*/
		QString lib() const { return mLib; }

		/*!Returns applet's config file*/
		QString configFile() const { return mConfigFile; }

		/*!Returns applet's desktop file*/
		QString desktopFile() const { return mDesktopFile; }

		bool unique() const { return mUnique; }

		~PluginInfo();
	protected:

		void setName(const QString &name) { mName = name; }
		void setDescription(const QString &description) { mDescription = description; }
		void setAuthor(const QString &author) { mAuthor = author; }
		void setVersion(const QString &version) { mVersion = version; }
		void setLib(const QString &lib) { mLib = lib; }
		void setType(const SlickerPluginType &type) { mType = type; }
		void setConfigFile( const QString &configFile ) { mConfigFile = configFile; }
		void setUnique( bool unique ) { mUnique = unique; }

	private:
		/*
		This function initliazes the PluginInfo with data from the supplied KDesktop file
		This sets everything apart from the plugin's configuration path
		*/
		void setFromDesktopFile( const QString &filePath );

		// Prints info to stdout
		void debug();

		QString mName;
		QString mDescription;
		QString mAuthor;
		QString mVersion;
		QString mLib;
		QString mConfigFile;
		QString mDesktopFile;
		int mType;
		bool mUnique;

};

#endif
