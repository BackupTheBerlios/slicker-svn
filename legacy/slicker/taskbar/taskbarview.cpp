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

#include <qregion.h>
#include <qtimer.h>
#include <qfont.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qsizepolicy.h>
#include <qstyle.h>
#include <qpaintdevice.h>
#include <qimage.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <kglobalsettings.h>

#include <kdebug.h>
#include <kwin.h>
#include <krootpixmap.h>
#include <kpixmapeffect.h>
#include <kimageeffect.h>
#include <kpixmap.h>
#include <kiconloader.h>
#include <kcolordialog.h>

#include "taskmanager/taskmanager.h"
#include "taskmanager/taskrmbmenu.h"
#include "taskbar.h"
#include "taskbarsection.h"
#include "taskbarview.h"
#include "PreferencesDialog.h"

#include "../core/PreferencesDialog.h"

#define TaskFilterID 1000

using namespace std;

static QStringList debug_items;

bool debugItemDone(QString item)
{
	if (debug_items.findIndex(item) != -1) return false;
	else debug_items.append(item);
	return true;
}

QString debugHeader()
{
	return "\n*Debug Information*";
}

/* --------------------- TaskMenuCombo --------------------- */ 

TaskMenuCombo::TaskMenuCombo(WId win, TaskManager *parent, int menu_id)
{ 
	task = new Task(win, parent);
	id = menu_id;
}

bool TaskMenuCombo::operator==(const TaskMenuCombo &compare) { 
	if (task->window() == compare.task->window() && id == compare.id) return true;
	else return false;
}

/* --------------------- TaskbarView --------------------- */ 

TaskbarView::TaskbarView(Taskbar *parent)
 : QWidget(NULL, "TBW",  Qt::WStyle_Customize | 
    Qt::WDestructiveClose | Qt::WStyle_StaysOnTop | 
    Qt::WStyle_NoBorder)
{
	m_parent = parent;	
	m_active = 0;
	kdDebug(155001) << "Taskbar::TaskbarUI()" << endl;

	KWin::setOnAllDesktops(winId(), true);
	KWin::setState(winId(), NET::StaysOnTop | NET::SkipTaskbar | NET::SkipPager);
	
	m_defaultSize = 120;
	m_userSize = 120;
	m_hintSize = 24;
	m_heightSize = 24;
	
	if (preferences()->mRightCorner) move(0, 0);
	else move(0-(downArrowRect().width()+1), 0);
	
	resize(m_hintSize, m_heightSize);
	
	if (preferences()->setWindowManager())
		setWindowManager(true);
	
		
	/* When rawler has his handy-dandy edgecomponent done
	   we will implement the edges. until then: we ain't
	   goin nowhere :) */
	preferences()->mEdge = edge_top;
	preferences()->mEdgeSector = 1;
	setBaseSize(m_hintSize, 20);
	setupMask();

	m_bg = new KRootPixmap(this);

	_drag = false;
	
	QTimer::singleShot(0, this, SLOT(setupBlending()));
	repaint();
	
	initDropDownMenu();
	
	TaskList tl = parent->manager()->tasks();
	Task *task;
	
	for(task = tl.first(); task; task = tl.next())
		addSection(new TaskbarSection(this, task));
	
	/* Task add/remove slots */
	connect(parent->manager(), SIGNAL(taskAdded(Task *)), this, SLOT(slotNewTask(Task *)));
	connect(parent->manager(), SIGNAL(taskRemoved(Task *)), this, SLOT(slotRemoveTask(Task *)));
	connect(parent->manager(), SIGNAL(desktopChanged(int)), this, SLOT(slotDesktopChanged(int)));
	connect(parent->preferences(), SIGNAL(configChanged()), SLOT(slotConfigChanged()));
	
	refreshTaskbar();
}

Taskbar *TaskbarView::parent()
{
	return (Taskbar *)m_parent;
}

TaskbarPreferences *TaskbarView::preferences()
{
	return ((Taskbar *)parent())->preferences();
}


/* ----------- Task activation ----------- */ 


void TaskbarView::slotActivated(TaskbarSection *s)
{
	if(m_active)
	{
		m_active->setActive(false);
		updateSection(m_active);
	}

	m_active = s;
	updateSection(s);
	if (preferences()->mShowTaskMode == all_icon_tasks || 
	    preferences()->mShowTaskMode == desktop_icon_tasks) refreshTaskbar();
}

