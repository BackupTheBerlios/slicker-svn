
#include "edgetest.h"
#include <qapplication.h>
#include <qpushbutton.h>
#include <qsize.h>
#include <qpoint.h>

#include "edgewidgetmanager.h"
#include "edgewidgethelpers.h"

#include <iostream>
using namespace std;

EdgeTest::EdgeTest()
  : EdgeWidget()
{
	_exitBtn = new QPushButton(this, "Olle bolle");
	_exitBtn->move(0,0);
	_exitBtn->resize(100,50);

	_exitBtn->show();

	connect(this, SIGNAL(edgeChanged(ScreenEdge, ScreenEdge)), this, SLOT(relayout(ScreenEdge, ScreenEdge)));
}


EdgeTest::~EdgeTest()
{
}

void EdgeTest::mousePressEvent ( QMouseEvent * e )
{
	// Ok, this demonstrates interactivity and CollisionPolicies
	// Left-Button: drag
	// Right-Button: resize
	// Middle-Button: change collision policy
	if (e->button() == LeftButton)
		beginPosDrag(e->pos());
	else if (e->button() == RightButton)
		beginSizeDrag(e->pos(), oppositeEdge(edge()));
	else
	{
		EdgeWidgetManager *ewm = EdgeWidgetManager::instance();
		if (ewm->policy() == EdgeWidgetManager::AttackerIsRight)
			ewm->setPolicy(EdgeWidgetManager::DefenderIsRight);
		else
			ewm->setPolicy(EdgeWidgetManager::AttackerIsRight);
	}

	e->accept();
}

void EdgeTest::enterEvent(QEvent *e)
{
	e->type();
	grabKeyboard();
}

void EdgeTest::leaveEvent(QEvent *e)
{
	e->type();
	releaseKeyboard();
}

void EdgeTest::keyPressEvent(QKeyEvent *e)
{
	if (e->text() == "h")
		hide();
	if (e->text() == "s")
		show();
}

void EdgeTest::mouseReleaseEvent(QMouseEvent *e)
{
	// Yeah, yeah, clear things up a bit.
	if (e->button() == LeftButton)
		endPosDrag();
	else if (e->button() == RightButton)
		endSizeDrag();

	e->accept();
}

void EdgeTest::relayout(ScreenEdge oldEdge, ScreenEdge newEdge)
{
	// IMPORTANT: This code demonstrates the transform-helpers
	// Transform-helper are not required to use EdgeWidget, but they
	// make life easier for you.

	// First we gotta initialize a helper.
	EdgeWidgetTransformHelper th(this, oldEdge, newEdge, EdgeWidgetTransformHelper::TransformMirror);

	// Transform the geometry
	th.transform(*_exitBtn);
}

// I think you all get the idea about this one
int main( int argc, char **argv )
{
	QApplication a(argc, argv);
	EdgeTest *e = new EdgeTest();
	EdgeTest *f = new EdgeTest();
	EdgeTest *g = new EdgeTest();

	a.setMainWidget(e);
	e->show();
	e->resize( QSize(200, 150));
	e->setPosition(140, BottomEdge);

	f->show();
	f->resize( QSize(200, 150));
	f->setPosition(140, LeftEdge);

	g->show();
	g->resize( QSize(200, 150));
	g->setPosition(140, LeftEdge);

	return a.exec();
}

