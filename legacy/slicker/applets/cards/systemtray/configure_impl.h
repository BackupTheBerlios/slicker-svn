/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONFIGURE_IMPL_H
#define CONFIGURE_IMPL_H

#include <qobject.h>
#include <qstringlist.h>

#include <qxembed.h>

class Configure;

class ConfigureImpl : public QObject
{
	Q_OBJECT
public:
	ConfigureImpl();
	~ConfigureImpl();

	uint getMaxtraylines() { return _maxtraylines; }
	uint getMaxcontentslines() { return _maxcontentslines; }
	bool getDefIcon() { return _deficon; }
	QStringList getBackgroundnames() { return _background; }

	void displayDialog(QPtrList<QXEmbed> *tray, QPtrList<QXEmbed> *contents);

protected slots:
	void slotCancel();
	void slotOk();
	void slotRemove();
	void slotAdd();

signals:
	void newConfig();

private:
	void readConfig();
	void setConfig();
	void writeConfig();
	void applyConfig();

protected:
	Configure *ui;
	QPtrList<QXEmbed> *_tray;
	QPtrList<QXEmbed> *_contents;

	uint _maxtraylines;
	uint _maxcontentslines;
	bool _deficon;
	QStringList _background;
};

#endif
