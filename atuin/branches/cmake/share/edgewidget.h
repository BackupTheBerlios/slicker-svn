/***************************************************************************
         Copyright: 2003-2004 Slicker Team (www.slicker.org)
 
  MAINTAINER: Ulrik Mikaelsson <rawler@users.berlios.de> 

 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 ***************************************************************************/

#ifndef _EDGEWIDGET_H_
#define _EDGEWIDGET_H_

#include <qwidget.h>
#include <qtimer.h>
#include <qptrlist.h>
#include <qnamespace.h>

/**
 * EdgeWidget is an extended QWidget, with the purpose of keeping widgets connected to a Screen Edge.
 * Hopefully, this Edge-connectivity will prevent the widget from ever beeing located anywhere, but along the side
 * of the screen.
 *
 * @author Ulrik Mikaelsson
 **/
class EdgeWidget : public QWidget
{
    Q_OBJECT
    friend class EdgeWidgetManager;
    friend class EdgeWidgetTransformHelper;
public:
    /**
      ScreenEdge represents the edge of the screen that a widget is attached to. So
      if widget->edge() returns RightEdge, the widget is attached to the right edge
      of the screen.
    */
    enum ScreenEdge { SameEdge = 1, TopEdge = 2, RightEdge = 4, BottomEdge = 8, LeftEdge = 16 };

public:
    explicit EdgeWidget(QWidget * parent = 0, const char * name = 0, WFlags f = Qt::WStyle_Customize | Qt::WDestructiveClose | Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder | Qt::WX11BypassWM);
    virtual ~EdgeWidget();

    /**
    	Moves widget to @location on @edge.
    	If @edge == Same (default) widget will be positioned on its current edge.
    	@return bool if move is successful, false if unhandled collison.
    */
    bool setPosition(int location, ScreenEdge edge, const QSize &newSize);

    /**
    	Convenice method for the above
    	*/
    bool setPosition(int location, ScreenEdge edge = SameEdge);

    /**
    	SetPosition - convenience method, most of the normal qwidget-geometry stuff are routed through
    	here.
    	@param reattach - Should we try to change edge if appropriate?
    */
    bool setPosition(const QRect &geometry, bool reattach);

    /**
    	Moves widget to @position, attaching it to the closest edge.
    */
    virtual void move(const QPoint &position);
    virtual void move(int x, int y);

    /**
    	Get the widget's location along the corresponding edge.
    	Location is calculated from the top or from the left, depending on the current edge.
    */
    int location() const;

    /**
    	Tells how much of the widget (in pixels) are outside of the screen.
    */
    int offset() const;

    /**
    	Set how much of the widget should be hidden outside screen.
    */
    int setOffset(int offset);

    /**
    	Get the edge the widget is currently attached to.
    */
    ScreenEdge edge() const;

    /**
    	Overridden resize
    */
    virtual void resize(int width, int height);

    /**
    	Overridden resize
    */
    virtual void resize(const QSize& size);

    /**
    	Overridden setGeometry
    	May change edge-alignment, if appropriate.
    */
    virtual void setGeometry(const QRect &geometry);

    /**
    *	Returns the length of the widget perpendicular to the edge it is attached.
    */
    int thickness( void ) const;

    void setThickness ( int value );

    /**
    	Returns the width of the widget along the edge it is attached.
    */
    int length( void ) const;

    void setLength ( int value );

    /**
    	Set the behaviour when changing edge
    */
    void setEdgeAdjust( bool value );

    /**
    	Are we horizontal or vertical?
    */
    Qt::Orientation orientation() const;

    /**
      Are we horizontal
    */
    bool isHorizontal() const;

    /**
      Are we vertical?
    */
    bool isVertical() const;
protected:
    /**
    	Check if we should do some kind of dragging?
    */
    virtual void mouseMoveEvent ( QMouseEvent * e );

public slots:
    /**
    	Set the widget hideability.
    */
    void setHideable( bool hideable );

    /**
    	Get the hideability status of the widget
    */
    bool hideable() const;

