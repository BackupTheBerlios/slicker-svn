#include <qapplication.h>
#include <qcursor.h>
#include <qpoint.h>
#include <kwin.h>

#include "edgewidget.h"
#include "edgewidgetmanager.h"

QWidget	*EdgeWidget::_screen = 0;


EdgeWidget::EdgeWidget(QWidget * parent, const char * name, WFlags f)
  : QWidget(parent, name, f)
{
	_hidden = false;
	_mayHide = true;
	_screen = QApplication::desktop()->screen();

	_posDragging = 0;
	_sizeDragging = 0;
	_resizeDirection = 0;

	_currentOffset = _offset = 0;

	_edge = TopEdge;
	_edgeAdjust = true;
	setPosition( 0, SameEdge );

	KWin::setOnAllDesktops(winId(), true);
	KWin::setState(winId(), NET::StaysOnTop | NET::SkipTaskbar | NET::SkipPager);

	EdgeWidgetManager::instance()->registerWidget(this);

	QWidget::show();
}

EdgeWidget::~EdgeWidget()
{
	EdgeWidgetManager::instance()->unregisterWidget(this);
}

/**
  This method is EXTREMELY important.
  Virtually every geometry-editing-function passes through here.
*/
bool EdgeWidget::setPosition(int location, ScreenEdge edge, const QSize &newSize)
{
	// Store some important variables to be able to restore them if things don't go well.
	QRect oldDg = _desiredGeometry;
	ScreenEdge oldEdge = _edge;

	// This really is not the best way to solve the problem with posdragging, but for now it will suffice.
	QPoint oldPosDragging;
	if (_posDragging)
		oldPosDragging = *_posDragging;

	// Set the size of desiredGeometry to do the coming calculations on the new desired size (location and such)
	_desiredGeometry.setSize(newSize);

	// Make sure we only deal with "real" edges, we don't want to have to check for SameEdge everywhere.
	if (edge == SameEdge)
		edge = oldEdge;

	// We only tranpose for now, we don't emit edgeChange-signals to trigger reorienting widgets,
	// in case things don't work out with the new geometry.
	if ((edge != oldEdge) && _edgeAdjust)
			adjustOrientation(_desiredGeometry, edge);

	// Do all coming calculations based on the new edge
	_edge = edge;

	// Check we're not moving out of screen
	int minLocation = 0;
	int maxLocation = (orientation(edge) == Horizontal)?
			_screen->width() - _desiredGeometry.width():
			_screen->height() - _desiredGeometry.height();
	if (location < minLocation)
		location = minLocation;
	if (location > maxLocation)
		location = maxLocation;

	// Now, we should have figured the final size, edge and location, just make it so.
	switch (edge)
	{
		case TopEdge:
			_desiredGeometry.moveLeft(location);
			_desiredGeometry.moveTop(-_currentOffset);
			break;
		case BottomEdge:
			_desiredGeometry.moveLeft(location);
			_desiredGeometry.moveBottom(_screen->height()+_currentOffset);
			break;
		case LeftEdge:
			_desiredGeometry.moveLeft(-_currentOffset);
			_desiredGeometry.moveTop(location);
			break;
		case RightEdge:
			_desiredGeometry.moveRight(_screen->width()+_currentOffset);
			_desiredGeometry.moveTop(location);
			break;
		default:
			_desiredGeometry.moveLeft(0);
			_desiredGeometry.moveTop(0);
	}

	// Check for collsion, this is a recursive call.
	if (EdgeWidgetManager::instance()->checkCollision(*this))
	{
		// If things did not work out, restore the desired geometry and edge to the way they were before.
		_desiredGeometry = oldDg;
		_edge = oldEdge;

		if (_posDragging)
			*_posDragging = oldPosDragging;

		// False indicates the new geometry had unresolved collisions.
		return false;
	}
	else
	{
		// Apply the new geometry
		QWidget::setGeometry(_desiredGeometry);

		// Ok, new geometry was OK, emit the edgechange signal and give the widget a chance to adjust.
		if (oldEdge != edge)
			emit edgeChanged(oldEdge, edge);

		if (pos() != _desiredGeometry.topLeft())
		{
			if (!_posDragging)
				emit positionChanged();
			emit positionChanging();
		}

		if (size() != _desiredGeometry.size())
		{
			if (!_sizeDragging)
				emit sizeChanged();
			emit sizeChanging();
		}

		// True indicates there were no unresolved problems with the new geometry.
		return true;
	}
}

bool EdgeWidget::setPosition(int location, ScreenEdge edge)
{
	return setPosition(location, edge, _desiredGeometry.size());
}

bool EdgeWidget::setPosition(const QRect &newGeometry, bool reattach)
{
	// Figure for what point we are to search for closest edge
	const QPoint &hotSpot = (_posDragging)?(newGeometry.topLeft() + *_posDragging):newGeometry.center();
	int location;

	// Only try to find new edge if the function desires it. Otherwise, keep attachment to old edge.
	ScreenEdge edge = reattach?closestEdge(hotSpot):SameEdge;

	// Deal with SameEdge (should only happen if the hotspot is not particurally close to any edge.
	if (edge == SameEdge)
		edge = _edge;

	// Ok, we now know what Edge we should try to connect to, figure the location.
	switch (edge)
	{
		case TopEdge:
		case BottomEdge:
			location = newGeometry.x();
			break;
		case LeftEdge:
		case RightEdge:
			location = newGeometry.y();
			break;
		default:
			location = 0;
	}

	// And do the actual relocation.
	return setPosition(location, edge, newGeometry.size());
}

