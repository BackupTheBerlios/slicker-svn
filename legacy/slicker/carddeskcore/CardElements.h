/***************************************************************************
                          CardElements.h  -  description
                             -------------------
    begin                : Wed Dec 25 2002
    copyright            : (C) 2002 by Shamyl Zakariya
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

#ifndef CARDELEMENTS_H
#define CARDELEMENTS_H

#include <qframe.h>
#include <qpixmap.h>
#include <kpixmap.h>
#include <qtimer.h>

#include "Const.h"

class QPopupMenu;

/**
	CardElement is the base class for widget components of a Card.
	CardElement provides:
		- preferred dimensions management
		- ability to smoothly snap shut (& hide) and back open
		- ability to manage "rotation" as its card might switch from
		  vertical to horizontal orientation.
*/

namespace CardDesk {

class Card;
class LayoutSwitcher;

class CardElement : public QFrame
{
	Q_OBJECT
	public:
		/**
			Creates a CardElement, inside of the card @parent.
		*/
		CardElement(Card *parent);

		/**
			Returns a pointer to the parent card.
		*/
		Card *card() { return _card; }

		/**
			The following methods allow for setting
			the preferred dimensions of this widget. Card will
			respect these dimensions and never shrink this widget
			smaller.

			Note: the preferred width and height are not related to
			the card's orientation. Width will always be the the width
			of the element against the screen edge. Height will always be
			the length of the element, perpendicular to the screen edge.
		*/
		virtual void setPreferredSize(int width, int height);
		virtual void setPreferredHeight( int height );
		virtual void setPreferredWidth( int width );
		virtual void restorePreferredWidth( bool relayout );

		virtual int preferredHeight( void ) const;
		virtual int preferredWidth( void ) const;


		/** returns actual height */
		int height( void );

		/** returns actual width */
		int width( void );

		/**
			if this CardElement is shut(), preferredHeight is zero, so
			to get the element's actual preferred height call this.
		*/
		int storedHeight( void );

		/**
			Closes this gui element, and removes it from the layout of the
			card. Does not delete it, just hides it. If @animate is true,
			the closing will be animated and relayout messages will be sent to
			the card. If false, this element will simply dissapear.
		*/
		virtual void collapse( bool animate = true );

		/**
			Similar to the above method, but opens the element.
		*/
		virtual void expand( bool animate = true );

		/**
			Returns true if this element is expanded.
		*/
		bool isOpen( void ) { return _open; }

		QColor lightColor( void );
		QColor mediumDarkColor( void );
		QColor darkColor( void );

	public slots:

		/**
			Internal -- handles animation of shutting process. Never call
			this directly. Use collapse()
		*/
		void animateCollapse();

		/**
			Internal -- handles animation of openeing process. Never call
			this directly. Use expand()	
		*/
		void animateExpand();

		/**
			Called by Card when its orientation changes. If you override this
			method, BE SURE TO CALL inherited version.
		*/
		virtual void cardOrientationChanged( void );

	protected:
		friend class Deck;

		/**
			INTERNAL. Used to override preferred width so that a deck
			of cards with different widths can be all sized the same.
		*/
		virtual void setDeckOverrideWidth( int w, bool relayout );

		/**
			INTERNAL. When setDeckOverrideWidth is called, the width is
			changed. This returns the width the element would like to
			be if it had its way.
		*/
		virtual int actualPreferredWidth( void );


		virtual void paintEvent( QPaintEvent *);

		Card *_card;
		int _width, _height, _storedHeight, _storedWidth;
		bool _open, _animating;
};

/////////////////////////////////////////////////////////////////////////////

class Tab;

/**
	TabDelegate is a class which can optionally be
	used to modify the way a tab draws itself. You have to subclass it to
	make it useful. Default version does *nothing*.

	TabDelegate provides for three extra mechanisms for a Tab
		- An optional icon.
		- An optional mini display area
		- Adding/modifying entries to a (default) context menu for the tab.

	You probably shouldn't directly subclass this. See CardApplet (which
	multiply inherits from TabDelegate and QObject)
*/

class TabDelegate
{
	public:
		TabDelegate( void );

