 /*
 * krootwm.cc Part of the KDE project.
 *
 * Copyright (C) 1997 Matthias Ettrich
 *           (C) 1997 Torben Weis, weis@kde.org
 *           (C) 1998 S.u.S.E. weis@suse.de

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License version 2 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <kstandarddirs.h>
#include <kpopupmenu.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kbookmarkmenu.h>
#include <konqbookmarkmanager.h>
#include <klocale.h>
#include <knewmenu.h>
#include <dcopclient.h>
#include <dcopref.h>
#include <khelpmenu.h>
#include <netwm.h>
#include <kdebug.h>
#include <kwindowlistmenu.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <qfile.h>

#include "krootwm.h"
#include "kdiconview.h"
#include "launcher.h"
//#include "desktop.h"
//#include "kcustommenu.h"

#include <X11/X.h>
#include <X11/Xlib.h>

KRootWm * KRootWm::s_rootWm = 0;


// for multihead
//extern int kdesktop_screen_number;


KRootWm::KRootWm(Launcher* _launcher) : QObject(_launcher)
{
  s_rootWm = this;
  m_actionCollection = new KActionCollection(_launcher, this, "KRootWm::m_actionCollection");
  m_pLauncher = _launcher;
  m_bDesktopEnabled = (m_pLauncher->iconView() != 0);
  customMenu1 = 0;
  customMenu2 = 0;


  // Creates the new menu
  menuBar = 0; // no menubar yet
  menuNew = 0;
  if (m_bDesktopEnabled && kapp->authorize("editable_desktop_icons"))
  {
     menuNew = new KNewMenu( m_actionCollection, "new_menu" );
     connect(menuNew->popupMenu(), SIGNAL( aboutToShow() ),
             this, SLOT( slotFileNewAboutToShow() ) );
     connect( menuNew, SIGNAL( activated() ),
              m_pLauncher->iconView(), SLOT( slotNewMenuActivated() ) );
  }

  if (kapp->authorizeKAction("bookmarks"))
  {
     bookmarks = new KActionMenu( i18n("Bookmarks"), "bookmark", m_actionCollection, "bookmarks" );
     bookmarkMenu = new KBookmarkMenu( KonqBookmarkManager::self(), new KBookmarkOwner(),
                                    bookmarks->popupMenu(),
                                    m_actionCollection,
                                    true, false );
  }
  else
  {
     bookmarks = 0;
     bookmarkMenu = 0;
  }

  // The windowList and desktop menus can be part of a menubar (Mac style)
  // so we create them here
  desktopMenu = new QPopupMenu;

  // Create the actions
  if (m_bDesktopEnabled)
  {
      // Don't do that! One action in two parent collections means some invalid write
      // during the second ~KActionCollection.
#if 0
     KAction *action = m_pLauncher->actionCollection()->action( "paste" );
     if (action)
        m_actionCollection->insert( action );
     action = m_pLauncher->actionCollection()->action( "undo" );
     if (action)
        m_actionCollection->insert( action );
#endif
  }

//   if (kapp->authorize("run_command"))
//   {
//      new KAction(i18n("Run Command..."), "run", 0, m_pLauncher, SLOT( slotExecuteCommand() ), m_actionCollection, "exec" );
//   }

  // arrange menu actions
  if (m_bDesktopEnabled && kapp->authorize("editable_desktop_icons"))
  {
     new KAction(i18n("By Name"), 0, this, SLOT( slotArrangeByNameCI() ),
                 m_actionCollection, "sort_nci");
     new KAction(i18n("By Size"), 0, this, SLOT( slotArrangeBySize() ),
                 m_actionCollection, "sort_size");
     new KAction(i18n("By Type"), 0, this, SLOT( slotArrangeByType() ),
                 m_actionCollection, "sort_type");

     KToggleAction *aSortDirsFirst = new KToggleAction( i18n("Directories First"), 0, m_actionCollection, "sort_directoriesfirst" );
     aSortDirsFirst->setChecked(true);
     //
     new KAction(i18n("Line Up Horizontally"), 0,
                 this, SLOT( slotLineupIconsHoriz() ),
                 m_actionCollection, "lineupHoriz" );
     new KAction(i18n("Line Up Vertically"), 0,
                 this, SLOT( slotLineupIconsVert() ),
                 m_actionCollection, "lineupVert" );
     new KAction(i18n("Align to Grid"), 0,
                 this, SLOT( slotLineupIcons() ),
                 m_actionCollection, "realign" );
  }
  // Icons in sync with kicker
  if (kapp->authorize("lock_screen"))
  {
      new KAction(i18n("Lock Screen"), "lock", 0, this, SLOT( slotLock() ),
                  m_actionCollection, "lock" );
  }
  if (kapp->authorize("logout"))
  {
      char *user = getlogin();
      if (user == 0) user = getenv("LOGNAME");
      QString username(user);

      new KAction(i18n("Logout \"%1\"...").arg(username), "exit", 0,
                  this, SLOT( slotLogout() ), m_actionCollection, "logout" );
  }

  QStringList dmopt =
      QStringList::split( QChar( ',' ),
                          QString::fromLatin1( ::getenv( "XDM_MANAGED" ) ) );
  if ( dmopt.findIndex( "rsvd" ) < 0 )
      xdmFifoName = QString::null;
  else {
      xdmFifoName = dmopt.first();
      new KAction(i18n("Start New Session"), "fork", 0, this,
                  SLOT( slotNewSession() ), m_actionCollection, "newsession" );
  }

  initConfig();
}

KRootWm::~KRootWm()
{
  delete m_actionCollection;
  delete desktopMenu;
}

void KRootWm::initConfig()
{
  kdDebug() << "KRootWm::initConfig" << endl;
  KConfig *kconfig = KGlobal::config();

  // parse the configuration
  kconfig->setGroup(QString::fromLatin1("KDE"));
  m_bGlobalMenuBar = kconfig->readBoolEntry(QString::fromLatin1("macStyle"), false);
  kconfig->setGroup(QString::fromLatin1("Menubar"));
  m_bShowMenuBar = m_bGlobalMenuBar || kconfig->readBoolEntry(QString::fromLatin1("ShowMenubar"), false );

  // read configuration for clicks on root window
  const char * s_choices[6] = { "", "WindowListMenu", "DesktopMenu", "AppMenu", "CustomMenu1", "CustomMenu2" };
  leftButtonChoice = middleButtonChoice = rightButtonChoice = NOTHING;
  kconfig->setGroup("Mouse Buttons");
  QString s = kconfig->readEntry("Left", "");
  for ( int c = 0 ; c < 6 ; c ++ )
    if (s == s_choices[c])
      { leftButtonChoice = (menuChoice) c; break; }
  s = kconfig->readEntry("Middle", "WindowListMenu");
  for ( int c = 0 ; c < 6 ; c ++ )
    if (s == s_choices[c])
      { middleButtonChoice = (menuChoice) c; break; }
  s = kconfig->readEntry("Right", "DesktopMenu");
  for ( int c = 0 ; c < 6 ; c ++ )
    if (s == s_choices[c])
      { rightButtonChoice = (menuChoice) c; break; }

	buildMenus();
}

void KRootWm::buildMenus()
{

    kdDebug() << "KRootWm::buildMenus" << endl;

    delete menuBar;
    menuBar = 0;

    delete customMenu1;
    customMenu1 = 0;
    delete customMenu2;
    customMenu2 = 0;

    if (m_bShowMenuBar)
    {
//        kdDebug() << "showMenuBar" << endl;
        QWidget* dummy = new QWidget;
        menuBar = new KMenuBar( dummy );
        disconnect( kapp, SIGNAL( appearanceChanged() ), menuBar, SLOT( slotReadConfig() ) );
        menuBar->setCaption("KDE Desktop");
    }
    kdDebug() << "KRootWm::buildMenus 1" << endl;
    // create Arrange menu
    QPopupMenu *pArrangeMenu = 0;
    QPopupMenu *pLineupMenu = 0;
    KAction *action;

    if (m_bDesktopEnabled && m_actionCollection->action("realign"))
    {
        pArrangeMenu = new QPopupMenu;
        m_actionCollection->action("sort_nci")->plug( pArrangeMenu );
        m_actionCollection->action("sort_size")->plug( pArrangeMenu );
        m_actionCollection->action("sort_type")->plug( pArrangeMenu );
        pArrangeMenu->insertSeparator();
        m_actionCollection->action("sort_directoriesfirst")->plug( pArrangeMenu );

        pLineupMenu = new QPopupMenu;
        m_actionCollection->action( "lineupHoriz" )->plug( pLineupMenu );
        m_actionCollection->action( "lineupVert" )->plug( pLineupMenu );
        pLineupMenu->insertSeparator();
        m_actionCollection->action( "realign" )->plug( pLineupMenu );
    }
    kdDebug() << "KRootWm::buildMenus 2" << endl;
    if (menuBar) {
        bool needSeparator = false;
        file = new QPopupMenu;

//         action = m_actionCollection->action("exec");
//         if (action)
//         {
//             action->plug( file );
//             file->insertSeparator();
//         }

        action = m_actionCollection->action("newsession");
        if (action)
            action->plug( file );

        action = m_actionCollection->action("lock");
        if (action)
            action->plug( file );

        action = m_actionCollection->action("logout");
        if (action)
            action->plug( file );

        desk = new QPopupMenu;
    kdDebug() << "KRootWm::buildMenus 3 " << endl;
        if (m_bDesktopEnabled)
        {

            if (pArrangeMenu)
                desk->insertItem(i18n("Sort Icons"), pArrangeMenu);
            if (pLineupMenu)
                desk->insertItem(i18n("Line Up Icons"), pLineupMenu );

            m_actionCollection->action("refresh")->plug( desk );
            needSeparator = true;
        }

    }

    kdDebug() << "KRootWm::buildMenus 4" << endl;
    desktopMenu->clear();
    desktopMenu->disconnect( this );

    if (menuNew)
    {
       menuNew->plug( desktopMenu );
    }

    if (bookmarks)
    {
       bookmarks->plug( desktopMenu );
    }


    if (m_bDesktopEnabled)
    {
        action = m_actionCollection->action("undo");
        if (action)
           action->plug( desktopMenu );
        action = m_actionCollection->action("paste");
        if (action)
           action->plug( desktopMenu );
    }
    action = m_actionCollection->action("exec");
    if (action)
    {
       action->plug( desktopMenu );
    }
    desktopMenu->insertSeparator();

    if (m_bDesktopEnabled && m_actionCollection->action("realign"))
    {
       QPopupMenu* pIconOperationsMenu = new QPopupMenu;

       pIconOperationsMenu->insertItem(i18n("Sort Icons"), pArrangeMenu);
       pIconOperationsMenu->insertSeparator();
       m_actionCollection->action( "lineupHoriz" )->plug( pIconOperationsMenu );
       m_actionCollection->action( "lineupVert" )->plug( pIconOperationsMenu );
       pIconOperationsMenu->insertSeparator();
       m_actionCollection->action( "realign" )->plug( pIconOperationsMenu );

       desktopMenu->insertItem(SmallIconSet("icons"), i18n("Icons"), pIconOperationsMenu);
    }

    kdDebug() << "KRootWm::buildMenus 5" << endl;

    action = m_actionCollection->action("lock");
    if (action)
        action->plug( desktopMenu );

    action = m_actionCollection->action("logout");
    if (action)
        action->plug( desktopMenu );
    kdDebug() << "KRootWm::buildMenus 6" << endl;
    connect( desktopMenu, SIGNAL( aboutToShow() ), this, SLOT( slotFileNewAboutToShow() ) );

    if (menuBar) {
        menuBar->insertItem(i18n("File"), file);
        if (menuNew)
        {
            menuBar->insertItem(i18n("New"), menuNew->popupMenu());
        }
        if (bookmarks)
        {
            menuBar->insertItem(i18n("Bookmarks"), bookmarks->popupMenu());
        }
        menuBar->insertItem(i18n("Desktop"), desk);

        menuBar->setTopLevelMenu( true );
        XSetTransientForHint( qt_xdisplay(), menuBar->winId(), m_pLauncher->winId() );
        menuBar->show(); // we need to call show() as we delayed the creation with the timer
    }
}

void KRootWm::slotFileNewAboutToShow()
{
  if (menuNew)
  {
//  kdDebug() << " KRootWm:: (" << this << ") slotFileNewAboutToShow() menuNew=" << menuNew << endl;
     // As requested by KNewMenu :
     menuNew->slotCheckUpToDate();
     // And set the files that the menu apply on :
     menuNew->setPopupFiles( m_pLauncher->url() );
  }
}


void KRootWm::activateMenu( menuChoice choice, const QPoint& global )
{
  switch ( choice )
  {
    case DESKTOPMENU:
      m_desktopMenuPosition = global; // for KDIconView::slotPaste
      desktopMenu->popup(global);
      break;
    case APPMENU:
    {
//FIXME
//       // This allows the menu to disappear when clicking on the background another time
//       XUngrabPointer(qt_xdisplay(), CurrentTime);
//       XSync(qt_xdisplay(), False);
//       // Ask kicker to showup the menu
//       // make sure we send the message to the correct kicker
//       QCString appname;
//       if (kdesktop_screen_number == 0)
// 	  appname = "kicker";
//       else
// 	  appname.sprintf("kicker-screen-%d", kdesktop_screen_number);
//
//       DCOPRef( appname.data(), appname.data() ).send( "popupKMenu", global );
       break;
    }
//FIXME
/*    case CUSTOMMENU1:
      if (!customMenu1)
         customMenu1 = new KCustomMenu("kdesktop_custom_menu1");
      customMenu1->popup(global);
      break;
    case CUSTOMMENU2:
      if (!customMenu2)
         customMenu2 = new KCustomMenu("kdesktop_custom_menu2");
      customMenu2->popup(global);*/
      break;
    case NOTHING:
    default:
      break;
  }
}

