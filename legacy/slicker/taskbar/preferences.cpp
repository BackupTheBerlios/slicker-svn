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

#include <klocale.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kdebug.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qcombobox.h>
#include <kcolorbutton.h>

#include "preferences.h"


TaskbarPreferences::TaskbarPreferences()
   : ConfigModule(i18n("Taskbar"), i18n("Taskbar settings"), "launch"),
     prefsChanged(false)
{
	kdDebug(155001) << "TaskbarPreferences::TaskbarPreferences" << endl;
	mDefaultFadeFactor = 0.3;
	mPreferencesUI = new PreferencesWidget(mpFrame);
	readConfig();
	mPreferencesUI->windowManagerCheck->setChecked(mSetWindowManager);
}

TaskbarPreferences::~TaskbarPreferences() 
{

}

void TaskbarPreferences::save()
{
	kdDebug(155001) << "TaskbarPreferenes::save" << endl;
	
	mSetWindowManager = mPreferencesUI->windowManagerCheck->isChecked();
	if (mPreferencesUI->groupTasks->currentItem() == 0) mGroupTaskMode = always;
	else if (mPreferencesUI->groupTasks->currentItem() == 1) mGroupTaskMode = taskbarFull;
	else mGroupTaskMode = never;
	
	if (mPreferencesUI->sortTasks->isChecked() && mPreferencesUI->onlyIconified->isChecked())
		mShowTaskMode = desktop_icon_tasks;
	else if (mPreferencesUI->sortTasks->isChecked()) 
		mShowTaskMode = desktop_tasks;
	else if (mPreferencesUI->onlyIconified->isChecked())
		mShowTaskMode = all_icon_tasks;
	else 
		mShowTaskMode = all_tasks;
	
	mFadeColor = mPreferencesUI->fadeColor->color();
	mTextColor = mPreferencesUI->textColor->color();
	mFadeFactor = (double) mPreferencesUI->transFactorSlider->value() / 10;
	mShowAppIcons = mPreferencesUI->showAppIcons->isChecked();
	mRightCorner = mPreferencesUI->rightCorner->isChecked();
	writeConfig();
	emit configChanged();
}

void TaskbarPreferences::readConfig()
{
	kdDebug(155001) << "TaskbarPreferences::readConfig" << endl;
	KGlobal::config()->setGroup("Taskbar");
	
	/* If prefs have been modified from the context menu, return */
	if (prefsChanged) return;
	mSetWindowManager = KGlobal::config()->readBoolEntry("SetWindowManager",false);
	mGroupTaskMode = (GroupTasks)KGlobal::config()->readNumEntry("GroupTasks",taskbarFull);
	mShowTaskMode = (ShowTasks)KGlobal::config()->readNumEntry("ShowTasks",desktop_tasks);
	mFadeColor = KGlobal::config()->readColorEntry("FadeColor",new QColor(255,255,255));
	mTextColor = KGlobal::config()->readColorEntry("TextColor",new QColor(0,0,0));
	kdDebug(155001) << endl << endl << endl << "Interestingly pointless debuggah: " << 
		mFadeColor.red() << "|" << mFadeColor.green() << "|" << 
		mFadeColor.blue() << endl;
	
	mFadeFactor = KGlobal::config()->readDoubleNumEntry("FadeFactor",0.3);
	mShowAppIcons = KGlobal::config()->readBoolEntry("ShowAppIcons", true);
	mRightCorner = KGlobal::config()->readBoolEntry("ShowAllEdges", false);
}

void TaskbarPreferences::writeConfig()
{
	kdDebug(155001) << "TaskbarPreferences::writeConfig" << endl;
	prefsChanged = false;
	KGlobal::config()->setGroup("Taskbar");
	
	KGlobal::config()->writeEntry("SetWindowManager", mSetWindowManager);
	KGlobal::config()->writeEntry("ShowTasks", mShowTaskMode);
	KGlobal::config()->writeEntry("GroupTasks", mGroupTaskMode);
	KGlobal::config()->writeEntry("FadeColor", mFadeColor);
	KGlobal::config()->writeEntry("TextColor", mTextColor);
	KGlobal::config()->writeEntry("FadeFactor", mFadeFactor);
	KGlobal::config()->writeEntry("ShowAppIcons", mShowAppIcons);
	KGlobal::config()->writeEntry("ShowAllEdges", mRightCorner);
}

void TaskbarPreferences::reopen()
{
	kdDebug(155001) << "TaskbarPreferences::reopen" << endl;
	readConfig();
	mPreferencesUI->windowManagerCheck->setChecked(mSetWindowManager);
	bool onlyIconsCheck = false;
	bool sortTasksCheck = false;
	if (mShowTaskMode == desktop_tasks)	 { sortTasksCheck = true; }
	if (mShowTaskMode == all_icon_tasks)	 { onlyIconsCheck = true; }
	if (mShowTaskMode == desktop_icon_tasks) { onlyIconsCheck = true; sortTasksCheck = true; }
	
	mPreferencesUI->transFactorSlider->setValue((int)(mFadeFactor*10));
	mPreferencesUI->sortTasks->setChecked(sortTasksCheck);
	mPreferencesUI->onlyIconified->setChecked(onlyIconsCheck);
	
	mPreferencesUI->fadeColor->setColor(mFadeColor);
	mPreferencesUI->textColor->setColor(mTextColor);
	mPreferencesUI->fadeColor->repaint();
	mPreferencesUI->textColor->repaint();
	mPreferencesUI->showAppIcons->setChecked(mShowAppIcons);
	mPreferencesUI->rightCorner->setChecked(mRightCorner);
}

#include "preferences.moc"

