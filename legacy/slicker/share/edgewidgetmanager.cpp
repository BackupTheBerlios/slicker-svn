#include <qapplication.h>
#include <qcursor.h>
#include <qpoint.h>
#include <kwin.h>
#include <kdebug.h>

#include "edgewidgetmanager.h"

EdgeWidgetManager *EdgeWidgetManager::_instance;

EdgeWidgetManager::EdgeWidgetManager()
{
	// Yeah, we like offensive thinking.
	_policy = AttackerIsRight;
}

EdgeWidgetManager::~EdgeWidgetManager()
{
}


// Ordinary singleton pattern implementation.
EdgeWidgetManager *EdgeWidgetManager::instance()
{
	if (!_instance)
		_instance = new EdgeWidgetManager();

	return _instance;
}

void EdgeWidgetManager::registerWidget(EdgeWidget *widget)
{
	_widgets.append(widget);
	emit widgetRegistered(widget);
}

void EdgeWidgetManager::unregisterWidget(EdgeWidget *widget)
{
	_widgets.remove(widget);
	emit widgetUnregistered(widget);
}

bool EdgeWidgetManager::checkCollision(EdgeWidget &widget)
{
  // Get the geom once and for all (as long as we stay in this function, at least)
	QRect myGeom = widget._desiredGeometry;

	// We are recursive, we have to use a QPtrListIterator
	QPtrListIterator<EdgeWidget> iterator(_widgets);

  for (EdgeWidget *victim = iterator.toFirst(); victim; victim = (++iterator))
  {
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
	else
	{
		// add this widget to the list of already checked widgets.
		_checkedWidgets.append(&attacker);

		// Find our victim
		EdgeWidget &victim = (_policy == AttackerIsRight)?defender:attacker;

		// New offset is found by simply trying to find the deltaOffset that would cause the least movement.
		// (we never move widgets off their edge in order to do collision-handling)
		int newLocation = victim.findLocation(victim._desiredGeometry.topLeft()) + getAdjustOffset(victim, intersection);

		// Remember the result of trying to move the victim.
		bool retVal = victim.setPosition(newLocation, SameEdge);

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

	if (widget.isHorizontal())
	{
		if (wc.x() > ic.x()) // widget is to the right of intersection
			return intersection.width();
		else
			return -intersection.width();
	}
	else
	{
		if (wc.y() > ic.y()) // widget is located below the intersection
			return intersection.height();
		else
			return -intersection.height();
	}
}

