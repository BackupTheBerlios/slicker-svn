/************************************************************

Copyright (c) 1996-2002 the kicker authors. See file AUTHORS.

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kcolorbutton.h>
#include "conf.h"
#include "settings.h"

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kfontdialog.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klistview.h>
#include <kstandarddirs.h>
#include <kstringhandler.h>

#include <qcheckbox.h>
#include <qcolor.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qfont.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qradiobutton.h>
#include <qregexp.h>
#include <qslider.h>
#include <qstringlist.h>
#include <qtabwidget.h>
#include <qtooltip.h>
#include <qdatetime.h>

#include <stdlib.h>
#include <time.h>

static void setCurrentTimeZone(const QString& zone)
{
  // set TZ variable for this program
  // Ever occuring error:
  // TZ="" means UTC
  // TZ unset means localtime
  // When changing code remember that.
  if (!zone.isEmpty()) {
    ::putenv(strdup(QString("TZ=" + zone).latin1()));
  } else {
    ::unsetenv("TZ");
  }
  tzset();
}

ClockSettings::ClockSettings(ClockApplet* app, KConfig* conf)
    : applet(app), config(conf), confDlg(0)
{
    _defaultTZ = ::getenv("TZ");
    tzset();

    config->setGroup("General");

    QString s = conf->readEntry("Type", "Analog");
    if (s == "Plain")
        _type = Plain;
    else if (s == "Digital")
        _type = Digital;
    else if (s == "Analog")
        _type = Analog;
    else
        _type = Fuzzy;

    /* default displayable timezones */
    QString tzList = config->readEntry("RemoteZones",
		    "America/Los_Angeles,America/New_York,"
		    "Australia/Sydney,Asia/Tokyo,"
		    "Europe/Berlin,Europe/London,Europe/Moscow");
    _remotezonelist = QStringList::split(QRegExp(","), tzList);
    _zoneIndex = config->readNumEntry("Initial_TZ", 0);
    if (_zoneIndex > _remotezonelist.count())
	    _zoneIndex = 0;

    config->setGroup("Date");
    _useColDate = config->readBoolEntry("Use_Custom_Colors",false);
    _foreColorDate = config->readColorEntry("Foreground_Color", &KApplication::palette().active().text());
    QFont defFont=KGlobalSettings::generalFont();
    defFont.setPixelSize(10);
    _fontDate = config->readFontEntry("Font",&defFont);

    config->setGroup("Plain");
    _useColPlain = config->readBoolEntry("Use_Custom_Colors",false);
    _foreColorPlain = config->readColorEntry("Foreground_Color", &KApplication::palette().active().text());
    _backColorPlain = config->readColorEntry("Background_Color", &KApplication::palette().active().background());
    _showSecsPlain = config->readBoolEntry("Show_Seconds",false);
    _showDatePlain = config->readBoolEntry("Show_Date",true);
    _showFramePlain= config->readBoolEntry("Show_Frame", true);
    defFont = KGlobalSettings::generalFont();
    defFont.setPixelSize(19);
    defFont.setBold(true);
    _fontPlain = config->readFontEntry("Font",&defFont);

    config->setGroup("Digital");
    _lcdStyleDig = config->readBoolEntry("LCD_Style",false);
    _useColDig = config->readBoolEntry("Use_Custom_Colors",false);
    _foreColorDig = config->readColorEntry("Foreground_Color", &KApplication::palette().active().text());
    _shadowColorDig = config->readColorEntry("Shadow_Color", &KApplication::palette().active().mid());
    _backColorDig = config->readColorEntry("Background_Color", &KApplication::palette().active().background());
    _showSecsDig = config->readBoolEntry("Show_Seconds",false);
    _showDateDig = config->readBoolEntry("Show_Date",true);
    _showFrameDig = config->readBoolEntry("Show_Frame", true);
    _blink = config->readBoolEntry("Blink",false);

    config->setGroup("Analog");
    _lcdStyleAna = config->readBoolEntry("LCD_Style",true);
    _useColAna = config->readBoolEntry("Use_Custom_Colors",false);
    _foreColorAna = config->readColorEntry("Foreground_Color", &KApplication::palette().active().text());
    _shadowColorAna = config->readColorEntry("Shadow_Color", &KApplication::palette().active().mid());
    _backColorAna = config->readColorEntry("Background_Color", &KApplication::palette().active().background());
    _showSecsAna = config->readBoolEntry("Show_Seconds",true);
    _showDateAna = config->readBoolEntry("Show_Date",false);
    _antialiasFactorAna = config->readNumEntry("Antialias", 1);
    _showFrameAna = config->readBoolEntry("Show_Frame", false);

    config->setGroup("Fuzzy");
    _useColFuz = config->readBoolEntry("Use_Custom_Colors",false);
    _foreColorFuz = config->readColorEntry("Foreground_Color", &KApplication::palette().active().text());
    _backColorFuz = config->readColorEntry("Background_Color", &KApplication::palette().active().background());
    _showDateFuz = config->readBoolEntry("Show_Date", true);
    defFont=KGlobalSettings::generalFont();
    _fontFuz = config->readFontEntry("Font",&defFont);
    _fuzzynessFuz = config->readNumEntry("Fuzzyness", 0);
    _showFrameFuz = config->readBoolEntry("Show_Frame", true);

}

