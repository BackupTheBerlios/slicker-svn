#include "edgewidgethelpers.h"

// ------- Transform helper-matrices -------------
/*
  Transform-matrices explained:
  A transform-matrix describes the geometrical state of an edge,
  according to some certain transform rules.
  The rows are in the order: TopEdge, RightEdge, BottomEdge, LeftEdge
  The first column shows whether this state is vertical
  The second column shows whether this state has a negative X-axis
  The third column shows whether this state has a negative Y-axis

  All this information is used in the applyMatrix, to transform geometry
  on EdgeChanges.
*/
char mirrorMatrix[4][3] = { {0, 0, 0},
														{1, 1, 0},
														{0, 0, 1},
														{1, 0, 0}};

char simpleMatrix[4][3] = { {0, 0, 0},
														{1, 0, 0},
														{0, 0, 0},
														{1, 0, 0}};

enum {Transpose = 0, NegativeX = 1, NegativeY = 2};

char *getRowForEdge(ScreenEdge edge, char matrix[4][3])
{
	switch (edge)
	{
		case TopEdge:
			return matrix[0];
		case RightEdge:
			return matrix[1];
		case BottomEdge:
			return matrix[2];
		case LeftEdge:
			return matrix[3];
		default:
			Q_ASSERT(0);
			return 0;
	}
}
// ------- End transform-helper matrices ----------

EdgeWidgetTransformHelper::EdgeWidgetTransformHelper(const QWidget *parent, ScreenEdge oldEdge, ScreenEdge newEdge, TransformMode mode)
{
	_parent = parent;
	_oldEdge = oldEdge;
	_newEdge = newEdge;
	_mode = mode;
}

EdgeWidgetTransformHelper::~EdgeWidgetTransformHelper()
{
}

QWidget &EdgeWidgetTransformHelper::transform(QWidget &widget)
{
	QRect geom = widget.geometry();
	transform(geom);
	widget.setGeometry(geom);
}

QRect &EdgeWidgetTransformHelper::transform( QRect &rect )
{
	// This function is simple, just find two opposite corners, apply the appropriate matrice to each point,
	// set the new corner coordinates and normalize the rectangle.

	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight();

	transform(topLeft);
	transform(bottomRight);

	rect.setTopLeft(topLeft);
	rect.setBottomRight(bottomRight);

	rect = rect.normalize();

	return rect;
}

QPoint &EdgeWidgetTransformHelper::transform( QPoint &point )
{
	switch (_mode)
	{
		case TransformMirror:
			return applyMatrix(point, mirrorMatrix);
		case TransformSimple:
		default:
			return applyMatrix(point, simpleMatrix);
	}
}

QPoint &EdgeWidgetTransformHelper::applyMatrix(QPoint &point, char matrix[4][3])
{
	// This function is a bit tricky.

	// First we get out all variables that matters
	char *oldState = getRowForEdge(_oldEdge, matrix);
	char *newState = getRowForEdge(_newEdge, matrix);
	int newWidth = _parent->width();
	int newHeight = _parent->height();
	int oldWidth = newWidth;   // We're gonna fix this in a couple of lines.
	int oldHeight = newHeight; // -||-
	int newX = point.x();
	int newY = point.y();

	// Actually, we don't know what the old size were, before we look at the oldState compared to the new state
	// and possibly transpose the size.
	if (oldState[Transpose] != newState[Transpose])
	{
		oldWidth = newHeight;
		oldHeight = newWidth;
	}

	// Adjust X and Y according to the used coordinate-system used in the old state.
	if (oldState[NegativeX])
		newX = oldWidth - newX;
	if (oldState[NegativeY])
		newY = oldHeight - newY;

	// Ok, now if the size were transposed during the change, we definately want to transpose the coordinates as well.
	if (oldState[Transpose] != newState[Transpose])
	{
		int temp;
		temp = newX;
		newX = newY;
		newY = temp;
	}

	// And we transform the coordinates according to the new coordinate-system.
	if (newState[NegativeX])
		newX = newWidth - newX;
	if (newState[NegativeY])
		newY = newHeight - newY;

	// We're done, update the info to the point.
	point.setX(newX);
	point.setY(newY);

	return point;
}

ScreenEdge oppositeEdge(ScreenEdge e)
{
	switch (e)
	{
		case TopEdge:
			return BottomEdge;
		case BottomEdge:
			return TopEdge;
		case LeftEdge:
			return RightEdge;
		case RightEdge:
			return LeftEdge;
		default:
			return SameEdge;
	}
}
