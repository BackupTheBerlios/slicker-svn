/*****************************************************************

Copyright (c) 1996-2000 the kicker authors. See file kdebase/kicker/AUTHORS.
Copyright (c) 2003 Kurre Ståhlberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#ifndef __CLOCK_H
#define __CLOCK_H

#include <qlcdnumber.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qguardedptr.h>
#include <qdatetime.h>
#include <qvbox.h>
#include <qstringlist.h>
#include <qevent.h>

#include <carddeskcore/CardApplet.h>
#include <carddeskcore/Const.h>
#include <kdirwatch.h>

class QTimer;
class QBoxLayout;
class KConfig;
class DatePicker;
class QPixmap;
class ClockSettings;

namespace KIO
{
  class Job;
}
namespace CardDesk
{

class ClockWidget;

class ClockApplet : public CardApplet
{
  Q_OBJECT

public:
	ClockApplet(const QString &configFile );
	~ClockApplet();

	int TZoffset;
	QTime clockGetTime();
	QDate clockGetDate();

	virtual QString defaultCardName( void ) { return QString("Clock"); }

  /**
			All cards have a contents area. You should create a Contents object, add your
			gui to it, (see Contents class doc) and return it here. Like above, this will
			only be called once, when the Card is created.
		 */
  Contents * createContents(Card * );
  /**
			Implement this to draw your mini display area. The painter will already
			be transformed and rotated so whatever you draw will be right-side-up.
			@width & @height are the area the painter is clipped to. Unless space is
			constrained, width & height will be the same as miniDisplaySize, taking rotation
			into account.
		 */
  void paintMiniDisplay(QPainter * p, int width, int height);
  /**
			If you want your tab to have a mini display area, return a QSize
			describing the area you want. Default returns (0,0)
		 */
  QSize miniDisplaySize();
  /**
			If you want your tab to provide some entries in the default right-click
			context menu, override this, and add some QMenuItems to @defaultMenu.
			This will be called once, the first time the user right clicks on a tab.
		 */
  void addToDefaultContextMenu(class QPopupMenu * menu);
  /**
			This is the pure virtual -- override this to return
			a unique plugin name. For example, your fancy kfm browser
			card might return "KFMCardPlugin"

			This will be used to identify your plugin class. E.g., so a user can
			say "Make me a card using KFMCardPlugin"
		 */
  virtual Separator *createSeparator( Card * );

protected slots:
	void slotApplySettings();
	void slotUpdate();
	void slotCopyMenuActivated( int id );

protected:
	void updateDateLabel();
	void showZone(int z);
	void nextZone();
	void prevZone();

	void paletteChange(const QPalette &)   { slotApplySettings(); }

	ClockSettings *_settings;
	DatePicker *_calendar;
	ClockWidget *_clock;
	QLabel *_date;
	QDate _lastDate;
	QTimer *_timer;

public slots: // Public slots
  void slotMenuActivated(int result);
};




// base class for all clock types
class ClockWidget
{

public:
	ClockWidget(ClockApplet *applet, ClockSettings* settings);
	virtual ~ClockWidget();

	virtual ClockWidget* widget()=0;
	virtual int preferedWidthForHeight(int h) const =0;
	virtual int preferedHeightForWidth(int w) const =0;
	virtual void updateClock()=0;
	virtual void forceUpdate()=0;
  virtual QSize size();
  virtual void paint(QPainter * p, int w, int h);

protected:
	ClockApplet *_applet;
	ClockSettings *_settings;
	QTime _time;
};


// Commented out as it's not yet ported to CardDesk
/*
class PlainClock : public QLabel, public ClockWidget
{
  Q_OBJECT

public:
	PlainClock(ClockApplet *applet, ClockSettings* settings, QWidget *parent=0, const char *name=0);
	~PlainClock();

	QWidget* widget()    { return this; }
	int preferedWidthForHeight(int h) const;
	int preferedHeightForWidth(int w) const;
	void updateClock();
	void forceUpdate();

protected:
	QString _timeStr;
};
*/

class DigitalClock : public QLCDNumber, public ClockWidget
{
  Q_OBJECT

public:
	DigitalClock(ClockApplet *applet, ClockSettings* settings);
	~DigitalClock();

	ClockWidget* widget()    { return this; }
	int preferedWidthForHeight(int h) const;
	int preferedHeightForWidth(int w) const;
	void updateClock();
	void forceUpdate();
  /** No descriptions */
  void paint(QPainter * p, int w, int h);

protected:
	void paintEvent( QPaintEvent*);
	void drawContents( QPainter * p);
	void resizeEvent ( QResizeEvent *ev);
	void styleChange( QStyle& );

	QPixmap *_buffer;
	QString _timeStr;
};


class AnalogClock : public QObject, public ClockWidget
{
  Q_OBJECT

public:
	AnalogClock(ClockApplet *applet, ClockSettings* settings);
	~AnalogClock();

	ClockWidget* widget()                        { return this; }
	int preferedWidthForHeight(int h) const  { return h; }
	int preferedHeightForWidth(int w) const  { return w; }
	void updateClock();
	void forceUpdate();
  QSize size();
  void paint(QPainter * p, int w, int h);

protected:

	QPixmap *_spPx;
	int _bgScale;
};


// Commented out as it's not yet ported to CardDesk
/*
class FuzzyClock : public ClockWidget
{
  Q_OBJECT

public:
	FuzzyClock(ClockApplet *applet, ClockSettings* settings);
	~FuzzyClock();

	ClockWidget* widget()    { return this; }
	int preferedWidthForHeight(int h) const;
	int preferedHeightForWidth(int w) const;
	void updateClock();
	void forceUpdate();

protected:
	virtual void drawContents(QPainter *p);

	QStringList hourNames;
	QStringList normalFuzzy;
	QStringList normalFuzzyOne;
	QStringList dayTime;

	QString _timeStr;
};
*/

}

#endif
