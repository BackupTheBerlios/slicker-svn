/***************************************************************************
                          CardApplet.h  -  description
                             -------------------
    begin                : Sun Jan 5 2003
    copyright            : (C) 2003 by Shamyl Zakariya
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

#ifndef CARDAPPLET_H
#define CARDAPPLET_H

#include <qobject.h>
#include <qstringlist.h>

#include <kconfig.h>

#include "CardElements.h"

namespace CardDesk {
class Card;

//!CardApplet class
/*!
Implement a CardApplet.  The main purpose of a CardApplets is to provide the contents for a card.
CardApplets are implemented as Dynamic Shared Objects and can be created and destroyed by Slicker during run time.

Slicker locates available applets by searching for applet desktop files in (ALL_KDEDIRS)/share/apps/slicker/cardapplets.
Every card applet should install a desktop file there to be recognised by the panel.
This desktop file should implement the standard keys "Name", "Comment", "Author" and "Version" one other specific key:

\b X-KDE-Library

X-KDE-Library is used by Slicker to locate the applet DSO (Dynamic Shared Object)\n
Example: X-KDE-Library=libexampleapplet

To implement a card applet it is not enough to simply inherit from CardApplet, you also have to provide a factory function in your DSO.
Example:

<pre>
using namespace CardDesk;
extern "C"
{
	CardApplet *init(const QString &configFile)
	{
		ExampleCardApplet *eca = new ExampletCardApplet(configFile);
		return eca;
	}

}

</pre>

Do not change the factory function signature as your applet will fail to load.  Your applet does not need to worry about creating or deleting config files, but should instead use the config() function to get a pointer to a KConfig object.  You should load settings and save settings in the applet's constructor andestructor respectively and should call config()->sync() after writing the settings in the destructor.

*/

class CardApplet : public QObject, public TabDelegate
{
	Q_OBJECT
	public:
	
		/**
			Will create a card applet.  CardApplets are parentless
			/param configFile The configFile handed over in the factory function.
			If you want your applet to load/store settings, use configFile() in the constructor and destructor
		*/
		CardApplet(const QString &configFile);

		/////////////////////////////////////////////////////////////////////////
		// The following methods implement TabDelegate

		/**
			If you want your tab to draw an icon, return a pointer to it here.
			You will maintain ownership of the icon. It's your responsibility to
			delete it. If you don't want an icon, return 0L (default)

			Tab will auto-resize to accommodate icon size. It can be big, small, etc.
			If icon size changes, emit propertiesChanged.

		*/
		virtual QPixmap *tabIcon( void );

