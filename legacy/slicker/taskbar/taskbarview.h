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

#ifndef TASKBARVIEW_H
#define TASKBARVIEW_H

#include <qwidget.h>
#include <qptrlist.h>
#include <qimage.h>
#include <kpopupmenu.h>
#include <kwindowlistmenu.h>
#include "taskmanager/taskmanager.h"
#include "taskmanager/tasklmbmenu.h"

class QColor;
class KRootPixmap;
class TaskbarSection;
class Taskbar;
class Task;
class TaskbarPreferences;

typedef QPtrList<TaskbarSection> Sections;

/* Basic enum to hold edge of screen */

class TaskMenuCombo {
	public:
	TaskMenuCombo(WId win, TaskManager *parent, int menu_id);
	TaskMenuCombo() { }
	bool operator==(const TaskMenuCombo&);
	Task	*task;
	int	id;
};

typedef QValueList<TaskMenuCombo> TaskMenuList;

class TaskbarView : public QWidget
{
	Q_OBJECT
	
 	public:
  		TaskbarView(Taskbar *parent);
  		~TaskbarView();
		

		TaskbarSection *sectionAt(int x);
		Taskbar *parent();
		void setWindowManager(bool);
		
	public slots:
		void	addSection(TaskbarSection *s);
		
	protected:
		friend class TaskbarSection;
		/* Preferences (read from parent()->preferences()) */
		TaskbarPreferences *preferences();
		
		/* Events */
		void	paintEvent(QPaintEvent *ev);
		void	mousePressEvent(QMouseEvent *ev);
		void	mouseReleaseEvent(QMouseEvent *ev);
		void	mouseMoveEvent(QMouseEvent *ev);
		
		/* Taskbar drawing related */
		bool	shouldDrawSection(TaskbarSection *);
		void	drawSection(QPainter *p, TaskbarSection *s, int x, int offset);
		void	drawSeparator(QPainter *p, int x);
		QPixmap	highlight(int x, int w, double);
		void	updateSection(TaskbarSection *s);
		QPixmap	fadeOutText(QPixmap*, QString, int);
		
		
		/* Taskbar setup (Mask/Size/Blending) */
		void	setupMask();
		QRegion edgeMask(int x, int y, int w, int h, int vert, int horiz);
		int	setupSize(int sections);
		Sections selectTasks(Sections);
		
		/* Context/Drag button */
		QRect	downArrowRect();
		bool	inDownArrowButton(int x, int y);
		
		/* Taskbar moving */
		void	changeOrientation();
		
		/* Context menux */
		void initDropDownMenu();
		void prepareWindowListMenu(KWindowListMenu *);
		
	protected slots:
		/* Drawing/updating taskbar */
		void	setupBlending();
		void	refreshTaskbar();
		void	refreshContextMenu();	
		
		/* Taskbar slots */
		void	slotNewTask(Task *t);
		void	slotRemoveTask(Task *t);
		void	slotActivated(TaskbarSection *);
		void	slotDesktopChanged(int desktop);
		void	slotConfigChanged();
		
		/* Context menu settings slots */
		void	tasksAllDesktops(int id);
		void	tasksCurrentDesktop(int id);
		void	tasksIAllDesktops(int id);
		void	tasksICurrentDesktop(int id);
		void	groupTaskbarFull(int id);
		void	groupTaskbarAlways(int id);
		void	setTranslucency(int factor);
		void	selectBgColor(int id);
		void	showSlickerConfig();
	private:
		/* Management */
		Taskbar		*m_parent;
	
		/* Content */
		Sections	m_sections;	/* All tasks */
		Sections	m_selsects;	/* Only selected tasks (Use this) */
		TaskbarSection	*m_active;	

		/* Size settings */
		int		m_defaultSize;
		int		m_userSize;
		int		m_hintSize;
		int		m_heightSize;

		/* Transparency/Color settings */
		KRootPixmap	*m_bg;
		QImage	blendGradient;
		
		/* Task presentation settings (Selective tasks/Grouping) */
		int		m_eshowTasksItem;
		int		m_egroupItem;
		
		/* Context menu related */
		KPopupMenu	taskbarPrefsMenu;
		KPopupMenu	taskbarTransPrefs;
		int		bgColorSelect_id;
		
		/* Taskbar moving */
		bool		_drag;
		bool		_drag_used;
};


#endif

