/**
 *  CScanMethod class.
 *  This class describes the Scan method
 *
 *  Created for Copasi by Rohan Luktuke 2002
 */

#include <string>

//#define COPASI_TRACE_CONSTRUCTION
#include "math.h"
#include "copasi.h"
#include "model/CModel.h"
#include "model/CState.h"
#include "utilities/CGlobals.h"
#include "utilities/CReadConfig.h"
#include "randomGenerator/CRandom.h"
#include "randomGenerator/Cr250.h"

#include "utilities/CWriteConfig.h"
#include "CScanProblem.h"
#include "CScanMethod.h"

// this will have to be defined somewhere else with the
// values of other distribution types
#define SD_UNIFORM 0
#define SD_GAUSS 1
#define SD_BOLTZ 2
#define SD_REGULAR 3

CScanMethod * CScanMethod::createMethod() {return new CScanMethod;}

CScanMethod::CScanMethod()
{
  CRandom *pRandomGenerator = CRandom::createGenerator();
  Cr250* pCr250Generator = (Cr250*)(CRandom::createGenerator(CRandom::r250));
}

CScanMethod::CScanMethod(const CScanMethod & src)
{}

CScanMethod::~CScanMethod(){}

void CScanMethod::scan(unsigned C_INT32 s, bool C_UNUSED(nl))
{
  unsigned C_INT32 scanDimension = scanProblem->getListSize();
  unsigned C_INT32 i, next, top;
  //1.  find the first/last master scan item

  if (s > 0)
    {
      for (i = s - 1; i > 0; i--)
        //if(scanItem[i]->Indp) break;
        if (scanProblem->getScanItemParameter(i, "Indp") == 1.0) break;

      next = i;
    }

  else
    next = 0;

  /* 2.  find the last slave item */

  //if(s<scandimension-1)
  if (s < scanDimension - 1)
    {
      for (i = s + 1; i < scanDimension; i++)
        if (scanProblem->getScanItemParameter(i, "Indp") == 1.0) break;
      top = i;
    }
  else
    top = scanDimension;

  /* 3.  switch the grid type (distribution) - regular, normal etc */

  /* The "gridType" is just a temporary name for the parameter.
     A proper name will have to be given at the time of entering
     this parameter into the vector.
  */ 
  //switch(gridtype[distribution])
  switch ((int)scanProblem->getScanItemParameter(i, "gridType"))
    {
    case SD_UNIFORM:
    case SD_GAUSS:
    case SD_BOLTZ:
      // start with the min values
      setScanParameterValue(0, s, top);
      //different from SD_REGULR by initial value
      for (i = 0; i < scanProblem->getScanItemParameter(s, "Density"); i++)
        {
          if (s != 0) scan(next, false);
          else
            // some function
            simulate();
          setScanParameterValue(i, s, top);
        }
      break;
    case SD_REGULAR:
      //start with min value - give 0 as first param in setscanparametervalue
      setScanParameterValue(0, s, top);

      for (i = 1; i < scanProblem->getScanItemParameter(i, "density"); i++)
        {
          if (s != 0)
            scan(next, false);
          else
            // some function
            simulate();
          setScanParameterValue(i, s, top);
        }
      break;
    }
} // scan() ends

C_FLOAT64 CScanMethod::simulate()
{
  scanProblem->calculate();
  return 0;
}

/**
  *  Set a pointer to the problem.
  *  This method is used by CTrajectory
  *  @param "CTrajectoryProblem *" problem
  */
void CScanMethod::setProblem(CScanProblem * problem)
{scanProblem = problem;}

/**
 *  set the values master and all slave parameters
 * @param "C_INT32 i" initial value
 * @param "C_INT32 first" first parameter (master)
 * @param "C_INT32 last" last slave parameter
 */

void CScanMethod::setScanParameterValue(unsigned C_INT32 i,
                                        unsigned C_INT32 first,
                                        unsigned C_INT32 last)
{
  unsigned C_INT32 j;
  double min, max, incr, ampl;
  for (j = first; j < last; j++)
    {
      // making a copy of the min and max parameters of the scanItem j
      min = scanProblem->getScanItemParameter(j, "min");
      max = scanProblem->getScanItemParameter(j, "max");
      ampl = scanProblem->getScanItemParameter(j, "Ampl");
      incr = scanProblem->getScanItemParameter(j, "incr");

      // switch the grid type and set values accordingly
      switch ((int)scanProblem->getScanItemParameter(j, "gridType"))
        {
        case SD_UNIFORM:
          if (scanProblem->getScanItemParameter(j, "log"))
            scanProblem->setScanItemParameter(j, "value", min* pow(10, ampl * pCr250Generator->getRandomCO())); //dr250()
          else
            scanProblem->setScanItemParameter(j, "value", min + ampl * pCr250Generator->getRandomCO()); //dr250()
          break;
        case SD_GAUSS:
          if (scanProblem->getScanItemParameter(j, "log"))
            //CRandom::getRandomNormalLog(const C_FLOAT64 & mean, const C_FLOAT64 & sd)
            scanProblem->setScanItemParameter(j, "value", pRandomGenerator->getRandomNormalLog(min, max));
          else
            scanProblem->setScanItemParameter(j, "value", pRandomGenerator->getRandomNormal(min, max));
          break;
        case SD_BOLTZ:
          if (scanProblem->getScanItemParameter(j, "log"))
            scanProblem->setScanItemParameter(j, "value", pRandomGenerator->getRandomNormalLog(min, max));
          else
            scanProblem->setScanItemParameter(j, "value", pRandomGenerator->getRandomNormalPositive(min, max));
          break;
        case SD_REGULAR:
          // log scale
          if (scanProblem->getScanItemParameter(j, "log"))
            scanProblem->setScanItemParameter(j, "value", (min*pow(10, incr*i)));
          // non-log scale
          else
            scanProblem->setScanItemParameter(j, "value", (min + incr*i));
          break;
        }
    }
}

void CScanMethod::InitScan(void)
{
  /*
  int i, density;
  // do nothing if ScanDimension is smaller than 1
  if(ScanDimension<1)
  {
   ScanDimension = 0;
   return;
  }
  // ensure that that the first item is a master
  ScanItem[0]->Indp = TRUE;
  // and that its density is >= 2
  if(ScanItem[0]->Density < 2) ScanItem[0]->Density = 2;
  TotIteration = 1;
  for(i=0, density=2; i<ScanDimension; i++)
  {
   // if this item is slave keep the density of the master
   if(ScanItem[i]->Indp)
   {
    density = ScanItem[i]->Density;
    TotIteration *= density;
   }
   // calculate the amplitude
   if(ScanItem[i]->Log)
   {
    if((ScanItem[i]->Min <= 0) || (ScanItem[i]->Max <= 0))
    {
     // logarithmic scanning requires positive arguments!
  // user should be warned, but this should never happen!
  ScanItem[i]->Min = 1.0;
  ScanItem[i]->Max = 2.0;
    }
    ScanItem[i]->Ampl =   log10(ScanItem[i]->Max)
                        - log10(ScanItem[i]->Min);
   }
   else
    ScanItem[i]->Ampl = ScanItem[i]->Max - ScanItem[i]->Min;
   // calculate the increment
   ScanItem[i]->Incr = ScanItem[i]->Ampl/(density-1);
  }
  */
}
