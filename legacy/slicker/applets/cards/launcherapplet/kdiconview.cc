/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
   Copyright (C) 2000, 2001 David Faure <faure@kde.org>

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

#include <kaccel.h>
#include <kapplication.h>
#include <kcolordrag.h>
#include <kdesktopfile.h>
#include <kdirlister.h>
#include <kfileivi.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <knewmenu.h>
#include <konqbookmarkmanager.h>
#include <konq_defaults.h>
#include <konq_drag.h>
#include <konq_operations.h>
#include <konq_popupmenu.h>
#include <konq_settings.h>
#include <konq_undo.h>
#include <kstdaction.h>
#include <kstandarddirs.h>
#include <kurldrag.h>
#include <kwin.h>
#include <kwinmodule.h>

#include <qdir.h>
#include <qclipboard.h>
#include <qregexp.h>
#include <unistd.h>
#include <kdebug.h>

#include "kdiconview.h"

// for multihead
//extern int kdesktop_screen_number;

KDIconView::KDIconView( QWidget *parent, const char* name )
    : KonqIconViewWidget( parent, name, WResizeNoErase, true ),
      m_actionCollection( this, "KDIconView::m_actionCollection" ),
      m_accel( 0L ),
      m_bNeedRepaint( false ),
      m_bNeedSave( false ),
      m_hasExistingPos( false ),
      m_bShowDot( false ),
      m_bVertAlign( true ),
      m_bWheelSwitchesWorkspace( false ),
      m_pSettings( 0L ),
      m_dirLister( 0L ),
      m_mergeDirs(),
      m_dotDirectory( 0L ),
      m_lastDeletedIconPos(),
      m_eSortCriterion( NameCaseInsensitive ),
      m_bSortDirectoriesFirst( true ),
      m_itemsAlwaysFirst()
{
   setResizeMode( Fixed );

    connect( QApplication::clipboard(), SIGNAL(dataChanged()),
             this, SLOT(slotClipboardDataChanged()) );



    connect( this, SIGNAL( executed( QIconViewItem * ) ),
             SLOT( slotReturnPressed( QIconViewItem * ) ) );
    connect( this, SIGNAL( returnPressed( QIconViewItem * ) ),
             SLOT( slotReturnPressed( QIconViewItem * ) ) );
    connect( this, SIGNAL( mouseButtonPressed(int, QIconViewItem*, const QPoint&)),
             SLOT( slotMouseButtonPressed(int, QIconViewItem*, const QPoint&)) );
    connect( this, SIGNAL( mouseButtonClicked(int, QIconViewItem*, const QPoint&)),
             SLOT( slotMouseButtonClickedKDesktop(int, QIconViewItem*, const QPoint&)) );

    connect( this, SIGNAL( enableAction( const char * , bool ) ),
             SLOT( slotEnableAction( const char * , bool ) ) );
    connect( this, SIGNAL(itemRenamed(QIconViewItem*)),
             SLOT( slotItemRenamed(QIconViewItem*)) );
    connect( this, SIGNAL( dropped( QDropEvent *, const QValueList<QIconDragItem> & ) ),
             this, SLOT( slotSaveDropPosition( QDropEvent *, const QValueList<QIconDragItem> & ) ) );
  

    setShowToolTips( false );

    if (!kapp->authorize("editable_desktop_icons"))
    {
       setItemsMovable(false);
       setAcceptDrops(false);
    }
    
#ifdef XRANDR_SUPPORT
    connect( kapp->desktop(), SIGNAL( resized( int )), SLOT( desktopResized()));
#endif
}

KDIconView::~KDIconView()
{
    delete m_dirLister;
}

