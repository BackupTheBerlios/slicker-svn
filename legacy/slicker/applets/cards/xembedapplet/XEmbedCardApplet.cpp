#include "XEmbedCardApplet.h"
#include <carddeskcore/Card.h>
#include <cardhandler/manager.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qrect.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <kwin.h>


//===========================================================================
// XEMBEDCARDAPPLET CODE

/*
*	THIS CODE IS NO WHERE NEAR DONE.  DON'T REPORT BUGS UNLESS YOU CAN HELP CODE THE FIX.
*/

/// @author Mark Piper

/// @todo: clean up embedded apps properly on removal of card.  send 'em a term sig, or something
/// @todo: attach a signal to the embedded app's dieing.
//		- for command, and for xwininfo ones
/// @todo: probably i should attach a signal to the embedded app's WINDOW dieing!

using namespace CardDesk;

XEmbedCardApplet::XEmbedCardApplet(const QString &configFile)
: CardApplet(configFile), _contentsWidget(NULL)
{
	kdDebug() << "XEmbedCardApplet::XEmbedCardApplet(configFile)" << endl;

	_proc_xwininfo = 0;
	_WId = 0;

	KIconLoader il;
	_icon = il.loadIcon("xapp", KIcon::Small, 0, KIcon::DefaultState);


	loadSettings(config());
}

XEmbedCardApplet::~XEmbedCardApplet()
{
	kdDebug() << "XEmbedCardApplet::~XEmbedCardApplet()" << endl;

	kill_xwininfo();
	delete _embedWidget;

	saveSettings(config());
}

Contents *XEmbedCardApplet::createContents( Card *c )
{
	_contents = new Contents(c);

	_contentsWidget = new QWidget(0, "XEmbedApplet-widget");
	QVBoxLayout *layout = new QVBoxLayout(_contentsWidget);
	_embedWidget = new QXEmbed(_contentsWidget, "embed window");
	_embedWidget->setAutoDelete(true);
	layout->addWidget(_embedWidget);

	_buttonEmbedLink= new QPushButton_FocusGrabbing( "&Choose X Window...", _contentsWidget );
	connect(_buttonEmbedLink, SIGNAL(clicked()), SLOT(clickChooseXWindow()));

	_contents->setContents(_contentsWidget);

	return _contents;
}


/**
	Embeds the window _Wid
*/
void XEmbedCardApplet::embedWindow()
{
	kdDebug() << "XEmbedCardApplet::embedWindow()" << endl;

	if (_WId)
	{
		_command = getQStringListProperty(_WId, XA_WM_COMMAND).join(" ");
		kdDebug() << " XA_WM_COMMAND returned " << _command << endl;

		KWin::Info winInfo = KWin::info(_WId);
		switch( _card->edge() )
		{
			case LeftEdge: case RightEdge: _contents->setPreferredSize(winInfo.geometry.height(), winInfo.geometry.width()); break;
			case TopEdge: case BottomEdge: _contents->setPreferredSize(winInfo.geometry.width(), winInfo.geometry.height()); break;
		}

		_embedWidget->embed(_WId);
		XReparentWindow(qt_xdisplay(), _WId, _embedWidget->winId(), 0, 0);
		//_embedWidget->resize(
		_buttonEmbedLink->hide();

		connect(_embedWidget, SIGNAL(embeddedWindowDestroyed()), SLOT(embeddedWindowDestroyed()));
	}
}

void XEmbedCardApplet::embeddedWindowDestroyed()
{
	kdDebug() << "XEmbedCardApplet::embeddedWindowDestroyed() " << endl;

	// remove the card when the embeded proc finishes
	SlickerCardManager::instance()->destroyCard( this->card() );
}

//---------------------------------------------------------------------------
// Start an command process from saved session information

