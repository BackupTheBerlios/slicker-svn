#include "kmenutab.h"
#include <kdebug.h>
#include <qapplication.h>

bool mDontOpenCard = false;

KMenuTab::KMenuTab(Card *card, TabDelegate *del)
	: Tab(card,del)
{
kdDebug() << "KMenuTab::KMenuTab" << endl;
}


KMenuTab::~KMenuTab()
{

}

void KMenuTab::setDontOpenCard(bool a)
{
		mDontOpenCard = a;
}

void KMenuTab::mouseReleaseEvent(QMouseEvent *e)
{
	printf("KMenuTab::mouseReleaseEvent()\n");
	qApp->setGlobalMouseTracking(false);
	setMouseTracking(false);

	_mouseTrackerTimer.stop();

	if (e->button() == Qt::LeftButton && !_dragger.tracking() && !_animating && !mDontOpenCard)
	{
		if (card()->isOpen())
		{
			card()->shut(true);
		}
		else
		{
			card()->open(true);
		}
	}

	if (e->button() == Qt::RightButton)
	{
		showContextMenu();
	}
	_dragger.done();
}

void KMenuTab::mousePressEvent(QMouseEvent *e)
{
	printf("KMenuTab::mousePressEvent()\n");
	mDontOpenCard = false;
	qApp->setGlobalMouseTracking(true);
	setMouseTracking(true);

	if (e->button() == Qt::LeftButton)
	{
		_mouseTrackerTimer.start( 30 );
		_dragger.mousePress(e->pos());
	}

}
