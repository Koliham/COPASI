// Copyright (C) 2010 - 2014 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2005 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/**
 * COptTask class.
 *
 * This class implements a optimization task which is comprised of a
 * of a problem and a method.
 *
 */
#include "copasi.h"

#include "COptTask.h"
#include "COptProblem.h"
#include "COptMethod.h"
#include "report/CKeyFactory.h"
#include "report/CReport.h"

#include "model/CModel.h"
#include "model/CState.h"

//#include "trajectory/CTrajectoryTask.h"
//#include "trajectory/CTrajectoryProblem.h"
//#include "steadystate/CSteadyStateTask.h"
//#include "steadystate/CSteadyStateProblem.h"
//#include "utilities/COutputHandler.h"

const unsigned int COptTask::ValidMethods[] =
{
  CCopasiMethod::Statistics,
#ifdef COPASI_DEBUG
  CCopasiMethod::CoranaWalk,
#endif // COPASI_DEBUG
  CCopasiMethod::DifferentialEvolution,
  CCopasiMethod::SRES,
  CCopasiMethod::EvolutionaryProgram,
  CCopasiMethod::GeneticAlgorithm,
  CCopasiMethod::GeneticAlgorithmSR,
  CCopasiMethod::HookeJeeves,
  CCopasiMethod::LevenbergMarquardt,
  CCopasiMethod::NelderMead,
  CCopasiMethod::ParticleSwarm,
  CCopasiMethod::Praxis,
  CCopasiMethod::RandomSearch,
  CCopasiMethod::ScatterSearch,
  CCopasiMethod::SimulatedAnnealing,
  CCopasiMethod::SteepestDescent,
  CCopasiMethod::TruncatedNewton,
  CCopasiMethod::unset
};

COptTask::COptTask(const CCopasiTask::Type & type,
                   const CCopasiContainer * pParent):
  CCopasiTask(type, pParent)
{
  mpProblem = new COptProblem(type, this);
  mpMethod = COptMethod::createMethod();
  this->add(mpMethod, true);
  //  mpMethod->setObjectParent(this);
  ((COptMethod *) mpMethod)->setProblem((COptProblem *) mpProblem);
}

COptTask::COptTask(const COptTask & src,
                   const CCopasiContainer * pParent):
  CCopasiTask(src, pParent)
{
  mpProblem = new COptProblem(*(COptProblem *) src.mpProblem, this);
  mpMethod = COptMethod::createMethod(src.mpMethod->getSubType());
  this->add(mpMethod, true);
  //  mpMethod->setObjectParent(this);
  ((COptMethod *) mpMethod)->setProblem((COptProblem *) mpProblem);
}

COptTask::~COptTask()
{cleanup();}

void COptTask::cleanup() {}

bool COptTask::setCallBack(CProcessReport * pCallBack)
{
  bool success = CCopasiTask::setCallBack(pCallBack);

  if (!mpProblem->setCallBack(pCallBack)) success = false;

  if (!mpMethod->setCallBack(pCallBack)) success = false;

  return success;
}

bool COptTask::initialize(const OutputFlag & of,
                          COutputHandler * pOutputHandler,
                          std::ostream * pOstream)
{
  COptProblem * pProblem = dynamic_cast<COptProblem *>(mpProblem);
  COptMethod * pMethod = dynamic_cast<COptMethod *>(mpMethod);

  if (!pProblem || !pMethod) return false;

  //initialize reporting
  bool success = true;

  //do the part of the initialization of the subtask that needs to be
  //performed before the output is initialized. This is kind of a hack,
  //we need to find a more general solution for this
  if (!pProblem->initializeSubtaskBeforeOutput()) success = false;

  if (!CCopasiTask::initialize(of, pOutputHandler, pOstream)) success = false;

  //if (!mReport.open(pOstream)) success = false;
  //if (!mReport.compile()) success = false;

  if (!pProblem->initialize()) success = false;

  pMethod->setProblem(pProblem);
  //  if (!pMethod->initialize()) return false;

  return success;
}

bool COptTask::process(const bool & useInitialValues)
{
  COptProblem * pProblem = dynamic_cast<COptProblem *>(mpProblem);
  COptMethod * pMethod = dynamic_cast<COptMethod *>(mpMethod);

  if (!pProblem || !pMethod) return false;

  mpMethod->isValidProblem(mpProblem);

  pProblem->randomizeStartValues();
  pProblem->rememberStartValues();

  if (useInitialValues) pProblem->resetEvaluations();

  output(COutputInterface::BEFORE);

  bool success = pMethod->optimise();

  pProblem->calculateStatistics();

  output(COutputInterface::AFTER);

  return success;
}

bool COptTask::setMethodType(const int & type)
{
  CCopasiMethod::SubType Type = (CCopasiMethod::SubType) type;

  if (mpMethod->getSubType() == Type) return true;

  pdelete(mpMethod);

  mpMethod = createMethod(Type);
  this->add(mpMethod, true);

  return true;
}

// virtual
CCopasiMethod * COptTask::createMethod(const int & type) const
{
  CCopasiMethod::SubType Type = (CCopasiMethod::SubType) type;

  return COptMethod::createMethod(Type);
}
