/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/model/CReaction.cpp,v $
   $Revision: 1.123 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2005/06/21 20:34:16 $
   End CVS Header */

// CReaction
//
// Derived from Gepasi's cstep.cpp
// (C) Pedro Mendes 1995-2000
//
// Converted for Copasi by Stefan Hoops

#define  COPASI_TRACE_CONSTRUCTION

#include "copasi.h"

#include <algorithm>
#include <stdio.h>

#include "CopasiDataModel/CCopasiDataModel.h"
#include "CReaction.h"
#include "CCompartment.h"
#include "CModel.h"
#include "utilities/CReadConfig.h"
#include "utilities/CCopasiMessage.h"
#include "utilities/CCopasiException.h"
#include "utilities/utility.h"
#include "function/CFunctionDB.h"
#include "report/CCopasiObjectReference.h"
#include "report/CKeyFactory.h"
#include "CMetabNameInterface.h"
#include "CChemEqInterface.h" //only for load()

C_FLOAT64 CReaction::mDefaultScalingFactor = 1.0;

CReaction::CReaction(const std::string & name,
                     const CCopasiContainer * pParent):
    CCopasiContainer(name, pParent, "Reaction"),
    mKey(GlobalKeys.add("Reaction", this)),
    mChemEq("Chemical Equation", this),
    mpFunction(NULL),
    mFlux(0),
    mParticleFlux(0),
    mScalingFactor(&mDefaultScalingFactor),
    mUnitScalingFactor(&mDefaultScalingFactor),
    mMetabKeyMap(),
    mParameters("Parameters", this)
{
  CONSTRUCTOR_TRACE;
  initObjects();
  setFunction(CCopasiDataModel::Global->mpUndefined);
}

CReaction::CReaction(const CReaction & src,
                     const CCopasiContainer * pParent):
    CCopasiContainer(src, pParent),
    mKey(GlobalKeys.add("Reaction", this)),
    mChemEq(src.mChemEq, this),
    mpFunction(src.mpFunction),
    mFlux(src.mFlux),
    mParticleFlux(src.mParticleFlux),
    mScalingFactor(src.mScalingFactor),
    mUnitScalingFactor(src.mUnitScalingFactor),
    mMap(src.mMap),
    mMetabKeyMap(src.mMetabKeyMap),
    mParameters(src.mParameters, this)
{
  CONSTRUCTOR_TRACE;
  initObjects();
  if (mpFunction)
    {
      //compileParameters();
    }
}

CReaction::~CReaction()
{
  GlobalKeys.remove(mKey);
  cleanup();
  DESTRUCTOR_TRACE;
}

void CReaction::cleanup()
{
  // TODO: mMap.cleanup();
  //mParameterDescription.cleanup();
}

C_INT32 CReaction::load(CReadConfig & configbuffer)
{
  C_INT32 Fail = 0;

  std::string tmp;

  if ((Fail = configbuffer.getVariable("Step", "string", &tmp,
                                       CReadConfig::SEARCH)))
    return Fail;
  setObjectName(tmp);

  std::string ChemEq;

  if ((Fail = configbuffer.getVariable("Equation", "string", &ChemEq)))
    return Fail;

  CModel * pModel = (CModel*) getObjectAncestor("Model");
  CChemEqInterface::setChemEqFromString(pModel, *this, ChemEq);

  if ((Fail = configbuffer.getVariable("KineticType", "string", &tmp)))
    return Fail;

  setFunction(tmp);

  if (mpFunction == NULL)
    return Fail = 1;

  bool revers;
  if ((Fail = configbuffer.getVariable("Reversible", "bool", &revers,
                                       CReadConfig::SEARCH)))
    return Fail;
  mChemEq.setReversibility(revers); // TODO: this should be consistent with the ChemEq string

  if (configbuffer.getVersion() < "4")
    Fail = loadOld(configbuffer);
  //else
  //Fail = loadNew(configbuffer);

  return Fail;
}

const std::string & CReaction::getKey() const {return mKey;}

const C_FLOAT64 & CReaction::getFlux() const
  {return mFlux;}

const C_FLOAT64 & CReaction::getParticleFlux() const
  {return mParticleFlux;}

//****************************************

const CChemEq & CReaction::getChemEq() const
  {return mChemEq;}

CChemEq & CReaction::getChemEq()
{return mChemEq;}

bool CReaction::isReversible() const
  {return mChemEq.getReversibility();}

bool CReaction::addSubstrate(const std::string & metabKey,
                             const C_FLOAT64 & multiplicity)
{return mChemEq.addMetabolite(metabKey, multiplicity, CChemEq::SUBSTRATE);}

