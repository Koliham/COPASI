// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef Q_COPASI_ANIMATION_H
#define Q_COPASI_ANIMATION_H

#include <vector>
#include <string>

#include <qlayout/CQLayoutScene.h>
#include <CopasiDataModel/CCopasiDataModel.h>

class CQEffectDescription;
class CQCopasiAnimation
{
public:
  enum ScaleMode
  {
    Global,
    Individual
  };

  enum Fitting
  {
	  ManualFit,
	  AutoFit
  };


  CQCopasiAnimation();
  virtual ~CQCopasiAnimation();
  virtual void removeFromScene(CQLayoutScene& scene);
  virtual void initialize(const CCopasiDataModel &model) = 0;
  virtual void getScales(std::vector<qreal>& scales, int step, std::vector<qreal>& values = std::vector<qreal>(), std::vector<qreal>& borders = std::vector<qreal>()); 
 // virtual void getScales(std::vector<qreal>& scales, int step); // The old one with just 2 arguments
  virtual void applyToScene(CQLayoutScene& scene, int step, bool concentrationflag = false);
  virtual int getNumSteps();
  ScaleMode getScaleMode() const;
  void setScaleMode(ScaleMode mode);
  Fitting getFittingMode() const; // created for AutoFit
  void setFittingMode(Fitting mode);
  std::vector<CQEffectDescription*>& getEntries();
protected:
  std::vector<CQEffectDescription*> mEntries;
  const CCopasiDataModel* mpDataModel;
  ScaleMode mMode;
  Fitting mFitting; // where I save the Fitting Information
  int mNumSteps; // populate with the number of steps
public:
	std::vector<qreal> values;
};

#endif //Q_COPASI_ANIMATION_H
