// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/optimization/COptProblem.cpp,v $
//   $Revision: 1.95.4.1 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2008/07/31 16:33:37 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/**
 *  File name: COptProblem.cpp
 *
 *  Programmer: Yongqun He
 *  Contact email: yohe@vt.edu
 *  Purpose: This is the source file of the COptProblem class.
 *           It specifies the optimization problem with its own members and
 *           functions. It's used by COptAlgorithm class and COptimization class
 */
#include <float.h>

#include "copasi.h"
#include "COptTask.h"
#include "COptProblem.h"
#include "COptItem.h"

#include "function/CFunctionDB.h"

#include "CopasiDataModel/CCopasiDataModel.h"

#include "steadystate/CSteadyStateTask.h"
#include "trajectory/CTrajectoryTask.h"
#include "steadystate/CSteadyStateProblem.h"
#include "trajectory/CTrajectoryProblem.h"

#include "model/CModel.h"
#include "model/CCompartment.h"

#include "report/CCopasiObjectReference.h"
#include "report/CKeyFactory.h"

#include "utilities/CProcessReport.h"
#include "utilities/CCopasiException.h"

C_FLOAT64 COptProblem::mInfinity;

//  Default constructor
COptProblem::COptProblem(const CCopasiTask::Type & type,
                         const CCopasiContainer * pParent):
    CCopasiProblem(type, pParent),
    mpParmSteadyStateKey(NULL),
    mpParmTimeCourseKey(NULL),
    mpParmObjectiveFunctionKey(NULL),
    mpParmMaximize(NULL),
    mpGrpItems(NULL),
    mpGrpConstraints(NULL),
    mpOptItems(NULL),
    mpConstraintItems(NULL),
    mpSteadyState(NULL),
    mpTrajectory(NULL),
    mpFunction(NULL),
    mUpdateMethods(),
    mInitialRefreshMethods(),
    mRefreshMethods(),
    mRefreshConstraints(),
    mCalculateValue(0),
    mSolutionVariables(),
    mOriginalVariables(),
    mSolutionValue(0),
    mCounter(0),
    mFailedCounter(0),
    mConstraintCounter(0),
    mFailedConstraintCounter(0),
    mCPUTime(CCopasiTimer::CPU, this),
    mhSolutionValue(C_INVALID_INDEX),
    mhCounter(C_INVALID_INDEX),
    mStoreResults(false),
    mHaveStatistics(false),
    mGradient(0)
{
  initializeParameter();
  initObjects();
}

// copy constructor
COptProblem::COptProblem(const COptProblem& src,
                         const CCopasiContainer * pParent):
    CCopasiProblem(src, pParent),
    mpParmSteadyStateKey(NULL),
    mpParmTimeCourseKey(NULL),
    mpParmObjectiveFunctionKey(NULL),
    mpParmMaximize(NULL),
    mpGrpItems(NULL),
    mpGrpConstraints(NULL),
    mpOptItems(NULL),
    mpConstraintItems(NULL),
    mpSteadyState(src.mpSteadyState),
    mpTrajectory(src.mpTrajectory),
    mpFunction(NULL),
    mUpdateMethods(),
    mInitialRefreshMethods(),
    mRefreshMethods(),
    mRefreshConstraints(),
    mCalculateValue(src.mCalculateValue),
    mSolutionVariables(src.mSolutionVariables),
    mOriginalVariables(src.mOriginalVariables),
    mSolutionValue(src.mCalculateValue),
    mCounter(0),
    mFailedCounter(0),
    mConstraintCounter(0),
    mFailedConstraintCounter(0),
    mCPUTime(CCopasiTimer::CPU, this),
    mhSolutionValue(C_INVALID_INDEX),
    mhCounter(C_INVALID_INDEX),
    mStoreResults(src.mStoreResults),
    mHaveStatistics(src.mHaveStatistics),
    mGradient(src.mGradient)
{
  initializeParameter();
  initObjects();

  if (src.mpFunction)
    {
      createObjectiveFunction();
      mpFunction->setInfix(src.mpFunction->getInfix());
      setValue("ObjectiveFunction", mpFunction->getKey());
    }
}

