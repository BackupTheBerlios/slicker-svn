/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#include <qapplication.h>
#include <qcursor.h>
#include <qpoint.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kwin.h>

#include "edgewidgetmanager.h"

EdgeWidgetManager *EdgeWidgetManager::_self = NULL;

EdgeWidgetManager::EdgeWidgetManager()
{
    // Yeah, we like offensive thinking.
    _policy = AttackerIsRight;

	_mousePoller = new QTimer(this);
	_lastTouchedEdge = (EdgeWidget::ScreenEdge)0;
	connect(_mousePoller, SIGNAL(timeout()), this, SLOT(pollMouse()));
	_mousePoller->start(100);
}

EdgeWidgetManager::~EdgeWidgetManager()
{
	_mousePoller->stop();
}

// Ordinary singleton pattern implementation.
EdgeWidgetManager *EdgeWidgetManager::self()
{
    if (!_self)
        _self = new EdgeWidgetManager();

    return _self;
}

void EdgeWidgetManager::registerWidget(EdgeWidget *widget)
{
    _widgets.append(widget);
    emit widgetRegistered(widget);
}

void EdgeWidgetManager::deregisterWidget(EdgeWidget *widget)
{
    _widgets.remove(widget);
    emit widgetDeregistered(widget);
}

void EdgeWidgetManager::pollMouse()
{
    EdgeWidget::ScreenEdge edge = (EdgeWidget::ScreenEdge)0;
	EdgeWidget::ScreenEdge filteredEdge;
	
	QPoint mousePos = QCursor::pos();
	QRect screen = Slicker::desktop()->screenGeometry();
	
	if (mousePos.x() == 0)
		edge = (EdgeWidget::ScreenEdge)(edge | EdgeWidget::LeftEdge);
	else if (mousePos.x() == screen.width())
		edge = (EdgeWidget::ScreenEdge)(edge | EdgeWidget::RightEdge);
	else
		_lastTouchedEdge = (EdgeWidget::ScreenEdge)(_lastTouchedEdge & (~(EdgeWidget::LeftEdge | EdgeWidget::RightEdge)));
	
	if (mousePos.y() == 0)
		edge = (EdgeWidget::ScreenEdge)(edge | EdgeWidget::TopEdge);
	else if (mousePos.y() == screen.height())
		edge = (EdgeWidget::ScreenEdge)(edge | EdgeWidget::BottomEdge);
	else
		_lastTouchedEdge = (EdgeWidget::ScreenEdge)(_lastTouchedEdge & (~(EdgeWidget::TopEdge | EdgeWidget::BottomEdge)));

	filteredEdge = (EdgeWidget::ScreenEdge)(edge & (~_lastTouchedEdge));
	
	if (filteredEdge)
	{
		_lastTouchedEdge = edge;
		QPtrListIterator<EdgeWidget> iterator(_widgets);
		EdgeWidget * candidate;
		
    	for (candidate = iterator.toFirst(); candidate; candidate = (++iterator))
		{
			if (candidate->edge() & filteredEdge)
				KWin::forceActiveWindow(candidate->winId());
		}
	}
}

bool EdgeWidgetManager::checkCollision(EdgeWidget &widget)
{
    // Get the geom once and for all (as long as we stay in this function, at least)
    QRect myGeom = widget._desiredGeometry;

    // We are recursive, we have to use a QPtrListIterator
    QPtrListIterator<EdgeWidget> iterator(_widgets);

    for (EdgeWidget *victim = iterator.toFirst(); victim; victim = (++iterator)) {
        // We are schizofrenic and would always attack ourselves, if allowed.
        if (&widget == victim)
            continue;

        // Figure the intersection.
        QRect const &intersection = myGeom.intersect(victim->_desiredGeometry);

        // If there were no intersection, we're clear.
        if (intersection.isEmpty())
            continue;

        // Ok, if we get here, there is a collision between the two current tested widgets, try to deal with it
        bool handled = false;

        // See if anyone else wants to deal with the collision for us. =)
        emit collision(widget, *victim, handled);
        if (handled)
            continue;

        if (!dealWithCollision(widget, *victim, intersection))
            // Damn, the collision could not be dealt with, return true, saying there were an unhandled collision.
            return true;
    }

    // We got so far, meaning everything is fine and we can return false, meaning nothing happened that could
    // not be dealt with.
    return false;
}

void EdgeWidgetManager::setPolicy(ConflictPolicy p)
{
    _policy = p;
}

EdgeWidgetManager::ConflictPolicy EdgeWidgetManager::policy() const
{
    return _policy;
}

bool EdgeWidgetManager::dealWithCollision(EdgeWidget &attacker, EdgeWidget &defender, const QRect &intersection)
{
    // Ok, in two situations we can say things are not nice at once, the first is when the policy is to simply
    // deny all changes that would lead to any collision. The second situation is when we have already tried to
    // move this widget once.
    if ((_policy == DenyMovement) || _checkedWidgets.contains(&attacker))
        return false;
    else {
        // add this widget to the list of already checked widgets.
        _checkedWidgets.append(&attacker);

        // Find our victim
        EdgeWidget &victim = (_policy == AttackerIsRight)?defender:attacker;

        // New offset is found by simply trying to find the deltaOffset that would cause the least movement.
        // (we never move widgets off their edge in order to do collision-handling)
        int newLocation = victim.findLocation(victim._desiredGeometry.topLeft()) + getAdjustOffset(victim, intersection);

        // Remember the result of trying to move the victim.
        bool retVal = victim.setPosition(newLocation, EdgeWidget::SameEdge);

        // remove us from the stack again
        _checkedWidgets.removeLast();

        // and return the result of this whole thing.
        return retVal;
    }
}

int EdgeWidgetManager::getAdjustOffset(const EdgeWidget &widget, const QRect &intersection)
{
    const QPoint &wc = widget._desiredGeometry.center();
    const QPoint &ic = intersection.center();

    if (widget.isHorizontal()) {
        if (wc.x() > ic.x()) // widget is to the right of intersection
            return intersection.width();
        else
            return -intersection.width();
    } else {
        if (wc.y() > ic.y()) // widget is located below the intersection
            return intersection.height();
        else
            return -intersection.height();
    }
}

#include "edgewidgetmanager.moc"


