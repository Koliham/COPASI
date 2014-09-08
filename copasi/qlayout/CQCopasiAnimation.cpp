// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <qlayout/CQCopasiAnimation.h>
#include <qlayout/CQEffectDescription.h>

CQCopasiAnimation::CQCopasiAnimation():
  mEntries(),
  mpDataModel(NULL),
  mMode(CQCopasiAnimation::Global),
  mNumSteps(0)
{}

CQCopasiAnimation::~CQCopasiAnimation()
{
  std::vector<CQEffectDescription*>::iterator it = mEntries.begin();

  while (it != mEntries.end())
    {
      delete *it;
      ++it;
    }

  mEntries.clear();
}

std::vector<CQEffectDescription*>& CQCopasiAnimation::getEntries()
{
  return mEntries;
}

int CQCopasiAnimation::getNumSteps()
{
  return mNumSteps;
}

void CQCopasiAnimation::removeFromScene(CQLayoutScene& scene)
{
  std::vector<CQEffectDescription*>::iterator it = mEntries.begin();

  while (it != mEntries.end())
    {
      (*it)->removeFromScene(scene);
      ++it;
    }
}



void CQCopasiAnimation::getScales(std::vector<qreal>& scales, int step, std::vector<qreal>& values) //= 0;
{
  scales.clear();
  std::vector<CQEffectDescription*>::iterator it = mEntries.begin();

  while (it != mEntries.end())
    {
      scales.push_back(step / 100.0);
      ++it;
    }
}


void CQCopasiAnimation::applyToScene(CQLayoutScene& scene, int step, bool concentrationflag)
{
	//if (concentrationflag == true)
  std::vector<qreal> values;

  std::vector<qreal> scales; getScales(scales, step, values);
  


  if (scales.size() != mEntries.size())
    return;
  if (concentrationflag == true)
  {
  for (size_t i = 0; i < scales.size(); ++i)
    {
      mEntries[i]->applyToScene(scene, scales[i], values[i*2], values[i*2+1]);
    }
  }
  else
  {
  for (size_t i = 0; i < scales.size(); ++i)
    {
      mEntries[i]->applyToScene(scene, scales[i]);
    }
  }

}

CQCopasiAnimation::ScaleMode CQCopasiAnimation::getScaleMode() const
{
  return mMode;
}

void CQCopasiAnimation::setScaleMode(CQCopasiAnimation::ScaleMode mode)
{
  mMode = mode;
}
