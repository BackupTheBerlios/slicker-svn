/*****************************************************************

Copyright (c) 1996-2000 the kicker authors. See file AUTHORS.

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

#ifndef SERVICE_MENU_H
#define SERVICE_MENU_H

#include <qmap.h>

#include <ksycocaentry.h>
#include <kservice.h>
#include <kpanelmenu.h>

/**
 * PanelServiceMenu is filled with KDE services and service groups. The sycoca
 * database is queried and the hierarchical structure built by creating child
 * menus of type PanelServiceMenu, so the creation is recursive.
 *
 * The entries are sorted alphabetically and groups come before services.
 *
 * @author Rik Hemsley <rik@kde.org>
 */

typedef QMap<int, KSycocaEntry::Ptr> EntryMap;
typedef QPtrList<QPopupMenu> PopupMenuList;

class PanelServiceMenu : public KPanelMenu
{
    Q_OBJECT

public:
    PanelServiceMenu(const QString & label, const QString & relPath,
		     QWidget* parent  = 0, const char* name = 0, bool addmenumode = false);

    virtual ~PanelServiceMenu();

    QString relPath() { return relPath_; }

    void createRecentMenuItems();
    void updateRecentMenuItems(KService::Ptr & s);
    void setExcludeNoDisplay( bool flag );

private:
    void insertMenuItem(KService::Ptr & s, int nId, int nIndex = -1);

protected slots:
    virtual void initialize();
    virtual void slotExec(int id);
    virtual void slotClearOnClose();
    virtual void slotClear();
    void updateRecent();
    virtual void configChanged();

protected:
    virtual PanelServiceMenu * newSubMenu(const QString & label, const QString & relPath,
					  QWidget * parent, const char * name);

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void closeEvent(QCloseEvent *);

    void doInitialize();
   
    QString relPath_;

    EntryMap entryMap_;

    bool merge_;
    bool detailed_;
    bool detailedNamesFirst_;
    bool loaded_;
    bool excludeNoDisplay_;

    QPopupMenu * opPopup_;
    bool clearOnClose_;
    bool addmenumode_;
    QPoint startPos_;
    PopupMenuList subMenus;
    
private:
    void readConfig();
 };

#endif // SERVICE_MENU_H