bool CReaction::addProduct(const std::string & metabKey,
                           const C_FLOAT64 & multiplicity)
{return mChemEq.addMetabolite(metabKey, multiplicity, CChemEq::PRODUCT);}

bool CReaction::addModifier(const std::string & metabKey,
                            const C_FLOAT64 & multiplicity)
{return mChemEq.addMetabolite(metabKey, multiplicity, CChemEq::MODIFIER);}

//bool CReaction::deleteModifier(const std::string &name)
//{return false;} /* :TODO: this needs to be implemented on CChemEq first. */

void CReaction::setReversible(bool reversible)
{mChemEq.setReversibility(reversible);}

//****************************************

const CFunction & CReaction::getFunction() const
  {return *mpFunction;}

bool CReaction::setFunction(const std::string & functionName)
{
  CFunction * pFunction =
    dynamic_cast<CFunction *>(CCopasiDataModel::Global->getFunctionList()->findLoadFunction(functionName));
  if (!pFunction)
    CCopasiMessage(CCopasiMessage::ERROR, MCReaction + 1, functionName.c_str());

  return setFunction(pFunction);
}

bool CReaction::setFunction(CFunction * pFunction)
{
  if (!pFunction)
    mpFunction = CCopasiDataModel::Global->mpUndefined;
  else
    mpFunction = pFunction;

  mMap.initializeFromFunctionParameters(mpFunction->getVariables());
  initializeMetaboliteKeyMap(); //needs to be called before initializeParamters();
  initializeParameters();

  return true;
}

//****************************************

// TODO: check if function is set and map initialized in the following methods

void CReaction::setParameterValue(const std::string & parameterName, C_FLOAT64 value, bool updateStatus)
{
  if (!mpFunction) fatalError();
  mParameters.setValue(parameterName, value);

  if (!updateStatus) return;

  //make shure that this local parameter is actually used:

  //first find index
  CFunctionParameter::DataType Type;
  C_INT32 index = mMap.findParameterByName(parameterName, Type);
  //std::cout << "reaction::setParameterValue  " << parameterName << " index " << index << std::endl;

  if (index == C_INVALID_INDEX)
    return;

  if (getFunctionParameters()[index]->getType() != CFunctionParameter::FLOAT64) fatalError(); //wrong data type

  //set the key map
  mMetabKeyMap[index][0] = mParameters.getParameter(parameterName)->getKey();
}

const C_FLOAT64 & CReaction::getParameterValue(const std::string & parameterName) const
  {
    if (!mpFunction) fatalError();
    return * mParameters.getValue(parameterName).pDOUBLE;
  }

const CCopasiParameterGroup & CReaction::getParameters() const
{return mParameters;}

CCopasiParameterGroup & CReaction::getParameters()
{return mParameters;}

void CReaction::setParameterMapping(C_INT32 index, const std::string & key)
{
  //std::cout << "CReaction::setParameterMapping, index = " << index << ", key = " << key << std::endl;
  if (!mpFunction) fatalError();
  if (getFunctionParameters()[index]->getType() != CFunctionParameter::FLOAT64) fatalError(); //wrong data type

  mMetabKeyMap[index][0] = key;
}

void CReaction::addParameterMapping(C_INT32 index, const std::string & key)
{
  if (!mpFunction) fatalError();
  if (getFunctionParameters()[index]->getType() != CFunctionParameter::VFLOAT64) fatalError(); //wrong data type

  mMetabKeyMap[index].push_back(key);
}

void CReaction::setParameterMapping(const std::string & parameterName, const std::string & key)
{
  if (!mpFunction) fatalError();

  CFunctionParameter::DataType type;
  unsigned C_INT32 index;
  index = mMap.findParameterByName(parameterName, type);
  if (type != CFunctionParameter::FLOAT64) fatalError();

  mMetabKeyMap[index][0] = key;
}

void CReaction::addParameterMapping(const std::string & parameterName, const std::string & key)
{
  if (!mpFunction) fatalError();

  CFunctionParameter::DataType type;
  unsigned C_INT32 index;
  index = mMap.findParameterByName(parameterName, type);
  if (type != CFunctionParameter::VFLOAT64) fatalError();

  mMetabKeyMap[index].push_back(key);
}

void CReaction::setParameterMappingVector(const std::string & parameterName,
    const std::vector<std::string> & keys)
{
  if (!mpFunction) fatalError();

  CFunctionParameter::DataType type;
  unsigned C_INT32 index;
  index = mMap.findParameterByName(parameterName, type);
  if ((type == CFunctionParameter::FLOAT64) && (keys.size() != 1)) fatalError();

  mMetabKeyMap[index] = keys;
}

