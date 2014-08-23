#include "vtekinterface.h"
#include "vtslider.h"
#include "vtcombobox.h"
#include "vtekbuttongroup.h"
#include "vttogglebutton.h"
#include "vtdrawimages.h"
#include "vtcheckbox.h"
#include "vtprogessbar.h"
#include "vtlabel.h"
#include "vtrxmonitorview.h"
#include "vtlineedit.h"
VtekInterface *VtekInterface::createVtekObject(QObject *view)
{
    const char* chars = view->metaObject()->className();
    QString className(chars);
    VtekInterface *result = NULL;
     if (className == "QSlider") {
        result = new VtSlider(view);
    }
    else if (className == "QComboBox") {
        result = new VtComboBox(view);
    }
    else if (className == "QButtonGroup") {
        result = new VtButtonGroup(view);
    }
    else if (className == "QToolButton") {
        result = new VtToggleButton(view);
    }
    else if(className == "DrawImages")
     {
         result = new vtdrawimages(view);
     }
     else if(className == "QCheckBox")
      {
          result = new vtcheckbox(view);
      }
     else if(className == "QProgressBar")
      {
          result = new vtprogessbar(view);
      }
     else if(className == "QLabel")
      {
          result = new vtlabel(view);
      }
     else if(className == "RxMonitorView")
      {
          result = new VtRxMonitorView(view);
      }
     else if(className == "QLineEdit")
      {
          result = new vtlineEdit(view);
      }
    return result;
}
