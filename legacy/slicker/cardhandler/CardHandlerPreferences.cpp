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

#include <qcheckbox.h>
#include <qlayout.h>
#include <klocale.h>
#include <klistview.h>
#include <klistbox.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kconfig.h>
#include <knuminput.h>

#include <qpushbutton.h>
#include "pluginmanager.h"
#include "carddeskcore/CardManager.h"
#include "carddeskcore/Card.h"
#include "carddeskcore/CardApplet.h"
#include "carddeskcore/Deck.h"
#include "carddeskcore/CardElements.h"
#include "carddeskcore/Const.h"
#include "manager.h"
#include "CardHandlerPreferences.h"

using namespace CardDesk;

CardHandlerPreferences::CardHandlerPreferences()
   : ConfigModule(i18n("Cards"), i18n("Card settings"), "contents") {

    mpUI = new CardHandlerPreferencesUI(mpFrame);
    mpConfig = KGlobal::config();
    readConfig();

    mpUI->mRemoveButton->setEnabled(false);
    mpUI->mCreateButton->setEnabled(false);

    connect(SlickerCardManager::instance(), SIGNAL(cardRegistered(QString, Card*)),
            SLOT(updateActiveCards()));
    connect(SlickerCardManager::instance(), SIGNAL(cardUnregistered(QString, Card*)),
            SLOT(updateActiveCards()));
    connect(SlickerCardManager::instance(), SIGNAL(cardRenamed(Card *, QString, QString)),
            SLOT(updateActiveCards()));

    connect(mpUI->mAvailableListBox, SIGNAL(highlighted(QListBoxItem*)),
            SLOT(selectedAvailableCard(QListBoxItem*)));
    connect(mpUI->mAvailableListBox, SIGNAL(doubleClicked(QListBoxItem*)),
            SLOT(createCard()));
    connect(mpUI->mActiveListBox, SIGNAL(highlighted(QListBoxItem*)),
            SLOT(selectedActiveCard(QListBoxItem*)));
    connect(mpUI->mCreateButton, SIGNAL(clicked()), SLOT(createCard()));
    connect(mpUI->mRemoveButton, SIGNAL(clicked()), SLOT(removeCard()));
}

CardHandlerPreferences::~CardHandlerPreferences() {
    delete mpUI;
}

void CardHandlerPreferences::save() {
    mEnableTestGui = mpUI->mEnableTestGui->isChecked();
    mCardspace = mpUI->cardspace->value();
    writeConfig();
}

void CardHandlerPreferences::reopen() {
    readConfig();
    mpUI->mEnableTestGui->setChecked(mEnableTestGui);
    mpUI->cardspace->setValue(mCardspace);

    // List available plugins
    mpUI->mAvailableListBox->clear();
    mPluginList= PluginManager::pluginManager()->cardPlugins();
    PluginInfo *info;
    for (info = mPluginList.first(); info !=0 ; info = mPluginList.next() ) {
        mpUI->mAvailableListBox->insertItem(info->name());
    }

    updateActiveCards();
}

void CardHandlerPreferences::updateActiveCards() {

    // List active cards
    mpUI->mActiveListBox->clear();
    mCards = Manager::instance()->cards();
    QPtrListIterator<CardDesk::Card> it(mCards);
    CardDesk::Card *card;
    while ( ( card = it.current()) != 0) {
        ++it;	
        if (!card) continue;
        mpUI->mActiveListBox->insertItem(card->applet()->cardName());
    }
}

// User selected an available card
void CardHandlerPreferences::selectedAvailableCard(QListBoxItem *item) {
    if (!item) return;
    mpUI->mRemoveButton->setEnabled(false);
    mpUI->mCreateButton->setEnabled(true);
    mpUI->mActiveListBox->clearSelection();
}

// User selected an active card
void CardHandlerPreferences::selectedActiveCard(QListBoxItem *item) {
    if (!item) return;
    mpUI->mRemoveButton->setEnabled(true);
    mpUI->mCreateButton->setEnabled(false);
    mpUI->mAvailableListBox->clearSelection();
}

// User clicked on "Create" button
void CardHandlerPreferences::createCard() {
    PluginInfo *info = mPluginList.at(mpUI->mAvailableListBox->currentItem());
    if (!info) return;
    SlickerCardManager::instance()->createCard(*info);
    reopen();
}

// User clicked on "Remove" button
void CardHandlerPreferences::removeCard() {
    Card *card = (Card *) mCards.at(mpUI->mActiveListBox->currentItem());
    if (!card) return;

    SlickerCardManager::instance()->destroyCard(card);
    reopen();
}

void CardHandlerPreferences::readConfig() {
    mpConfig->setGroup("CardHandler");
    mEnableTestGui = mpConfig->readBoolEntry("enableTestGui", true);
    mCardspace = mpConfig->readNumEntry("cardspace", 5);
}

void CardHandlerPreferences::writeConfig() {
    mpConfig->setGroup("CardHandler");
    mpConfig->writeEntry("enableTestGui", mEnableTestGui);
    mpConfig->writeEntry("cardspace", mCardspace);

    emit configChanged();
}