void EdgeWidget::move(const QPoint &position)
{
	QRect newGeometry = geometry();
	newGeometry.moveTopLeft(position);
	setPosition(newGeometry, true);
}

void EdgeWidget::move(int newX, int newY)
{
  move(QPoint(newX, newY));
}

int EdgeWidget::location() const
{
	return findLocation(pos());
}

int EdgeWidget::findLocation(const QPoint &p) const
{
	return isHorizontal()?p.x():p.y();
}

ScreenEdge EdgeWidget::edge() const
{
	return _edge;
}

void EdgeWidget::show()
{
	_hidden = false;

	_currentOffset = _offset;
	setPosition(location());
}

void EdgeWidget::hide()
{
	if (_mayHide)
	{
		_hidden = true;
		_currentOffset = thickness();
		setPosition(location());
	}
}

bool EdgeWidget::isHidden() const
{
	return _hidden;
}

void EdgeWidget::transpose(QRect &geometry)
{
	// As mentioned before, this is prolly not the best way to deal with posdragging, but for now it will suffice.
	if (_posDragging)
	{
		int temp = _posDragging->x();
		_posDragging->setX(_posDragging->y());
		_posDragging->setY(temp);
	}

	QSize newSize = geometry.size();
	newSize.transpose();
	geometry.setSize(newSize);
}

void EdgeWidget::adjustOrientation(QRect &geometry, ScreenEdge newEdge)
{
	if (orientation(_edge) != orientation(newEdge))
		transpose(geometry);
}

void EdgeWidget::resize(int width, int height)
{
	resize(QSize(width, height));
}

void EdgeWidget::resize(const QSize &size)
{
	setPosition(location(), SameEdge, size);
}

void EdgeWidget::setGeometry(const QRect &newGeometry)
{
	setPosition(newGeometry, false);
}

int EdgeWidget::thickness() const
{
	return isHorizontal()?height():width();
}

int EdgeWidget::length() const
{
	return isHorizontal()?width():height();
}

void EdgeWidget::setEdgeAdjust ( bool value )
{
  _edgeAdjust = value;
}

Qt::Orientation EdgeWidget::orientation() const
{
	return orientation(_edge);
}

Qt::Orientation EdgeWidget::orientation(ScreenEdge e)
{
	switch (e)
	{
		case TopEdge:
		case BottomEdge:
			return Horizontal;
		case LeftEdge:
		case RightEdge:
		default:									// Shouldn't happen, but keep the compiler happy
			return Vertical;
	}
}

// There are more effective ways to do this, but this is simple
bool EdgeWidget::isHorizontal() const
{
  return orientation(_edge) == Horizontal;
}

bool EdgeWidget::isVertical() const
{
  return orientation(_edge) == Vertical;
}

void EdgeWidget::mouseMoveEvent ( QMouseEvent * e )
{
	const QPoint &mousePos = e->globalPos();

	if (_posDragging)
		move(mousePos - *_posDragging);

	if (_sizeDragging)
		doSizeDrag(mousePos);

	e->accept();
}

void EdgeWidget::doSizeDrag(const QPoint &mousePos)
{
		QRect newGeometry = geometry();

		QPoint mouseMovement = mousePos - *_sizeDragging;

		if (_resizeDirection & TopEdge)
			newGeometry.setTop(newGeometry.bottom() - _initialSize.height() + mouseMovement.y());
		if (_resizeDirection & BottomEdge)
			newGeometry.setBottom(newGeometry.top() + _initialSize.height() + mouseMovement.y());
		if (_resizeDirection & LeftEdge)
			newGeometry.setLeft(newGeometry.right() - _initialSize.width() + mouseMovement.x());
		if (_resizeDirection & RightEdge)
			newGeometry.setRight(newGeometry.left() + _initialSize.width() + mouseMovement.x());

		setPosition(newGeometry, false);
}

ScreenEdge EdgeWidget::closestEdge(const QPoint &p)
{
  // Setup which array item corresponds to which edge.
  ScreenEdge edges[] = { TopEdge, RightEdge, LeftEdge, BottomEdge };
  // Calculate the distances to each edge
  int distances[] = { p.y(), _screen->width() - p.x(), p.x(), _screen->height() - p.y() };

	ScreenEdge result = SameEdge;
	int oldMin = 32000;

  // Compare the found distances to find the closest edge.
	for (int i = 0; i < 4; i++)
	{
		if (distances[i] < oldMin)
		{
			result = edges[i];
			oldMin = distances[i];
		}
	}

  // If the distance is over 200, we attach to the same edge again.
  if (oldMin > 200)
		result = SameEdge;

	return result;
}

void EdgeWidget::setHideable(bool value)
{
	_mayHide = value;
}

bool EdgeWidget::hideable() const
{
	return _mayHide;
}

void EdgeWidget::beginPosDrag(const QPoint &pos)
{
	_posDragging = new QPoint(pos);
	setMouseTracking( true );
}

void EdgeWidget::endPosDrag()
{
	delete _posDragging;
	_posDragging = 0;
	setMouseTracking( false );
	emit positionChanged();
}

void EdgeWidget::beginSizeDrag(const QPoint &pos, int anchor)
{
	_sizeDragging = new QPoint(pos + geometry().topLeft());
	_resizeDirection = anchor;
	_initialSize = geometry().size();
	setMouseTracking( true );
}

void EdgeWidget::endSizeDrag()
{
	delete _sizeDragging;
	_sizeDragging = 0;
	setMouseTracking( false );
	emit sizeChanged();
}