ClockSettings::~ClockSettings()
{
    delete confDlg;
}

QString ClockSettings::zone(int z) const
{
    return (z==0 ? _defaultTZ : _remotezonelist[z-1]);
}

int ClockSettings::calc_TZ_offset(const QString& zone)
{
  setCurrentTimeZone(zone);
  QDateTime t1( QDateTime::currentDateTime() );
  setCurrentTimeZone(_defaultTZ);
  return QDateTime::currentDateTime().secsTo(t1);
}

void ClockSettings::readZoneList( const QStringList & tzDefaults )
{
  QFile f("/usr/share/zoneinfo/zone.tab");

  if (!f.open(IO_ReadOnly)) {
	kdDebug() << "Can't open zone.tab" << endl;
	confDlg->tzListView->setEnabled(false);
	QToolTip::add(confDlg->tzListView, i18n("Can't generate time zone list"));
	return;
  }

  QTextStream str(&f);
  QRegExp rx("[ \t]");
  QMap<QString, QListViewItem*> KontinentMap;
  QListViewItem *Kontinent;

  confDlg->tzListView->setRootIsDecorated(true);

  while (!str.atEnd())
  {
    QString line = str.readLine();
    if (line.isEmpty() || '#' == line[0])
	continue;

    QStringList tokens = KStringHandler::perlSplit(rx, line, 4);
    if (tokens.count() < 3)
	continue;

    QString tzName = tokens[2];
    tokens[2] = i18n(tokens[2].utf8());
    tokens[2].replace(QRegExp("_"), " ");

    QStringList KontCity = QStringList::split("/", tokens[2]);
    Kontinent = KontinentMap[KontCity[0]];
    if (!Kontinent) {
	KontinentMap[KontCity[0]] = new QListViewItem(confDlg->tzListView, KontCity[0]);
	Kontinent = KontinentMap[KontCity[0]];
	Kontinent->setExpandable(true);
    }

    QCheckListItem *li = new QCheckListItem(Kontinent, KontCity[1], QCheckListItem::CheckBox);
    li->setText(1, i18n(tokens[3].utf8()));
    li->setText(2, tzName); /* store complete path in ListView */

    if (tzDefaults.findIndex(tzName) != -1)
       li->setOn(true);

    // locate the flag from /l10n/%1/flag.png
    // if not available select default "C" flag
    QString flag = locate( "locale", QString("l10n/%1/flag.png").arg(tokens[0].lower()) );
    if (!QFile::exists(flag))
       flag = locate( "locale", "l10n/C/flag.png" );
    if (QFile::exists(flag))
       li->setPixmap(0, QPixmap(flag));
  }
}

