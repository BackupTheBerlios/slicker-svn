
ADD_LIBRARY(slicker_base SHARED slicker_base_sources slicker_base_moc)

QT_WRAP_CPP(slicker_base slicker_base_moc 
            appletmanager.h cardstack.h edgewidgetmanager.h pluginmanager.h pluginprefs.h sessionmanager.h slider.h )

AUX_SOURCE_DIRECTORY( . slicker_base_sources )

INSTALL_TARGETS( /lib slicker_base )