		/**
			If you want your tab to draw an icon, return a pointer to it here.
			You will maintain ownership of the icon. It's your responsibility to
			delete it. If you don't want an icon, return 0L (default)
		*/
		virtual QPixmap *tabIcon( void );

		/**
			If you want your tab to have a mini display area, return a QSize
			describing the area you want. Default returns (0,0)
		*/
		virtual QSize miniDisplaySize( void );

		/**
			Implement this to draw your mini display area. The painter will already
			be transformed and rotated so whatever you draw will be right-side-up.
			@width & @height are the area the painter is clipped to. Unless space is
			constrained, width & height will be the same as miniDisplaySize, taking rotation
			into account.
		*/
		virtual void paintMiniDisplay(QPainter *, int width, int height);

		/**
			If you want your tab to provide some entries in the default right-click
			context menu, override this, and add some QMenuItems to @defaultMenu.
			This will be called once, the first time the user right clicks on a tab.
		*/
		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );

		/**
			If you have menu items which are context sensitive, e.g. should be
			enabled or disabled, override this to make said changes. This will be called
			every time the context menu is about to be shown.
		*/
		virtual void aboutToShowContextMenu( class QPopupMenu *menu );


		/**
			If you want the tab to allow a drag-hover to open it, reimplement
			this to return 'true'. Nonetheless, you still need to reimplement canDecodeDrag()
			Also, and this is not important but a good idea, whatever container accepts the drop in
			your Contents area, when the drop is made, you should call Tab::dropProcessed() (it's
			also a public slot). This will make the tab auto-close.
		*/
		virtual bool allowDragHoverToOpen( void );

		
		/**
			Reimplement this to return true if you want to allow hover opening
		*/
		virtual bool allowHoverToOpen( void );

		/**
			Reimplement this to return true if you want to allow drag-n-drop drops on
			the tab itself. If you return'true', you need to reimplement two methods:
				canDecode() -- you need to specify allowed mimetypes for DND
				tabDragDropEvent() -- drop event will be forwarded here.
		*/
		virtual bool allowDropOnTab( void );

		/**
			If you want your tab to respond to DND events, and you've set the
			above methods (allowDragHoverToOpen and/or allowDropOnTab) to true,
			you still need to override this to return true for mimetypes you can
			process.
		*/
		virtual bool canDecodeDrag( class QMimeSource * );

		/**
			Called when a drop event occurs on the tab
		*/
		virtual void tabDragDropEvent( QDropEvent * );

		/**
			call this to repaint the tab -- this is useful if your minidisplay area
			is animated, and you want continuous redrawing.
		*/
		void repaintTab( void );

		Tab *tab( void ) { return _tab; }

	protected:
		friend class Tab;
		void attachToTab( Tab * );

		Tab *_tab;
};

/**
	Tab is the base class for the main tab widget for
	all cards. Tab handles mouse events for opening, shutting
	and dragging the card to new positions.
*/

class Tab : public CardElement
{
	Q_OBJECT
	public:

		Tab(Card *parent, TabDelegate *extra = 0L);

		void setDelegate( TabDelegate * );
		TabDelegate *delegate( void );

		/**
			Paints the tab, everything but the text.
		*/
		virtual void paintTabBase(QPainter *);

		/**
			Paints text and calls paintTabIcon and paintTabMiniDisplay
			if the TabDelegate implementation calls for it.
		*/
		virtual void paintTabContents(QPainter *);

		/**
			Paints the icon for the tab, using delegate to provide the icon ptr.
		*/
		virtual void paintTabIcon(QPainter *);

		/**
			If delegate() requested a non zero size for the mini display, this
			method will set up the painter and call paintMiniDisplay on the
			delgate implementation.
		*/
		virtual void paintTabMiniDisplay(QPainter *);

