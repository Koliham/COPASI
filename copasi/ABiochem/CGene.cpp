/**
 *  CGene class.
 *  Written by Pedro Mendes September 2002.
 *  
 *  For representing a gene and all the elements needed for COPASI 
 *  to represent it in a model
 */

#include <iostream>
#include <string>
#include <vector>

#define  COPASI_TRACE_CONSTRUCTION

#include "copasi.h"
#include "utilities/CGlobals.h"
#include "CGene.h"

CGeneModifier::CGeneModifier()
{
  mModifier = NULL;
  mType = 0;
  mK = 1.0;
}

CGeneModifier::CGeneModifier(CGene * modf, C_INT32 type, C_FLOAT64 K)
{
  mModifier = modf;
  if ((type >= 0) && (type < 2))
    mType = type;
  else
    type = 0;
  mK = K > 0.0 ? K : 1.0;
}

CGeneModifier::~CGeneModifier()
{}

CGene * CGeneModifier::getModifier(void)
{
  return mModifier;
}

C_INT32 CGeneModifier::getType(void)
{
  return mType;
}

C_FLOAT64 CGeneModifier::getK(void)
{
  return mK;
}
void CGeneModifier::cleanup() {}

CGene::CGene()
{}

CGene::~CGene()
{}

void CGene::setName(const string & name)
{
  mName = name;
}

const string & CGene::getName() const
  {
    return mName;
  }

C_INT32 CGene::getModifierNumber()
{
  return mModifier.size();
}

CGene * CGene::getModifier(C_INT32 n)
{
  return mModifier[n]->getModifier();
}

void CGene::setRate(C_FLOAT64 rate)
{
  mRate = rate;
}

C_FLOAT64 CGene::getRate(void)
{
  return mRate;
}

void CGene::setDegradationRate(C_FLOAT64 rate)
{
  mDegradationRate = rate;
}

C_FLOAT64 CGene::getDegradationRate(void)
{
  return mDegradationRate;
}

void CGene::addModifier(CGene *modf, C_INT32 type, C_FLOAT64 K)
{
  CGeneModifier *temp;
  temp = new CGeneModifier(modf, type, K);
  mModifier.add(temp);
}

C_INT32 CGene::getModifierType(C_INT32 n)
{
  return mModifier[n]->getType();
}

C_FLOAT64 CGene::getK(C_INT32 n)
{
  return mModifier[n]->getK();
}

void CGene::cleanup()
{
  mModifier.cleanup();
}

C_INT32 CGene::getNegativeModifiers(void)
{
  C_INT32 i, n, s;
  s = mModifier.size();
  for (i = n = 0; i < s; i++)
    if (mModifier[i]->getType() == 0)
      n++;
  return n;
}

C_INT32 CGene::getPositiveModifiers(void)
{
  C_INT32 i, n, s;
  s = mModifier.size();
  for (i = n = 0; i < s; i++)
    if (mModifier[i]->getType() == 1)
      n++;
  return n;
}