void TaskbarView::slotDesktopChanged(int desktop)
{
	kdDebug(155001) << "TaskbarView::slotDesktopChanged: desktop " << desktop << endl;
	refreshTaskbar();
}

void TaskbarView::slotConfigChanged() 
{
	refreshTaskbar();
}

/* ----------- Context menu settings ----------- */ 

void TaskbarView::tasksAllDesktops(int id)
{
	taskbarPrefsMenu.setItemChecked(m_eshowTasksItem, false);
	m_eshowTasksItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mShowTaskMode = all_tasks;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}

void  TaskbarView::tasksCurrentDesktop(int id)
{
	taskbarPrefsMenu.setItemChecked(m_eshowTasksItem, false);
	m_eshowTasksItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mShowTaskMode = desktop_tasks;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}

void  TaskbarView::tasksIAllDesktops(int id) 
{
	taskbarPrefsMenu.setItemChecked(m_eshowTasksItem, false);
	m_eshowTasksItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mShowTaskMode = all_icon_tasks;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}

void  TaskbarView::tasksICurrentDesktop(int id)
{
	taskbarPrefsMenu.setItemChecked(m_eshowTasksItem, false);
	m_eshowTasksItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mShowTaskMode = desktop_icon_tasks;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}
		
void  TaskbarView::groupTaskbarFull(int id) 
{
	taskbarPrefsMenu.setItemChecked(m_egroupItem, false);
	m_egroupItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mGroupTaskMode = taskbarFull;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}

void  TaskbarView::groupTaskbarAlways(int id)
{
	taskbarPrefsMenu.setItemChecked(m_egroupItem, false);
	m_egroupItem = id;
	taskbarPrefsMenu.setItemChecked(id, true);
	preferences()->mGroupTaskMode = always;
	preferences()->prefsChanged = true;
	refreshTaskbar();
}

void TaskbarView::setTranslucency(int factor) 
{
	kdDebug(155001) << "TaskbarView::setTranslucency" << endl;
	
	taskbarTransPrefs.setItemChecked((int) preferences()->mFadeFactor*10, false);
	taskbarTransPrefs.setItemChecked(factor, true);
	
	preferences()->mFadeFactor = (double) factor / 10;
	preferences()->prefsChanged = true;
	setupBlending();
	refreshContextMenu();
}

void TaskbarView::selectBgColor(int id)
{
	KColorDialog kcd(this, "KCDbg");
	kcd.getColor(preferences()->mFadeColor, this);
	QPixmap px_bgcolor(QSize(16, 16));
	px_bgcolor.fill(preferences()->mFadeColor);
	preferences()->prefsChanged = true;
	taskbarPrefsMenu.changeItem(id, px_bgcolor, "Select background color..."); 
	setupBlending();
}
		
void TaskbarView::showSlickerConfig()
{
	PreferencesDialog::instance()->show();
	PreferencesDialog::instance()->raise();
}



/* ----------- Taskbar setup and update ----------- */ 

void TaskbarView::refreshTaskbar()
{
	kdDebug(155001) << "TaskbarView::refreshTaskbar" << endl;
	if (preferences()->mRightCorner) move(0, 0);
	else move(0-(downArrowRect().width()+1), 0);
	
	m_selsects = selectTasks(m_sections);		/* Remake the selective task list */
	changeOrientation();				/* Make sure orientation is on par */
	setupSize(m_selsects.count());		/* Make sure size is on par */
	setupMask();					/* Set mask up if size changed */
	setupBlending();				/* Do translucency update */
	refreshContextMenu();
}

void TaskbarView::refreshContextMenu()
{
	QPixmap bgcolor_px;
	bgcolor_px.fill(preferences()->mFadeColor);
	taskbarPrefsMenu.changeItem(bgColorSelect_id, QIconSet(bgcolor_px),
	
	taskbarPrefsMenu.text(bgColorSelect_id));
	for (int i = 0; i <= 10; ++i) {
		if (((int)(preferences()->mFadeFactor*10))==i-1) 
			taskbarTransPrefs.setItemChecked(taskbarTransPrefs.indexOf(i), true);
		else 
			taskbarTransPrefs.setItemChecked(i, false);
	}
	
	for (int i = TaskFilterID; i <= TaskFilterID+4; ++i) {
		taskbarPrefsMenu.setItemChecked(i, false);
	}
	
	int ShowTasksSet = 2;
	ShowTasks st = preferences()->mShowTaskMode;
	if (st==all_tasks)		ShowTasksSet = 1;
	if (st==all_icon_tasks)		ShowTasksSet = 3;
	if (st==desktop_icon_tasks)	ShowTasksSet = 4;
	taskbarPrefsMenu.setItemChecked(TaskFilterID+ShowTasksSet, true);
}