		int contentsSpacing( void ) { return 3; }

	public slots:

		/**
			This slot notifies the Tab that the delegate has changed some properties
			of the display. Will accommodate,and redraw.
		*/
		void delegatePropertiesChanged( void );

		/**
			Notifies the tab that the contents area has processed a drag-n-drop
			event. This is so the tab can auto close if it auto opened.
		*/
		void dropProcessed( void );

	protected slots:
		void mouseTrackerTimeout( void );

		void slotCMOpen( void );
		void slotCMClose( void );
		void slotCMDestroy( void );
		void slotCMRename( void );

		void showContextMenu( void );

		/**
			If TabDelegate has supportsDragOverToOpen() set to true,
			when a drag enter event occurs on the tab, a timer will be
			set to call this method in some period of time (probably 1 second)
			If no dragLeaveEvent occurs, expand() will be called on the tab.
		*/
		void dragHoverTimeout();

	protected:

		virtual void createContextMenu( void );

		void calculatePreferredSize( void );

		QRegion tabInnerRegion( void );
		QRegion tabOuterRegion( void );
public:
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
protected:
		virtual void resizeEvent(QResizeEvent *);
		virtual void paintEvent(QPaintEvent *);

		virtual void dragEnterEvent( QDragEnterEvent * );
		virtual void dragMoveEvent( QDragMoveEvent * );
		virtual void dragLeaveEvent( QDragLeaveEvent * );
		virtual void dropEvent( QDropEvent * );
		
		virtual void mouseMoveEvent( QMouseEvent * );

		/**
			we're not using QWidget::mouseMoveEvent because it seems
			that the event is lost when widgets reparent. Instead, when
			the mouse is pressed, a timer starts and this method will be called
			repeatedly.
		*/
		virtual void mouseMove(QPoint widgetRelativePoint);

		/////////////////////////////////////////////////////////////////////////

		/**
			Helper logic class for dragging cards. Tab class forwards mouse movement
			events to DragLogic, which makes appropriate calls to setPosition on the card.
			The only reason DragLogic is separate is to keep Tab code relatively small. Tab is
			waaay too big as it is :P
		*/

		class DragLogic
		{
			public:
				DragLogic(Card *);

				void mousePress(QPoint point);
				void mouseMove(QPoint point);
				void done( void );
				bool tracking( void );

				/**
					Given the mouse position and the current edge, new position and (potentially) new edge
					will be written in to @position & @newEdge
				*/
				void positionForMouse(QPoint &mouse, ScreenEdge starterEdge,
				                      double &position, ScreenEdge &newEdge,
				                      int &distanceFromEdge);

				double topEdgeDrag(QPoint &mouse, int &distanceFromEdge);
				double rightEdgeDrag(QPoint &mouse, int &distanceFromEdge);
				double bottomEdgeDrag(QPoint &mouse, int &distanceFromEdge);
				double leftEdgeDrag(QPoint &mouse, int &distanceFromEdge);

				void centerMouseDown( void );

			protected:

				bool _mouseDownCentered;
				QPoint _mouseDown;
				QRect _screen;
				Card *_card;
				bool _tracking, _cardWasShutOnDrag;
		};

		/////////////////////////////////////////////////////////////////////////

		QPixmap _px;
		KPixmap _shadowPx;
		DragLogic _dragger;
		QTimer _mouseTrackerTimer;
		TabDelegate *_delegate;
		QPopupMenu *_contextMenu;
		bool _stillHovering, _hoverOpened;

		enum _CMIds { ID_Open, ID_Close, ID_Kill, ID_Configure, ID_Exit, ID_New, ID_Rename };

};

class Tray;

/////////////////////////////////////////////////////////////////////////////

