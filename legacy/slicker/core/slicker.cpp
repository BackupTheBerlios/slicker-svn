/*****************************************************************

Copyright (c) 2003 the slicker authors. See file AUTHORS.

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

#include "slicker.h"

#include "taskbar.h"
#include "cardhandler.h"
#include "slider.h"

#include "PreferencesDialog.h"


#include <kdebug.h>
#include <qvaluelist.h>
#include <dcopclient.h>

#include "ConfigModule.h"

Slicker::Slicker()
	: KUniqueApplication()
{
   mpPreferences = new SlickerPreferences();
   mpCardHandler = new CardHandler();
   mpSlider = mpPreferences->mCreateSlider ? new Slider() : 0;
   mpTaskbar = mpPreferences->mCreateTaskbar ? new Taskbar() : 0;

   dcopClient()->setDefaultObject("Panel");
   dcopClient()->send( "ksplash", "", "upAndRunning(QString)", "slicker");

   connect(mpPreferences, SIGNAL(configChanged()), SLOT(configChanged()));
}

Slicker::~Slicker()
{
   if (mpTaskbar) delete mpTaskbar;
   if (mpSlider)  delete mpSlider;
   delete mpCardHandler;
   delete mpPreferences;
}

void Slicker::configChanged() {
   if (mpPreferences->mCreateTaskbar) {
      if (!mpTaskbar) {
         mpTaskbar = new Taskbar();
      }
   } else {
      if (mpTaskbar) {
         delete mpTaskbar;
         mpTaskbar=0;
      }
   }

   if (mpPreferences->mCreateSlider) {
      if (!mpSlider) {
         mpSlider = new Slider();
      }
   } else {
      if (mpSlider) {
         delete mpSlider;
         mpSlider=0;
      }
   }
}