// Destructor
COptProblem::~COptProblem()
{
  if (mpFunction && CCopasiDataModel::Global &&
      CCopasiDataModel::Global->getFunctionList() &&
      CCopasiDataModel::Global->getFunctionList()->loadedFunctions()[mpFunction->getObjectName()] == mpFunction)
    CCopasiDataModel::Global->getFunctionList()->loadedFunctions().remove(mpFunction->getObjectName());

  pdelete(mpFunction);
}

void COptProblem::initializeParameter()
{
  mpParmSteadyStateKey =
    assertParameter("Steady-State", CCopasiParameter::KEY, std::string(""))->getValue().pKEY;
  mpParmTimeCourseKey =
    assertParameter("Time-Course", CCopasiParameter::KEY, std::string(""))->getValue().pKEY;
  mpParmObjectiveFunctionKey =
    assertParameter("ObjectiveFunction", CCopasiParameter::KEY, std::string(""))->getValue().pKEY;
  mpParmMaximize =
    assertParameter("Maximize", CCopasiParameter::BOOL, false)-> getValue().pBOOL;

  mpGrpItems = assertGroup("OptimizationItemList");
  mpGrpConstraints = assertGroup("OptimizationConstraintList");

  elevateChildren();
}

bool COptProblem::elevateChildren()
{
  mpGrpItems =
    elevate<CCopasiParameterGroup, CCopasiParameterGroup>(mpGrpItems);
  if (!mpGrpItems) return false;

  std::vector<CCopasiParameter *> * pValue =
    mpGrpItems->CCopasiParameter::getValue().pGROUP;

  index_iterator it = pValue->begin();
  index_iterator end = pValue->end();

  for (; it != end; ++it)
    if (!elevate<COptItem, CCopasiParameterGroup>(*it)) return false;

  mpOptItems =
    static_cast<std::vector<COptItem * > * >(mpGrpItems->CCopasiParameter::getValue().pVOID);

  mpGrpConstraints =
    elevate<CCopasiParameterGroup, CCopasiParameterGroup>(mpGrpConstraints);
  if (!mpGrpConstraints) return false;

  pValue = mpGrpConstraints->CCopasiParameter::getValue().pGROUP;

  it = pValue->begin();
  end = pValue->end();

  for (; it != end; ++it)
    if (!elevate<COptItem, CCopasiParameterGroup>(*it)) return false;

  mpConstraintItems =
    static_cast<std::vector<COptItem * > * >(mpGrpConstraints->CCopasiParameter::getValue().pVOID);

  return true;
}

bool COptProblem::setModel(CModel * pModel)
{
  mpModel = pModel;
  return true;
}

bool COptProblem::setCallBack(CProcessReport * pCallBack)
{
  CCopasiProblem::setCallBack(pCallBack);

  if (pCallBack)
    {
      mSolutionValue = mInfinity;
      mhSolutionValue =
        mpCallBack->addItem("Best Value",
                            CCopasiParameter::DOUBLE,
                            & mSolutionValue);
      mCounter = 0;
      mhCounter =
        mpCallBack->addItem("Function Evaluations",
                            CCopasiParameter::UINT,
                            & mCounter);
    }

  return true;
}

void COptProblem::initObjects()
{
  addObjectReference("Function Evaluations", mCounter, CCopasiObject::ValueInt);
  addObjectReference("Best Value", mSolutionValue, CCopasiObject::ValueDbl);
  addVectorReference("Best Parameters", mSolutionVariables, CCopasiObject::ValueDbl);
}

#ifdef WIN32
// warning C4056: overflow in floating-point constant arithmetic
// warning C4756: overflow in constant arithmetic
# pragma warning (disable: 4056 4756)
#endif