void CReaction::clearParameterMapping(const std::string & parameterName)
{
  if (!mpFunction) fatalError();
  CFunctionParameter::DataType type;
  unsigned C_INT32 index;
  index = mMap.findParameterByName(parameterName, type);
  if (type != CFunctionParameter::VFLOAT64) fatalError(); //wrong data type
  mMetabKeyMap[index].clear();
  //mMap.clearCallParameter(parameterName);
}

void CReaction::clearParameterMapping(C_INT32 index)
{
  if (!mpFunction) fatalError();
  if (getFunctionParameters()[index]->getType() != CFunctionParameter::VFLOAT64) fatalError();
  mMetabKeyMap[index].clear();
  //mMap.clearCallParameter(parameterName);
}

bool CReaction::isLocalParameter(C_INT32 index) const
  {
    unsigned C_INT32 i, imax = mParameters.size();
    for (i = 0; i < imax; ++i)
      {
        if (mParameters.getParameter(i)->getKey() == mMetabKeyMap[index][0])
          return true;
      }
    return false;
  }

//***********************************************************************************************

void CReaction::initializeParameters()
{
  if (!mpFunction) fatalError();
  unsigned C_INT32 i;
  unsigned C_INT32 imax = mMap.getFunctionParameters().getNumberOfParametersByUsage("PARAMETER");
  unsigned C_INT32 pos;
  std::string name;

  /* We have to be more intelligent here because during an XML load we have
     already the correct parameters */

  /* Add missing parameters with default value 1.0. */
  for (i = 0, pos = 0; i < imax; ++i)
    {
      name = mMap.getFunctionParameters().getParameterByUsage("PARAMETER", pos).getObjectName();
      //      param.setName(name);
      if (!mParameters.getParameter(name))
        {
          mParameters.addParameter(name,
                                   CCopasiParameter::DOUBLE,
                                   (C_FLOAT64) 1.0);
        }
      CCopasiParameter * tmpPar = mParameters.getParameter(name);
      mMetabKeyMap[pos - 1][0] = tmpPar->getKey();
    }

  /* Remove parameters not fitting current function */
  CCopasiParameterGroup::index_iterator begin = mParameters.beginIndex();
  CCopasiParameterGroup::index_iterator it = mParameters.endIndex();
  CFunctionParameter::DataType Type;

  while (it != begin)
    {
      --it;

      name = (*it)->getObjectName();
      if (mMap.findParameterByName(name, Type) == C_INVALID_INDEX)
        mParameters.removeParameter(name);
    }
}

void CReaction::initializeMetaboliteKeyMap()
{
  if (!mpFunction) fatalError();
  unsigned C_INT32 i;
  unsigned C_INT32 imax = mMap.getFunctionParameters().size();

  mMetabKeyMap.resize(imax);
  for (i = 0; i < imax; ++i)
    {
      if (mMap.getFunctionParameters()[i]->getType() >= CFunctionParameter::VINT32)
        mMetabKeyMap[i].resize(0);
      else
        mMetabKeyMap[i].resize(1);
    }
}

const CFunctionParameters & CReaction::getFunctionParameters() const
  {
    if (!mpFunction) fatalError();
    return mMap.getFunctionParameters();
  }

void CReaction::compile()
{
  //mChemEq.compile(compartments);

  if (mpFunction)
    {
      unsigned C_INT32 i, j, jmax;
      unsigned C_INT32 imax = mMap.getFunctionParameters().size();
      std::string paramName;

      for (i = 0; i < imax; ++i)
        {
          paramName = getFunctionParameters()[i]->getObjectName();
          if (mMap.getFunctionParameters()[i]->getType() >= CFunctionParameter::VINT32)
            {
              mMap.clearCallParameter(paramName);
              jmax = mMetabKeyMap[i].size();
              for (j = 0; j < jmax; ++j)
                mMap.addCallParameter(paramName, GlobalKeys.get(mMetabKeyMap[i][j]));
            }
          else
            mMap.setCallParameter(paramName, GlobalKeys.get(mMetabKeyMap[i][0]));
        }
    }

  setScalingFactor();
}