void KDIconView::initConfig( bool init )
{
    //kdDebug() << "initConfig " << init << endl;

    if ( !init )
        KonqFMSettings::reparseConfiguration();

    KConfig * config = KGlobal::config();

    config->setGroup("Mouse Buttons");
    m_bWheelSwitchesWorkspace = config->readBoolEntry("WheelSwitchesWorkspace", false);

    setMaySetWallpaper(!config->isImmutable());
    config->setGroup( "Desktop Icons" );
    m_bShowDot = config->readBoolEntry("ShowHidden", DEFAULT_SHOW_HIDDEN_ROOT_ICONS);
    m_bVertAlign = config->readBoolEntry("VertAlign", DEFAULT_VERT_ALIGN);
    QStringList oldPreview = previewSettings();
    setPreviewSettings( config->readListEntry( "Preview" ) );

    // read arrange configuration
    m_eSortCriterion  = (SortCriterion)config->readNumEntry("SortCriterion", NameCaseInsensitive);
    m_bSortDirectoriesFirst = config->readBoolEntry("DirectoriesFirst", true);
    m_itemsAlwaysFirst = config->readListEntry("AlwaysFirstItems"); // Distributor plug-in

    config->setGroup( "Devices" );
    m_enableDevices=config->readBoolEntry("enabled",false);
    QString tmpList=config->readEntry("exclude","kdedevice/hdd_mounted,kdedevice/hdd_unmounted,kdedevice/floppy_unmounted,kdedevice/cdrom_unmounted,kdedevice/floppy5_unmounted");
    kdDebug(1204)<<"m_excludeList"<<tmpList<<endl;
    m_excludedDevices=QStringList::split(",",tmpList,false);
    kdDebug(1204)<<" m_excludeList / item count:" <<m_excludedDevices.count()<<endl;
    if ( m_dirLister ) // only when called while running - not on first startup
    {
        configureDevices();
        m_dirLister->setShowingDotFiles( m_bShowDot );
        m_dirLister->emitChanges();
    }

    setArrangement(m_bVertAlign ? TopToBottom : LeftToRight);

    if ( KonqIconViewWidget::initConfig( init ) )
        lineupIcons(); // called if the font changed.

    setAutoArrange( false );

    if ( previewSettings().count() )
    {
        for ( QStringList::ConstIterator it = oldPreview.begin(); it != oldPreview.end(); ++it)
            if ( !previewSettings().contains( *it ) ){
                kdDebug(1204) << "Disabling preview for " << *it << endl;
                setIcons( iconSize(), (*it).latin1() /* revert no-longer wanted previews to icons */ );
            }
        startImagePreview( QStringList(), true );
    }
    else
    {
        stopImagePreview();
        setIcons( iconSize(), "" /* stopImagePreview */ );
    }

    if ( !init )
        updateContents();
}

void KDIconView::start( KRootWm *wm)
{
    // We can only start once
    Q_ASSERT(!m_dirLister);
    if (m_dirLister)
        return;

    kdDebug(1204) << "KDIconView::start" << endl;

    _krootwm = wm;
    
    // Create the directory lister
    m_dirLister = new KDirLister();

    connect( m_dirLister, SIGNAL( clear() ), this, SLOT( slotClear() ) );
    connect( m_dirLister, SIGNAL( started(const KURL&) ),
             this, SLOT( slotStarted(const KURL&) ) );
    connect( m_dirLister, SIGNAL( completed() ), this, SLOT( slotCompleted() ) );
    connect( m_dirLister, SIGNAL( newItems( const KFileItemList & ) ),
             this, SLOT( slotNewItems( const KFileItemList & ) ) );
    connect( m_dirLister, SIGNAL( deleteItem( KFileItem * ) ),
             this, SLOT( slotDeleteItem( KFileItem * ) ) );
    connect( m_dirLister, SIGNAL( refreshItems( const KFileItemList & ) ),
             this, SLOT( slotRefreshItems( const KFileItemList & ) ) );

    // Start the directory lister !
    m_dirLister->setShowingDotFiles( m_bShowDot );
    m_dirLister->openURL( url() );

    // Gather the list of directories to merge into the desktop
    // (the main URL is desktopURL(), no need for it in the m_mergeDirs list)
    m_mergeDirs.clear();

    //configureDevices();
    createActions();
}


void KDIconView::configureDevices()
{
 m_enableDevices=false;
 return;


}

void KDIconView::createActions()
{
    if (kapp->authorize("editable_desktop_icons"))
    {
        KAction *undo = KStdAction::undo( KonqUndoManager::self(), SLOT( undo() ), &m_actionCollection, "undo" );
        connect( KonqUndoManager::self(), SIGNAL( undoAvailable( bool ) ),
             undo, SLOT( setEnabled( bool ) ) );
        connect( KonqUndoManager::self(), SIGNAL( undoTextChanged( const QString & ) ),
             undo, SLOT( setText( const QString & ) ) );
        undo->setEnabled( KonqUndoManager::self()->undoAvailable() );

        KStdAction::cut( this, SLOT( slotCut() ), &m_actionCollection, "cut" );
        KStdAction::copy( this, SLOT( slotCopy() ), &m_actionCollection, "copy" );
        KStdAction::paste( this, SLOT( slotPaste() ), &m_actionCollection, "paste" );
        KAction *pasteTo = KStdAction::paste( this, SLOT( slotPopupPasteTo() ), &m_actionCollection, "pasteto" );
        pasteTo->setEnabled( false ); // only enabled during popupMenu()

        (void) new KAction( i18n( "&Rename" ), /*"editrename",*/ Key_F2, this, SLOT( renameSelectedItem() ), &m_actionCollection, "rename" );
        (void) new KAction( i18n( "&Move to Trash" ), "edittrash", Key_Delete, this, SLOT( slotTrash() ), &m_actionCollection, "trash" );
        (void) new KAction( i18n( "&Delete" ), "editdelete", SHIFT+Key_Delete, this, SLOT( slotDelete() ), &m_actionCollection, "del" );

        (void) new KAction( i18n( "&Shred" ), "editshred", CTRL+SHIFT+Key_Delete, this, SLOT( slotShred() ), &m_actionCollection, "shred" );

        // Initial state of the actions (cut/copy/paste/...)
        slotSelectionChanged();
        //init paste action
        slotClipboardDataChanged();
    }
}

