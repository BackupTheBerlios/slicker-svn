
#ifndef _EDGEWIDGETHELPERS_H_
#define _EDGEWIDGETHELPERS_H_

#include "edgewidget.h"
#include <qframe.h>
#include <qlayout.h>

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
    EdgeWidgetTransformHelper(const QWidget *parent, EdgeWidget::ScreenEdge oldEdge, EdgeWidget::ScreenEdge newEdge, TransformMode mode = TransformSimple);
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
    	Transforms a given size.
    */
    QSize &transform( QSize &size );

    /**
    	Same as above, but for a point instead
    */
    QPoint &transform( QPoint &point );

private:
    /**
    	Applies a 4 by 3 matrix on a QPoint
    */
    QPoint &applyMatrix(QPoint &point);

private:
    const QWidget *_parent;
    EdgeWidget::ScreenEdge _oldEdge, _newEdge;
    char _matrix[4][3];
};

/**
	Returns the screenedge on the opposite side of e.
*/
EdgeWidget::ScreenEdge oppositeEdge(EdgeWidget::ScreenEdge e);

/** A dynamic QBoxLayout that follows the edge of the specified EdgeWidget.
If you need to fill out your EdgeWidget with a bunch of widgets that should be
reorganized (alter type of layout) when the EdgeWidget changes edge, you can use
this class to provide semi-automatic layouting.
All you have to do to make it work, is to call invalidate() as soon as the edge
changes.
*/
class EdgeWidgetBoxLayout : public QBoxLayout
{
    Q_OBJECT
public:
    /**
     * The orientation relative to the orientation of the EdgeWidget.
     */
    enum Orientation { Colinear, Perpendicular };

    EdgeWidgetBoxLayout( QWidget* parent, EdgeWidget *edgeWidget, Orientation orientation=Colinear, const char* name=0 );

    virtual void invalidate();

protected:
    QBoxLayout::Direction realDirection() const;

private:
    Orientation _orientation;
    EdgeWidget *_edgeWidget;
};

/** A dynamic QHBox/QVBox that follows the edge of the specified EdgeWidget.
If you need to fill out your EdgeWidget with a bunch of widgets that should be
reorganized (alter type of layout) when the EdgeWidget changes edge, you can use
this class to provide semi-automatic layouting.
All you have to do to make it work, is to call relayout() as soon as the edge
changes.
*/
class EdgeWidgetLayoutBox : public QFrame
{
    Q_OBJECT
public:
    EdgeWidgetLayoutBox( QWidget* parent, EdgeWidget *edgeWidget, EdgeWidgetBoxLayout::Orientation orientation=EdgeWidgetBoxLayout::Colinear, const char* name=0 );

    void relayout();
private:
    EdgeWidgetBoxLayout *_layout;
};

#endif
