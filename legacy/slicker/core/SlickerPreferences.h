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
#ifndef _SLICKERPREFERENCES_H
#define _SLICKERPREFERENCES_H

#include <kconfig.h>
#include "ConfigModule.h"
#include "SlickerPreferencesUI.h"

class Slicker;

class SlickerPreferences : public ConfigModule {
    friend class Slicker;

    Q_OBJECT

    public:
        SlickerPreferences();
        virtual ~SlickerPreferences();

        void readConfig();
        void writeConfig();
        virtual void save();
        virtual void reopen();

    signals:
        void configChanged();

    private:
        SlickerPreferencesUI *mpUI;
        KConfig *mpConfig;

        bool mCreateTaskbar;
        bool mCreateSlider;
};

#endif // _SLICKERPREFERENCES_H
