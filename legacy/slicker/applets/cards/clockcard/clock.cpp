/************************************************************

Copyright (c) 1996-2002 the kicker authors. See file kdebase/kicker/AUTHORS.
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

#include <cstdlib>
#include <ctime>
#include <time.h>

#include "clock.h"
#include "datepicker.h"
#include "settings.h"

#include <qtimer.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qlayout.h>
#include <qbitmap.h>
#include <qimage.h>
#include <qpainter.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qtooltip.h>
#include <qclipboard.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>

#include <kdebug.h>
#include <ktar.h>
#include <kdatepicker.h>
#include <kstandarddirs.h>
#include <kcolorbutton.h>
#include <kapplication.h>
#include <kprocess.h>
#include <kwin.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kconfig.h>
#include <klocale.h>
#include <kpopupmenu.h>
#include <kfontdialog.h>
#include <kglobalsettings.h>
#include <kstringhandler.h>
#include <kfiledialog.h>
#include <klistview.h>
#include <kio/job.h>

#include <kdatetbl.h>

#include <carddeskcore/Card.h>

using namespace CardDesk;

extern "C"
{
	CardApplet *init (const QString &configFile)
	{
		ClockApplet *clockApplet = new ClockApplet(configFile);
		return clockApplet;
	}
}

//************************************************************


ClockWidget::ClockWidget(ClockApplet *applet, ClockSettings* settings)
    : _applet(applet), _settings(settings)
{}


ClockWidget::~ClockWidget()
{}


//************************************************************

/*
PlainClock::PlainClock(ClockApplet *applet, ClockSettings* settings,
    : ClockWidget(applet, settings)
{
//    setFrameStyle(Panel | (_settings->showFrame() ? Sunken : 0));
//    setAlignment(AlignVCenter | AlignHCenter | WordBreak);
    setFont(_settings->font());
    QPalette pal = palette();
    pal.setColor( QColorGroup::Foreground, _settings->foreColor());
    pal.setColor( QColorGroup::Background, _settings->backColor());
    setPalette( pal );
    updateClock();
}


PlainClock::~PlainClock()
{
}


int PlainClock::preferedWidthForHeight(int ) const
{
    return kMax(sizeHint().width()+4, 0);
}


int PlainClock::preferedHeightForWidth(int w) const
{
    return kMax(heightForWidth(w)-7, 0);
}


void PlainClock::updateClock()
{
    QString newStr =
      KGlobal::locale()->formatTime(_applet->clockGetTime(),
		      		_settings->showSeconds());

    if (newStr != _timeStr) {
        _timeStr = newStr;
        setText(_timeStr);
    }
}

void PlainClock::forceUpdate()
{
    _timeStr = QString::null;
}
*/

//************************************************************


DigitalClock::DigitalClock(ClockApplet *applet, ClockSettings* settings)
    : ClockWidget(applet, settings)
{
    setFrameStyle(Panel | (_settings->showFrame() ? Sunken : 0));
    setMargin( 4 );
    setSegmentStyle(QLCDNumber::Flat);

    if (_settings->lcdStyle())
        setBackgroundPixmap(KIconLoader("clockapplet").loadIcon("lcd",KIcon::User));
    else
        setBackgroundColor(settings->backColor());

    setNumDigits(settings->showSeconds()? 8:5);

    _buffer = new QPixmap( width(), height() );

    updateClock();
}


DigitalClock::~DigitalClock()
{
    delete _buffer;
}


int DigitalClock::preferedWidthForHeight(int h) const
{
    if (h > 29) h = 29;
    if (h < 0) h = 0;
    return (numDigits()*h*5/11)+2;
}


int DigitalClock::preferedHeightForWidth(int w) const
{
    if (w < 0) w = 0;
    return((w / numDigits() * 2) + 6);
}


void DigitalClock::updateClock()
{
    static bool colon = true;
    QString newStr;
    QTime t(_applet->clockGetTime());

    int h = t.hour();
    int m = t.minute();
    int s = t.second();

    QString format("%02d");

    QString sep(!colon && _settings->blink() ? " " : ":");

    if (_settings->showSeconds())
        format += sep + "%02d";

    if (KGlobal::locale()->use12Clock()) {
        if (h > 12)
            h -= 12;
        else if( h == 0)
            h = 12;

        format.prepend("%2d" + sep);
    } else
        format.prepend("%02d" + sep);


    if (_settings->showSeconds())
        newStr.sprintf(format.latin1(), h, m, s);
    else
        newStr.sprintf(format.latin1(), h, m);

    if (newStr != _timeStr){
        _timeStr = newStr;
        setUpdatesEnabled( FALSE );
        display(_timeStr);
        setUpdatesEnabled( TRUE );
        repaint( FALSE );
    }
    if (_settings->blink())
        colon = !colon;
}

