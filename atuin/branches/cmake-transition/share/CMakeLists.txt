

ADD_LIBRARY( slicker_share SHARED slicker_share_sources slicker_share_moc)

QT_WRAP_CPP(slicker_share slicker_share_moc 
            appletdef.h applethostmenu.h edgewidget.h edgewidgethelpers.h plugin.h pluginprefspage.h)

AUX_SOURCE_DIRECTORY( . slicker_share_sources )

INSTALL_TARGETS( /lib slicker_share )
