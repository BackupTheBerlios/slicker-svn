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

#ifndef CONFIGDIALOG_IMPL_H
#define CONFIGDIALOG_IMPL_H

#include <qobject.h>

class Configdialog;

class ConfigDialogImpl : public QObject
{
    Q_OBJECT
public:
    ConfigDialogImpl();
    ~ConfigDialogImpl();
    
    bool getHost() { return _host; }
    bool getUser() { return _user; }
    bool getKDE() { return _kde; }

    void displayDialog();

protected slots:
    void slotCancel();
    void slotOk();

signals:
    void newSettings();

private:
    void readConfig();
    void setConfig();
    void writeConfig();
    void applyConfig();

protected:
    Configdialog *ui;

    bool _host, _user, _kde;

};

#endif
