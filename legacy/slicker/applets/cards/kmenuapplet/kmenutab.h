#ifndef KMENUTAB_H
#define KMENUTAB_H

#include "../../../carddeskcore/Card.h"
#include "../../../carddeskcore/CardElements.h"

using namespace CardDesk;

class KMenuTab : public Tab
{
Q_OBJECT
public:
		KMenuTab(Card *parent, TabDelegate *extra = NULL);
		~KMenuTab();

		virtual void mouseReleaseEvent(QMouseEvent *);
		virtual void mousePressEvent(QMouseEvent *);
		static void setDontOpenCard(bool a);


};

#endif