void ClockSettings::getSelectedZonelist()
{
     _remotezonelist.clear();

     /* loop through all entries */
     QListViewItem *root = confDlg->tzListView->firstChild();
     while (root) {
	if (root->firstChild()) {
		root = root->firstChild();
		continue;
	}

	QCheckListItem *cl = (QCheckListItem*) root;
	if (cl->isOn()) {
		_remotezonelist.append(cl->text(2));
		kdWarning() << cl->text(2) << endl;
	}

	if (root->nextSibling()) {
		root = root->nextSibling();
		continue;
	}
	root = root->parent();
	if (root)
		root = root->nextSibling();
     }
}

void ClockSettings::nextZone()
{
    if (++_zoneIndex > _remotezonelist.count())
	_zoneIndex = 0;
}

void ClockSettings::prevZone()
{
    if (_zoneIndex == 0)
	_zoneIndex = _remotezonelist.count();
    else
	--_zoneIndex;
}

void ClockSettings::writeSettings()
{
//    clock_panelapplet ?
    config->setGroup("General");

    switch (_type) {
        case Plain:
            config->writeEntry("Type", "Plain");
            break;
        case Digital:
            config->writeEntry("Type", "Digital");
            break;
        case Analog:
            config->writeEntry("Type", "Analog");
            break;
        case Fuzzy:
            config->writeEntry("Type", "Fuzzy");
            break;
    }

    config->writeEntry("RemoteZones", _remotezonelist.join(","));
    config->writeEntry("Initial_TZ",_zoneIndex);

    config->setGroup("Date");
    config->writeEntry("Use_Custom_Colors",_useColDate);
    config->writeEntry("Foreground_Color",_foreColorDate);
    config->writeEntry("Font", _fontDate);

    config->setGroup("Plain");
    config->writeEntry("Use_Custom_Colors",_useColPlain);
    config->writeEntry("Foreground_Color",_foreColorPlain);
    config->writeEntry("Background_Color",_backColorPlain);
    config->writeEntry("Show_Seconds",_showSecsPlain);
    config->writeEntry("Show_Date",_showDatePlain);
    config->writeEntry("Font", _fontPlain);
    config->writeEntry("Show_Frame", _showFramePlain);

    config->setGroup("Digital");
    config->writeEntry("LCD_Style",_lcdStyleDig);
    config->writeEntry("Use_Custom_Colors",_useColDig);
    config->writeEntry("Foreground_Color",_foreColorDig);
    config->writeEntry("Shadow_Color",_shadowColorDig);
    config->writeEntry("Background_Color",_backColorDig);
    config->writeEntry("Show_Seconds",_showSecsDig);
    config->writeEntry("Show_Date",_showDateDig);
    config->writeEntry("Blink",_blink);
    config->writeEntry("Show_Frame", _showFrameDig);

    config->setGroup("Analog");
    config->writeEntry("LCD_Style",_lcdStyleAna);
    config->writeEntry("Use_Custom_Colors",_useColAna);
    config->writeEntry("Foreground_Color",_foreColorAna);
    config->writeEntry("Shadow_Color",_shadowColorAna);
    config->writeEntry("Background_Color",_backColorAna);
    config->writeEntry("Show_Seconds",_showSecsAna);
    config->writeEntry("Show_Date",_showDateAna);
    config->writeEntry("Antialias", _antialiasFactorAna);
    config->writeEntry("Show_Frame", _showFrameAna);

    config->setGroup("Fuzzy");
    config->writeEntry("Use_Custom_Colors",_useColFuz);
    config->writeEntry("Foreground_Color", _foreColorFuz);
    config->writeEntry("Background_Color", _backColorFuz);
    config->writeEntry("Show_Date", _showDateFuz);
    config->writeEntry("Font", _fontFuz);
    config->writeEntry("Fuzzyness", _fuzzynessFuz);
    config->writeEntry("Show_Frame", _showFrameFuz);

    config->sync();
}


