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


#ifndef MANAGER_H
#define MANAGER_H

#include <kdebug.h>
#include <qobject.h>
#include <kconfigbase.h>

#include "../carddeskcore/CardManager.h"
#include "../carddeskcore/Const.h"

#include <share/plugininfo.h>

/**SlickerCardManager is a singelton class used to create, destroy and keep track of Cards*/

class SlickerCardManager : public CardDesk::Manager
{
	Q_OBJECT

public:

	SlickerCardManager();
	~SlickerCardManager();
	/*!Returns an instance of SlickerCardManager. If this is the first time the function has been called, an instance will be created*/
	static SlickerCardManager *instance( void );

	/*!A Card calls this function in order to have itself destroyed*/
	
	void suicide(CardDesk::Card*);

	/*!This function is called for the destruction of a card. First the applet is asked if it will allow the destruction, then the Card's section in slickerrc is removed and the Card is destroyed*/
	void destroyCard(CardDesk::Card*);

	void registerCard(CardDesk::Card *);

	/*!Returns true if an instance of the given plugin already exists*/
	bool existsInstanceOfPlugin( const QString &pluginName );
	
public slots:
	/*!Loads cards on the basis of what is written in the slicker configuration file
	 * Each card has its own group in this file, and they follow the format:\n
	 * [Card_1]\n
	 * AppletDesktopFile=exampleapplet.desktop\n
	 * AppletConfigFile=exampleapplet_32354334_rc\n
	 * ScreenEdge=TopEdge\n
	 * Position=0.54\n
	 *
	 * When this function is called, the config file is parsed, a PluginInfo is created for each entry in the config file and createCard is then called()*/
	void loadCards();

	void configureDialog();

	/*!This function is called as slicker is about to be shut down.  Stores config information about all currently existing cards in the slicker config file so they can be reproduced on startup by loadCards() */
	void slotShutdown( void );

	/*!This function is called in order to create a new Card from a plugin selected by the user*/
	CardDesk::Card* newCardDialog();

	/*!This function reloads a card given its PluginInfo and its group in slickerrc.  A unique id is generated if necessary*/
	CardDesk::Card* createCard(PluginInfo &info, KConfigGroup *group = 0L);

	CardDesk::Card* createCard(const QString & = QString::null, const QString & = QString::null, KConfigGroup * = 0L);


private:

	static SlickerCardManager *mInstance;
	QString generateUniqueId() const;
};

#endif