bool COptProblem::initialize()
{
  mInfinity = 2.0 * DBL_MAX;

  if (!mpModel) return false;
  mpModel->compileIfNecessary(mpCallBack);

  bool success = true;

  mpReport = NULL;
  mCounter = 0;
  mFailedCounter = 0;
  mConstraintCounter = 0;
  mFailedConstraintCounter = 0;

  mSolutionValue = mInfinity;

  std::vector< CCopasiContainer * > ContainerList;
  ContainerList.push_back(mpModel);

  COptTask * pTask = dynamic_cast<COptTask *>(getObjectParent());
  if (pTask)
    {
      ContainerList.push_back(pTask);
      mpReport = &pTask->getReport();
      if (!mpReport->getStream()) mpReport = NULL;
    }

  // This is extremely vulnerable to human COPASI file manipulations
  mpSteadyState =
    dynamic_cast< CSteadyStateTask * >(GlobalKeys.get(* getValue("Steady-State").pKEY));
  if (!mpSteadyState && * getValue("Steady-State").pKEY != "")
    {
      mpSteadyState =
        dynamic_cast< CSteadyStateTask * >((*CCopasiDataModel::Global->getTaskList())["Steady-State"]);

      if (mpSteadyState != NULL) setValue("Steady-State", mpSteadyState->getKey());
    }

  mpTrajectory =
    dynamic_cast< CTrajectoryTask * >(GlobalKeys.get(* getValue("Time-Course").pKEY));
  if (!mpTrajectory && * getValue("Time-Course").pKEY != "")
    {
      mpTrajectory =
        dynamic_cast< CTrajectoryTask * >((*CCopasiDataModel::Global->getTaskList())["Time-Course"]);

      if (mpTrajectory != NULL) setValue("Time-Course", mpTrajectory->getKey());
    }

  if (!mpSteadyState && !mpTrajectory)
    {
      CCopasiMessage(CCopasiMessage::ERROR, MCOptimization + 7);
      success = false;
    }

  if (mpSteadyState)
    {
      mpSteadyState->initialize(CCopasiTask::NO_OUTPUT, NULL, NULL);
      ContainerList.push_back(mpSteadyState);
    }
  if (mpTrajectory)
    {
      mpTrajectory->initialize(CCopasiTask::NO_OUTPUT, NULL, NULL);
      ContainerList.push_back(mpTrajectory);
    }

  unsigned C_INT32 i;
  unsigned C_INT32 Size = mpOptItems->size();

  mUpdateMethods.resize(Size);
  mSolutionVariables.resize(Size);
  mOriginalVariables.resize(Size);

  mSolutionVariables = std::numeric_limits<C_FLOAT64>::quiet_NaN();
  mOriginalVariables = std::numeric_limits<C_FLOAT64>::quiet_NaN();

  std::vector< COptItem * >::iterator it = mpOptItems->begin();
  std::vector< COptItem * >::iterator end = mpOptItems->end();

  if (it == end)
    {
      CCopasiMessage(CCopasiMessage::ERROR, MCOptimization + 6);
      return false;
    }

  std::set< const CCopasiObject * > changedObjects;

  for (i = 0; it != end; ++it, i++)
    {
      success &= (*it)->compile(ContainerList);

      mUpdateMethods[i] = (*it)->getUpdateMethod();
      changedObjects.insert((*it)->getObject());
      mOriginalVariables[i] = *(*it)->COptItem::getObjectValue();
    }

  changedObjects.erase(NULL);
  mInitialRefreshMethods = mpModel->buildInitialRefreshSequence(changedObjects);

  if (!success) return false;

  it = mpConstraintItems->begin();
  end = mpConstraintItems->end();

  // We need to build a refresh sequence so the constraint values are updated
  std::set< const CCopasiObject * > Objects;

  for (i = 0; it != end; ++it, i++)
    {
      if (!(*it)->compile(ContainerList)) return false;

      Objects.insert((*it)->getDirectDependencies().begin(),
                     (*it)->getDirectDependencies().end());
    }

  mRefreshConstraints = CCopasiObject::buildUpdateSequence(Objects, mpModel->getUptoDateObjects());

  createObjectiveFunction();

  mCPUTime.start();

  if (!mpFunction ||
      mpFunction->getInfix() == "" ||
      !mpFunction->compile(ContainerList))
    {
      mRefreshMethods.clear();
      CCopasiMessage(CCopasiMessage::ERROR, MCOptimization + 5);
      return false;
    }

  mRefreshMethods = CCopasiObject::buildUpdateSequence(mpFunction->getDirectDependencies(), mpModel->getUptoDateObjects());

  return success;
}

