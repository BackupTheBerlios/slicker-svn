
#ifndef _EDGETEST_H_
#define _EDGETEST_H_

#include "edgewidget.h"

class QPushButton;

class EdgeTest : public EdgeWidget
{
	Q_OBJECT
public:
  EdgeTest();
  ~EdgeTest();
protected:
	virtual void mousePressEvent ( QMouseEvent *e );
	virtual void mouseReleaseEvent ( QMouseEvent *e );
	virtual void enterEvent ( QEvent *e );
	virtual void leaveEvent ( QEvent *e );
  virtual void keyPressEvent ( QKeyEvent *e );
public slots:
	void relayout(ScreenEdge oldEdge, ScreenEdge newEdge);
private:
	QPushButton *_exitBtn;
};

#endif
