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

#include <qlayout.h>
#include <kiconloader.h>
#include "slicker.h"
#include "PreferencesDialog.h"
#include "ConfigModule.h"

ConfigModule::ConfigModule(const QString &name, const QString &description) {
    mpFrame = PreferencesDialog::instance()->addPage(name, description);
    PreferencesDialog::instance()->add(this);
}

ConfigModule::ConfigModule(const QString &name, const QString &description,
                           const QString &icon) {

    mpFrame = PreferencesDialog::instance()->addPage(name, description,
               KGlobal::iconLoader()->loadIcon(icon, KIcon::NoGroup, KIcon::SizeMedium));
    PreferencesDialog::instance()->add(this);
}

ConfigModule::~ConfigModule() {
   PreferencesDialog::instance()->remove(this);
   delete mpFrame;
}