void KDIconView::rearrangeIcons( SortCriterion sc, bool bSortDirectoriesFirst )
{
    m_eSortCriterion = sc;
    m_bSortDirectoriesFirst = bSortDirectoriesFirst;
    rearrangeIcons();
}

void KDIconView::rearrangeIcons()
{
    setupSortKeys();
    sort();
    arrangeItemsInGrid();
    slotSaveIconPositions();
}

void KDIconView::lineupIcons()
{
    KonqIconViewWidget::lineupIcons();
    slotSaveIconPositions();
}

void KDIconView::desktopResized()
{
    resize( kapp->desktop()->size());
    lineupIcons();
}

void KDIconView::lineupIcons(QIconView::Arrangement align)
{
    if (m_bVertAlign == align)
    {
        lineupIcons();
    }
    else
    {
        m_bVertAlign = align;
        setArrangement(m_bVertAlign ? TopToBottom : LeftToRight);
        rearrangeIcons();

        KConfig * config = KGlobal::config();
        config->setGroup( "Desktop Icons" );
        config->writeEntry( "VertAlign", m_bVertAlign );
        config->sync();
    }
}

QStringList KDIconView::selectedURLs()
{
    QStringList seq;

    QIconViewItem *it = firstItem();
    for (; it; it = it->nextItem() )
        if ( it->isSelected() ) {
            KFileItem *fItem = ((KFileIVI *)it)->item();
            seq.append( fItem->url().url() ); // copy the URL
        }

    return seq;
}

void KDIconView::recheckDesktopURL()
{
    // Did someone change the path to the desktop ?
    kdDebug(1204) << desktopURL().url() << endl;
    kdDebug(1204) << url().url() << endl;
    if ( desktopURL() != url() )
    {
        kdDebug(1204) << "Desktop path changed from " << url().url() <<
            " to " << desktopURL().url() << endl;
        setURL( desktopURL() ); // sets m_url and m_dotDirectoryPath
        delete m_dotDirectory;
        m_dotDirectory = 0L;
        m_dirLister->openURL( url() );
    }
}

KURL KDIconView::desktopURL()
{
    // Support both paths and URLs
    QString desktopPath = KGlobalSettings::desktopPath();
//     if (kdesktop_screen_number != 0) {
//         QString dn = "Desktop";
//         dn += QString::number(kdesktop_screen_number);
//         desktopPath.replace("Desktop", dn);
//     }

    KURL desktopURL;
    if (desktopPath[0] == '/')
        desktopURL.setPath(desktopPath);
    else
        desktopURL = desktopPath;

    Q_ASSERT( !desktopURL.isMalformed() );
    if ( desktopURL.isMalformed() ) // should never happen
        return QDir::homeDirPath() + "/" + "Desktop" + "/";

    return desktopURL;
}

void KDIconView::contentsMousePressEvent( QMouseEvent *e )
{
    if (!m_dirLister) return;
    //kdDebug(1204) << "KDIconView::contentsMousePressEvent" << endl;
    // QIconView, as of Qt 2.2, doesn't emit mouseButtonPressed for LMB on background
    if ( e->button() == LeftButton)
    {
        QIconViewItem *item = findItem( e->pos() );
        if ( !item )
        {
            // Left click menu
            KRootWm::self()->mousePressed( e->globalPos(), e->button() );
            return;
        }
    }
    KonqIconViewWidget::contentsMousePressEvent( e );
}

void KDIconView::mousePressEvent( QMouseEvent *e )
{
    KRootWm::self()->mousePressed( e->globalPos(), e->button() );
}

void KDIconView::wheelEvent( QWheelEvent* e )
{
    if (!m_dirLister) return;
    //kdDebug(1204) << "KDIconView::wheelEvent" << endl;
    if ( m_bWheelSwitchesWorkspace && KWin::numberOfDesktops() > 1 )
    {
        QIconViewItem *item = findItem( e->pos() );
        if ( !item )
        {
//             KDesktop * desk=(KDesktop *) parent();
//             int newDesk, curDesk = desk->kwinModule()->currentDesktop();
//             if( e->delta() < 0 )
//                 newDesk = curDesk % KWin::numberOfDesktops() + 1;
//             else
//                 newDesk = ( KWin::numberOfDesktops() + curDesk - 2 ) % KWin::numberOfDesktops() + 1;
//             KWin::setCurrentDesktop( newDesk );
            return;
         }
    }
    KonqIconViewWidget::wheelEvent( e );
}

void KDIconView::slotMouseButtonPressed(int _button, QIconViewItem* _item, const QPoint& _global)
{
    kdDebug() << "KDIconView::slotMouseButtonPressed" << endl;
    if (!m_dirLister) return;
    m_lastDeletedIconPos = QPoint(); // user action -> not renaming an icon
    if(_item) {
        if ( _button == RightButton )
        {
            ((KFileIVI*)_item)->setSelected( true );
            popupMenu( _global, selectedFileItems() );
        }
    }
    else
    {
        printf("KDIconView: calling KRootWm::self()->mousePressed()\n");
        _krootwm->mousePressed( _global, _button );
    }
}

