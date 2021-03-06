/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef _EDGEWIDGETMANAGER_H_
#define _EDGEWIDGETMANAGER_H_

#include <qwidget.h>
#include <qtimer.h>
#include <qptrlist.h>
#include <qnamespace.h>

#include "edgewidget.h"
#include "slicker.h"

/**
 * EdgeWidgetManager deals with several things.
 * First of all, it simply keeps a list of all running EdgeWidgets.
 * Second of all, it uses this information to do the collision-handling.
 * Third, it uses this info to deal with all other global operation, such as global hide/show etc.
 *
 * @author Ulrik Mikaelsson
 **/
class EdgeWidgetManager : public QObject
{
    Q_OBJECT
    friend class EdgeWidget;
public:
    EdgeWidgetManager();
    virtual ~EdgeWidgetManager();

    /**
    	Get the single running self of EdgeWidgetManager
    */
    static EdgeWidgetManager *self();

    /**
      Instructs the EdgeWidgetManager on how to deal with conflicts.
    */
    enum ConflictPolicy { AttackerIsRight, DefenderIsRight, DenyMovement };

    /**
      Set conflict managment policy
    */
    void setPolicy(ConflictPolicy p);

    /**
      Get the conflict management policy
    */
    ConflictPolicy policy() const;

public slots:
    /**
    	Checks for collisions on this location and tries to resolv it using the
       current conflict resolution policy.
    */
    bool checkCollision(EdgeWidget &widget);

	
	void pollMouse();


signals:
    /**
    	Upon a collision-check, if a collision is detected emit this signal.
    	@attacker The moving widget
    	@defender The passive widget whose area were intruded upon.
    	@handled Whether the collision have already been dealt with. (IN/OUT parameter)
    */
    void collision(EdgeWidget &attacker, EdgeWidget &defender, bool &handled);

    /**
    	Whenever a widget gets created we notify whoever is listening.
    */
    void widgetRegistered(EdgeWidget *widget);

    /**
    	Whenever a widget is unregistered we notify whoever is listening.
    */
    void widgetDeregistered(EdgeWidget *widget);

private:
    void registerWidget(EdgeWidget *widget);
    void deregisterWidget(EdgeWidget *widget);

    /**
    	This method is invoked as soon as an collision is detected and tries to deal
    	with it.
    	@return true if collision could be dealt with, false otherwise.
    */
    bool dealWithCollision(EdgeWidget &attacker, EdgeWidget &defender, const QRect &intersection);

    /**
    	Finds the offset appropriate to move the widget by, to stay clear
    	of the intersection.
    */
    int getAdjustOffset(const EdgeWidget &e, const QRect &intersection);

private:
    static EdgeWidgetManager *_self;
    ConflictPolicy _policy;

    QPtrList<EdgeWidget> _widgets;
    QPtrList<EdgeWidget> _checkedWidgets;
	
	QTimer * _mousePoller;
	EdgeWidget::ScreenEdge _lastTouchedEdge;
};

#endif
