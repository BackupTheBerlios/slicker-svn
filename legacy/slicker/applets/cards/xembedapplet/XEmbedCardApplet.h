#ifndef XEMBEDCARDAPPLET_H
#define XEMBEDCARDAPPLET_H

#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qxembed.h>

#include <kprocio.h>

#include <carddeskcore/CardApplet.h>

#include <qpushbutton.h>

#include <kwinmodule.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

class QPushButton_FocusGrabbing;
using namespace CardDesk;
//class XEmbedCardApplet : public CardApplet
class XEmbedCardApplet : public CardApplet
{
	Q_OBJECT
	public:
		XEmbedCardApplet(const QString &configFile);
		~XEmbedCardApplet();

		virtual QString defaultCardName( void ) { return QString("XEmbed Card"); }
		virtual QPixmap *tabIcon( void );

		virtual QSize miniDisplaySize( void );
		virtual void paintMiniDisplay(QPainter *, int width, int height);

		virtual void addToDefaultContextMenu( class QPopupMenu *defaultMenu );
		virtual void aboutToShowContextMenu( class QPopupMenu *menu );

		virtual bool allowDragHoverToOpen( void );
		virtual bool allowHoverToOpen( void );

		virtual bool allowDropOnTab( void );
		virtual bool canDecodeDrag( class QMimeSource * );
		virtual void tabDragDropEvent( QDropEvent * );

		virtual Tray *createTray( Card * );
		virtual Separator *createSeparator( Card * );
		virtual Contents *createContents( Card * );

		virtual void saveSettings( class KConfig * );
		virtual void loadSettings( class KConfig * );

		virtual void cardReady( void );

	public slots:
		void menuOne( void );
		void menuTwo( void );

		void toggleResizeGrip( bool yesno );
		void toggleMiniDisplay(bool yesno);
		void toggleIcon( bool yesno );

		void clickChooseXWindow();
		void processExited_xwininfo(KProcess *proc);

	protected slots:
		void kill_xwininfo();
		//void finishedCommandProcess(KProcess *proc);
		void embeddedWindowDestroyed();
		void windowAdded(WId);

	protected:
		void startCommandProcess();
		void embedWindow();

		QPixmap _icon;
		QString _name;
		Contents *_contents;
		QWidget *_contentsWidget;

		WId _WId;

		QXEmbed *_embedWidget;
		QPushButton_FocusGrabbing* _buttonEmbedLink;

		KProcIO *_proc_xwininfo;
		KProcIO *_proc_command;
		KWinModule _winModule;

		QString _command;
};

QStringList getQStringListProperty(WId w, Atom prop);

class QPushButton_FocusGrabbing : public QPushButton
{
	friend class XEmbedCardApplet;

	Q_OBJECT
	public:
		QPushButton_FocusGrabbing( const QString & text, QWidget * parent, const char * name = 0 );

	signals:
		void keyEscapeSignal();

	protected:
		virtual void keyPressEvent ( QKeyEvent * e );
};

#endif