void DigitalClock::forceUpdate()
{
    _timeStr = QString::null;
}


void DigitalClock::paintEvent( QPaintEvent*)
{
    _buffer->fill( this, 0, 0 );
    QPainter p( _buffer );
    drawContents( &p );
    if (_settings->showFrame())
        drawFrame( &p );
    p.end();
    bitBlt( this, 0, 0, _buffer, 0, 0);
}


// yes, the colors for the lcd-lock are hardcoded,
// but other colors would break the lcd-lock anyway
void DigitalClock::drawContents( QPainter * p)
{
    setUpdatesEnabled( FALSE );
    QPalette pal = palette();
    if (_settings->lcdStyle())
        pal.setColor( QColorGroup::Foreground, QColor(128,128,128));
    else
        pal.setColor( QColorGroup::Foreground, _settings->shadowColor());
    setPalette( pal );
    p->translate( +1, +1 );
    QLCDNumber::drawContents( p );
    if (_settings->lcdStyle())
        pal.setColor( QColorGroup::Foreground, Qt::black);
    else
        pal.setColor( QColorGroup::Foreground, _settings->foreColor());
    setPalette( pal );
    p->translate( -2, -2 );
    setUpdatesEnabled( TRUE );
    QLCDNumber::drawContents( p );
    p->translate( +1, +1 );
}

/** No descriptions */
void DigitalClock::paint(QPainter * p, int w, int h){
  setFixedSize(QSize(w, h));
  drawContents(p);
}

// reallocate buffer pixmap
void DigitalClock::resizeEvent ( QResizeEvent *)
{
    delete _buffer;
    _buffer = new QPixmap( width(), height() );
}


// the background pixmap disappears during a style change
void DigitalClock::styleChange ( QStyle &)
{
    if (_settings->lcdStyle())
        setBackgroundPixmap(KIconLoader("clockapplet").loadIcon("lcd",KIcon::User));
}


//************************************************************


AnalogClock::AnalogClock(ClockApplet *applet, ClockSettings* settings)
    : ClockWidget(applet, settings), _spPx(NULL)
{
    _time = _applet->clockGetTime();
    _spPx = new QPixmap(size().width() * settings->antialiasFactor(),
                       size().height() * settings->antialiasFactor());
}


AnalogClock::~AnalogClock()
{
    if (_spPx) delete _spPx;
}

void AnalogClock::updateClock()
{
    if (!_settings->showSeconds())
         if (_time.minute() == _applet->clockGetTime().minute())
            return;

    _time = _applet->clockGetTime();
}

void AnalogClock::forceUpdate()
{
    _time = QTime(0,0);
}


//************************************************************

