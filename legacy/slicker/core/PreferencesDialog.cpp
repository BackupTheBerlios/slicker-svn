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

#include <kdebug.h>
#include <klocale.h>
#include "PreferencesDialog.h"
#include "ConfigModule.h"

PreferencesDialog* PreferencesDialog::spInstance=0;

PreferencesDialog* PreferencesDialog::instance() {
    if (spInstance) {
        return spInstance;
    }
    spInstance = new PreferencesDialog();
    return spInstance;
}

PreferencesDialog::PreferencesDialog()
    : KDialogBase(IconList, i18n("Preferences"),
                  Ok|Apply|Close, Ok, 0, 0, false) {

    resize(640, 480);
}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::add(ConfigModule *page) {
    mModules.append(page);
}

void PreferencesDialog::remove(ConfigModule *page) {
    mModules.removeRef(page);
}

void PreferencesDialog::show() {
    for (ConfigModule *i=mModules.first(); i; i=mModules.next()) {
        i->reopen();
    }
    KDialogBase::show();
}

void PreferencesDialog::slotApply() {
    for (ConfigModule *i=mModules.first(); i; i=mModules.next()) {
        i->save();
    }
}

void PreferencesDialog::slotOk() {
    slotApply();
    hide();
}

#include "PreferencesDialog.moc"