void KDIconView::slotMouseButtonClickedKDesktop(int _button, QIconViewItem* _item, const QPoint&)
{
    if (!m_dirLister) return;
    //kdDebug(1204) << "KDIconView::slotMouseButtonClickedKDesktop" << endl;
    if ( _item && _button == MidButton )
        slotReturnPressed( _item );
}

// -----------------------------------------------------------------------------

void KDIconView::slotReturnPressed( QIconViewItem *item )
{
    kapp->propagateSessionManager();
    m_lastDeletedIconPos = QPoint(); // user action -> not renaming an icon
    if (item) {
        visualActivate(item);
        ((KFileIVI*)item)->returnPressed();
    }
}

// -----------------------------------------------------------------------------

void KDIconView::slotCut()
{
    cutSelection();
}

// -----------------------------------------------------------------------------

void KDIconView::slotCopy()
{
    copySelection();
}

// -----------------------------------------------------------------------------

void KDIconView::slotPaste()
{
    QMimeSource *data = QApplication::clipboard()->data();
    KURL::List urls;
    if ( QUriDrag::canDecode( data ) && KURLDrag::decode( data, urls ) && urls.count() == 1 )
    {
        // TODO support for multiple urls (see slotSaveDropPosition)
//         QPoint pos = KRootWm::self()->desktopMenuPosition(); // maybe some translation is needed ?
//         KDIconViewDragData dragData( pos.x(), pos.y(), urls.first().fileName() );
//         QValueList<KDIconViewDragData> lst;
//         lst.append( dragData );
//         saveFuturePosition( lst );
    }
    pasteSelection();
}

void KDIconView::slotPopupPasteTo()
{
    Q_ASSERT( !m_popupURL.isEmpty() );
    if ( !m_popupURL.isEmpty() )
        paste( m_popupURL );
}

void KDIconView::slotTrash()
{
    KonqOperations::del(this, KonqOperations::TRASH, selectedUrls());
}

void KDIconView::slotDelete()
{
    KonqOperations::del(this, KonqOperations::DEL, selectedUrls());
}

void KDIconView::slotShred()
{
    KonqOperations::del(this, KonqOperations::SHRED, selectedUrls());
}

// -----------------------------------------------------------------------------

// This method is called when right-clicking over one or more items
// Not to be confused with the global popup-menu, KRootWm, when doing RMB on the desktop
void KDIconView::popupMenu( const QPoint &_global, KFileItemList _items )
{
    if (!m_dirLister) return;
    if ( _items.count() == 1 )
        m_popupURL = _items.getFirst()->url();

    KAction* pasteTo = m_actionCollection.action( "pasteto" );
    if (pasteTo)
        pasteTo->setEnabled( m_actionCollection.action( "paste" )->isEnabled() );
//FIXME - should probably keep a KNewMenu about
	  KNewMenu *menuNew = new KNewMenu( &m_actionCollection, "new_menu" );
     KonqPopupMenu * popupMenu = new KonqPopupMenu( KonqBookmarkManager::self(), _items,
                                                    url(),
                                                    m_actionCollection, menuNew );

     popupMenu->exec( _global );
     delete popupMenu;
    m_popupURL = KURL();
    if (pasteTo)
        pasteTo->setEnabled( false );
}

void KDIconView::slotNewMenuActivated()
{
    //kdDebug(1204) << "KDIconView::slotNewMenuActivated" << endl;
    // New / <template> was chosen, a new file is going to appear soon,
    // make it appear at the position of the popupmenu.
    m_nextItemPos = KRootWm::self()->desktopMenuPosition();
}

// -----------------------------------------------------------------------------

void KDIconView::slotEnableAction( const char * name, bool enabled )
{
  //kdDebug(1204) << "slotEnableAction " << name << " enabled=" << enabled << endl;
  QCString sName( name );
  // No such actions here... konqpopupmenu provides them.
  if ( sName == "properties" || sName == "editMimeType" )
    return;

  KAction * act = m_actionCollection.action( sName.data() );
  if (act)
    act->setEnabled( enabled );
}

// -----------------------------------------------------------------------------

// Straight from kpropsdlg :)
bool KDIconView::isDesktopFile( KFileItem * _item ) const
{
  // only local files
  if ( !_item->isLocalFile() )
    return false;

  // only regular files
  if ( !S_ISREG( _item->mode() ) )
    return false;

  QString t( _item->url().path() );

  // only if readable
  if ( access( QFile::encodeName(t), R_OK ) != 0 )
    return false;

  // return true if desktop file
  return ( _item->mimetype() == QString::fromLatin1("application/x-desktop") );
}

QString KDIconView::stripDesktopExtension( const QString & text )
{
    if (text.right(7) == QString::fromLatin1(".kdelnk"))
      return text.left(text.length() - 7);
    else if (text.right(8) == QString::fromLatin1(".desktop"))
      return text.left(text.length() - 8);
    return text;
}