void XEmbedCardApplet::startCommandProcess()
{
	kdDebug() << "XEmbedCardApplet::startCommandProcess()" << endl;

	_proc_command = new KProcIO;
	*_proc_command << _command;
	//connect(_proc_command, SIGNAL(processExited(KProcess *)), this, SLOT(finishedCommandProcess(KProcess *)));
  	connect(&_winModule, SIGNAL(windowAdded(WId)), this, SLOT(windowAdded(WId)));

	_proc_command->start(KProcess::NotifyOnExit);
}

/*
void XEmbedCardApplet::finishedCommandProcess(KProcess* proc)
{
	kdDebug() << "XEmbedCardApplet::finishedCommandProcess()" << endl;

	// Check if the process forked and no window has been created yet
	if (!_WId) return;

	// Check if the X still has the WId registered (the proc forked and left its window open)
	if (getQStringListProperty(_WId, XA_WM_COMMAND).join(" ") == "__XFree_Error_BadWindow__")
		return;

	// remove the card when the embeded proc finishes
	SlickerCardManager::instance()->destroyCard( this->card() );
}
*/

void XEmbedCardApplet::windowAdded(WId WIdAdded)
{
	kdDebug() << "XEmbedCardApplet::windowAdded()" << endl;

	QString commandAdded = getQStringListProperty(WIdAdded, XA_WM_COMMAND).join(" ");
	kdDebug() << " XA_WM_COMMAND returned " << commandAdded << endl;

	// There's a race condition here.  Two XEmbedCardApplets with the identical windows to create will both respond to the same windowAdded event.
	// Someday i'll fix that by using XReparentNotify

	if (_command == commandAdded)
	{
		kdDebug() << " COMMANDs matched.  Embedding WId " << WIdAdded << endl;
		_WId = WIdAdded;
		embedWindow();
		_winModule.disconnect( SIGNAL(windowAdded(WId)) );
	}
}

//---------------------------------------------------------------------------
// Embed a running command process

void XEmbedCardApplet::clickChooseXWindow()
{
	kdDebug() << "XEmbedCardApplet::clickChooseXWindow()" << endl;

	// Make sure we can get keyboard focus, if the user presses escape
	connect(_buttonEmbedLink, SIGNAL(keyEscapeSignal()), SLOT(kill_xwininfo()));
	_buttonEmbedLink->grabKeyboard();

	// Start the xwininfo application
	_proc_xwininfo = new KProcIO;

	*_proc_xwininfo << "xwininfo";
	*_proc_xwininfo << "-stats" << "-int";
	connect(_proc_xwininfo, SIGNAL(processExited(KProcess *)),
			this, SLOT(processExited_xwininfo(KProcess *)));
	_proc_xwininfo->start(KProcess::NotifyOnExit);
}


void XEmbedCardApplet::processExited_xwininfo(KProcess* proc)
{
	kdDebug() << "XEmbedCardApplet::processExited_xwininfo()" << endl;

	_buttonEmbedLink->releaseKeyboard();

	// Now we should be able to pull the window ID from the buffer, and embed it.
	QString line;
	QString searchString = "Window id: ";

	int readStatus = 1;
	_WId = 0;

	while(readStatus != -1)
	{
		readStatus = _proc_xwininfo->readln(line);
		kdDebug() << "  line is " << line << endl;

		int pos = line.find(searchString);
		if (pos != -1)
		{
			kdDebug() << " found " << searchString << " at " << pos << endl;
			pos += searchString.length();
			int posEnd = line.find(' ', pos);
			if (posEnd != -1)
			{
				kdDebug() << " ended " << searchString << " at " << pos << endl;
				_WId = line.mid(pos, posEnd - pos).toULong();
				kdDebug() << "WId to embed is " << _WId << endl;
				break;
			}
		}
	}

	embedWindow();

	kill_xwininfo();
}

void XEmbedCardApplet::kill_xwininfo()
{
	kdDebug() << " XEmbedCardApplet::kill_xwininfo() " << endl;
	if (_proc_xwininfo)
	{
		_proc_xwininfo->kill();
		delete _proc_xwininfo;
		_proc_xwininfo = 0;
	}

	_buttonEmbedLink->disconnect( SIGNAL(keyEscapeSignal()) );
}


