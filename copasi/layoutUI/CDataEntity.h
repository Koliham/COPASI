// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/layoutUI/CDataEntity.h,v $
//   $Revision: 1.6 $
//   $Name:  $
//   $Author: urost $
//   $Date: 2007/07/22 19:18:18 $
// End CVS Header

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef CDATAENTITY_H
#define CDATAENTITY_H

#include <string>
#include <map>

#include "copasi.h"

class CDataEntity
  {
  private:
    C_FLOAT64 mTime;
    std::map<std::string, C_FLOAT64> mSpeciesValueMap;

  public:
    CDataEntity();
    CDataEntity(C_FLOAT64 t);
    CDataEntity(const CDataEntity & src);

    void putValueForSpecies(std::string nodeKey, C_FLOAT64 value);
    C_FLOAT64 getValueForSpecies(std::string nodeKey);
    int getNumberOfElements(){return mSpeciesValueMap.size();}
    int getTime(){return mTime;}
  };
#endif