void KDIconView::makeFriendlyText( KFileIVI *fileIVI )
{
    KFileItem *item = fileIVI->item();
    QString desktopFile;
    if ( item->isDir() && item->isLocalFile() )
    {
        KURL u( item->url() );
        u.addPath( ".directory" );
        // using KStandardDirs as this one checks for path beeing
        // a file instead of a directory
        if ( KStandardDirs::exists( u.path() ) )
            desktopFile = u.path();
    }
    else if ( isDesktopFile( item ) )
    {
        desktopFile = item->url().path();
    }

    if ( !desktopFile.isEmpty() )
    {
        KSimpleConfig cfg( desktopFile, true );
        cfg.setDesktopGroup();
        QString name = cfg.readEntry("Name");
        if ( !name.isEmpty() )
            fileIVI->setText( name );
        else
            // For compatibility
            fileIVI->setText( stripDesktopExtension( fileIVI->text() ) );
    }
}

// -----------------------------------------------------------------------------

void KDIconView::slotClear()
{
    clear();
}

// -----------------------------------------------------------------------------

void KDIconView::slotNewItems( const KFileItemList & entries )
{
  // We have new items, so we'll need to repaint in slotCompleted
  m_bNeedRepaint = true;
  kdDebug() << "KDIconView::slotNewItems count=" << entries.count() << endl;
  KFileItemListIterator it(entries);
  KFileIVI* fileIVI = 0L;
  for (; it.current(); ++it)
  {
    // No delayed mimetype determination on the desktop
    it.current()->determineMimeType();
    fileIVI = new KFileIVI( this, it.current(), iconSize() );
    makeFriendlyText( fileIVI );

    kdDebug() << " slotNewItems: " << it.current()->url().url() << " text: " << fileIVI->text() << endl;
    fileIVI->setRenameEnabled( false );

    if ( !m_nextItemPos.isNull() ) // position remembered from e.g. RMB-popupmenu position, when doing New/...
    {
      kdDebug() << "slotNewItems : using popupmenu position " << m_nextItemPos.x() << "," << m_nextItemPos.y() << endl;
      fileIVI->move( m_nextItemPos.x(), m_nextItemPos.y() );
      m_nextItemPos = QPoint();
    }
    else if ( m_dotDirectory )
    {
      kdDebug() << "slotNewItems : trying to read position from .directory file"<<endl;
      QString group = iconPositionGroupPrefix();
      QString filename = it.current()->url().fileName();
      if ( filename.endsWith(".part") && !m_dotDirectory->hasGroup( group + filename ) )
          filename = filename.left( filename.length() - 5 );
      group.append( filename );
      kdDebug() << "slotNewItems : looking for group " << group << endl;
      if ( m_dotDirectory->hasGroup( group ) )
      {
        m_dotDirectory->setGroup( group );
        m_hasExistingPos = true;
        int x = m_dotDirectory->readNumEntry( "X" );
        int y = m_dotDirectory->readNumEntry( "Y" );

        kdDebug()<<"slotNewItems() x: "<<x<<" y: "<<y<<endl;

        QRect oldPos = fileIVI->rect();
        fileIVI->move( x, y );
        if ( !isFreePosition( fileIVI ) ) // if we can't put it there, then let QIconView decide
        {
            kdDebug()<<"slotNewItems() pos was not free :-("<<endl;
            fileIVI->move( oldPos.x(), oldPos.y() );
            m_dotDirectory->deleteGroup( group );
        } else kdDebug()<<"Using saved position"<<endl;
      }
      else
	{
		// Not found, we'll need to save the new pos
		kdDebug()<<"slotNewItems(): New item without position information, try to find a sane location"<<endl;

		moveToFreePosition(fileIVI);

          	m_bNeedSave = true;
	}
    } else
	{
		// Not found, we'll need to save the new pos
		 kdDebug()<<"Neither a  drop position stored nor m_dotDirectory set"<<endl;
		moveToFreePosition(fileIVI);

          	m_bNeedSave = true;

	}
  }
}

// -----------------------------------------------------------------------------