#ifdef WIN32
# pragma warning (default: 4056 4756)
#endif

bool COptProblem::restore(const bool & updateModel)
{
  bool success = true;

  std::vector<COptItem * >::iterator it = mpOptItems->begin();
  std::vector<COptItem * >::iterator end = mpOptItems->end();
  C_FLOAT64 * pTmp;

  if (updateModel && mSolutionValue != mInfinity)
    {
      // Set the model values ans start values to the solution values
      pTmp = mSolutionVariables.array();

      for (; it != end; ++it, pTmp++)
        {
          (*(*it)->COptItem::getUpdateMethod())(*pTmp);
          (*it)->setStartValue(*pTmp);
        }
    }
  else
    {
      // Reset the model values to the original values
      pTmp = mOriginalVariables.array();

      for (; it != end; ++it, pTmp++)
        if (!isnan(*pTmp))
          (*(*it)->COptItem::getUpdateMethod())(*pTmp);
    }

  if (mFailedCounter * 20 > mCounter) // > 5% failure rate
    CCopasiMessage(CCopasiMessage::WARNING, MCOptimization + 8, mFailedCounter, mCounter);

  if (10 * mFailedConstraintCounter > 8 * mConstraintCounter) // > 80 % failure rate
    CCopasiMessage(CCopasiMessage::WARNING, MCOptimization + 9, mFailedConstraintCounter, mConstraintCounter);

  return success;
}

bool COptProblem::checkParametricConstraints()
{
  std::vector< COptItem * >::const_iterator it = mpOptItems->begin();
  std::vector< COptItem * >::const_iterator end = mpOptItems->end();

  for (; it != end; ++it)
    if ((*it)->checkConstraint()) return false;

  return true;
}

bool COptProblem::checkFunctionalConstraints()
{
  // Make sure the constraint values are up to date.
  std::vector< Refresh *>::const_iterator itRefresh = mRefreshConstraints.begin();
  std::vector< Refresh *>::const_iterator endRefresh = mRefreshConstraints.end();
  for (; itRefresh != endRefresh; ++itRefresh)
    (**itRefresh)();

  std::vector< COptItem * >::const_iterator it = mpConstraintItems->begin();
  std::vector< COptItem * >::const_iterator end = mpConstraintItems->end();

  mConstraintCounter++;
  for (; it != end; ++it)
    if ((*it)->checkConstraint())
      {
        mFailedConstraintCounter++;
        return false;
      }

  return true;
}

/**
 * calculate() decides whether the problem is a steady state problem or a
 * trajectory problem based on whether the pointer to that type of problem
 * is null or not.  It then calls the process() method for that type of
 * problem.  Currently process takes ofstream& as a parameter but it will
 * change so that process() takes no parameters.
 */
bool COptProblem::calculate()
{
  mCounter++;
  bool success = false;
  COutputHandler * pOutputHandler = NULL;

  if (mStoreResults && mpTrajectory != NULL)
    {
      static_cast< CTrajectoryProblem * >(mpTrajectory->getProblem())->setTimeSeriesRequested(true);

      pOutputHandler = new COutputHandler();
      mpTrajectory->initialize(CCopasiTask::ONLY_TIME_SERIES, pOutputHandler, NULL);
    }

  try
    {
      // Update all initial values which depend on the optimization items.
      std::vector< Refresh * >::const_iterator it = mInitialRefreshMethods.begin();
      std::vector< Refresh * >::const_iterator end = mInitialRefreshMethods.end();
      for (; it != end; ++it)
        (**it)();

      if (mpSteadyState != NULL)
        success = mpSteadyState->process(true);
      else if (mpTrajectory != NULL)
        success = mpTrajectory->process(true);

      // Refresh all values needed to calculate the objective function.
      it = mRefreshMethods.begin();
      end = mRefreshMethods.end();
      for (; it != end; ++it)
        (**it)();

      mCalculateValue = mpFunction->calcValue();
    }

  catch (CCopasiException)
    {
      // We do not want to clog the message cue.
      CCopasiMessage::getLastMessage();

      mFailedCounter++;
      success = false;
    }

  catch (...)
    {
      mFailedCounter++;
      success = false;
    }

  if (mStoreResults && mpTrajectory != NULL)
    {
      mStoreResults = false;
      mpTrajectory->initialize(CCopasiTask::NO_OUTPUT, NULL, NULL);
      pdelete(pOutputHandler);
    }

  if (!success || isnan(mCalculateValue)) mCalculateValue = mInfinity;

  if (mpCallBack) return mpCallBack->progress(mhCounter);

  return true;
}

