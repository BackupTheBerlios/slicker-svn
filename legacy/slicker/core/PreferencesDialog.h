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
#ifndef _PREFERENCESDIALOG_H
#define _PREFERENCESDIALOG_H

#include <qptrlist.h>
#include <kdialogbase.h>

class ConfigModule;

class PreferencesDialog : public KDialogBase {
    Q_OBJECT

    friend class ConfigModule;

    public:
        static PreferencesDialog *instance();
        ~PreferencesDialog();
        virtual void show();

    public slots:
        void slotOk();
        void slotApply();

    private:
        PreferencesDialog();

        static PreferencesDialog *spInstance;

        QPtrList<ConfigModule> mModules;

        void add (ConfigModule *page);
        void remove(ConfigModule *page);
};

#endif // _PREFERENCESDIALOG_H
