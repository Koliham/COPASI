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
  mNumSteps(0),
  mFitting(CQCopasiAnimation::ManualFit)
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



void CQCopasiAnimation::getScales(std::vector<qreal>& scales, int step, std::vector<qreal>& values, std::vector<qreal>& borders) //= 0;
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
  std::vector<qreal> borders;

  std::vector<qreal> scales; getScales(scales, step, values, borders);
  


  if (scales.size() != mEntries.size())
    return;
  if (concentrationflag == true)
  {
  for (size_t i = 0; i < scales.size(); ++i)
    {
		//I want to add boundaries, but have to check, if I need individual oder global boundaries
		
		if (mFitting == CQCopasiAnimation::AutoFit) //boundaries are autocalculated, so it is needed for AutoFit
		{
			if (mMode == CQCopasiAnimation::Individual)
			mEntries[i]->applyToScene(scene, scales[i], values[i*2], values[i*2+1], borders[i*2], borders[i*2+1]);
    
			if (mMode == CQCopasiAnimation::Global)
				mEntries[i]->applyToScene(scene, scales[i], values[i*2], values[i*2+1], borders[borders.size()-2], borders[borders.size()-1]);
		}
		else //if no AutoFit, then we can ignore and give zero
		{
			mEntries[i]->applyToScene(scene, scales[i], values[i*2], values[i*2+1]);
		}

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

CQCopasiAnimation::Fitting CQCopasiAnimation::getFittingMode() const
{
	return mFitting;
}

void CQCopasiAnimation::setFittingMode(CQCopasiAnimation::Fitting mode)
{
	mFitting = mode;
}