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

#include <kdesktopfile.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kglobal.h>
#include <kstandarddirs.h>

#include <qfileinfo.h>

#include "plugininfo.h"

PluginInfo::PluginInfo( const QString &desktopFile, const QString  &configFile )
{
	kdDebug(155006) << "PluginInfo::PluginInfo: " << desktopFile << endl;
	mName = QString::null;
	mDescription = QString::null;
	mAuthor = QString::null;
	mVersion = QString::null;
	mLib = QString::null;
	mType = NoPlugin;
	mUnique = false;

	QFileInfo file(desktopFile);
	mDesktopFile = file.fileName();

	setFromDesktopFile(desktopFile);

	if( configFile.isNull() ) {
		// generate a config file base name from the applet name
		mConfigFile = "slicker_";
		mConfigFile += mName.lower();
		mConfigFile += "_";
		mConfigFile += kapp->randomString(20).lower();
		mConfigFile += "_rc";
	} else {
		mConfigFile = configFile;
	}

}

void PluginInfo::debug()
{
	kdDebug(155006) << "PluginInfo::debug " << endl;
	kdDebug(155006) << "name: " << mName << endl;
	kdDebug(155006) << "version: " << mVersion << endl;
	kdDebug(155006) << "lib: " << mLib << endl;
}

void PluginInfo::setFromDesktopFile( const QString &filePath )
{
	KDesktopFile df(filePath);
	mName = df.readEntry("Name");
	mDescription = df.readEntry("Comment");
	mAuthor = df.readEntry("Author");
	mVersion = df.readEntry("Version");
	mLib = df.readEntry("X-KDE-Library");
	mUnique = df.readBoolEntry("X-KDE-UniquePlugin");

	QString type = df.readEntry("Provides").lower();
	if (type == "appletonly") mType = CardDeskAppletOnly;
	else if (type == "customcardandapplet") mType = CardDeskCustomCardAndApplet;
	else if (type == "customcardonly") mType = CardDeskCustomCardOnly;
	else mType = NoPlugin;
}


PluginInfo::~PluginInfo()
{


}