bool COptProblem::calculateStatistics(const C_FLOAT64 & factor,
                                      const C_FLOAT64 & resolution)
{
  // Set the current values to the solution values.
  unsigned C_INT32 i, imax = mSolutionVariables.size();

  mGradient.resize(imax);
  mGradient = std::numeric_limits<C_FLOAT64>::quiet_NaN();

  // Recalcuate the best solution.
  for (i = 0; i < imax; i++)
    (*mUpdateMethods[i])(mSolutionVariables[i]);

  calculate();

  if (mSolutionValue == mInfinity)
    return false;

  mHaveStatistics = true;

  C_FLOAT64 Current;
  C_FLOAT64 Delta;

  // Calculate the gradient
  for (i = 0; i < imax; i++)
    {
      Current = mSolutionVariables[i];

      if (fabs(Current) > resolution)
        {
          (*mUpdateMethods[i])(Current * (1.0 + factor));
          Delta = 1.0 / (Current * factor);
        }
      else
        {
          (*mUpdateMethods[i])(resolution);
          Delta = 1.0 / resolution;
        }

      calculate();

      mGradient[i] = (mCalculateValue - mSolutionValue) * Delta;

      // Restore the value
      (*mUpdateMethods[i])(Current);
    }

  // This is necessary so that the result can be displayed.
  mStoreResults = true;
  calculate();
  mStoreResults = false;

  // Make sure the timer is acurate.
  (*mCPUTime.getRefresh())();

  return true;
}

const C_FLOAT64 & COptProblem::getCalculateValue() const
  {return mCalculateValue;}

const CVector< C_FLOAT64 > & COptProblem::getSolutionVariables() const
  {return mSolutionVariables;}

bool COptProblem::setSolution(const C_FLOAT64 & value,
                              const CVector< C_FLOAT64 > & variables)
{
  mSolutionValue = value;
  mSolutionVariables = variables;

  if (mpCallBack) return mpCallBack->progress(mhSolutionValue);

  return true;
}

const C_FLOAT64 & COptProblem::getSolutionValue() const
{return mSolutionValue;}

// set the type of problem : Steady State OR Trajectory
void COptProblem::setProblemType(ProblemType type)
{
  // :TODO:
  if (type == SteadyState)
    mpSteadyState = new CSteadyStateTask(/*this*/NULL);
  if (type == Trajectory)
    mpTrajectory = new CTrajectoryTask(/*this*/NULL);
}

COptItem & COptProblem::getOptItem(const unsigned C_INT32 & index)
{return *(*mpOptItems)[index];}

const unsigned C_INT32 COptProblem::getOptItemSize() const
  {return mpGrpItems->size();}

COptItem & COptProblem::addOptItem(const CCopasiObjectName & objectCN)
{
  COptItem * pItem = new COptItem();
  pItem->setObjectCN(objectCN);

  mpGrpItems->addParameter(pItem);

  return *pItem;
}

bool COptProblem::removeOptItem(const unsigned C_INT32 & index)
{return mpGrpItems->removeParameter(index);}

bool COptProblem::swapOptItem(const unsigned C_INT32 & iFrom,
                              const unsigned C_INT32 & iTo)
{return mpGrpItems->swap(iFrom, iTo);}

const std::vector< COptItem * > & COptProblem::getOptItemList() const
  {return *mpOptItems;}

const std::vector< COptItem * > & COptProblem::getConstraintList() const
  {return *mpConstraintItems;}

const std::vector< UpdateMethod * > & COptProblem::getCalculateVariableUpdateMethods() const
  {return mUpdateMethods;}

bool COptProblem::setObjectiveFunction(const std::string & infix)
{
  if (!mpFunction) createObjectiveFunction();

  return mpFunction->setInfix(infix);
}

