/***************************************************************************
                          TestCardApplet.cpp  -  description
                             -------------------
    begin                : Sun Jan 5 2003
    copyright            : (C) 2003 by Shamyl Zakariya
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

#include "TestCardApplet.h"
#include <carddeskcore/Card.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>

namespace CardDesk {

TestCardApplet::TestCardApplet(const QString &configFile)
: CardApplet(configFile), _contentsWidget(NULL),
	_showIcon(false), _showMiniDisplay(false), _showResizeGrip(true)
{
	kdDebug() << "TestCardApplet::TestCardApplet(configFile)" << endl;
	_name = "NoNameYet";
	KIconLoader il;
	_icon = il.loadIcon("folder", KIcon::Desktop, 0, KIcon::DefaultState);
	config()->setGroup("General");
	_name = config()->readEntry("TabName", "I Have No Name");
	_showMiniDisplay = config()->readBoolEntry("MiniDisplay", _showMiniDisplay);
	_showResizeGrip = config()->readBoolEntry("Resizeable", _showResizeGrip);
	_showIcon = config()->readBoolEntry("Icon", _showIcon);
}

TestCardApplet::~TestCardApplet()
{
	kdDebug() << "TestCardApplet::~TestCardApplet()" << endl;
	config()->writeEntry("TabName", _name);
	config()->writeEntry("MiniDisplay", _showMiniDisplay);
	config()->writeEntry("Resizeable", _showResizeGrip);
	config()->writeEntry("Icon", _showIcon);
	config()->sync();
}

void TestCardApplet::setCardName(QString newName)
{
	_name = newName;
	CardApplet::setCardName();
}

QPixmap *TestCardApplet::tabIcon( void )
{
	return _showIcon ? &_icon : NULL;
}

QSize TestCardApplet::miniDisplaySize( void )
{
	return _showMiniDisplay ? (QSize( 40, 18 )) : QSize(0, 0);
}

void TestCardApplet::paintMiniDisplay(QPainter *paint, int width, int height)
{

	paint->setPen(_tab->darkColor());
	paint->setBrush(_tab->backgroundBrush());
	paint->drawRect(0, 0, width, height);

	paint->setPen(_tab->lightColor());
	paint->drawLine(1, height - 2, width - 2, height - 2);
	paint->drawLine(width - 2, 2, width - 2, height - 2);

	paint->setPen(_tab->mediumDarkColor());
	paint->drawLine(1, 1, width - 2, 1);
	paint->drawLine(1, 1, 1, height - 2);

	paint->setPen(Qt::black);
	paint->drawLine(width / 2, 4, width / 2, height - 4);
	paint->drawLine(width / 2, 4, width / 2 - 4, 8);
	paint->drawLine(width / 2, 4, width / 2 + 4, 8);

}


void TestCardApplet::addToDefaultContextMenu( QPopupMenu *defaultMenu )
{
	defaultMenu->insertItem("Test Item &One", this, SLOT(menuOne()));
	defaultMenu->insertItem("Test Item &Two", this, SLOT(menuTwo()));
}

void TestCardApplet::aboutToShowContextMenu( class QPopupMenu * )
{
	printf("TestCardApplet::aboutToShowContextMenu()\n");
}

bool TestCardApplet::allowDragHoverToOpen( void )
{
	return true;
}

bool TestCardApplet::allowHoverToOpen( void )
{
	return true;
}

bool TestCardApplet::allowDropOnTab( void )
{
	return true;
}

bool TestCardApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void TestCardApplet::tabDragDropEvent( QDropEvent *e )
{
	_dtArea->dropEvent(e);
}

QString TestCardApplet::cardName( void )
{
	kdDebug() << "TestCardApplet::cardName" << endl;
	return _name;
}

Tray *TestCardApplet::createTray( Card * )
{
	return NULL;
}

Separator *TestCardApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

Contents *TestCardApplet::createContents( Card *c )
{
	Contents *contents = new Contents(c);

	_contentsWidget = new QWidget(0, "TestApplet-widget");

	QVBoxLayout *layout = new QVBoxLayout(_contentsWidget);
	layout->setSpacing(4);
	layout->setMargin(4);

	QCheckBox *showIconCB = new QCheckBox("Icon", _contentsWidget, "showIconCB");
	layout->addWidget(showIconCB);

	QCheckBox *showMiniDisplayCB = new QCheckBox("Mini Display", _contentsWidget, "showMiniDisplayCB");
	layout->addWidget(showMiniDisplayCB);

	QCheckBox *showResizerCB = new QCheckBox("Resizeable?", _contentsWidget, "resizeableCB");
	layout->addWidget(showResizerCB);

	QHBoxLayout *nameLayout = new QHBoxLayout(layout);
	QLabel *nameLabel = new QLabel("Name", _contentsWidget);
	nameLayout->addWidget(nameLabel);
	_nameLE = new QLineEdit(_contentsWidget, "name-lineedit");
	_nameLE->setText(_name);
	nameLayout->addWidget(_nameLE, 2.0);

	_dtArea = new DropTestArea(_contentsWidget, "drop-test-area");
	layout->addWidget(_dtArea, 10.0);
	QObject::connect(_dtArea, SIGNAL(dropProcessed()), card()->getTab(), SLOT(dropProcessed()));

	showResizerCB->setChecked(_showResizeGrip);
	showIconCB->setChecked(_showIcon);
	showMiniDisplayCB->setChecked(_showMiniDisplay);

	connect(showResizerCB, SIGNAL(toggled(bool)), SLOT(toggleResizeGrip(bool)));
	connect(showIconCB, SIGNAL(toggled(bool)), SLOT(toggleIcon(bool)));
	connect(showMiniDisplayCB, SIGNAL(toggled(bool)), SLOT(toggleMiniDisplay(bool)));
	connect(_nameLE, SIGNAL(returnPressed()), SLOT(nameChanged( void )));

	contents->setContents(_contentsWidget);
	contents->setPreferredHeight(200);

	return contents;
}

void TestCardApplet::saveSettings( KConfig *config )
{
	config->writeEntry("TabName", _name);
	config->writeEntry("MiniDisplay", _showMiniDisplay);
	config->writeEntry("Resizeable", _showResizeGrip);
	config->writeEntry("Icon", _showIcon);
}

void TestCardApplet::loadSettings( KConfig *config )
{
	_name = config->readEntry("TabName", "I Have No Name");
	_showMiniDisplay = config->readBoolEntry("MiniDisplay", _showMiniDisplay);
	_showResizeGrip = config->readBoolEntry("Resizeable", _showResizeGrip);
	_showIcon = config->readBoolEntry("Icon", _showIcon);
}

void TestCardApplet::cardReady( void )
{
	CardApplet::cardReady();
	toggleResizeGrip(_showResizeGrip);
	emit propertiesChanged();	
}

void TestCardApplet::cardOpened( void )
{
   kdDebug() << "TestCardApplet::cardOpened()" << endl;
}

void TestCardApplet::cardShut( void )
{
	kdDebug() << "TestCardApplet::cardShut()" << endl;
}

void TestCardApplet::menuOne( void )
{
	printf("TestCardApplet::menuOne()\n");
}

void TestCardApplet::menuTwo( void )
{
	printf("TestCardApplet::menuTwo()\n");
}

void TestCardApplet::nameChanged( void )
{
	printf("TestCardApplet::nameChanged()\n");
	setCardName(_nameLE->text());
}

void TestCardApplet::toggleResizeGrip( bool yesno )
{
	_showResizeGrip = yesno;
	card()->getSeparator()->setGripper(yesno);
}

void TestCardApplet::toggleMiniDisplay(bool yesno)
{
	_showMiniDisplay = yesno;
	emit propertiesChanged();
}

void TestCardApplet::toggleIcon( bool yesno )
{
	_showIcon = yesno;
	emit propertiesChanged();
}

/////////////////////////////////////////////////////////////////////////////
// DragTestArea

DropTestArea::DropTestArea(QWidget *parent, const char *name)
	: QLabel("Drop Stuff!", parent, name)
{
	setFrameStyle(StyledPanel | Sunken);
	setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
	setAcceptDrops(true);
}

void DropTestArea::dragEnterEvent( QDragEnterEvent *e )
{
//	printf("DropTestArea::dragEnterEvent()\n");
	if (QUriDrag::canDecode(e))
		e->accept();
	else
		e->ignore();
}

void DropTestArea::dragMoveEvent( QDragMoveEvent *e )
{
//	printf("DropTestArea::dragMoveEvent()\n");
	if (QUriDrag::canDecode(e))
		e->accept();
	else
		e->ignore();
}

void DropTestArea::dragLeaveEvent( QDragLeaveEvent * )
{}

void DropTestArea::dropEvent( QDropEvent *e )
{
	QStringList files;
	if ( QUriDrag::decodeLocalFiles( e, files ) )
	{
		setText(files.join("\n"));
		emit dropProcessed();
	}
}

}