		/**
			If you want your tab to have a mini display area, return a QSize
			describing the area you want. Default returns (0,0).
			Mini display, like tab, can be big or small. Tab will accommodate.

			If size changes at runtime, emit propertiesChanged.
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
			The ptr will remain valid for life of Card.
		*/
		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );

		/**
			If you have menu items which are context sensitive, e.g. should be
			enabled or disabled, override this to make said changes. This will be called
			every time the context menu is about to be shown. This menu will be the same
			as the one in addToDefaultContextMenu.
		*/
		virtual void aboutToShowContextMenu( class QPopupMenu *menu );


		/**
			If you want the tab to allow a drag-hover to open it, reimplement
			this to return 'true'. Nonetheless, you still need to reimplement canDecodeDrag()
			Also, and this is not important but a good idea, whatever container accepts the drop in
			your Contents area, when the drop is made, you should call Tab::dropProcessed() (it's
			also a public slot). This will make the tab auto-close.

			If this changes during runtime, emit propertiesChanged.
		*/
		virtual bool allowDragHoverToOpen( void );

		/**
			Reimplement this in order to allow opening on hover
		*/
		virtual bool allowHoverToOpen( void );
		/**
			Reimplement this to return true if you want to allow drag-n-drop drops on
			the tab itself. If you return'true', you need to reimplement two methods:
				canDecode() -- you need to specify allowed mimetypes for DND
				tabDragDropEvent() -- drop event will be forwarded here.

			If this changes during runtime, emit propertiesChanged.
		*/
		virtual bool allowDropOnTab( void );

		/**
			If you want your tab to respond to DND events, and you've set the
			above methods (allowDragHoverToOpen and/or allowDropOnTab) to true,
			you still need to override this to return true for mimetypes you can
			process.

			The idea is, when the user drags something over the tab, it will call this
			to see if the drag should be accepted or ignored.

			Default returns false.
		*/
		virtual bool canDecodeDrag( class QMimeSource * );
		/*
			If you don't want the card which the applet sits to be renamable, return true here.

			Default returns true
		*/

		virtual bool allowRenameCard( void );


		/**
			Called when a drop event occurs on the tab. If your applet has an internal
			area in its contents which handles drops, you might want to forward this
			event.

			e.g.

			void MyApplet::tabDragDropEvent( QDropEvent *e)
			{
				_myDropArea->dropEvent(e);
				emit dropProcessed();
			}

		*/
		virtual void tabDragDropEvent( QDropEvent * );

		/////////////////////////////////////////////////////////////////////////
		// The following methods fill out the rest of CardApplet -- they provide
		// the contents mechanisms &cetera

		/**
			Calling this will change the name of the card to whatever the
			name returned by ::cardName() currently is. If your applet changes
			its name (for whatever reason, perhaps a status update), whenever
			the name changes, just call setCardName() to notify the card that
			its name changed.
		*/
		void setCardName(const QString& = QString::null);

		/**
			You must Implement this to return default card name (ie, before user has renamed)
		*/
		virtual QString defaultCardName( void ) = 0L;

		/**
			Implement this to return a name for the card. If your card expects to change names
			for example, to show progress in a calculation, you'll want to call setCardName() for
			each change.
		*/

		virtual QString cardName( void );

		/**
			If your card needs a Tray, build and return a tray here. This will be called
			only once, when the card is created and calls ::build(); You can return a
			custom Tray implementation. If you don't want a tray, return 0L here.
		*/
		virtual Tray *createTray( Card * );

		/**
			All cards have a separator, create one and return it. Like above, this will
			be called only once. If you want, you can return a custom implementation, but
			the separator, for GUI consistency, should probably not be messed with.
		*/
		virtual Separator *createSeparator( Card * );

		/**
			All cards have a contents area. You should create a Contents object, add your
			gui to it, (see Contents class doc) and return it here. Like above, this will
			only be called once, when the Card is created.
		*/
		virtual Contents *createContents( Card * );

		/**
			Create a tab and return it.  This will only be called once.  You can return a
			custom tab implementation.
		*/
		
		virtual Tab *createTab( Card *);

		/** 
			Create a Resizer
		*/
		virtual Resizer *createResizer( Card *);
		
		/**
			Returns a pointer to the card this is in.
		*/
		
		Card *card( void );

		/**
			Called by card, at the end of Card::build -- when all the elements
			are inserted and the card is showing.
			
			IMPORTANT: in your implementation, be sure to call CardApplet::cardReady()
		*/
		virtual void cardReady( void );

		/**This function returns a pointer to the KConfig object that the Applet should use for storing any settings.  The applet is not limited to use of a specific group in the config file, nor does the applet have to store settings about the position of its associated card etc. */
		KConfig* config() const { return _config; }

	public slots:

		/**
			This will be called when the card is opened. If your applet does heavy things and
			doesn't need them running when shut (for example, and animated display in its
			contents area), you can use this as a trigger. See ::cardShut
			for the complement of this method.
		*/
		virtual void cardOpened( void );

		/**
			This will be called when the card is shut. See ::cardOpened() for reasoning.
		*/
		virtual void cardShut( void );

		/**
			This will be called when the card is about to be deleted (removed).  Return true to continue with
			destruction process, return false to cancel it.  Defaults to true
		*/
		virtual bool cardDestroy( void );

		/**
			This will be called when there is a need to notify the applets that the environment has changed.
			Example of possible environment changes are change of edge, sizechange, decking change etc.
		*/
		virtual void environmentChanged( void );

	signals:

		/**
			Emit this when something changes which requires a relayout
			or redraw of the card or its elements.
		*/
		void propertiesChanged( void );

		/**
			Emit this when your applet handles a drag-drop event. The purpose is
			to notify the card that the drop was handled, so that if the card
			hover-opened, it can automatically shut.
		*/
		void dropProcessed( void );
		
	protected:
		friend class Card;
		void attachToCard( Card *);


		Card *_card;

	private:
		KConfig* _config;
		QString _name;
};

}; //CardDesk namespace

#endif
