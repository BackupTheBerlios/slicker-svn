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

#include "manager.h"
#include "newcarddialog.h"
#include "PreferencesDialog.h"

#include "pluginmanager.h"
#include "../carddeskcore/CardApplet.h"
#include <kapplication.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <qtimer.h>
#include <qfile.h>

#include "../share/pluginloader.h"
#include "../carddeskcore/Card.h"
#include "../carddeskcore/Deck.h"


SlickerCardManager *SlickerCardManager::mInstance = 0L;

SlickerCardManager *SlickerCardManager::instance()
{
	if (mInstance == 0L)
		mInstance = new SlickerCardManager();
	return mInstance;
}

SlickerCardManager::SlickerCardManager() : CardDesk::Manager()
{

}

SlickerCardManager::~SlickerCardManager()
{


}

CardDesk::Card *SlickerCardManager::createCard(const QString &desktopFile, const QString &configFile,
											KConfigGroup *group)
{
		KGlobal::dirs()->addResourceType("cardapplets", KStandardDirs::kde_default("data") + "slicker/cardapplets");
		QString desktopPath = KGlobal::dirs()->findResource("cardapplets", desktopFile);

		if (desktopPath == QString::null) return 0L;

		PluginInfo info(desktopPath, configFile);
		return createCard(info, group);
}

CardDesk::Card *SlickerCardManager::createCard(PluginInfo &info, KConfigGroup *group)
{

	if (info.unique() && existsInstanceOfPlugin(info.name()))
	{
		kdDebug(155004) << "Unique plugin, instance exists, bail!" << endl;
		return 0L;
	}


	CardDesk::Card *card = new CardDesk::Card(info,group);

	if (!card->isOkay())
	{
		delete card;
		return 0L;
	}

	return card;
}

void SlickerCardManager::registerCard(CardDesk::Card *card)
{
 	kdDebug(155003) << "Manager::registerCard()" << endl;
	/*
		we're not concerned with performance when adding/removing
		cards, since it won't happen but a handful of times while
		the program is running. So we'll resize the deck for each
		insertion/deletion
	*/

	if (card->cardId().isNull())
		card->setCardId(generateUniqueId());

	_cards.append(card);

	connect(card, SIGNAL(deckingChanged(Card *)), SLOT(slotDeckingModified(Card *)));
	connect(card, SIGNAL(deckingRulesChanged(Card *)), SLOT(slotDeckingRulesModified(Card *)));
	connect(card, SIGNAL(opened(Card *)), SLOT(slotCardOpened(Card *)));
	connect(card, SIGNAL(closed(Card *)), SLOT(slotCardClosed(Card *)));
	connect(card, SIGNAL(nameChanged(Card *, QString, QString)), SLOT(slotCardRenamed(Card *, QString, QString)));
	connect(card, SIGNAL(positionChanged(Card *, double, double, int)),
	        SLOT(slotCardMoved(Card *, double, double, int)));

	emit cardRegistered(QString(card->name()), card);
}


void SlickerCardManager::suicide( CardDesk::Card *woeIsMe )
{
	KGlobal::config()->deleteGroup(QString("Card_").append(woeIsMe->cardId()));
	KGlobal::config()->sync();
	_moribundCards.append(woeIsMe);
	QTimer::singleShot(500, this, SLOT(deleteMoribundCards()));
}

void SlickerCardManager::destroyCard( CardDesk::Card *woeIsMe )
{
	if (woeIsMe->destroy())
	{
		KGlobal::config()->deleteGroup(QString("Card_").append(woeIsMe->cardId()));
		KGlobal::config()->sync();
		_moribundCards.append(woeIsMe);
		QTimer::singleShot(500, this, SLOT(deleteMoribundCards()));
	}
}


CardDesk::Card *SlickerCardManager::newCardDialog()
{
	//should new card dialog return an object rather than pointer?
	QString cardDesktopFile = NewCardDialog::selectPlugin();
	if (cardDesktopFile == QString::null)
		return 0L;
	CardDesk::Card *card = createCard(cardDesktopFile);
	if (card) {
		registerCard(card);
		return card;
	}
	//cancel pressed
	return 0L;
}

void SlickerCardManager::loadCards()
{
 	kdDebug(155004) << ("SlickerCardManager::loadCards()\n");
 	KConfig *config = kapp->config();

	config->setGroup("General");
 	QStringList ids = config->readListEntry("Cards");

 	for (QStringList::Iterator it = ids.begin(); it != ids.end(); ++it)
 	{
 		kdDebug(155004) << "found card: " << *it << endl;
		QString groupName = QString("Card_").append(*it);
		
		if (!config->hasGroup(groupName)) continue;
		KConfigGroup group(config,groupName);

 		CardDesk::Card *card = createCard(group.readEntry("AppletDesktopFile"), group.readEntry("AppletConfigFile"), &group);

		if (!card) continue;
		card->setCardId(*it);
		registerCard(card);

 	}
}

void SlickerCardManager::slotShutdown( void )
{
  	KConfig *config = kapp->config();

  	config->setGroup("General");

  	QStringList instances;
  	CardDesk::CardList roots = rootCards();

	//write list of cards (in the correct (important)) order

  	for (CardDesk::CardListIterator it(roots); it.current(); ++it)
  	{

  		CardDesk::Card *root = it.current();

  		for (int i = 0; i < root->deck()->count(); i++)
  		{
  			instances.append(root->deck()->cardAt(i)->cardId());
  		}
  	}
  	config->writeEntry("Cards", instances);

 	/*
 		Write out the cards settings
 	*/

 	CardDesk::CardList all = cards();
 	for (CardDesk::CardListIterator it(all); it.current(); ++it)
 	{
 		CardDesk::Card *card = it.current();
		KConfigGroup group(config,QString("Card_").append(card->cardId()));
 		card->saveConfig(group);
 	}
	config->sync();

	_cards.setAutoDelete(true);
	_cards.clear();
	_cards.setAutoDelete(false);

}


void SlickerCardManager::configureDialog()
{
   PreferencesDialog::instance()->show();
   PreferencesDialog::instance()->raise();
}


QString SlickerCardManager::generateUniqueId() const
{
	int newId;
	int i = 0;
	bool unique = false;

	while(!unique) {
		i++;
		newId = i;
		unique = true;

		if (kapp->config()->hasGroup(QString("Card_").append(QString::number(newId))))
		{
			unique = false;
			continue;
		}
		for( QPtrListIterator<CardDesk::Card> it(_cards); it.current() ; ++it )
		{
			CardDesk::Card* c = it.current();

			if (c->cardId() == QString::number(newId))
			{
				unique = false;
				break;
			}

		}
	}

	return QString::number(newId);
}


bool SlickerCardManager::existsInstanceOfPlugin( const QString &pluginName )
{
	for ( QPtrListIterator<CardDesk::Card> it(_cards); it.current() ; ++ it ) {
		if (it.current()->pluginInfo()->name() == pluginName)
			return true;
	}

	return false;
}

