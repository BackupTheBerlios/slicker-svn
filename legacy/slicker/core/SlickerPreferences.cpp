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

#include <qcheckbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <klocale.h>
#include <kdebug.h>
#include <kglobal.h>
#include "SlickerPreferences.h"
#include "PreferencesDialog.h"

SlickerPreferences::SlickerPreferences()
    : ConfigModule(i18n("Slicker"), i18n("Settings for all components of slicker"), "go") {

    mpUI = new SlickerPreferencesUI(mpFrame);
    mpConfig = KGlobal::config();
    readConfig();
}

SlickerPreferences::~SlickerPreferences() {
    delete mpUI;
}

void SlickerPreferences::save() {
    mCreateTaskbar = mpUI->mCreateTaskbar->isChecked();
    mCreateSlider = mpUI->mCreateSlider->isChecked();
    writeConfig();
}

void SlickerPreferences::reopen() {
    readConfig();
    mpUI->mCreateTaskbar->setChecked(mCreateTaskbar);
    mpUI->mCreateSlider->setChecked(mCreateSlider);
}

void SlickerPreferences::readConfig() {
    mpConfig->setGroup("Slicker");
    mCreateTaskbar = mpConfig->readBoolEntry("createTaskbar", true);
    mCreateSlider = mpConfig->readBoolEntry("createSlider", true);
}

void SlickerPreferences::writeConfig() {
    mpConfig->setGroup("Slicker");
    mpConfig->writeEntry("createTaskbar", mCreateTaskbar);
    mpConfig->writeEntry("createSlider", mCreateSlider);
    emit configChanged();
}