QRegion TaskbarView::edgeMask(int x, int y, int w, int h, int vert, int horiz)
{
	// vert = 1, horiz = 2

	x-=w; y-=h; w*=2; h*=2;
	x += (2-vert)*(w/2);
	y += (2-horiz)*(h/2);
	
	QRect r(x,y,w,h);
	QRegion full(r, QRegion::Ellipse);
	QRegion edge(r);
	edge -= full;
	
	/* Reverse (1=2,2=1) */
	vert = 3 - vert;  // 2
	horiz = 3 - horiz; // 1
	
	/* Remove */
	edge -= QRegion(r.x()+(r.width()/2)*(vert-1), r.y(), r.width()/2, r.height());
	edge -= QRegion(r.x(), r.y()+(r.height()/2)*(horiz-1), r.width(), r.height()/2);
	
	return edge;
}

void TaskbarView::setupMask()
{
	QRegion mask(0, 0, width(), height());
	Edge edge = preferences()->mEdge;
	
	if (edge == edge_top) { 
		mask -= edgeMask(0, height()-18, 18, 18, 1, 2);
		mask -= edgeMask(width()-18, height()-18, 18, 18, 2, 2);
	} else if (edge == edge_bottom) {
		mask -= edgeMask(18, 18, 18, 18, 1, 1);
		mask -= edgeMask(width()-18, height()-18, 18, 18, 2, 1);
	} else if (edge == edge_left) {
		mask -= edgeMask(18, 18, 18, 18, 2, 1);
		mask -= edgeMask(width()-18, height()-18, 18, 18, 2, 2);
	} else if (edge == edge_right) {
		mask -= edgeMask(18, 18, 18, 18, 1, 1);
		mask -= edgeMask(width()-18, height()-18, 18, 18, 1, 2);
	}
	
	setMask(mask);
}

int TaskbarView::setupSize(int sections)
{
	Edge edge = preferences()->mEdge;
	int sector = preferences()->mEdgeSector;
	
	if(sections < 1) 
	{
		resize(downArrowRect().width()*2, 20);
		return 0;
	}
	
	int screen = QApplication::desktop()->width();
	int size = (screen - m_hintSize*2 - 15) / sections;
	if(size > m_userSize)
		size = m_userSize;

	m_defaultSize = size;
	resize((size + 2) * sections + m_hintSize*2, m_heightSize);
	
	QDesktopWidget dw;
	QRect scrGeom = dw.screenGeometry();
	
	if (edge == edge_top || edge == edge_bottom) {
		if (sector == 2) move(scrGeom.width()/2-width()/2,y());
	} else if (edge == edge_left || edge == edge_right) {
		if (sector == 2) move(x(), scrGeom.height()/2-height()/2);
	}
	return size;
}

void TaskbarView::setupBlending()
{
	kdDebug(155001) << "TaskbarView::setupBlending()" << endl;
	m_bg->setFadeEffect(preferences()->mFadeFactor, preferences()->mFadeColor);
	m_bg->start();
}



/* ----------- Add/Remove Tasks ----------- */

void
TaskbarView::slotNewTask(Task *task)
{
		addSection(new TaskbarSection(this, task));
}

void
TaskbarView::slotRemoveTask(Task *task)
{
	TaskbarSection *it;
	for(it = m_sections.first(); it; it = m_sections.next())
	{
		if(it->task() == task)
		{
			m_sections.remove(it);
			m_selsects.remove(it);
//			resize(width() - m_defaultSize - 2, 20);
			setupSize(m_selsects.count());
			setupMask();
			repaint();
			return;
		}
	}
}

void TaskbarView::addSection(TaskbarSection *s)
{
	kdDebug(155001) << "TaskbarView::addSection(): width: " << width() + m_defaultSize << endl;
	connect(s, SIGNAL(activated(TaskbarSection *)), this, SLOT(slotActivated(TaskbarSection *)));
	m_sections.append(s);
	if (shouldDrawSection(s)) m_selsects.append(s);
	setupSize(m_selsects.count());
	setupMask();
}