    /**
    	Hide the widget, if hideable.
    */
    virtual void hide();

    /**
    	Show the widget, if hideable.
    */
    virtual void show();

    /**
    	Hidden?
    */
    bool isHidden() const;

    /**
    	Start a position drag action
    */
    void beginPosDrag(const QPoint &pos);

    /**
    	End a position drag action
    */
    void endPosDrag();

    /**
    	Start a size drag action
    	@parm direction is where what edges will change, the value is a bitwise or of one or two
    		ScreenEdges. A conflicting combination, such as anchor the Top and Bottom at the same time,
    		will lead to undefined behaviour.
    */
    void beginSizeDrag(const QPoint &pos, int direction);

    /**
    	End a size drag action
    */
    void endSizeDrag();

protected slots:
    /**
    	Executed when the edge of a widget is changed, forcing derived classes to adjust accordingly, such
    	as relayouting their widgets.
    	This function is executed as soon as the edge change, even during a drag
    	operation.
    */
    virtual void edgeChanged(ScreenEdge oldEdge) = 0;

signals:
    /**
    	Emitted when the size of a widget is changed, allowing listeners to adjust accordingly, such
    	as relayout their widgets.
    	This signal is only emitted when the change of size is done. E.g this signal is not emitted
    	while	dragging.
    */
    void sizeChanged();

    /**
    	Emitted when the size of a widget is changed, allowing listeners to adjust accordingly, such
    	as relayout their widgets.
    	This signal is emitted as soon as the size changes, even under dragging.
    */
    void sizeChanging();

    /**
    Emitted when the position of a widget is changed, allowing listeners to adjust accordingly, such
    as relayout their widgets.
    This signal is only emitted when the change of position is done. E.g this signal is not emitted
    while	dragging.
    */
    void positionChanged();

    /**
    	Emitted when the position of a widget is changed, allowing listeners to adjust accordingly, such
    	as relayout their widgets.
    	This signal is emitted as soon as the position changes, even under dragging.
    */
    void positionChanging();

    /**
    	Emitted when the widget's visibility changes to shown from being hidden.
    */
    void shown();

    /**
    	Emitted when the widget's visibility changes to hidden from being shown.
    */
    void hidden();
private:		// private helper methods
    /**
    	Find the closest edge, given a coordinate
    */
    static ScreenEdge closestEdge(const QPoint &p);

    /**
    	Helper method to determine whether an edge is horizontal or vertical.
    */
    static Qt::Orientation orientation(ScreenEdge e);

    /**
    	Figure the offset of a point, according to the current Edge
    */
    int findLocation(const QPoint &p) const;

    /**
    	Do the sizeDragging stuff
    */
    void doSizeDrag(const QPoint &mousePos);
private slots:
    /**
    	Call transpose, if appropriate.
    */
    void adjustOrientation(QRect &geometry, ScreenEdge newEdge);

    /**
    	Do the actual transpose.
    */
    void transpose(QRect &geometry);

private:		// private variables
    /**
    	What edge is the widget currently attached to?
    */
    ScreenEdge _edge;

    /**
    	Desired geometry
    */
    QRect _desiredGeometry;

    /**
    	Should we transpose on EdgeChanges?
    */
    bool _edgeAdjust;

    /**
    	Is the widget currently hidden?
    */
    bool _hidden;

    /**
    	May the widget be hidden?
    */
    bool _mayHide;

    /**
    	Are we dragging/resizing?
    */
    QPoint *_posDragging;
    QPoint *_sizeDragging;

    /**
    	Where are we anchored while sizeDragging?
    */
    int _resizeDirection;

    /**
    	What was the size when we began sizeDragging?
    */
    QSize _initialSize;

    /**
    	The Screen geometry
    */
    static QWidget *_screen;

    /**
    	The part of the Widget hidden outSide of screen.
    */
    int _offset;

    /**
    	The part of the Widget hidden outside of screen right now
    */
    int _currentOffset;
};

#endif