void ClockSettings::openPreferences(int startindex)
{
    if (confDlg) {
        confDlg->raise();
        return;
    }

    confDlg = new ClockConfDialog( NULL, 0, FALSE, WDestructiveClose );
    confDlg->setIcon(SmallIcon("date"));

    connect(confDlg->buttonOk, SIGNAL(clicked()), this, SLOT(dlgOkClicked()));
    connect(confDlg->buttonApply, SIGNAL(clicked()), this, SLOT(dlgApplyClicked()));
    connect(confDlg->buttonCancel, SIGNAL(clicked()), this, SLOT(dlgCancelClicked()));
    connect(confDlg, SIGNAL( destroyed() ), SLOT( dlgDeleted() ));
    connect(confDlg->chooseFontDate, SIGNAL(clicked()), this, SLOT(dlgChooseFontButtonClicked()));
    connect(confDlg->chooseFontPlain, SIGNAL(clicked()), this, SLOT(dlgChooseFontButtonClicked()));
    connect(confDlg->chooseFontFuzzy, SIGNAL(clicked()), this, SLOT(dlgChooseFontButtonClicked()));

    readZoneList(_remotezonelist);

    confDlg->clockCombo->setCurrentItem(_type - 10);
    confDlg->useColorsDate->setChecked(_useColDate);
//    confDlg->foregroundDate->setColor(_foreColorDate);
    confDlg->sampleDate->setFont(_fontDate);

    confDlg->showDatePlain->setChecked(_showDatePlain);
    confDlg->showSecsPlain->setChecked(_showSecsPlain);
    confDlg->useColorsPlain->setChecked(_useColPlain);
//    confDlg->foregroundPlain->setColor(_foreColorPlain);
//    confDlg->backgroundPlain->setColor(_backColorPlain);
    confDlg->samplePlain->setFont(_fontPlain);

    confDlg->showDateDigital->setChecked(_showDateDig);
    confDlg->showSecsDigital->setChecked(_showSecsDig);
    confDlg->blinkingDigital->setChecked(_blink);
    confDlg->useColorsDigital->setChecked(_useColDig);
//    confDlg->foregroundDigital->setColor(_foreColorDig);
//    confDlg->shadowDigital->setColor(_shadowColorDig);
//    confDlg->backgroundDigital->setColor(_backColorDig);
    connect(confDlg->lcdDigital, SIGNAL(toggled(bool)), SLOT(dlgLCDDigitalToggled(bool)));
    dlgLCDDigitalToggled(_lcdStyleDig);
    confDlg->lcdDigital->setChecked(_lcdStyleDig);
    confDlg->plainDigital->setChecked(!_lcdStyleDig);

    confDlg->showDateAnalog->setChecked(_showDateAna);
    confDlg->showSecsAnalog->setChecked(_showSecsAna);
    confDlg->useColorsAnalog->setChecked(_useColDig);
//    confDlg->foregroundAnalog->setColor(_foreColorAna);
//    confDlg->shadowAnalog->setColor(_shadowColorAna);
//    confDlg->backgroundAnalog->setColor(_backColorAna);
    connect(confDlg->lcdAnalog, SIGNAL(toggled(bool)), SLOT(dlgLCDAnalogToggled(bool)));
    dlgLCDAnalogToggled(_lcdStyleAna);
    confDlg->lcdAnalog->setChecked(_lcdStyleAna);
    confDlg->plainAnalog->setChecked(!_lcdStyleAna);

    switch (_antialiasFactorAna)
    {
       case(1): { confDlg->antialiasAnalog->setCurrentItem(0); break; }
       case(2): { confDlg->antialiasAnalog->setCurrentItem(1); break; }
       case(4): { confDlg->antialiasAnalog->setCurrentItem(2); break; }
       default: { confDlg->antialiasAnalog->setCurrentItem(0); break; }
    }

    confDlg->drawFramePlain->setChecked(_showFramePlain);
    confDlg->drawFrameDigital->setChecked(_showFrameDig);
    confDlg->drawFrameAnalog->setChecked(_showFrameAna);
    confDlg->drawFrameFuzzy->setChecked(_showFrameFuz);

    confDlg->showDateFuzzy->setChecked(_showDateFuz);
    confDlg->fuzzyness->setValue(_fuzzynessFuz);
    confDlg->useColorsFuzzy->setChecked(_useColFuz);
//    confDlg->foregroundFuzzy->setColor(_foreColorFuz);
//    confDlg->backgroundFuzzy->setColor(_backColorFuz);
    confDlg->sampleFuzzy->setFont(_fontFuz);

    int cur;
    switch (_type) {
	case Plain:	cur = 2; break;
	case Digital:	cur = 3; break;
	case Analog:	cur = 4; break;
	case Fuzzy:	cur = 5; break;
	default:	cur = 0;
    }
    if (startindex>=0)
	    cur = startindex;
    confDlg->TabWidget2->setCurrentPage(cur);

    confDlg->show();
}


