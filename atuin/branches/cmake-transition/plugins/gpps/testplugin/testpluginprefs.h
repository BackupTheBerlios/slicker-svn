#ifndef TESTPLUGINPREFERENCES_H
#define TESTPLUGINPREFERENCES_H

#include <kcmodule.h>
#include <qstring.h>
#include <qlineedit.h>

#include "pluginprefspage.h"

/**
  *@author Olivier Goffart
  */

class TestpluginPreferences : public PluginPrefsPage
{
   Q_OBJECT
public:

    TestpluginPreferences(QWidget *parent = 0, const char* name = 0, const QStringList &args = QStringList());
    ~TestpluginPreferences();

    virtual void save();
    virtual void load();

private slots:
    void textChanged();

private:
    QLineEdit *_lineEdit;
};

#endif

