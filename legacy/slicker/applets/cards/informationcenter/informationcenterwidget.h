/***************************************************************************
    copyright (C) 2003 by Sven Leiber <s.leiber@web.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef INFORMATIONCENTERWIDGET_H
#define INFORMATIONCENTERWIDGET_H

#include <qwidget.h>

class QVBoxLayout;
class BaseinfoWidget;
//class InfoWidget;
class DiskInfoWidget;
class ConfigDialogImpl;

class InformationcenterWidget : public QWidget
{
  Q_OBJECT

public:

  InformationcenterWidget(QWidget* parent, const char* name=0, ConfigDialogImpl *config=0L);
  ~InformationcenterWidget();

protected:
  QVBoxLayout *baselayout;

  BaseinfoWidget *infowidget;
//  InfoWidget *allinfowidget;
  DiskInfoWidget *diskinfowidget;
  ConfigDialogImpl *_config;
};

#endif