/* ----------- Taskbar drawing ----------- */

void TaskbarView::paintEvent(QPaintEvent *ev)
{
	QPainter *p = new QPainter(this);
	p->setPen(QPen(preferences()->mTextColor, 0));
	QRect end = downArrowRect();
	style().drawPrimitive(QStyle::PE_ArrowDown, p, end, colorGroup(), QStyle::Style_Enabled);

	int first = ev->rect().x()/m_defaultSize;
	int last = (ev->rect().width() + ev->rect().x())/m_defaultSize;

	drawSeparator(p, downArrowRect().width());
	for(int i=first; i <= last; i++)
	{
		TaskbarSection *it = m_selsects.at(i);
		if (!it) continue;
		

		int x = downArrowRect().width() + i*(m_defaultSize + 2) + 2;
//		p->drawRect(x, 0, m_defaultSize, height());
//		p->drawRect(x-1, 0, m_defaultSize + 1, height());
		drawSection(p, it, x, 0);
		drawSeparator(p, downArrowRect().width()+(i+1)*(m_defaultSize + 2) + 1);
	}

	p->end();
}

bool TaskbarView::shouldDrawSection(TaskbarSection *it) 
{
	int desktop = parent()->manager()->currentDesktop();
	ShowTasks showtasks = preferences()->mShowTaskMode;
	if (showtasks == desktop_tasks) {
		if (it->task()->desktop() != desktop && it->task()->desktop() != -1)
			return false;
	} else if (showtasks == desktop_icon_tasks) {
		if (it->task()->desktop() != desktop && it->task()->desktop() != -1)
			return false;
		if (!it->task()->isIconified()) 
			return false;
	} else if (showtasks == all_icon_tasks) {
		if (!it->task()->isIconified())
			return false;
	}	
	return true;
}

Sections TaskbarView::selectTasks(Sections sl) 
{
	Sections retval;
	for (unsigned int i = 0; i < sl.count() - 1; ++i)
		if (shouldDrawSection(sl.at(i))) retval.append(sl.at(i));
	return retval;
}

void TaskbarView::drawSection(QPainter *p, TaskbarSection *it, int x, int offset)
{
	offset = offset;
	if(it->isActive())
		p->drawPixmap(x, 0, highlight(x, m_defaultSize + 3, 
					      preferences()->mFadeFactor+.2));
	      
	int iconw = 2;
	if (preferences()->mShowAppIcons) { 
		p->drawPixmap(x + 2, height()/2-(it->pixmap().height()/2), it->pixmap());
		iconw += 20;
	}

	/* Bow to KDE settings */
	QFont font(KGlobalSettings::taskbarFont());
	QFontMetrics metrics(font);
	p->setFont(font);

	QString shown = it->label();
	if (m_defaultSize - (iconw+4) < metrics.width(it->label())) {
		for (int i = 0; it->label().length()-i==0; ++i) {
			shown = it->label().left(it->label().length()-i)+"...";
			if (m_defaultSize - (iconw+4) >= metrics.width(it->label())) break;
		}
	}
	kdDebug(155001) << "Chosen caption: '" << shown << "'" << endl;
		
	//p->drawPixmap(x + iconw, 0, fadeOutText((QPixmap*)p->device(), it->label(), m_defaultSize - iconw));
	p->drawText(x + iconw, height()/2-(metrics.height()/2), m_defaultSize - iconw, 
			height(), AlignLeft, shown);
}

void TaskbarView::drawSeparator(QPainter *p, int x)
{
	QPen pen = p->pen();
	p->setPen(preferences()->mFadeColor);
	p->drawLine(x, 0, x, height());
	p->setPen(pen);
}

QPixmap
TaskbarView::highlight(int x, int w, double val)
{
	QPixmap dest(w, height());
	
	bitBlt(&dest, 0, 0, this, x, 0, w, height(),
	 CopyROP, true);

	KPixmap blend(dest);
	return KPixmapEffect::fade(blend, val, preferences()->mFadeColor);
//	return *dest;
}

/* 
 * Credit for fadeOutText() goes to the kicker authors.
 * Shamelessly taken :).
 *
 */