/*
FuzzyClock::FuzzyClock(ClockApplet *applet, ClockSettings* settings, QWidget *parent, const char *name)
    : QFrame(parent, name), ClockWidget(applet, settings)
{
    setFrameStyle(Panel | (_settings->showFrame() ? Sunken : 0));
    setBackgroundColor(_settings->backColor());

    hourNames   << i18n("hour","one") << i18n("hour","two")
                << i18n("hour","three") << i18n("hour","four") << i18n("hour","five")
                << i18n("hour","six") << i18n("hour","seven") << i18n("hour","eight")
                << i18n("hour","nine") << i18n("hour","ten") << i18n("hour","eleven")
                << i18n("hour","twelve");

    // xgettext:no-c-format
    normalFuzzy << i18n("%0 o'clock") // xgettext:no-c-format
                << i18n("five past %0") // xgettext:no-c-format
                << i18n("ten past %0") // xgettext:no-c-format
                << i18n("quarter past %0") // xgettext:no-c-format
                << i18n("twenty past %0") // xgettext:no-c-format
                << i18n("twenty five past %0") // xgettext:no-c-format
                << i18n("half past %0") // xgettext:no-c-format
                << i18n("twenty five to %1") // xgettext:no-c-format
                << i18n("twenty to %1") // xgettext:no-c-format
                << i18n("quarter to %1") // xgettext:no-c-format
                << i18n("ten to %1") // xgettext:no-c-format
                << i18n("five to %1") // xgettext:no-c-format
                << i18n("%1 o'clock");

    // xgettext:no-c-format
    normalFuzzyOne << i18n("one","%0 o'clock") // xgettext:no-c-format
                   << i18n("one","five past %0") // xgettext:no-c-format
                   << i18n("one","ten past %0") // xgettext:no-c-format
                   << i18n("one","quarter past %0") // xgettext:no-c-format
                   << i18n("one","twenty past %0") // xgettext:no-c-format
                   << i18n("one","twenty five past %0") // xgettext:no-c-format
                   << i18n("one","half past %0") // xgettext:no-c-format
                   << i18n("one","twenty five to %1") // xgettext:no-c-format
                   << i18n("one","twenty to %1") // xgettext:no-c-format
                   << i18n("one","quarter to %1") // xgettext:no-c-format
                   << i18n("one","ten to %1") // xgettext:no-c-format
                   << i18n("one","five to %1") // xgettext:no-c-format
                   << i18n("one","%1 o'clock"); 

    dayTime << i18n("Night")
            << i18n("Early morning") << i18n("Morning") << i18n("Almost noon")
            << i18n("Noon") << i18n("Afternoon") << i18n("Evening")
            << i18n("Late evening");

    _time = _applet->clockGetTime();
    repaint();
}


FuzzyClock::~FuzzyClock()
{
}


int FuzzyClock::preferedWidthForHeight(int ) const
{
    QFontMetrics fm(_settings->font());
    return fm.width(_timeStr) + 8;
}


int FuzzyClock::preferedHeightForWidth(int ) const
{
    QFontMetrics fm(_settings->font());
    return fm.width(_timeStr) + 8;
}


void FuzzyClock::updateClock()
{
  if (_time.hour() == _applet->clockGetTime().hour() &&
      _time.minute() == _applet->clockGetTime().minute())
     return;

  _time = _applet->clockGetTime();
  repaint();
}

void FuzzyClock::forceUpdate()
{
    _time = QTime(0,0);
}


void FuzzyClock::drawContents(QPainter *p)
{
    if (!isVisible())
        return;

    QString newTimeStr;

    if (_settings->fuzzyness() == 1 || _settings->fuzzyness() == 2) {
      int minute = _time.minute();
      int sector = 0;
      int realHour = 0;

      if (_settings->fuzzyness() == 1) {
          if (minute > 2)
              sector = (minute - 3) / 5 + 1;
      } else {
          if (minute > 6)
              sector = ((minute - 7) / 15 + 1) * 3;
      }

      newTimeStr = normalFuzzy[sector];
      int phStart = newTimeStr.find("%");
      int phLength = newTimeStr.find(" ", phStart) - phStart;

      // larrosa: we want the exact length, in case the translation needs it,
      // in other case, we would cut off the end of the translation.
      if (phLength < 0) phLength = newTimeStr.length() - phStart;
      int deltaHour = newTimeStr.mid(phStart + 1, phLength - 1).toInt();

      if ((_time.hour() + deltaHour) % 12 > 0)
          realHour = (_time.hour() + deltaHour) % 12 - 1;
      else
          realHour = 12 - ((_time.hour() + deltaHour) % 12 + 1);
      if (realHour==0) {
          newTimeStr = normalFuzzyOne[sector];
          phStart = newTimeStr.find("%");
         // larrosa: Note that length is the same,
         // so we only have to update phStart
      }
      newTimeStr.replace(phStart, phLength, hourNames[realHour]);
      newTimeStr.replace(0, 1, QString(newTimeStr.at(0).upper()));

    } else if (_settings->fuzzyness() == 3) {
        newTimeStr = dayTime[_time.hour() / 3];
    } else {
        int dow = _applet->clockGetDate().dayOfWeek();

        if (dow == 1)
            newTimeStr = i18n("Start of week");
        else if (dow >= 2 && dow <= 4)
            newTimeStr = i18n("Middle of week");
        else if (dow == 5)
            newTimeStr = i18n("End of week");
        else
            newTimeStr = i18n("Weekend!");
    }

    if (_timeStr != newTimeStr) {
        _timeStr = newTimeStr;
        _applet->resizeRequest();
    }

    p->setFont(_settings->font());
    p->setPen(_settings->foreColor());
    if (_applet->getOrientation() == Vertical) {
        p->rotate(90);
        p->drawText(4, -2, height() - 8, -(width()) + 2, AlignCenter, _timeStr);
    } else {
        p->drawText(4, 2, width() - 8, height() - 4, AlignCenter, _timeStr);
    }
}
*/

