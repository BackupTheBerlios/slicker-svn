/***************************************************************************
                          Test.cpp  -  description
                             -------------------
    begin                : Mon Dec 23 19:30:00 EST 2002
    copyright            : (C) 2002 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "TestGui.h"

#include "carddeskcore/CardManager.h"
#include "carddeskcore/Card.h"
#include "carddeskcore/CardApplet.h"
#include "carddeskcore/Deck.h"
#include "carddeskcore/CardElements.h"
#include "carddeskcore/Const.h"

#include <qpushbutton.h>
#include <qlabel.h>
#include <qslider.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qcolor.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qstringlist.h>

#include <klistview.h>
#include <klocale.h>
#include <klineeditdlg.h>
#include <kapplication.h>
#include <kiconloader.h>
#include <kglobal.h>
#include "manager.h"


/*
*/


TestGui::TestGui(QWidget* parent, const char *name)
	: KDialog(parent, name), _sliderActive(false)
{
	
	setIcon(KGlobal::iconLoader()->loadIcon("tab_new",KIcon::Small));
	setCaption(i18n("TestGUI"));
	QVBoxLayout *top = new QVBoxLayout(this);
	top->setSpacing(4);
	top->setMargin(4);

	connect(Manager::instance(), SIGNAL(cardRegistered(QString, Card*)),
	        SLOT(cardRegistered(QString, Card *)));

	connect(Manager::instance(), SIGNAL(cardUnregistered(QString, Card*)),
	        SLOT(cardUnregistered(QString, Card *)));

	connect(Manager::instance(), SIGNAL(deckingChanged(void)),
	        SLOT(deckingChanged(void)));

	connect(Manager::instance(), SIGNAL(cardOpened(Card *)),
	        SLOT(cardOpenedOrShut(Card *)));

	connect(Manager::instance(), SIGNAL(cardClosed(Card *)),
	        SLOT(cardOpenedOrShut(Card *)));

	connect(Manager::instance(), SIGNAL(cardRenamed(Card *, QString, QString)),
	        SLOT(cardRenamed(Card *, QString, QString)));

	connect(Manager::instance(), SIGNAL(cardMoved(Card *, double, double, int)),
	        SLOT(cardMoved(Card *, double, double, int)));

	_cardLV = new KListView(this, "tree-listview");

	_cardLV->addColumn("Cards"); 					//0
	_cardLV->addColumn("Pos (actual)");		//1
	_cardLV->addColumn("Pos (desired)");	//2
	_cardLV->addColumn("ScreenEdge");			//3
	_cardLV->addColumn("Open");						//4
	_cardLV->addColumn("CardId");					//5
	_cardLV->addColumn("Plugin");					//6


	_cardLV->setColumnWidthMode(0, QListView::Maximum);
	_cardLV->setResizeMode(QListView::LastColumn);
	_cardLV->setAllColumnsShowFocus(true);
	top->addWidget(_cardLV, 1);

	QObject::connect(_cardLV, SIGNAL(clicked(QListViewItem *)), this, SLOT(cardClicked(QListViewItem *)));

	QHBoxLayout *positionRow = new QHBoxLayout(top);
	_positionSlider = new QSlider( -1, 201, 5, 0, Qt::Horizontal, this, "pos-slider");
	_positionSlider->setTracking(true);
	positionRow->addWidget(_positionSlider, 2.0);
	connect(_positionSlider, SIGNAL(valueChanged(int)), SLOT(positionSlider(int)));

	_edgeCB = new QComboBox(false, this, "edge-combobox");
	_edgeCB->insertItem("Top");
	_edgeCB->insertItem("Right");
	_edgeCB->insertItem("Bottom");
	_edgeCB->insertItem("Left");
	connect(_edgeCB, SIGNAL(activated(const QString &)), SLOT(edgeCB(const QString &)));
	positionRow->addWidget(_edgeCB);


	QHBoxLayout *buttonRow = new QHBoxLayout(top);
	buttonRow->setSpacing(4);

	_killPB = new QPushButton("Delete", this);
	QPushButton *newButton = new QPushButton("New...", this);
	buttonRow->insertStretch(0, 10.0);
	buttonRow->addWidget(_killPB);
	buttonRow->addWidget(newButton);

	QObject::connect(_killPB, SIGNAL(clicked()), this, SLOT(killCard()));
	QObject::connect(newButton, SIGNAL(clicked()), Manager::instance(), SLOT(newCardDialog()));

	_killPB->setEnabled(false);
	_positionSlider->setEnabled(false);
	_edgeCB->setEnabled(false);

	deckingChanged();
	resize(sizeHint().width() + 60, 240);
}

TestGui::~TestGui()
{}

void TestGui::cardRegistered(QString, Card *)
{
	rebuildTreeView();
}

void TestGui::cardUnregistered(QString, Card *)
{
	rebuildTreeView();
}

void TestGui::deckingChanged(void)
{
	rebuildTreeView();
}

void TestGui::killCard( void )
{
	Card *moribund = Manager::instance()->card(_currentCard);

	if (moribund)
	{
		Manager::instance()->destroyCard(moribund);
	}

	_killPB->setEnabled(false);
	_positionSlider->setEnabled(false);
	_edgeCB->setEnabled(false);
}

