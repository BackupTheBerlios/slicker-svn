#ifndef SLIDER_H
#define SLIDER_H

#include "edgewidget.h"
#include "edgewidgethelpers.h"
#include "applethost.h"
#include "appletdef.h"
#include "sessionitem.h"
#include "sessionitemfactory.h"

class SliderTray;

/**
* The Slider is probably the simplest form of an AppletHost.
* It displays the Icon of the Applet in a small widget.
* On clicking on the icon, if there is any content, the content pops up.
*
* @author Ulrik Mikaelsson
*/
class Slider : public EdgeWidget, public AppletHost, public SessionItem
{
public:
    Slider(const QString & id);
    ~Slider();

    bool accept(Applet * applet);
    void detach(Applet * applet);

    void mousePressEvent ( QMouseEvent *e );
    void mouseReleaseEvent ( QMouseEvent *e );
    void contextMenuEvent ( QContextMenuEvent *e );
    
    virtual void restore(KConfigBase * config);
    virtual void store(KConfigBase * config);

protected slots:
    virtual void edgeChanged(EdgeWidget::ScreenEdge oldEdge);

private:
    SliderTray * _tray1;
    SliderTray * _tray2;
    EdgeWidgetLayoutBox *_content;
    EdgeWidgetBoxLayout *_mainLayout;
    AppletDefsMenu * _appletMenu;
};

/**
* The widget for the small handles on each side of the slider.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class SliderTray : public QWidget
{
public:
    SliderTray(QWidget * parent);
};

/**
* The SessionItemFactory for the slider.
* @author Ulrik Mikaelsson <rawler@users.sourceforge.net>
*/
class SliderFactory : public SessionItemFactory
{
public:
    SliderFactory();
    virtual ~SliderFactory();
    
    virtual SessionItem * loadItem(KConfigBase * configGroup);
    virtual SessionItem * createItem();
};


#endif