//************************************************************


ClockApplet::ClockApplet(const QString &configFile)
 : CardApplet(configFile), _calendar(0),  _clock(0)
{

    _settings = new ClockSettings(this, config());
    connect(_settings, SIGNAL(newSettings()), SLOT(slotApplySettings()));

    _calendar = new DatePicker(NULL);
    _calendar->move(QPoint(0,0));

//    _date = new QLabel(NULL);
//    _date->setAlignment(AlignVCenter | AlignHCenter | WordBreak);
//    _date->setBackgroundMode(QWidget::X11ParentRelative);
//    _date->installEventFilter(this);   // catch mouse clicks

    slotApplySettings();    // initialize clock widget
    slotUpdate();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), SLOT(slotUpdate()));
    _timer->start(500);
}


ClockApplet::~ClockApplet()
{
    if (_calendar)
        _calendar->close();
    config()->sync();
    delete _settings;
}


Separator *ClockApplet::createSeparator( Card *c )
{
	return new Separator(c, false);
}

/*
void ClockApplet::preferences()
{
    _settings->openPreferences();
}
*/

void ClockApplet::slotApplySettings()
{
    if (_clock)
	delete _clock;

    if (!_settings->showDate())
	    _settings->resetZone();
    TZoffset = _settings->calc_TZ_offset( _settings->zone() );

    switch (_settings->type()) {
        case ClockSettings::Fuzzy:
//            _clock = new FuzzyClock(this, _settings);
//            break;
        case ClockSettings::Plain:
//            _clock = new PlainClock(this, _settings);
//            break;
        case ClockSettings::Digital:
            _clock = new DigitalClock(this, _settings);
            break;
        case ClockSettings::Analog:
        default:
            _clock = new AnalogClock(this, _settings);
            break;
    }

    _clock->updateClock();

//    _date->setFont(_settings->dateFont());
//    updateDateLabel();
//    QPalette pal = _date->palette();
//    pal.setColor(QColorGroup::Foreground, _settings->dateForeColor());
//    _date->setPalette(pal);
//    if (_settings->showDate()) {
//        _date->show();
//        _date->repaint(true);
//    } else
//        _date->hide();

//    emit(updateLayout());
}


void ClockApplet::slotUpdate()
{
//  if (_lastDate != clockGetDate())
//    updateDateLabel();

  _clock->updateClock();
  repaintTab();
}

void ClockApplet::slotCopyMenuActivated( int id )
{
    QPopupMenu *m = (QPopupMenu *) sender();
    QString s = m->text(id);
    QApplication::clipboard()->setText(s);
}

QTime ClockApplet::clockGetTime()
{
    return QTime::currentTime().addSecs(TZoffset);
}

QDate ClockApplet::clockGetDate()
{
    return QDate::currentDate().addDays(TZoffset/(24*60*60));
}
	
void ClockApplet::showZone(int z)
{
    _settings->resetZone(z);
    TZoffset = _settings->calc_TZ_offset( _settings->zone() );
    updateDateLabel();
    _clock->forceUpdate(); // force repaint 
    slotUpdate();
    _settings->writeSettings();
}

void ClockApplet::nextZone()
{
    if (!_settings->showDate())
	return;
    _settings->nextZone();
    showZone(_settings->zoneIndex());
}

void ClockApplet::prevZone()
{
    if (!_settings->showDate())
	return;
    _settings->prevZone();
    showZone(_settings->zoneIndex());
}
void ClockApplet::updateDateLabel()
{
    _lastDate = clockGetDate();

    if (_settings->zoneIndex())
    {
	QString zone = i18n(_settings->zone().utf8());
//	_date->setText(
//          zone.mid(zone.find('/') + 1).replace(QRegExp("_"), " ")
//	);
    }
    else
    {
	QString dateStr = KGlobal::locale()->formatDate(_lastDate, true);
	if ( card()->edge() == LeftEdge ||
	     card()->edge() == RightEdge ) {
	  // search year number (4 digits) and insert a space for line-break 
	  int p = dateStr.find(QRegExp("\\d\\d\\d\\d"));
	  if (p==0) dateStr[4]=' '; else if (p>=0) dateStr.insert(p," ");
	}
//	_date->setText(dateStr);
    }
}


