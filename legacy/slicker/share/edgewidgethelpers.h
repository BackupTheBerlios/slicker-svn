
#ifndef _EDGEWIDGETHELPERS_H_
#define _EDGEWIDGETHELPERS_H_

#include "edgewidget.h"

/**
 * This transformhelper-class are intended to easen the job of relocating components on EdgeChange.
 * Hopefully, it can be of use to anyone.
 */
class EdgeWidgetTransformHelper
{
public:
	enum TransformMode {
		TransformSimple,
		TransformMirror
	};

public:
	EdgeWidgetTransformHelper(const QWidget *parent, ScreenEdge oldEdge, ScreenEdge newEdge, TransformMode mode = TransformSimple);
  virtual ~EdgeWidgetTransformHelper();

public:
	/**
		Transforms a given widget.
	*/
	QWidget &transform( QWidget &widget );

	/**
		Transforms a given rectangle.
	*/
	QRect &transform( QRect &rect );

	/**
		Same as above, but for a point instead
	*/
	QPoint &transform( QPoint &point );

private:
	/**
		Applies a 4 by 3 matrix on a QPoint
	*/
	QPoint &applyMatrix(QPoint &point, char matrix[4][3] );

private:
	const QWidget *_parent;
	ScreenEdge _oldEdge, _newEdge;
	TransformMode _mode;
};

/**
	Returns the screenedge on the opposite side of e.
*/
ScreenEdge oppositeEdge(ScreenEdge e);

#endif