//---------------------------------------------------------------------------
// Settings Code

void XEmbedCardApplet::saveSettings( KConfig *config )
{
	config->writeEntry("Command", _command);
	config->sync();
}

void XEmbedCardApplet::loadSettings( KConfig *config )
{
	config->setGroup("General");
	_command = config->readEntry("Command", "");
	if (_command != "")
		startCommandProcess();
}

void XEmbedCardApplet::cardReady( void )
{
	CardApplet::cardReady();
	emit propertiesChanged();
}


//---------------------------------------------------------------------------
// Menu Code

void XEmbedCardApplet::addToDefaultContextMenu( QPopupMenu *defaultMenu )
{
	defaultMenu->insertItem("Test Item &One", this, SLOT(menuOne()));
	defaultMenu->insertItem("Test Item &Two", this, SLOT(menuTwo()));
}

void XEmbedCardApplet::menuOne( void )
{
	printf("XEmbedCardApplet::menuOne()\n");
}

void XEmbedCardApplet::menuTwo( void )
{
	printf("XEmbedCardApplet::menuTwo()\n");
}



//---------------------------------------------------------------------------
// Boilerplate Code


QPixmap *XEmbedCardApplet::tabIcon( void )
{
	return &_icon;
}

QSize XEmbedCardApplet::miniDisplaySize( void )
{
	return QSize( 40, 18 );
}

void XEmbedCardApplet::paintMiniDisplay(QPainter *paint, int width, int height)
{
}

void XEmbedCardApplet::aboutToShowContextMenu( class QPopupMenu * )
{
	printf("XEmbedCardApplet::aboutToShowContextMenu()\n");
}

bool XEmbedCardApplet::allowDragHoverToOpen( void )
{
	return false;
}

bool XEmbedCardApplet::allowHoverToOpen( void )
{
	return false;
}

bool XEmbedCardApplet::allowDropOnTab( void )
{
	return true;
}

bool XEmbedCardApplet::canDecodeDrag( QMimeSource *e )
{
	return QUriDrag::canDecode( e );
}

void XEmbedCardApplet::tabDragDropEvent( QDropEvent *e )
{
}


Tray *XEmbedCardApplet::createTray( Card * )
{
	return NULL;
}

Separator *XEmbedCardApplet::createSeparator( Card *c )
{
	return new Separator(c, true);
}

//===========================================================================
// UTILITY CODE

//---------------------------------------------------------------------------
// XAtom Helper

QStringList getQStringListProperty(WId w, Atom prop)
{
	Atom type;
	int format, status;
	unsigned long nitems = 0;
	unsigned long extra = 0;
	unsigned char *data = 0;
	QStringList result;

	status = XGetWindowProperty( qt_xdisplay(), w, prop, 0, 10000,
					FALSE, XA_STRING, &type, &format,
					&nitems, &extra, &data );
	if (status == Success)
	{
		if (!data)
			return result;
		for (int i=0; i<(int)nitems; i++)
		{
			result << QString::fromLatin1( (const char*)data + i );
			while(data[i]) i++;
		}
		XFree(data);
	}

	if (status == BadWindow)
		result << "__XFree_Error_BadWindow__";

	return result;
}

//---------------------------------------------------------------------------
// QPushButton_FocusGrabbing

QPushButton_FocusGrabbing::QPushButton_FocusGrabbing( const QString & text, QWidget * parent, const char * name ) :
	QPushButton(text, parent, name)
{
	setFocusPolicy(QWidget::StrongFocus);

}

void QPushButton_FocusGrabbing::keyPressEvent(QKeyEvent *e)
{
	kdDebug() << " QPushButton_FocusGrabbing::keyPressEvent " << endl;
	releaseKeyboard();

	if ( e->key() == Qt::Key_Escape )
	{
		kdDebug() << " got the escape key! " << endl;
		emit keyEscapeSignal();
	}

	QPushButton::keyPressEvent(e);
}