/** 
			All cards have a contents area. You should create a Contents object, add your
			gui to it, (see Contents class doc) and return it here. Like above, this will
			only be called once, when the Card is created.
		 */
Contents * ClockApplet::createContents(Card * c){
  Contents * con = new Contents(c);
  con->setContents(_calendar);
  con->setPreferredSize(_calendar->sizeHint().width(), _calendar->sizeHint().height());
  return con;
}

QSize ClockWidget::size(){
  return QSize(40, 20);
}

QSize AnalogClock::size(){
  return QSize(40, 40);
}
/** 
			If you want your tab to have a mini display area, return a QSize
			describing the area you want. Default returns (0,0)
		 */
QSize ClockApplet::miniDisplaySize(){
  return _clock->size();
}
/** 
			Implement this to draw your mini display area. The painter will already
			be transformed and rotated so whatever you draw will be right-side-up.
			@width & @height are the area the painter is clipped to. Unless space is
			constrained, width & height will be the same as miniDisplaySize, taking rotation
			into account.
		 */
void ClockApplet::paintMiniDisplay(QPainter * p, int width, int height){
  _clock->paint(p, width, height);
}

void ClockWidget::paint(QPainter * p, int w, int h){
  
}

void AnalogClock::paint(QPainter * p, int w, int h){
    int aaFactor = _settings->antialiasFactor();
    int spWidth = size().width() * aaFactor;
    int spHeight = size().height() * aaFactor;

    if ((spWidth != _spPx->size().width()) ||
        (spHeight != _spPx->size().height()))
    {
        delete _spPx;
        _spPx = new QPixmap(spWidth, spHeight);
    }


    QPainter paint;
    paint.begin( _spPx );

    _spPx->fill(_settings->backColor());

    QPointArray pts;
    QPoint cp(spWidth / 2, spHeight / 2);

    int d = QMIN(spWidth,spHeight)-(10 * aaFactor);

    if (_settings->lcdStyle()) {
        paint.setPen( QColor(100,100,100) );
        paint.setBrush( QColor(100,100,100) );
    } else {
        paint.setPen( _settings->shadowColor() );
        paint.setBrush( _settings->shadowColor() );
    }

    paint.setViewport(2,2,spWidth,spHeight);

    for ( int c=0 ; c < 2 ; c++ ) {
        QWMatrix matrix;
        matrix.translate( cp.x(), cp.y());
        matrix.scale( d/1000.0F, d/1000.0F );

        // hour
        float h_angle = 30*(_time.hour()%12-3) + _time.minute()/2;
        matrix.rotate( h_angle );
        paint.setWorldMatrix( matrix );
        pts.setPoints( 4, -20,0,  0,-20, 300,0, 0,20 );
        paint.drawPolygon( pts );
        matrix.rotate( -h_angle );

        // minute
        float m_angle = (_time.minute()-15)*6;
        matrix.rotate( m_angle );
        paint.setWorldMatrix( matrix );
        pts.setPoints( 4, -10,0, 0,-10, 400,0, 0,10 );
        paint.drawPolygon( pts );
        matrix.rotate( -m_angle );

        if (_settings->showSeconds()) {   // second
            float s_angle = (_time.second()-15)*6;
            matrix.rotate( s_angle );
            paint.setWorldMatrix( matrix );
            pts.setPoints(4,0,0,0,0,400,0,0,0);
            paint.drawPolygon( pts );
            matrix.rotate( -s_angle );
        }

        QWMatrix matrix2;
        matrix2.translate( cp.x(), cp.y());
        matrix2.scale( d/1000.0F, d/1000.0F );

        // quadrante
        for ( int i=0 ; i < 12 ; i++ ) {
            paint.setWorldMatrix( matrix2 );
            paint.drawLine( 460,0, 500,0 );	// draw hour lines
            // paint.drawEllipse( 450, -15, 30, 30 );
            matrix2.rotate( 30 );
        }

        if (_settings->lcdStyle()) {
            paint.setPen( Qt::black );
            paint.setBrush( Qt::black );
        } else {
            paint.setPen( _settings->foreColor() );
            paint.setBrush( _settings->foreColor() );
        }

        paint.setViewport(0,0,spWidth,spHeight);
    }
    paint.end();

    if (aaFactor != 1)
    {
        QImage spImage = _spPx->convertToImage();
        QImage displayImage = spImage.smoothScale(size());

        p->drawImage(0, 0, displayImage);
    }
    else
    {
        p->drawPixmap(0, 0, *_spPx);
    }


}
/** 
			If you want your tab to provide some entries in the default right-click
			context menu, override this, and add some QMenuItems to @defaultMenu.
			This will be called once, the first time the user right clicks on a tab.
		 */
