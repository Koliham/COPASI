/**
 *  CTrajectoryProblem class.
 *  This class describes the trajectory problem, i.e., it allows to specify
 *  for example initial conditions and number of steps.
 *
 *  Created for Copasi by Stefan Hoops 2002
 */

#include <string>

//#define COPASI_TRACE_CONSTRUCTION
#include "copasi.h"
#include "model/CModel.h"
#include "model/CState.h"
#include "utilities/CGlobals.h"
#include "utilities/CReadConfig.h"
#include "utilities/CWriteConfig.h"
#include "CScanProblem.h"

/**
 *  Default constructor.
 *
 */
CScanProblem::CScanProblem()
{
  CONSTRUCTOR_TRACE;
  //scanItem = new CMethodParameterList*[5];

  //CMethodParameterList *cmpl = new CMethodParameterList();
  //cmpl->setName("MyParameter");

  //cout<<"\ndone";
}

/**
 *  Copy constructor.
 *  @param "const CTrajectoryProblem &" src
 */
CScanProblem::CScanProblem(const CScanProblem & C_UNUSED(src))
{CONSTRUCTOR_TRACE;}

/**
 *  Destructor.
 */
CScanProblem::~CScanProblem()
{DESTRUCTOR_TRACE;}

/**
 *  Size of the scanItem vector
 */
C_INT32 CScanProblem::scanItemSize()
{
  return scanItem.size();
}

/**
 *  Add a Scan Item to the vector ScanItem
 * @param "CMethodParameterList *" Item
 */
void CScanProblem::addScanItem(CMethodParameterList * Item)
{
  scanItem.add(Item);
}
/**
 *  Get a Scan Item from the vector ScanItem
 * @param "C_INT32" itemNumber
 */
CMethodParameterList * CScanProblem::getScanItem(C_INT32 itemNumber)
{
  return scanItem[itemNumber];
}

/**
 *  Add a parameter to a scan item
 * @param "const C_INT32" itemNumber
 * @param "const std::string &" name
 * @param "const double &" value
 */
void CScanProblem::addScanItemParameter(const C_INT32 itemNumber,
                                        const std::string & name,
                                        const double & value)
{
  scanItem[itemNumber]->add(name, value);
}

/**
 *  Get a parameter from a scan item
 * @param "const C_INT32" itemNumber
 * @param "const std::string &" name
 */
const double & CScanProblem::getScanItemParameter(const C_INT32 itemNumber,
    const std::string & name)
{
  return scanItem[itemNumber]->getValue(name);
}

/**
 *  Set the value of a parameter in a scan item
 * @param "const C_INT32" itemNumber
 * @param "const std::string &" name
 * @param "const double &" value
 */
void CScanProblem::setScanItemParameter(const C_INT32 itemNumber,
                                        const std::string & name,
                                        const double & value)
{
  scanItem[itemNumber]->setValue(name, value);
}

/**
 * Load a trajectory problem
 * @param "CReadConfig &" configBuffer
 */

/*
void CScanProblem::load(CReadConfig & configBuffer,
                              CReadConfig::Mode mode)
{
  if (configBuffer.getVersion() < "4.0")
    {
      mpModel = Copasi->Model;
      configBuffer.getVariable("EndTime", "C_FLOAT64",
                               (void *) & mEndTime,
                               CReadConfig::LOOP);
      configBuffer.getVariable("Points", "C_INT32",
                               (void *) & mStepNumber);
      mStepNumberSetLast = true;
      mStartTime = 0.0;
      sync();
      mpInitialState = mpModel->getInitialState();
    }
  else
    {
      string Tmp;
 
      configBuffer.getVariable("TrajectoryProblemModel", "string", &Tmp, mode);
      if (Tmp == Copasi->Model->getTitle())
        mpModel = Copasi->Model;
      else
        fatalError();
 
      configBuffer.getVariable("TrajectoryProblemStepNumber",
                               "C_INT32", &mStepNumber);
      configBuffer.getVariable("TrajectoryProblemStepSize",
                               "C_FLOAT64", &mStepSize);
      configBuffer.getVariable("TrajectoryProblemSetLast",
                               "bool", &mStepNumberSetLast);
      configBuffer.getVariable("TrajectoryProblemStartTime",
                               "C_FLOAT64", &mStartTime);
      configBuffer.getVariable("TrajectoryProblemEndTime",
                               "C_FLOAT64", &mEndTime);
      mpInitialState = new CState;
      mpInitialState->load(configBuffer);
    }
}
 
 */

/**
 * Save a trajectory problem
 * @param "CWriteConfig &" configBuffer
 */

/*
void CScanProblem::save(CWriteConfig & configBuffer) const
  {
    string Tmp = mpModel->getTitle();
    configBuffer.setVariable("TrajectoryProblemModel", "string", &Tmp);
 
    configBuffer.setVariable("TrajectoryProblemStepNumber",
                             "C_INT32", &mStepNumber);
    configBuffer.setVariable("TrajectoryProblemStepSize",
                             "C_FLOAT64", &mStepSize);
    configBuffer.setVariable("TrajectoryProblemSetLast",
                             "bool", &mStepNumberSetLast);
    configBuffer.setVariable("TrajectoryProblemStartTime",
                             "C_FLOAT64", &mStartTime);
    configBuffer.setVariable("TrajectoryProblemEndTime",
                             "C_FLOAT64", &mEndTime);
    mpInitialState->save(configBuffer);
  }
 */