QPixmap TaskbarView::fadeOutText(QPixmap *bg, QString text, int length)
{
	QPixmap pm(*bg);
	QFont font( KGlobalSettings::taskbarFont() );
	
	if ( !text.isEmpty() ) {
		if ( blendGradient.isNull() || blendGradient.size() != size() ) {

			QPixmap bgpm(length, height());
			QPainter bgp( &bgpm );
			bgpm.fill( black );

			bgpm = KImageEffect::gradient( QSize( 30, height() ), QColor( 0,0,0 ),
								QColor( 255,255,255 ), KImageEffect::HorizontalGradient );
			blendGradient = bgpm.convertToImage();
		}

		// draw text into overlay pixmap
		QPixmap tpm(pm);
		QPainter tp(&tpm);

		tp.setFont(font);
		tp.setPen(QPen(preferences()->mTextColor));

		tp.drawText(0, 1, text);
		
		QImage img = pm.convertToImage();
		QImage timg = tpm.convertToImage();
		KImageEffect::blend( img, timg, blendGradient, KImageEffect::Red );
		pm.convertFromImage( img );
	}
	return pm;
}

void
TaskbarView::updateSection(TaskbarSection *s)
{
	int no = m_selsects.find(s);
	if(no == -1)
		return;
 
	repaint(downArrowRect().width()+no*(m_defaultSize + 2) + 2, 
		0, 
		m_defaultSize + 1, 
		height(), 
		true);
}

void
TaskbarView::mousePressEvent(QMouseEvent *ev)
{

	kdDebug(155001) << "TaskbarView::mousePressEvent" << endl;
	
	if ( inDownArrowButton(ev->x(), ev->y()) ) {
		if (_drag) kdDebug(155001) << "TaskbarView::mousePressEvent: _drag was pre-enabled!" << endl;
		_drag = true;
		_drag_used = false;
		return;
	}

	TaskbarSection *s = sectionAt(ev->x());
	
	if(s)
	{
		if(ev->button() == Qt::RightButton)
		{
			TaskList *tl = new TaskList();
			tl->append(s->task());
			TaskRMBMenu m(tl, parent()->manager(), this);
			m.exec(ev->pos());
		}
		else
		{
			s->task()->activateRaiseOrIconify();
			/* If we are to only show iconified tasks: update the taskbar after
			   change */
			ShowTasks showTasks = preferences()->mShowTaskMode;
			if (showTasks == all_icon_tasks || showTasks == desktop_icon_tasks)
				refreshTaskbar();
		}
	} else {
		kdDebug(155001) << "TaskbarView::mousePressEvent: Unhandled region!" << endl;
	}
}


void TaskbarView::mouseReleaseEvent(QMouseEvent *ev)
{
	kdDebug(155001) << "TaskbarView::mouseReleaseEvent(QMouseEvent *ev)" << endl;
	
	if (!_drag_used && inDownArrowButton(ev->x(), ev->y())) {
		/* Show the context menu */
		highlight(downArrowRect().x(), downArrowRect().width(), 
			preferences()->mFadeFactor+.2);
		KWindowListMenu *menu = new KWindowListMenu;
		menu->init();
		prepareWindowListMenu(menu);
		menu->exec(QCursor::pos());
		delete menu;
		repaint(downArrowRect());
	}
	
	_drag = false;
	_drag_used = false;
}

void TaskbarView::mouseMoveEvent(QMouseEvent *ev)
{
	if (_drag) _drag_used = true;
}
 
TaskbarSection *TaskbarView::sectionAt(int x)
{
	if (x <= downArrowRect().width() || x >= downArrowRect().x()) return NULL;
	kdDebug(155001) << "Sure Sign: Its the .at() :)" << endl;
	return m_selsects.at((x-downArrowRect().width())/m_defaultSize);
}

QRect TaskbarView::downArrowRect(void)
{
	return QRect(width() - m_hintSize, 0, m_hintSize, height());
}

bool TaskbarView::inDownArrowButton(int x, int y)
{
	QRect button = downArrowRect();
	if (x >= button.x() && x <= button.x() + button.width() && 
	    y >= button.y() && y <= button.y() + button.height())
		return true;
	else 
		return false;
}