// see also KonqKfmIconView::slotRefreshItems
void KDIconView::slotRefreshItems( const KFileItemList & entries )
{
    //kdDebug(1204) << "KDIconView::slotRefreshItems" << endl;
    bool bNeedPreviewJob = false;
    KFileItemListIterator rit(entries);
    for (; rit.current(); ++rit)
    {
        bool found = false;
        QIconViewItem *it = firstItem();
        for ( ; it ; it = it->nextItem() )
        {
            KFileIVI * fileIVI = static_cast<KFileIVI *>(it);
            if ( fileIVI->item() == rit.current() ) // compare the pointers
            {
                kdDebug(1204) << "KDIconView::slotRefreshItems refreshing icon " << fileIVI->item()->url().url() << endl;
                if ( fileIVI->isThumbnail() ) {
                    bNeedPreviewJob = true;
                    fileIVI->invalidateThumbnail();
                }
                else
                    fileIVI->refreshIcon( true );
                fileIVI->setText( rit.current()->text() );
                makeFriendlyText( fileIVI );
                if ( rit.current()->isMimeTypeKnown() )
                    fileIVI->setMouseOverAnimation( rit.current()->iconName() );
                found = true;
                break;
            }
        }
	if ( !found )
            kdDebug(1204) << "Item not found: " << rit.current()->url().url() << endl;
    }
    if ( bNeedPreviewJob && previewSettings().count() )
    {
        startImagePreview( QStringList(), false );
    }
    else
    {
        // In case we replace a big icon with a small one, need to repaint.
        updateContents();
        // Can't do that with m_bNeedRepaint since slotCompleted isn't called
    }
}


void KDIconView::refreshIcons()
{
    QIconViewItem *it = firstItem();
    for ( ; it ; it = it->nextItem() )
    {
        KFileIVI * fileIVI = static_cast<KFileIVI *>(it);
        fileIVI->refreshIcon( true );
        makeFriendlyText( fileIVI );
    }
}


// -----------------------------------------------------------------------------

void KDIconView::slotDeleteItem( KFileItem * _fileitem )
{
//    kdDebug(1204) << "KDIconView::slotDeleteItems" << endl;
    // we need to find out the KFileIVI containing the fileitem
    QIconViewItem *it = firstItem();
    while ( it ) {
      KFileIVI * fileIVI = static_cast<KFileIVI *>(it);
      if ( fileIVI->item() == _fileitem ) { // compare the pointers
        // Delete this item.
        //kdDebug(1204) << fileIVI->text() << endl;

        if ( m_dotDirectory )
        {
            QString group = iconPositionGroupPrefix();
            group.append( fileIVI->item()->url().fileName() );
            if ( m_dotDirectory->hasGroup( group ) )
                m_dotDirectory->deleteGroup( group );
        }

        m_lastDeletedIconPos = fileIVI->pos();
        delete fileIVI;
        break;
      }
      it = it->nextItem();
    }
    m_bNeedRepaint = true;
}

// -----------------------------------------------------------------------------

void KDIconView::slotStarted( const KURL& _url )
{
//   kdDebug(1204) << "KDIconView::slotStarted url: " << _url.url() << " url().url(): "<<url().url()<<endl;
    // main directory only
    if ( _url.cmp( url(), true /*ignore slash*/) && !m_dotDirectory )
    {
        m_dotDirectory = new KSimpleConfig( dotDirectoryPath(), true );
        m_bNeedSave = false;
        m_bNeedRepaint = false;
    }
}

void KDIconView::slotCompleted()
{
    // Root item ? Store in konqiconviewwidget (used for drops onto the background, for instance)
    if ( m_dirLister->rootItem() )
      setRootItem( m_dirLister->rootItem() );

    if ( m_dotDirectory )
    {
      delete m_dotDirectory;
      m_dotDirectory = 0;
    }

    if ( previewSettings().count() )
        startImagePreview( QStringList(), true );
    else
    {
        stopImagePreview();
        setIcons( iconSize(), "" /* stopImagePreview */ );
    }

    // during first run need to rearrange all icons so default config settings will be used
    kdDebug(1204)<<"slotCompleted() m_hasExistingPos: "<<(m_hasExistingPos?(int)1:(int)0)<<endl;
    if (!m_hasExistingPos)
        rearrangeIcons();

//    kdDebug(1204) << "KDIconView::slotCompleted save:" << m_bNeedSave << " repaint:" << m_bNeedRepaint << endl;
    if ( m_bNeedSave )
    {
        slotSaveIconPositions();
        m_hasExistingPos = true; // if we didn't have positions, we have now.
    }
    if ( m_bNeedRepaint )
        viewport()->repaint();
}

void KDIconView::slotClipboardDataChanged()
{
    // This is very related to KonqDirPart::slotClipboardDataChanged

    KURL::List lst;
    QMimeSource *data = QApplication::clipboard()->data();
    if ( data->provides( "application/x-kde-cutselection" ) && data->provides( "text/uri-list" ) )
        if ( KonqDrag::decodeIsCutSelection( data ) )
            (void) KURLDrag::decode( data, lst );

    disableIcons( lst );

    bool paste = ( data->format() != 0 ); // check if there's something in the clipboard
    slotEnableAction( "paste", paste );
}

