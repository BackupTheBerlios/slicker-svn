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

#include <qpixmap.h>
#include <kdebug.h>

#include "taskbarview.h"
#include "taskbarsection.h"

TaskbarSection::TaskbarSection(TaskbarView *parent, Task *task)
 : QObject(parent, "tb-section")
{
	m_pix = task->pixmap();
	m_label = task->visibleName();
	m_task = task;
	m_parent = parent;

	m_group = false;
	m_active = false;

	connect(task, SIGNAL(activated()), this, SLOT(slotActivated()));
	connect(task, SIGNAL(changed()), this, SLOT(slotChanged()));
}

void
TaskbarSection::slotActivated()
{
	m_active = true;
	emit activated(this);
}

void
TaskbarSection::slotChanged()
{
	m_pix = m_task->pixmap();
	m_label = m_task->visibleName();

	m_parent->updateSection(this);
}

TaskbarSection::~TaskbarSection()
{
}

#include "taskbarsection.moc"