void TaskbarView::initDropDownMenu()
{
	kdDebug(155001) << "TaskbarView::initDropDownMenu" << endl;
	
	
	taskbarPrefsMenu.insertTitle("Tasks shown");
	int tasks_fromall = taskbarPrefsMenu.insertItem("Show all tasks from all desktops", this, 
							SLOT(tasksAllDesktops(int)), 0, TaskFilterID+1);
	taskbarPrefsMenu.setItemParameter(tasks_fromall, tasks_fromall);
	int tasks_fromcurr = taskbarPrefsMenu.insertItem("Show all tasks from current desktop only",
							 this, SLOT(tasksCurrentDesktop(int)), 0, TaskFilterID+2);
	taskbarPrefsMenu.setItemParameter(tasks_fromcurr, tasks_fromcurr);
	int tasks_ifromall = taskbarPrefsMenu.insertItem("Show iconified tasks from all desktops",
							 this, SLOT(tasksIAllDesktops(int)), 0, TaskFilterID+3);
	taskbarPrefsMenu.setItemParameter(tasks_ifromall, tasks_ifromall);
	int tasks_ifromcurr = taskbarPrefsMenu.insertItem("Show iconified tasks from current desktop only",
							 this, SLOT(tasksICurrentDesktop(int)), 0, TaskFilterID+4);
	taskbarPrefsMenu.setItemParameter(tasks_ifromcurr, tasks_ifromcurr);
	
	/* Title: Group tasks */
	taskbarPrefsMenu.insertTitle("Group tasks");
	int group_tbfull = taskbarPrefsMenu.insertItem("Group tasks when taskbar is full",
							this, SLOT(groupTaskbarFull(int)));
	taskbarPrefsMenu.setItemParameter(group_tbfull, group_tbfull);
	int group_always = taskbarPrefsMenu.insertItem("Always group tasks", this, 
							SLOT(groupTaskbarAlways(int)));
	taskbarPrefsMenu.setItemParameter(group_always, group_always);
													
	taskbarPrefsMenu.insertTitle("Background");
	for (int i = 0; i <= 10; ++i) {
		QString percent = QString( "%1%" ).arg(i*10);
		double translucency = (double) i/10;
		if (translucency == preferences()->mDefaultFadeFactor) percent += " (default)";
		taskbarTransPrefs.insertItem(percent, this, SLOT(setTranslucency(int)),
						0, i);
		taskbarTransPrefs.setItemParameter(i, i);
	}
	
	QIconSet configureIcon(SmallIconSet("configure"));
	taskbarPrefsMenu.insertItem(configureIcon, "Translucency",
				   (QPopupMenu*)&taskbarTransPrefs);
	
	/* Item: Select background color... */
	QPixmap px_bgcolor;
	px_bgcolor.fill(preferences()->mFadeColor);
	bgColorSelect_id = taskbarPrefsMenu.insertItem(QIconSet(px_bgcolor), "Select background color...", 
				    this, SLOT(selectBgColor(int)));
	taskbarPrefsMenu.setItemParameter(bgColorSelect_id, bgColorSelect_id);
	
}

void TaskbarView::prepareWindowListMenu(KWindowListMenu *menu)
{
	/* Merge in Taskbar Preferences etc to @menu */
	QIconSet configureIcon(SmallIconSet("configure")); 
	menu->insertItem("Taskbar Preferences", (QPopupMenu*)&taskbarPrefsMenu);
	menu->insertItem(configureIcon, "Slicker Preferences", this, 
				SLOT(showSlickerConfig()));
}


void TaskbarView::setWindowManager(bool set)
{
	kdDebug(155001) << "TaskbarView::setWindowManager" << endl;
	if (set) {
		kdDebug(155001) << "TaskbarView::setWindowManager" << endl;
	  	NETStrut strut;
  		kdDebug(155001) << "Strut setting: height() = " << height() << endl;
	  	kdDebug(155001) << "Strut setting: mapToGlobal() = " << mapToGlobal(QPoint(0,0)).y() << endl;
	  	kdDebug(155001) << "Strut setting: top = " << height() + mapToGlobal(QPoint(0,0)).y() << endl;
	 	strut.top = height() + mapToGlobal(QPoint(0,0)).y();
	  	KWin::setStrut( winId(), strut.left, strut.right,  strut.top, strut.bottom );
	} else {
		//clear strut
		KWin::setStrut( winId(), 0, 0, 0, 0);
	}
}

void TaskbarView::changeOrientation()
{
	kdDebug(155001) << "TaskbarView::changeOrientation" << endl;
	setupMask();
}

TaskbarView::~TaskbarView()
{
}

#include "taskbarview.moc"