class TrayWidget : public QWidget
{
	Q_OBJECT
	public:
		/**
			Constructs a TrayWidget, with tray as parent. Calls tray->addTrayWidget
			automatically.
			@name is the QObject name of this TrayWidget
			@breadth is the amount of space along the length of the tray
			@length is the amount of space taken perpendicular to the
			length of the tray. default value (0) will make length = breadth.
		*/
		TrayWidget(Tray *tray, const char *name, int breadth, int length = 0);

		/**

		*/
		void setDimensions(int breadth, int length = 0);

		/**
			@horizontal: if true the Card is on top or bottom of screen
			edge and the widget can be thought of as horizontal.
			If false, the Card is on the side of the screen and the
			widget can be thought of as vertical.
			Default implementation calls resize on the widget setting
			it to preferredWidth/Height accordingly.
		*/
		virtual void orientationChanged( bool horizontal );

		int length( void ) { return _length; }
		int breadth( void ) { return _breadth; }

	protected:
		int _breadth, _length;
		Tray *_tray;
};

/**
	The tray is an optional element. It is a widget container
	that is always visible, on open and shut cards.

	FIXME: It is not fleshed out yet. As of now, the Tray does
	precisely NOTHING, aside from drawing itself.
*/

class Tray : public CardElement
{
	Q_OBJECT
	public:
		Tray(Card *parent);
		~Tray();
		
		void addTrayWidget( TrayWidget *);
		void removeTrayWidget( TrayWidget * );

		int countTrayWidgets( void );
		TrayWidget *getTrayWidget( int at );

		void fitTrayToContents( void );
		
	public slots:
		virtual void cardOrientationChanged( void );

	protected:
		virtual void paintEvent(QPaintEvent *);

		KPixmap _shadowPx;
		LayoutSwitcher *_switcher;
};

/**
	The Separator is the bar between the tab/tray top and Contents bottom.
	If the gripper is turned on, it can be dragged to resize the length of
	the card. If not, it simply presents a line (as gui separator)
*/

class Separator : public CardElement
{
	Q_OBJECT
	public:

		Separator(Card *parent, bool gripper = true);

		/**
			Turns on or off gripper. If on, you can resize the card by dragging
			the gripper. If not, you just get a separator line.
		*/
		void setGripper( bool t );

		/*
			Returns wether the separator is currently resizable.
		*/
		bool gripper( void );

	public slots:
		virtual void cardOrientationChanged( void );

		virtual void resizeTimeout( void );


	protected:
		void doCursor( void );
		virtual void paintEvent(QPaintEvent *);

		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseMoveEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);

		QPoint _mouseDownPos, _deckULPos;

		bool _dragging, _gripperActive;
		int _originalContentsHeight, _newContentsHeight;
};

/**
	Contents is the container for whatever gui element your card
	presents when open.
*/
class Contents : public CardElement
{
	Q_OBJECT
	public:

		Contents(Card *parent);

		/**
			Sets the contents. If there is an existing contents
			widget, it will be reparented to zero, hidden, and returned.
			It's your responsibility to delete it.
			The current contents widget will be deleted when this is deleted.
		*/
		QWidget * setContents(QWidget *);

		/**
			Returns the current contents widget.
		*/
		QWidget *getContents( void );

	public slots:
		virtual void cardOrientationChanged( void );

	protected:
		void resizeEvent(QResizeEvent *);
		void relayoutContents( void );

		/////////////////////////////////////////////////////////////////////////

		QWidget *_contents;
};

class Resizer : public CardElement
{
	Q_OBJECT
	public:
		Resizer(Card *parent, bool resizer = true);

		void setResizer( bool t );
		bool resizer( void );
		void setSide( bool Rightside ); //True if the resizer is on the rightside of the card. default: Rightside = true;
		
	
	public slots:
		virtual void cardOrientationChanged( void );
		
	protected:
		void doCursor( void );
		virtual void paintEvent(QPaintEvent *);
		
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseMoveEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
		
		QPoint _mouseDownPos, _deckULPos;
		bool _dragging, _resizerActive, _RightSide;
		int _originalContentsWidth, _newContentsWidth;
};

}; //namespace

#endif


