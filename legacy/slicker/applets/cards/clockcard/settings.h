/*****************************************************************

Copyright (c) 1996-2000 the kicker authors. See file AUTHORS.

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

#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <qcolor.h>
#include <qfont.h>
#include <qguardedptr.h>
#include <qstringlist.h>
#include "clock.h"

class KConfig;
class ClockConfDialog;

using namespace CardDesk;

class ClockSettings : public QObject
{
  Q_OBJECT

public:
	enum ClockType { Plain = 10, Digital, Analog, Fuzzy };

	ClockSettings(ClockApplet* app, KConfig* conf);
	~ClockSettings();
	void writeSettings();

	void openPreferences(int startindex=-1);

	void setType(ClockType type);

	ClockType type()     { return _type; }
	bool lcdStyle()      { return _type == Digital?_lcdStyleDig:_lcdStyleAna; }
	bool showSeconds();
	bool showDate();
	bool showFrame();
	QColor foreColor();
	QColor shadowColor();
	QColor backColor();
	bool blink()         { return _blink; }
	QFont font()         { return _type == Plain? _fontPlain:_fontFuz; }
	int fuzzyness()      { return _fuzzynessFuz; }
	int antialiasFactor() { return  _antialiasFactorAna; }

	QFont dateFont()     { return _fontDate; }
	QColor dateForeColor();

	QString zone() const { return zone(_zoneIndex); }
	QString zone(int z) const;

	QStringList remoteZoneList() const { return _remotezonelist; }
	int remoteZoneCount() { return _remotezonelist.count(); }
	void nextZone();
	void prevZone();
	unsigned int zoneIndex() { return _zoneIndex; }
	void resetZone(int z=0) { _zoneIndex = z; }

	int calc_TZ_offset(const QString& zone);

signals:
	void newSettings();

protected slots:
	void dlgOkClicked();
	void dlgApplyClicked();
	void dlgCancelClicked();
	void dlgDeleted();
	void dlgLCDDigitalToggled(bool);
	void dlgLCDAnalogToggled(bool);
	void dlgChooseFontButtonClicked();

protected:

	void readZoneList(const QStringList & tzDefaults);
	void getSelectedZonelist();

	ClockApplet *applet;
	KConfig *config;
	ClockType _type;
        QString _defaultTZ;
	QGuardedPtr<ClockConfDialog> confDlg;

	bool _lcdStyleDig, _lcdStyleAna,
	     _showSecsPlain,  _showSecsDig,  _showSecsAna,
	     _showDatePlain,  _showDateDig,  _showDateAna,  _showDateFuz,
	     _showFramePlain, _showFrameDig, _showFrameAna, _showFrameFuz,
	     _useColPlain,    _useColDig,    _useColAna,    _useColFuz,
	     _useColDate, _blink;

	QColor _foreColorDate, _foreColorPlain, _foreColorDig, _foreColorAna,  _foreColorFuz,
	       _backColorPlain, _backColorDig, _backColorAna,  _backColorFuz,
	       _shadowColorAna, _shadowColorDig;
	int _fuzzynessFuz, _antialiasFactorAna;
	QFont _fontDate, _fontPlain, _fontFuz;

	unsigned int _zoneIndex;
	QStringList _remotezonelist;
};

#endif
