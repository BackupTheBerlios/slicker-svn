/***************************************************************************
    copyright            : (C) 2003 by Markus Breitenberger
    email                : breitenberger@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _CONFIGMODULE_H
#define _CONFIGMODULE_H

#include <qwidget.h>
#include <qframe.h>

/**
    This is the base class for all configuration dialogs. All objects will
    be automatically added or removed to slicker's preferences dialog.

    @author Markus Breitenberger
*/

class ConfigModule : public QWidget {
    Q_OBJECT

    public:
        ConfigModule(const QString &name, const QString &description);
        ConfigModule(const QString &name, const QString &description,
                     const QString &icon);
        virtual ~ConfigModule();

    public slots:
        /**
            Gets called when the user wants to save the settings, so
            you should save your settings here...
        */
        virtual void save() {};

        /**
            Gets called before the configuration dialog is shown. You
            should load your settings and update your widgets in the
            dialog according to these settings.
        */
        virtual void reopen() {};

    protected:
        /**
            The parent for your configuration widget.
        */
        QFrame *mpFrame;
};

#endif // _CONFIGMODULE_H