const std::string COptProblem::getObjectiveFunction()
{
  if (!mpFunction) createObjectiveFunction();

  return mpFunction->getInfix();
}

bool COptProblem::createObjectiveFunction()
{
  CCopasiParameter * pParm = getParameter("ObjectiveFunction");
  if (!pParm) return false;

  mpFunction =
    dynamic_cast<CExpression *>(GlobalKeys.get(* pParm->getValue().pKEY));

  CCopasiVectorN<CEvaluationTree> & FunctionList =
    CCopasiDataModel::Global->getFunctionList()->loadedFunctions();

  if (!mpFunction)
    {
      std::ostringstream Name;
      Name << "Objective Function";

      int i = 0;

      while (FunctionList.getIndex(Name.str()) != C_INVALID_INDEX)
        {
          i++;
          Name.str("");
          Name << "Objective Function " << i;
        }

      mpFunction = new CExpression(Name.str(), this);
      FunctionList.add(mpFunction, false);

      setValue("ObjectiveFunction", mpFunction->getKey());
    }
  else
    {
      mpFunction->setObjectParent(this);

      if (FunctionList.getIndex(mpFunction->getObjectName()) == C_INVALID_INDEX)
        FunctionList.add(mpFunction, false);
    }

  return true;
}

const unsigned C_INT32 & COptProblem::getFunctionEvaluations() const
{return mCounter;}

const C_FLOAT64 & COptProblem::getExecutionTime() const
  {return * (C_FLOAT64 *) mCPUTime.getValuePointer();}

void COptProblem::print(std::ostream * ostream) const
  {*ostream << *this;}

void COptProblem::printResult(std::ostream * ostream) const
  {
    std::ostream & os = *ostream;

    if (mSolutionVariables.size() == 0)
      {
        return;
      }
    os << "    Objective Function Value:\t" << mSolutionValue << std::endl;

    CCopasiTimeVariable CPUTime = const_cast<COptProblem *>(this)->mCPUTime.getElapsedTime();

    os << "    Function Evaluations:\t" << mCounter << std::endl;
    os << "    CPU Time [s]:\t"
    << CCopasiTimeVariable::LL2String(CPUTime.getSeconds(), 1) << "."
    << CCopasiTimeVariable::LL2String(CPUTime.getMilliSeconds(true), 3) << std::endl;
    os << "    Evaluations/Second [1/s]:\t" << mCounter / (C_FLOAT64) (CPUTime.getMilliSeconds() / 1e3) << std::endl;
    os << std::endl;

    std::vector< COptItem * >::const_iterator itItem =
      mpOptItems->begin();
    std::vector< COptItem * >::const_iterator endItem =
      mpOptItems->end();

    unsigned C_INT32 i;

    for (i = 0; itItem != endItem; ++itItem, i++)
      {
        os << "    " << (*itItem)->getObjectDisplayName() << ": "
        << mSolutionVariables[i] << std::endl;
      }
  }

std::ostream &operator<<(std::ostream &os, const COptProblem & o)
{
  os << "Problem Description:" << std::endl;

  os << "Subtask: " << std::endl;

  if (o.mpSteadyState)
    o.mpSteadyState->getDescription().print(&os);

  if (o.mpTrajectory)
    o.mpTrajectory->getDescription().print(&os);

  if (!o.mpTrajectory && !o.mpSteadyState)
    os << "No Subtask specified.";

  os << std::endl;

  if (o.mpFunction)
    {
      os << "Objective Function:" << std::endl;
      os << "    " << o.mpFunction->getDisplayString() << std::endl;
      os << std:: endl;
    }

  os << "List of Optimization Items:" << std::endl;

  std::vector< COptItem * >::const_iterator itItem =
    o.mpOptItems->begin();
  std::vector< COptItem * >::const_iterator endItem =
    o.mpOptItems->end();

  for (; itItem != endItem; ++itItem)
    os << "    " << **itItem << std::endl;

  itItem = o.mpConstraintItems->begin();
  endItem = o.mpConstraintItems->end();

  for (; itItem != endItem; ++itItem)
    os << "    " << **itItem << std::endl;

  return os;
}