void KRootWm::mousePressed( const QPoint& _global, int _button )
{
	 if (!desktopMenu) return; // initialisation not yet done
    switch ( _button ) {
    case LeftButton:
		  if ( m_bShowMenuBar && menuBar )
		  {
				menuBar->raise();
        }
        activateMenu( leftButtonChoice, _global );

        break;
    case MidButton:
        activateMenu( middleButtonChoice, _global );
        break;
    case RightButton:
        activateMenu( rightButtonChoice, _global );
        break;
    default:
        // nothing
        break;
    }
}



void KRootWm::slotArrangeByNameCS()
{
    if (m_bDesktopEnabled)
    {
        bool b = static_cast<KToggleAction *>(m_actionCollection->action("sort_directoriesfirst"))->isChecked();
        m_pLauncher->iconView()->rearrangeIcons( KDIconView::NameCaseSensitive, b);
    }
}

void KRootWm::slotArrangeByNameCI()
{
    if (m_bDesktopEnabled)
    {
        bool b = static_cast<KToggleAction *>(m_actionCollection->action("sort_directoriesfirst"))->isChecked();
        m_pLauncher->iconView()->rearrangeIcons( KDIconView::NameCaseInsensitive, b);
    }
}

void KRootWm::slotArrangeBySize()
{
    if (m_bDesktopEnabled)
    {
        bool b = static_cast<KToggleAction *>(m_actionCollection->action("sort_directoriesfirst"))->isChecked();
        m_pLauncher->iconView()->rearrangeIcons( KDIconView::Size, b);
    }
}