void KDIconView::slotItemRenamed(QIconViewItem* _item)
{
    //kdDebug(1204) << "KDIconView::slotItemRenamed(item)" << endl;
    if ( !_item )
      return;

    KFileIVI *fileItem = static_cast< KFileIVI* >( _item );
    if ( !fileItem->item() )
      return;

    QString desktopFile( fileItem->item()->url().path() );
    // first and foremost, we make sure that this is a .desktop file
    // before we write anything to it
    KMimeType::Ptr type = KMimeType::findByURL( fileItem->item()->url() );
    if ( type->name() != "application/x-desktop") {
        if(type->name() == "inode/directory" && !desktopFile.isEmpty()) {
            desktopFile += "/.directory";
        } else {
            return;
        }
    }

    if ( desktopFile.isEmpty() )
      return;

    KDesktopFile cfg( desktopFile, false );

    // if we don't have the desktop entry group, then we assume that
    // it's not a config file (and we don't nuke it!)
    if ( !cfg.hasGroup( "Desktop Entry" ) )
      return;

    if ( cfg.readName() == _item->text() )
      return;

    cfg.writeEntry( "Name", _item->text(), true, false, true );
    cfg.sync();
}

void KDIconView::slotSaveDropPosition( QDropEvent *ev, const QValueList<QIconDragItem> & )
{
    m_lastDeletedIconPos = QPoint();
    //kdDebug() << "KDIconView::slotSaveDropPosition will proceed: " << (m_dirLister && !m_dotDirectory) << "  urilist: " << ev->provides( "text/uri-list" ) << endl;
    if (!m_dirLister) return; // too early
    if (m_dotDirectory) return; // we are listing the dir...
    if (ev->provides( "text/uri-list" ))
    {
        KURL::List lst;
        if ( KURLDrag::decode( ev, lst ) ) // Are they urls ?
        {
            //kdDebug() << "KDIconView::slotSaveDropPosition found " << lst.count() << " urls" << endl;
            // For now, deal with only one icon
            // TODO: if we can decode as application/x-qiconlist then we
            // can even store the position of each icon (to keep their relative position)
            // Problem: QIconDragPrivate::decode is, well, private ;)
            //
            // Also: this would enable to position the icon using the hotspot of the drag
            // (the relative position of the mouse wrt the icon topleft corner),
            // instead of the "centering" code below.
            if ( lst.count() == 1 )
            {
                KURL url = lst.first();
                int x = QMAX( 0 , ev->pos().x() - gridXValue()/2 );
                int y = QMAX( 0, ev->pos().y() - (firstItem() ? firstItem()->height()/2 : 20) );
                kdDebug() << "Saving drop position for " << url.fileName() << " at " << x << "," << y << endl;
                KDIconViewDragData dragData( x, y, url.fileName() );
                QValueList<KDIconViewDragData> lst;
                lst.append( dragData );
                saveFuturePosition( lst );
            }
        }
    }
}

void KDIconView::saveFuturePosition( const QValueList<KDIconViewDragData> &lst )
{
    if (m_dotDirectory) return; // this should be checked by the caller !
    m_dotDirectory = new KSimpleConfig( dotDirectoryPath() );
    QValueList<KDIconViewDragData>::ConstIterator it = lst.begin();
    for ( ; it!= lst.end() ; ++it )
    {
        kdDebug() << "KDIconView::saveFuturePosition x=" << (*it).x << " y=" << (*it).y << " filename=" << (*it).filename << endl;
        m_dotDirectory->setGroup( iconPositionGroupPrefix() + (*it).filename );
        m_dotDirectory->writeEntry( "X", (*it).x );
        m_dotDirectory->writeEntry( "Y", (*it).y );
    }
    m_dotDirectory->sync();
    delete m_dotDirectory;
    m_dotDirectory = 0L;
}

// -----------------------------------------------------------------------------

void KDIconView::showEvent( QShowEvent *e )
{
    //HACK to avoid QIconView calling arrangeItemsInGrid (Simon)
    //EVEN MORE HACK: unfortunately, QScrollView has no concept of
    //TopToBottom, therefore, it always adds LeftToRight.  So, if any of
    //the icons have a setting, we'll use QScrollView.. but otherwise,
    //we use the iconview
   //kdDebug(1204)<<"showEvent() m_hasExistingPos: "<<(m_hasExistingPos?(int)1:(int)0)<<endl;
    if (m_hasExistingPos)
        QScrollView::showEvent( e );
    else
        KIconView::showEvent( e );
}

void KDIconView::contentsDropEvent( QDropEvent * e )
{
    // mind: if it's a filedrag which itself is an image, libkonq is called. There's a popup for drops as well
    // that contains the same line "Set as Wallpaper" in void KonqOperations::asyncDrop
    bool isColorDrag = KColorDrag::canDecode(e);
    bool isImageDrag = QImageDrag::canDecode(e);

    bool isImmutable = KGlobal::config()->isImmutable();

    if ( isColorDrag ) {
        if ( isImmutable ) // just ignore event in kiosk-mode
            emit colorDropEvent( e );
    }
    else if ( isImageDrag ) {
        if ( isImmutable ) // just ignore event in kiosk-mode
            emit imageDropEvent( e );
    } else
        KonqIconViewWidget::contentsDropEvent( e );
}

// don't scroll when someone uses his nifty mouse wheel
void KDIconView::viewportWheelEvent( QWheelEvent * e )
{
    e->accept();
}