void ClockApplet::addToDefaultContextMenu(class QPopupMenu * menu){
    KLocale *loc = KGlobal::locale();
    QDateTime dt = QDateTime::currentDateTime();
    dt = dt.addSecs(TZoffset);

    KPopupMenu *copyMenu = new KPopupMenu( menu );
    copyMenu->insertItem(loc->formatDateTime(dt), 201);
    copyMenu->insertItem(loc->formatDate(dt.date()), 202);
    copyMenu->insertItem(loc->formatDate(dt.date(), true), 203);
    copyMenu->insertItem(loc->formatTime(dt.time()), 204);
    copyMenu->insertItem(loc->formatTime(dt.time(), true), 205);
    copyMenu->insertItem(dt.date().toString(), 206);
    copyMenu->insertItem(dt.time().toString(), 207);
    copyMenu->insertItem(dt.toString(), 208);
    connect( copyMenu, SIGNAL( activated(int) ), this, SLOT( slotCopyMenuActivated(int) ) );

    KPopupMenu *zoneMenu = new KPopupMenu( menu );
    int i;
    for (i=0; i<=_settings->remoteZoneCount(); i++)
 	zoneMenu->insertItem(i==0 ? i18n("Local Timezone")
			:i18n(_settings->zone(i).utf8()).replace(QRegExp("_"), " "), 500+i);
    zoneMenu->setItemChecked(500+_settings->zoneIndex(),true);
    zoneMenu->insertSeparator();
    zoneMenu->insertItem(SmallIcon("configure"), i18n("&Configure Timezones..."), 110);

    KPopupMenu *type = new KPopupMenu(menu);
    type->insertItem(i18n("&Plain"), ClockSettings::Plain, 1);
    type->insertItem(i18n("&Digital"), ClockSettings::Digital, 2);
    type->insertItem(i18n("&Analog"), ClockSettings::Analog, 3);
    type->insertItem(i18n("&Fuzzy"), ClockSettings::Fuzzy, 4);
    type->setItemChecked((int)_settings->type(),true);

    menu->insertItem(i18n("&Type"), type, 101, 1);
    menu->insertItem(i18n("Show Time&zone"), zoneMenu, 110, 2);
    menu->insertItem(SmallIcon("date"), i18n("&Adjust Date && Time..."), 103, 4);
    menu->insertItem(SmallIcon("kcontrol"), i18n("Date && Time &Format..."), 104, 5);
    menu->insertItem(SmallIcon("editcopy"), i18n("C&opy to Clipboard"), copyMenu, 105, 6);
    menu->insertSeparator(7);
    menu->insertItem(SmallIcon("configure"), i18n("&Configure Clock..."), 102, 8);

    connect( menu, SIGNAL( activated(int) ), this, SLOT( slotMenuActivated(int) ) );
    connect( zoneMenu, SIGNAL( activated(int) ), this, SLOT( slotMenuActivated(int) ) );
    connect( type, SIGNAL( activated(int) ), this, SLOT( slotMenuActivated(int) ) );
    
}

void ClockApplet::slotMenuActivated(int result){
    if(result < 10) return;
    if (result < 100) {
        _settings->setType((ClockSettings::ClockType) result);
        _settings->writeSettings();
        slotApplySettings();
        tab()->delegatePropertiesChanged();
    };

    if ((result >= 500) && (result < 600)) {
	showZone(result-500);
    };

    KProcess proc;
    switch (result) {
	case 102:
		_settings->openPreferences();
		break;
	case 103:
		proc << locate("exe", "kdesu");
		proc << "--nonewdcop";
		proc << QString("%1 clock --lang %2")
				.arg(locate("exe", "kcmshell"))
				.arg(KGlobal::locale()->language());
		proc.start(KProcess::DontCare);
		break;
	case 104:
		proc << locate("exe", "kcmshell");
		proc << "language";
        	proc.start(KProcess::DontCare);
		break;
	case 110:
		_settings->openPreferences(1);
		break;
    } // switch()
}

#include "clock.moc"

