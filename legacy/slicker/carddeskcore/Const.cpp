/***************************************************************************
                          Const.cpp  -  description
                             -------------------
    begin                : Thu Dec 26 2002
    copyright            : (C) 2002 by Shamyl Zakariya
    email                : zakariya@earthlink.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Const.h"
#include <math.h>

namespace CardDesk {

QString EdgeToString(ScreenEdge edge)
{
	QString str;
	switch (edge)
	{
		case TopEdge: str = "TopEdge"; break;
		case RightEdge: str = "RightEdge"; break;
		case BottomEdge: str = "BottomEdge"; break;
		case LeftEdge: str = "LeftEdge"; break;
		default: break;
	}

	return str;
}

ScreenEdge StringToEdge(const QString &e)
{
	QString str(e.lower());
	if ((str == "topedge") || (str == "top")) return TopEdge;
	if ((str == "rightedge") || (str == "right")) return RightEdge;
	if ((str == "bottomedge") || (str == "bottom")) return BottomEdge;
	if ((str == "leftedge") || (str == "left")) return LeftEdge;

	return None;
}

QString PluginTypeToString( PluginType type )
{
	QString str;
	switch( type )
	{
		case AppletOnly: str = "AppletOnly"; break;
		case CustomCardAndApplet: str = "CustomCardAndApplet"; break;
		case CustomCardOnly: str = "CustomCardOnly"; break;
		default: str = "None";
	}

	return str;
}


double distance(QPoint a, QPoint b)
{
	return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}

}
