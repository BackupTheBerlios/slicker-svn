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

#ifndef TASKBARPREFERENCES_H
#define TASKBARPREFERENCES_H

#include "ConfigModule.h"
#include "preferencesUI.h"

#include "taskbarview.h"
#include "taskbar.h"

#include <qcolor.h>

enum Edge { edge_left, edge_right, edge_top, edge_bottom };
enum ShowTasks { all_tasks, desktop_tasks, all_icon_tasks, desktop_icon_tasks };
enum GroupTasks { taskbarFull, always, never };


class TaskbarPreferences : public ConfigModule {

  Q_OBJECT
	
   public:
      friend class TaskbarView;
      friend class Taskbar;
      
      TaskbarPreferences();
      virtual ~TaskbarPreferences();

      virtual void save();
      virtual void reopen();

      void readConfig();
      void writeConfig();

      bool setWindowManager() { return mSetWindowManager; }
      ShowTasks showTaskMode() { return mShowTaskMode; }
      GroupTasks groupTaskMode() { return mGroupTaskMode; }
      QColor fadeColor() { return mFadeColor; }
      QColor textColor() { return mTextColor; }
      double fadeFactor() { return mFadeFactor; }

   signals:
      void configChanged();
   private:
      bool		mSetWindowManager;
      ShowTasks		mShowTaskMode;
      GroupTasks	mGroupTaskMode;
      QColor		mFadeColor;
      QColor		mTextColor;
      double		mFadeFactor;
      double		mDefaultFadeFactor;
      bool		mShowAppIcons;
      bool		mRightCorner;
      
      Edge		mEdge;
      int		mEdgeSector;
      
      /* TaskbarView changes this when settings change from
       * Context menu->Taskbar Preferences menu. 
       */
      bool		prefsChanged;
      
      PreferencesWidget *mPreferencesUI; 
};

#endif // _TASKBARPREFERENCES_H
