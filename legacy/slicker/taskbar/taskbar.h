/*****************************************************************

Copyright (c) 2003 the slicker authors. See file AUTHORS.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#ifndef TASKBAR_H
#define TASKBAR_H

#include <qobject.h>
#include "preferences.h"
#include "taskmanager/taskmanager.h"

class TaskbarView;

class Taskbar : public QObject
{
	Q_OBJECT
	
 	public:
  		Taskbar();
  		~Taskbar();

		TaskManager		*manager() { return m_manager; }
		TaskbarPreferences 	*preferences() { return mpPreferences; }	

	public slots:
		void configChanged();	
	private:
                TaskbarPreferences	*mpPreferences;
		TaskManager		*m_manager;
                TaskbarView		*mpView;
};

#endif