void TestGui::cardSelected(const QString &n)
{
	_currentCard = n;
	_killPB->setEnabled(true);
	_positionSlider->setEnabled(true);
	_edgeCB->setEnabled(true);

	if (Card *card = Manager::instance()->card(_currentCard))
	{
		double position = card->position();
		ScreenEdge e = card->edge();

		_positionSlider->blockSignals(true);
		_positionSlider->setValue((int) ((double)position * 200.0));
		_positionSlider->blockSignals(false);

		_edgeCB->blockSignals(true);
		switch (e)
		{
			case TopEdge: _edgeCB->setCurrentItem(0); break;
			case RightEdge: _edgeCB->setCurrentItem(1); break;
			case BottomEdge: _edgeCB->setCurrentItem(2); break;
			case LeftEdge: _edgeCB->setCurrentItem(3); break;
			default: break;
		}
		_edgeCB->blockSignals(false);

	}
}

void TestGui::cardClicked(QListViewItem *cardItem)
{
	if (!cardItem) return;
	cardSelected(cardItem->text(0));
}

void TestGui::positionSlider( int value )
{
	_sliderActive = true;
	Card *card = Manager::instance()->card(_currentCard);
	if (card)
	{
		double pos = (double)value / 200.0;
		card->setPosition(pos, card->edge());
	}
	_sliderActive = false;
}

void TestGui::edgeCB( const QString &e )
{
	Card *card = Manager::instance()->card(_currentCard);
	if (card)
	{
		card->setPosition(card->position(), StringToEdge(e));
	}
}

void TestGui::closeEvent(QCloseEvent *e)
{
	kapp->quit();
}

void TestGui::rebuildTreeView()
{
	KListViewItem *current = 0L;
	_cardLV->clear();
	CardList cards = Manager::instance()->rootCards();
	CardListIterator it(cards);
	Card *card;
	QString buf;

	while ( (card = it.current()) != 0)
	{
		++it;

		KListViewItem *item = new KListViewItem(_cardLV,
		                           card->cardName(), 		//col 0
		                           buf.sprintf("%.2f", card->position()).latin1(),
		                           buf.sprintf("%.2f", card->desiredPosition()).latin1(),
			                         EdgeToString(card->edge()), //col 3
			                         card->isOpen() ? "Open" : "Shut",
											 card->cardId(),
			                         card->pluginInfo()->name());
		item->setExpandable(true);

		if (_currentCard == item->text(0)) current = item;

		if (card->deck()->count() > 1)
		{
			Card *iterator = 0L;
			for (int i = 1; i < card->deck()->count(); i++)
			{
				iterator = (Card *) card->deck()->cardAt(i);

				item->setOpen(true);
				item = new KListViewItem(item,
																 iterator->cardName(), 		//col 0
																 buf.sprintf("%.2f", iterator->position()).latin1(),
																 buf.sprintf("%.2f", iterator->desiredPosition()).latin1(),
																 EdgeToString(iterator->edge()), //col 3
																 iterator->isOpen() ? "Open" : "Shut",
																 iterator->cardId(),
																 iterator->pluginInfo()->name());

				if (_currentCard == item->text(0)) current = item;
			}
		}
	}

	if (current)
	{
		//re-select the current card.
		_cardLV->blockSignals(true);
		_cardLV->setSelected(current, true);
		_cardLV->blockSignals(false);
	}
}

void TestGui::cardMoved(Card *card, double pos, double desiredPos, int e)
{
	if (Card *cCard = Manager::instance()->card(_currentCard))
	{
		if (cCard && !_sliderActive)
		{
			_positionSlider->blockSignals(true);
			_positionSlider->setValue((int)((double)pos * 200.0));
			_positionSlider->blockSignals(false);
		}

		if (cCard)
		{
			_edgeCB->blockSignals(true);
			switch (cCard->edge())
			{
				case TopEdge: _edgeCB->setCurrentItem(0); break;
				case RightEdge: _edgeCB->setCurrentItem(1); break;
				case BottomEdge: _edgeCB->setCurrentItem(2); break;
				case LeftEdge: _edgeCB->setCurrentItem(3); break;
				default: break;
			}
			_edgeCB->blockSignals(false);
		}
	}

	QListViewItem *treeEntry = _cardLV->findItem( card->cardName(), 0);
	if (treeEntry)
	{
		QString buf;
		treeEntry->setText(1, buf.sprintf("%.2f", pos).latin1());
		treeEntry->setText(2, buf.sprintf("%.2f", desiredPos).latin1());
		treeEntry->setText(3, EdgeToString((ScreenEdge)e));
	}

}

void TestGui::cardOpenedOrShut(Card *card)
{
	if (!card) return;
	QListViewItem *treeEntry = _cardLV->findItem( card->cardName(), 0);
	if (treeEntry)
	{
		treeEntry->setText(4, card->isOpen() ? "Open" : "Shut");
	}
}

void TestGui::cardRenamed( Card *card, QString oldName, QString newName )
{
	if (!card) return;

	//can't perform a lookup in tree of empty name
	if (newName == "") newName = "Unnamed";
	if (oldName == "") oldName = "Unnamed";

	QListViewItem *treeEntry = _cardLV->findItem( oldName, 0);
	if (treeEntry)
	{
		treeEntry->setText(0, newName);
		treeEntry->setText(5, card->cardId());
	}
}