void KRootWm::slotArrangeByType()
{
    if (m_bDesktopEnabled)
    {
        bool b = static_cast<KToggleAction *>(m_actionCollection->action("sort_directoriesfirst"))->isChecked();
        m_pLauncher->iconView()->rearrangeIcons( KDIconView::Type, b);
    }
}

void KRootWm::slotLineupIconsHoriz() {
    if (m_bDesktopEnabled)
    {
        m_pLauncher->iconView()->lineupIcons(QIconView::LeftToRight);
    }
}

void KRootWm::slotLineupIconsVert()  {
    if (m_bDesktopEnabled)
    {
        m_pLauncher->iconView()->lineupIcons(QIconView::TopToBottom);
    }
}

void KRootWm::slotLineupIcons() {
    if (m_bDesktopEnabled)
    {
        m_pLauncher->iconView()->lineupIcons();
    }
}

void KRootWm::slotLock() {

}


void KRootWm::slotLogout() {
//FIXME    m_pLauncher->logout(KApplication::ShutdownConfirmDefault, KApplication::ShutdownTypeDefault);
}

void KRootWm::slotNewSession() {
    if (KMessageBox::warningContinueCancel(m_pLauncher,
          i18n("<p>You have chosen to open another desktop session.<br>"
               "The current session will be hidden "
               "and a new login screen will be displayed.<br>"
               "An F-key is assigned to each session; "
               "F%1 is usually assigned to the first session, "
               "F%2 to the second session and so on. "
               "You can switch between sessions by pressing "
               "CTRL, ALT and the appropriate F-key at the same time.</p>")
            .arg(7).arg(8)) != KMessageBox::Continue)
        return;
    QFile fifo(xdmFifoName);
    if (fifo.open(IO_WriteOnly | IO_Raw)) {
        fifo.writeBlock( "reserve\n", 8 );
        fifo.close();
    }
}

void KRootWm::slotMenuItemActivated(int /* item */ )
{
}

#include "krootwm.moc"
