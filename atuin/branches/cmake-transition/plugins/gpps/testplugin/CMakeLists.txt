

ADD_LIBRARY( slicker_testplugin MODULE testplugin.cpp slicker_testplugin_moc)
QT_WRAP_CPP( slicker_testplugin slicker_testplugin_moc testplugin.h )

ADD_LIBRARY( kcm_slicker_testplugin MODULE testpluginprefs.cpp slicker_testpluginprefs_moc )
QT_WRAP_CPP( kcm_slicker_testplugin slicker_testpluginprefs_moc testpluginprefs.h )

INSTALL_TARGETS( ${SLICKER_PLUGINS_DIR} slicker_testplugin kcm_slicker_testplugin )
INSTALL_FILES( ${SLICKER_SERVICES_DIR} slicker_testplugin.desktop slicker_testplugin_config.desktop )
