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

#ifndef TASKBARSECTION_H
#define TASKBARSECTION_H

#include <qobject.h>
#include "taskmanager/taskmanager.h"

class QPixmap;
class Task;
class TaskbarView;

class TaskbarSection : public QObject
{
	Q_OBJECT

 	public:
  		TaskbarSection(TaskbarView *parent, Task *task);
  		~TaskbarSection();

		QPixmap		pixmap() { return m_pix; }
		QString		label() { return m_label; }
		bool		isGroup() { return m_group; }

		Task		*task() { return m_task; }
		bool		isActive() { return m_active; }
		void		setActive(bool a) { m_active = a; }

	signals:
		void		activated(TaskbarSection *);
		
	protected slots:
		void		slotActivated();
		void		slotChanged();
		
	private:
		QPixmap		m_pix;
		QString		m_label;
		bool		m_group;
		Task		*m_task;

		bool		m_active;
		
		TaskbarView	*m_parent;
};

#endif