C_INT32 CReaction::loadOld(CReadConfig & configbuffer)
{
  std::string name;

  C_INT32 Fail = 0;
  unsigned C_INT32 SubstrateSize, ProductSize, ModifierSize, ParameterSize;
  unsigned C_INT32 i, imax, pos;
  C_INT32 index;
  std::string parName, metabName;

  CFunctionParameter::DataType Type;

  configbuffer.getVariable("Substrates", "C_INT32", &SubstrateSize);
  configbuffer.getVariable("Products", "C_INT32", &ProductSize);
  configbuffer.getVariable("Modifiers", "C_INT32", &ModifierSize);
  configbuffer.getVariable("Constants", "C_INT32", &ParameterSize);

  // Construct substrates mapping
  // this works only because of a strange signed->unsigned cast in usageRangeSize()!
  imax = std::min(SubstrateSize, usageRangeSize("SUBSTRATE")); //some checks?

  CModel * pModel = (CModel*) getObjectAncestor("Model");
  const CCopasiVector< CMetab > & Metabolites = pModel->getMetabolites();

  for (i = 0, pos = 0, Type = CFunctionParameter::INT32; i < imax; i++)
    {
      name = StringPrint("Subs%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);

      metabName = (*CCopasiDataModel::Global->pOldMetabolites)[index]->getObjectName();

      if (Type < CFunctionParameter::VINT32)
        {
          Type = mMap.getFunctionParameters().getParameterByUsage("SUBSTRATE", pos).getType();
          parName = mMap.getFunctionParameters()[pos - 1]->getObjectName();
          if (Type >= CFunctionParameter::VINT32)
            clearParameterMapping(parName);
        }

      if (Type >= CFunctionParameter::VINT32)
        addParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());
      else
        setParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());
    }

  for (i = imax; i < SubstrateSize; i++)
    {
      name = StringPrint("Subs%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);
    }

  // Construct Products mapping
  imax = std::min(ProductSize, usageRangeSize("PRODUCT"));

  for (i = 0, pos = 0, Type = CFunctionParameter::INT32; i < imax; i++)
    {
      name = StringPrint("Prod%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);

      metabName = (*CCopasiDataModel::Global->pOldMetabolites)[index]->getObjectName();

      if (Type < CFunctionParameter::VINT32)
        {
          Type = mMap.getFunctionParameters().getParameterByUsage("PRODUCT", pos).getType();
          parName = mMap.getFunctionParameters()[pos - 1]->getObjectName();
          if (Type >= CFunctionParameter::VINT32)
            clearParameterMapping(parName);
        }

      if (Type >= CFunctionParameter::VINT32)
        addParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());
      else
        setParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());
    }

  for (i = imax; i < ProductSize; i++)
    {
      name = StringPrint("Prod%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);
    }

  // Construct modifiers mapping
  imax = std::min(ModifierSize, usageRangeSize("MODIFIER"));

  for (i = 0, pos = 0, Type = CFunctionParameter::INT32; i < imax; i++)
    {
      name = StringPrint("Modf%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);

      metabName = (*CCopasiDataModel::Global->pOldMetabolites)[index]->getObjectName();

      if (Type < CFunctionParameter::VINT32)
        {
          Type = mMap.getFunctionParameters().getParameterByUsage("MODIFIER", pos).getType();
          parName = mMap.getFunctionParameters()[pos - 1]->getObjectName();
          if (Type >= CFunctionParameter::VINT32)
            clearParameterMapping(parName);
        }

      if (Type >= CFunctionParameter::VINT32)
        addParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());
      else
        setParameterMapping(parName, Metabolites[pModel->findMetabByName(metabName)]->getKey());

      // in the old files the chemical equation does not contain
      // information about modifiers. This has to be extracted from here.
      mChemEq.addMetabolite(Metabolites[pModel->findMetabByName(metabName)]->getKey(), 1, CChemEq::MODIFIER);
    }

  for (i = imax; i < ModifierSize; i++)
    {
      name = StringPrint("Modf%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);
    }

  // Construct parameters
  imax = ParameterSize;
  C_FLOAT64 value;
  for (i = 0, pos = 0; i < imax; i++)
    {
      name = StringPrint("Param%d", i);
      configbuffer.getVariable(name, "C_FLOAT64", &value);

      Type = mMap.getFunctionParameters().getParameterByUsage("PARAMETER", pos).getType();
      if (Type != CFunctionParameter::FLOAT64) {Fail = 1; return Fail;}

      setParameterValue(mMap.getFunctionParameters()[pos - 1]->getObjectName(), value);
    }

  for (i = imax; i < ParameterSize; i++)
    {
      name = StringPrint("Param%d", i);
      configbuffer.getVariable(name, "C_INT32", &index);
    }

  return Fail;
}

C_FLOAT64 CReaction::calculate()
{
  mFlux = *mScalingFactor * mpFunction->calcValue(mMap.getPointers());
  mParticleFlux = *mUnitScalingFactor * mFlux;
  return mFlux;
}

C_FLOAT64 CReaction::calculatePartialDerivative(C_FLOAT64 & xi,
    const C_FLOAT64 & derivationFactor,
    const C_FLOAT64 & resolution)
{
  if (mpFunction->dependsOn(&xi, mMap.getPointers()))
    {
      C_FLOAT64 store = xi;
      C_FLOAT64 f1, f2;
      C_FLOAT64 tmp =
        (store < resolution) ? resolution * (1.0 + derivationFactor) : store; //TODO: why assymmetric?

      xi = tmp * (1.0 + derivationFactor);
      f1 = mpFunction->calcValue(mMap.getPointers());

      xi = tmp * (1.0 - derivationFactor);
      f2 = mpFunction->calcValue(mMap.getPointers());

      xi = store;

      return *mScalingFactor * (f1 - f2) / (2.0 * tmp * derivationFactor);
      //this is d(flow)/d(concentration)
    }
  else
    return 0.0;
}

unsigned C_INT32 CReaction::usageRangeSize(const std::string & usage) const
  {
    if (!mpFunction) fatalError();
    const CUsageRange * pUsageRange = NULL;
    C_INT32 Size = 0;
    unsigned C_INT32 Index;

    Index = mMap.getFunctionParameters().getUsageRanges().getIndex(usage);
    if (Index != C_INVALID_INDEX)
      pUsageRange = mMap.getFunctionParameters().getUsageRanges()[Index];

    if (pUsageRange)
      Size = std::max(pUsageRange->getLow(), pUsageRange->getHigh());

    return Size;
    // if I understand correctly this works only because infinity=-1 becomes a very big number when
    // converted to unsigned int
  }

unsigned C_INT32 CReaction::getCompartmentNumber() const
{return mChemEq.getCompartmentNumber();}

const CCompartment & CReaction::getLargestCompartment() const
  {return mChemEq.getLargestCompartment();}

/*const CCompartment & CReaction::getSmallestCompartment() const
{return mChemEq.getSmallestCompartment();}*/

void CReaction::setScalingFactor()
{
  if (1 == getCompartmentNumber())
    mScalingFactor = & mChemEq.getBalances()[0]->getMetabolite().getCompartment()->getVolume();
  else
    mScalingFactor = &mDefaultScalingFactor;

#ifdef XXXX
  if (mpFunctionCompartment)
    {
      // should propably check if the compartment appears in the chemical equation
      mScalingFactor = & mpFunctionCompartment->getVolume();
    }
  else
    {
      try
      {mScalingFactor = & mChemEq.CheckAndGetFunctionCompartment()->getVolume();}
      catch (CCopasiException Exc)
        {
          unsigned C_INT32 nr = Exc.getMessage().getNumber();
          if ((MCChemEq + 2 == nr) || (MCChemEq + 3 == nr))
            CCopasiMessage(CCopasiMessage::ERROR, MCReaction + 2, getObjectName().c_str());
          if (MCChemEq + 1 == nr)
            CCopasiMessage(CCopasiMessage::ERROR, MCReaction + 3, getObjectName().c_str());
          throw;
        }
    }
#endif // XXXX

  CModel * pModel = (CModel *) getObjectAncestor("Model");
  if (pModel)
    mUnitScalingFactor = & pModel->getQuantity2NumberFactor();
  else
    mUnitScalingFactor = & mDefaultScalingFactor;
}

void CReaction::initObjects()
{
  addObjectReference("Flux", mFlux, CCopasiObject::ValueDbl);
  addObjectReference("ParticleFlux", mParticleFlux, CCopasiObject::ValueDbl);
  //add(&mParameters);
  //add(&mMap);
}

std::ostream & operator<<(std::ostream &os, const CReaction & d)
{
  os << "CReaction:  " << d.getObjectName() << std::endl;

  os << "   mChemEq " << std::endl;
  os << d.mChemEq;

  if (d.mpFunction)
    os << "   *mpFunction " << d.mpFunction->getObjectName() << std::endl;
  else
    os << "   mpFunction == 0 " << std::endl;

  //os << "   mParameterDescription: " << std::endl << d.mParameterDescription;
  os << "   mFlux: " << d.mFlux << std::endl;

  if (d.mScalingFactor)
    os << "   *mScalingFactor " << *(d.mScalingFactor) << std::endl;
  else
    os << "   mScalingFactor == 0 " << std::endl;

  os << "   mUnitScalingFactor: " << d.mUnitScalingFactor << std::endl;
  os << "----CReaction" << std::endl;

  return os;
}