void ClockSettings::setType(ClockType type)
{
    _type = type;
    if (confDlg)
        confDlg->clockCombo->setCurrentItem(_type - 10);
}


bool ClockSettings::showSeconds()
{
    switch (_type) {
        case Plain:
            return _showSecsPlain;
        case Digital:
            return _showSecsDig;
        case Analog:
            return _showSecsAna;
        default:
            return false;
    }
}


bool ClockSettings::showDate()
{
    switch (_type) {
        case Plain:
            return _showDatePlain;
        case Digital:
            return _showDateDig;
        case Analog:
            return _showDateAna;
        default:         // fuzzy clock
            return _showDateFuz;
    }
}

bool ClockSettings::showFrame()
{
    switch (_type) {
        case Plain:
            return _showFramePlain;
        case Digital:
            return _showFrameDig;
        case Analog:
            return _showFrameAna;
        default:         // fuzzy clock
            return _showFrameFuz;
    }
}


QColor ClockSettings::foreColor()
{
    switch (_type) {
        case Plain:
            if (_useColPlain)
                return _foreColorPlain;
	    break;
        case Digital:
            if (_useColDig)
                return _foreColorDig;
	    break;
        case Analog:
            if (_useColAna)
                return _foreColorAna;
	    break;
        default:       // fuzzy clock
            if (_useColFuz)
                return _foreColorFuz;
	    break;
    }
    return KApplication::palette().active().text();
}


QColor ClockSettings::shadowColor()
{
    if (_type == Digital) {
        if (_useColDig)
            return _shadowColorDig;
    } else {
        if (_useColAna)
            return _shadowColorAna;
    }
    return KApplication::palette().active().mid();
}


QColor ClockSettings::backColor()
{
    switch (_type) {
        case Plain:
            if (_useColPlain)
                return _backColorPlain;
	    break;
        case Digital:
            if (_useColDig)
                return _backColorDig;
	    break;
        case Analog:
            if (_useColAna)
                return _backColorAna;
	    break;
        default:      // fuzzy clock
            if (_useColFuz)
                return _backColorFuz;
	    break;
    }
    return KApplication::palette().active().background();
}


QColor ClockSettings::dateForeColor()
{
    if (_useColDate)
        return _foreColorDate;
    else
        return KApplication::palette().active().text();
}


void ClockSettings::dlgOkClicked()
{
    dlgApplyClicked();
    delete confDlg;
}


