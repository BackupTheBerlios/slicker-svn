/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@rsn.bth.se> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
#ifndef CARDSTACK_H
#define CARDSTACK_H

#include "applet.h"
#include "applethost.h"
#include "edgewidget.h"
#include "sessionitem.h"
#include "sessionitemfactory.h"

/**
* The basic CardStack class
* @author Ulrik Mikaelsson
*/
class CardStack : public EdgeWidget, public AppletHost, public SessionItem
{
Q_OBJECT
public:
	CardStack(const QString & id);
	~CardStack();
	
	bool accept(Applet * applet);
	void detach(Applet * applet);
	    
	virtual void restore(KConfigBase * config);
	virtual void store(KConfigBase * config);


protected slots:
	virtual void edgeChanged(EdgeWidget::ScreenEdge oldEdge);
};

/**
* The SessionItemFactory for the CardStack.
* @author Ulrik Mikaelsson <rawler@rsn.bth.se>
*/
class CardStackFactory : public SessionItemFactory
{
public:
	CardStackFactory();
	virtual ~CardStackFactory();
	
	virtual SessionItem * loadItem(KConfigBase * configGroup);
	virtual SessionItem * createItem();
};

#endif
