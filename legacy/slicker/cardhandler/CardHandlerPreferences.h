/***************************************************************************
    copyright            : (C) 2003 by Markus Breitenberger
    email                : breitenberger@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _CARDHANDLERPREFERENCES_H
#define _CARDHANDLERPREFERENCES_H

#include <kconfig.h>
#include <qlistbox.h>
#include "pluginmanager.h"
#include "ConfigModule.h"
#include "CardHandlerPreferencesUI.h"
#include "../carddeskcore/Card.h"

class CardHandler;

class CardHandlerPreferences : public ConfigModule {

    friend class CardHandler;

    Q_OBJECT

    public:
        CardHandlerPreferences();
        virtual ~CardHandlerPreferences();

        void readConfig();
        void writeConfig();
        virtual void save();
        virtual void reopen();

    public slots:
        void selectedAvailableCard(QListBoxItem *item);
        void selectedActiveCard(QListBoxItem *item);
        void updateActiveCards();
        void createCard();
        void removeCard();

    signals:
        void configChanged();

    private:
        CardHandlerPreferencesUI *mpUI;
        QPtrList<PluginInfo>      mPluginList;
        CardDesk::CardList      mCards;

        KConfig *mpConfig;
        bool mEnableTestGui;
	uint mCardspace;
};

#endif // _CARDHANDLERPREFERENCES_H
