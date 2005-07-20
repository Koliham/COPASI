/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/optimization/COptMethod.cpp,v $
   $Revision: 1.16 $
   $Name:  $
   $Author: anuragr $ 
   $Date: 2005/07/20 05:10:51 $
   End CVS Header */

/**
 *  COptMethod class
 *  This class describes the interface to all optimization methods.
 *  The various method like RandomSearch or GA have to be derived from
 *  this class.
 *  
 *  Created for Copasi by Stefan Hoops 2002
 */

#define COPASI_TRACE_CONSTRUCTION

#include "copasi.h"

#include "COptTask.h"
#include "COptMethod.h"
#include "COptProblem.h"

const std::string COptMethod::TypeName[] =
  {
    "RandomSearch",
    "RandomSearchMaster",
    "SimulatedAnnealing",
    "GeneticAlgorithm",
    "EvolutionaryProgram2",
    "SteepestDescent"
    "HybridGASA",
    "GeneticAlgorithmSR"
  };

COptMethod * COptMethod::createMethod(CCopasiMethod::SubType subType)
{
  COptMethod * pMethod = NULL;

  switch (subType)
    {
    case GeneticAlgorithmSR:
      pMethod = new COptMethodGASR();
      break;

    case RandomSearch:
      pMethod = new CRandomSearch();
      break;

    case SteepestDescent:
      pMethod = new COptMethodSteepestDescent();
      break;

      /*case RandomSearchMaster:
        //      pMethod = new CRandomSearchMaster();
        break;

      case SimulatedAnnealing:
        //      pMethod = new COptMethodSA();
        break;*/

      /*       case GeneticAlgorithm:
               pMethod = new COptMethodGA();
               break;
      /*
             case EvolutionaryProgram2:
               //      pMethod = new COptMethodEP2();
               break;
       
             case HybridGASA:
               //      pMethod = new COptMethodHGASA();
               break;*/

    default:
      pMethod = new COptMethodGA();
      //fatalError();
    }

  return pMethod;
}

// Default constructor
COptMethod::COptMethod():
    CCopasiMethod(CCopasiTask::optimization, CCopasiMethod::unset),
    mpOptProblem(NULL),
    mBounds(false)
{CONSTRUCTOR_TRACE;}

COptMethod::COptMethod(CCopasiMethod::SubType subType,
                       const CCopasiContainer * pParent):
    CCopasiMethod(CCopasiTask::optimization, subType, pParent),
    mpOptProblem(NULL),
    mBounds(false)
{CONSTRUCTOR_TRACE;}

COptMethod::COptMethod(const COptMethod & src,
                       const CCopasiContainer * pParent):
    CCopasiMethod(src, pParent),
    mpOptProblem(src.mpOptProblem),
    mBounds(src.mBounds)
{CONSTRUCTOR_TRACE;}

//YOHE: seems "virtual" cannot be outside of class declaration
COptMethod::~COptMethod()
{}

void COptMethod::setProblem(COptProblem * problem)
{
  assert(problem);
  mpOptProblem = problem;
}

// Returns True if this method is capable of handling adjustable parameter boundary
// constraints, False otherwise
bool COptMethod::isBounded(void)
{
  return mBounds;
}

//virtual C_INT32 COptMethod::Optimise(C_FLOAT64 (*func) (void))
bool COptMethod::optimise(void)
{
  return false;
}

bool COptMethod::initialize()
{
  if (!mpOptProblem)
    return false;
  if (!(mpOptItem = &mpOptProblem->getOptItemList()))
    return false;
  if (!(mpSetCalculateVariable = &mpOptProblem->getCalculateVariableUpdateMethods()))
    return false;

  COptTask * pTask = dynamic_cast<COptTask *>(getObjectParent());
  if (pTask &&
      (mpReport = &pTask->getReport()) &&
      !mpReport->getStream())
    mpReport = NULL;

  return true;
}

bool COptMethod::cleanup()
{return true;}