void ClockSettings::dlgApplyClicked()
{
    getSelectedZonelist();
    _zoneIndex = 0;

    _type = (ClockType)(confDlg->clockCombo->currentItem() + 10);
    printf("Type is %d\n", _type);
    _useColDate = confDlg->useColorsDate->isChecked();
//    _foreColorDate = confDlg->foregroundDate->color();
    _fontDate = confDlg->sampleDate->font();

    _showDatePlain = confDlg->showDatePlain->isChecked();
    _showSecsPlain = confDlg->showSecsPlain->isChecked();
    _showFramePlain= confDlg->drawFramePlain->isChecked();
    _useColPlain = confDlg->useColorsPlain->isChecked();
//    _foreColorPlain = confDlg->foregroundPlain->color();
//    _backColorPlain = confDlg->backgroundPlain->color();
    _fontPlain = confDlg->samplePlain->font();

    _showDateDig = confDlg->showDateDigital->isChecked();
    _showSecsDig = confDlg->showSecsDigital->isChecked();
    _blink = confDlg->blinkingDigital->isChecked();
    _showFrameDig = confDlg->drawFrameDigital->isChecked();
    _lcdStyleDig = confDlg->lcdDigital->isChecked();
    _useColDig = confDlg->useColorsDigital->isChecked();
//    _foreColorDig = confDlg->foregroundDigital->color();
//    _shadowColorDig = confDlg->shadowDigital->color();
//    _backColorDig = confDlg->backgroundDigital->color();

    _showDateAna = confDlg->showDateAnalog->isChecked();
    _showSecsAna = confDlg->showSecsAnalog->isChecked();
    _lcdStyleAna = confDlg->lcdAnalog->isChecked();
    _showFrameAna = confDlg->drawFrameAnalog->isChecked();
    _useColAna = confDlg->useColorsAnalog->isChecked();
//    _foreColorAna = confDlg->foregroundAnalog->color();
//    _shadowColorAna = confDlg->shadowAnalog->color();
//    _backColorAna = confDlg->backgroundAnalog->color();

    switch (confDlg->antialiasAnalog->currentItem())
    {
        case(0): { _antialiasFactorAna = 1; break; }
        case(1): { _antialiasFactorAna = 2; break; }
        case(2): { _antialiasFactorAna = 4; break; }
        default: { _antialiasFactorAna = 1; break; }
    }

    _showDateFuz = confDlg->showDateFuzzy->isChecked();
    _fuzzynessFuz = confDlg->fuzzyness->value();
    _showFrameFuz = confDlg->drawFrameFuzzy->isChecked();
    _useColFuz = confDlg->useColorsFuzzy->isChecked();
//    _foreColorFuz = confDlg->foregroundFuzzy->color();
//    _backColorFuz = confDlg->backgroundFuzzy->color();
    _fontFuz = confDlg->sampleFuzzy->font();

    writeSettings();
    emit(newSettings());
}


void ClockSettings::dlgCancelClicked()
{
    delete confDlg;
}


void ClockSettings::dlgDeleted()
{
    confDlg = 0;
}


void ClockSettings::dlgLCDDigitalToggled(bool b)
{
    bool c = !b && confDlg->useColorsDigital->isChecked();
    confDlg->useColorsDigital->setEnabled(!b);
    confDlg->foregroundDigitalLabel->setEnabled(c);
    confDlg->foregroundDigital->setEnabled(c);
    confDlg->shadowDigitalLabel->setEnabled(c);
    confDlg->shadowDigital->setEnabled(c);
    confDlg->backgroundDigitalLabel->setEnabled(c);
    confDlg->backgroundDigital->setEnabled(c);
}


void ClockSettings::dlgLCDAnalogToggled(bool b)
{
    bool c = !b && confDlg->useColorsDigital->isChecked();
    confDlg->useColorsAnalog->setEnabled(!b);
    confDlg->foregroundAnalogLabel->setEnabled(c);
    confDlg->foregroundAnalog->setEnabled(c);
    confDlg->shadowAnalogLabel->setEnabled(c);
    confDlg->shadowAnalog->setEnabled(c);
    confDlg->backgroundAnalogLabel->setEnabled(c);
    confDlg->backgroundAnalog->setEnabled(c);
}


void ClockSettings::dlgChooseFontButtonClicked()
{
    const QObject *button=sender();
    KFontDialog *fd = new KFontDialog(confDlg, "Font Dialog", false, true);

    if (button==confDlg->chooseFontDate)
      fd->setFont(confDlg->sampleDate->font());
    else if (button==confDlg->chooseFontPlain)
      fd->setFont(confDlg->samplePlain->font());
    else
      fd->setFont(confDlg->sampleFuzzy->font());

    if (fd->exec() == KFontDialog::Accepted) {
        if (button==confDlg->chooseFontDate) {
            _fontDate = fd->font();
            confDlg->sampleDate->setFont(_fontDate);
        } else if (button==confDlg->chooseFontPlain) {
            _fontPlain = fd->font();
            confDlg->samplePlain->setFont(_fontPlain);
        } else {
            _fontFuz = fd->font();
            confDlg->sampleFuzzy->setFont(_fontFuz);
        }
    }

    delete fd;
}

#include "settings.moc"
