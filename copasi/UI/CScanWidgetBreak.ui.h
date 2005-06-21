/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/Attic/CScanWidgetBreak.ui.h,v $
   $Revision: 1.2 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2005/06/21 20:33:59 $
   End CVS Header */

#include <qvalidator.h>
#include <CCopasiSelectionDialog.h>
#include "qtUtilities.h"

void CScanWidgetBreak::init()
{}

//comboBoxReport  line|page|file|no
//comboBoxPlot   curve|window|no

#include "report/CCopasiObjectName.h"
bool CScanWidgetBreak::initFromScanItem(CCopasiParameterGroup * pg)
{
  void* tmp;

  if (!(tmp = pg->getValue("Type").pVOID)) return false;
  CScanProblem::Type type = *(CScanProblem::Type*)tmp;
  if (type != CScanProblem::SCAN_BREAK)
    return false;

  if (!(tmp = pg->getValue("Report break").pVOID)) return false;
  comboBoxReport->setCurrentItem(*(unsigned C_INT32*)tmp);

  if (!(tmp = pg->getValue("Plot break").pVOID)) return false;
  comboBoxPlot->setCurrentItem(*(unsigned C_INT32*)tmp);

  return true;
}

bool CScanWidgetBreak::saveToScanItem(CScanProblem * pg) const
  {
    CScanProblem::Type type = CScanProblem::SCAN_BREAK;

    unsigned C_INT32 steps = 1;

    CCopasiParameterGroup* tmp = pg->createScanItem(type, steps);
    assert(tmp);

    tmp->setValue("Report break", (unsigned C_INT32)comboBoxReport->currentItem());
    tmp->setValue("Plot break", (unsigned C_INT32)comboBoxPlot->currentItem());

    return true;
  }