void KDIconView::updateWorkArea( const QRect &wr )
{
//    kdDebug(1204) << "KDIconView::updateWorkArea wr: " << wr.x() << "," << wr.y()
//              << " " << wr.width() << "x" << wr.height() << endl;
    setMargins( wr.left(), wr.top(),
                QApplication::desktop()->width() - wr.right() - 1,
                QApplication::desktop()->height() - wr.bottom() - 1 );
    resizeContents( viewport()->width(), viewport()->height() );
//    kdDebug(1204) << "resizeContents " << viewport()->width() << "x" << viewport()->height() << endl;

    for ( QIconViewItem *item = firstItem(); item; item = item->nextItem() ) {
        QRect r( item->rect() );
        int dx = 0, dy = 0;
        if ( r.bottom() > visibleHeight() )
            dy = visibleHeight() - r.bottom() - 1;
        if ( r.right() > visibleWidth() )
            dx = visibleWidth() - r.right() - 1;
        if ( dx != 0 || dy != 0 )
            item->moveBy( dx, dy );
    }

    viewport()->repaint( FALSE );
    repaint( FALSE );
}

void KDIconView::setupSortKeys()
{
    // can't use sorting in KFileIVI::setKey()
    setProperty("sortDirectoriesFirst", QVariant(false, 0));

    for (QIconViewItem *it = firstItem(); it; it = it->nextItem())
    {
        QString strKey;

        if (!m_itemsAlwaysFirst.isEmpty())
        {
            QString strFileName = static_cast<KFileIVI *>( it )->item()->url().fileName();
            int nFind = m_itemsAlwaysFirst.findIndex(strFileName);
            if (nFind >= 0)
                strKey = "0" + QString::number(nFind);
        }

        if (strKey.isEmpty())
        {
            switch (m_eSortCriterion)
            {
            case NameCaseSensitive:
                strKey = it->text();
                break;
            case NameCaseInsensitive:
                strKey = it->text().lower();
                break;
            case Size:
                strKey = KIO::number(static_cast<KFileIVI *>( it )->item()->size()).rightJustify(20, '0');
                break;
            case Type:
                // Sort by Type + Name (#17014)
                strKey = static_cast<KFileIVI *>( it )->item()->mimetype() + '~' + it->text().lower();
                break;
            }

            if (m_bSortDirectoriesFirst)
            {
                if (S_ISDIR(static_cast<KFileIVI *>( it )->item()->mode()))
                    strKey.prepend(sortDirection() ? '1' : '2');
                else
                    strKey.prepend(sortDirection() ? '2' : '1' );
            }
            else
                strKey.prepend('1');
        }

        it->setKey(strKey);
    }
}

bool KDIconView::isFreePosition( const QIconViewItem *item ) const
{
    QRect r = item->rect();
    QIconViewItem *it = firstItem();
    for (; it; it = it->nextItem() )
    {
        if ( !it->rect().isValid() || it == item )
            continue;

        if ( it->intersects( r ) )
            return false;
    }

    return true;
}

bool KDIconView::isFreePosition( const QIconViewItem *item ,const QRect& rect) const
{
    QIconViewItem *it = firstItem();
    for (; it; it = it->nextItem() )
    {
        if ( !rect.isValid() || it == item )
            continue;

        if ( it->intersects( rect ) )
            return false;
    }

    return true;
}

void KDIconView::moveToFreePosition(QIconViewItem *item )
{
    bool success;
    // It may be that a file has been renamed. In this case,
    // m_lastDeletedIconPos is the position to use for this "apparently new" item.
    // (We rely on deleteItem being now emitted before newItems).
    if ( !m_lastDeletedIconPos.isNull() )
        // Problem is: I'd like to compare those two file's attributes
        // (size, creation time, modification time... etc.) but since renaming
        // is done by kpropsdlg, all of those can have changed (and creation time
        // is different since the new file is a copy!)
    {
        kdDebug() << "Moving " << item->text() << " to position of last deleted icon." << endl;
        item->move( m_lastDeletedIconPos );
        m_lastDeletedIconPos = QPoint();
    }

    //try to find a free place to put the item, honouring the m_bVertAlign property
    QRect rect=item->rect();
    if (m_bVertAlign)
    {
	kdDebug()<<"moveToFreePosition for vertical alignment"<<endl;

	rect.moveTopLeft(QPoint(spacing(),spacing()));
      do
      {
          success=false;
          while (rect.bottom()<height())
          {
   	     if (!isFreePosition(item,rect))
		{
	                rect.moveBy(0,rect.height()+spacing());
		}
	     else
	      {
                 success=true;
                 break;
	      }
          }

          if (!success)
          {
		rect.moveTopLeft(QPoint(rect.right()+spacing(),spacing()));
          } else break;
      }
      while (item->rect().right()<width());
      if (success)
	item->move(rect.x(),rect.y());
      else
        item->move(width()-spacing()-item->rect().width(),height()-spacing()-item->rect().height());

    }

}

#include "kdiconview.moc"
