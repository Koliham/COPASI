// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/model/CEvent.cpp,v $
//   $Revision: 1.20 $
//   $Name:  $
//   $Author: gauges $
//   $Date: 2009/05/07 10:45:11 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#include "copasi.h"

#include <stdio.h>

#include "CopasiDataModel/CCopasiDataModel.h"
#include "CModel.h"
#include "CEvent.h"
#include "utilities/CCopasiMessage.h"
#include "utilities/CCopasiException.h"
#include "utilities/utility.h"
#include "report/CCopasiObjectReference.h"
#include "report/CKeyFactory.h"
#include "copasi/report/CCopasiRootContainer.h"
#include "function/CExpression.h"

// The default constructor is intentionally not implemented.
// CEventAssignment::CEventAssignment() {}

CEventAssignment::CEventAssignment(const std::string & targetKey,
                                   const CCopasiContainer * pParent) :
    CCopasiContainer(targetKey, pParent, "EventAssignment"),
    mKey(CCopasiRootContainer::getKeyFactory()->add("EventAssignment", this)),
    mpExpression(NULL)
{}

CEventAssignment::CEventAssignment(const CEventAssignment & src,
                                   const CCopasiContainer * pParent):
    CCopasiContainer(src, pParent),
    mKey(CCopasiRootContainer::getKeyFactory()->add("EventAssignment", this)),
    mpExpression(NULL)
{
  setExpression(src.getExpression());
}

CEventAssignment::~CEventAssignment()
{
  pdelete(mpExpression);
}

const std::string & CEventAssignment::getKey() const
{
  return mKey;
}

const std::string & CEventAssignment::getTargetKey() const
{
  return getObjectName();
}

bool CEventAssignment::setExpression(const std::string & expression)
{
  if (mpExpression == NULL)
    mpExpression = new CExpression("Expression", this);

  return mpExpression->setInfix(expression);
}

void CEventAssignment::setExpressionPtr(CExpression * pExpression)
{
  pdelete(mpExpression);

  if (pExpression != NULL)
    {
      mpExpression = pExpression;
      mpExpression->setObjectParent(this);
      mpExpression->setObjectName("Expression");

      mpExpression->compile();
    }
}

std::string CEventAssignment::getExpression() const
{
  if (mpExpression == NULL)
    return "";

  mpExpression->updateInfix();

  return mpExpression->getInfix();
}

const CExpression* CEventAssignment::getExpressionPtr() const
{
  return mpExpression;
}

CExpression* CEventAssignment::getExpressionPtr()
{
  return mpExpression;
}

CEvent::CEvent(const std::string & name,
               const CCopasiContainer * pParent):
    CCopasiContainer(name, pParent, "Event"),
    mKey(CCopasiRootContainer::getKeyFactory()->add("Event", this)),
    mAssignments("ListOfAssignments", this),
    mDelayAssignment(true),
    mpTriggerExpression(NULL),
    mpDelayExpression(NULL)
{
  initObjects();
}

CEvent::CEvent(const CEvent & src,
               const CCopasiContainer * pParent):
    CCopasiContainer(src, pParent),
    mKey(CCopasiRootContainer::getKeyFactory()->add("Event", this)),
    mAssignments(src.mAssignments, this),
    mDelayAssignment(src.mDelayAssignment),
    mpTriggerExpression(src.mpTriggerExpression == NULL ? NULL : new CExpression(*src.mpTriggerExpression)),
    mpDelayExpression(src.mpDelayExpression == NULL ? NULL : new CExpression(*src.mpDelayExpression))
{
  initObjects();
}

CEvent::~CEvent()
{
  CCopasiRootContainer::getKeyFactory()->remove(mKey);
  pdelete(mpTriggerExpression);
  pdelete(mpDelayExpression);
}

const std::string & CEvent::getKey() const
{
  return mKey;
}

bool CEvent::compile()
{
  bool success = true;

  // TODO We need build the list of direct dependencies to assure the events are deleted
  // whenever an object used in an expression or assignment is deleted.

  return success;
}

void CEvent::initObjects()
{}

std::ostream & operator<<(std::ostream &os, const CEvent & d)
{
  os << "CEvent:  " << d.getObjectName() << std::endl;
  os << "   SBML id:  " << d.mSBMLId << std::endl;

  os << "----CEvent" << std::endl;

  return os;
}

void CEvent::setSBMLId(const std::string& id)
{
  this->mSBMLId = id;
}

const std::string& CEvent::getSBMLId() const
{
  return this->mSBMLId;
}

void CEvent::setDelayAssignment(const bool & delayAssignment)
{
  mDelayAssignment = delayAssignment;
}

const bool & CEvent::getDelayAssignment() const
{
  return mDelayAssignment;
}

std::string CEvent::getObjectDisplayName(bool regular, bool richtext) const
{
  CModel* tmp = dynamic_cast<CModel*>(this->getObjectAncestor("Model"));

  if (tmp)
    return "((" + getObjectName() + "))";

  return CCopasiObject::getObjectDisplayName(regular, richtext);
}

bool CEvent::setTriggerExpression(const std::string & expression)
{
  if (mpTriggerExpression == NULL)
    mpTriggerExpression = new CExpression("TriggerExpression", this);

  return mpTriggerExpression->setInfix(expression);
}

void CEvent::setTriggerExpressionPtr(CExpression * pExpression)
{
  pdelete(mpTriggerExpression);

  if (pExpression)
    {
      mpTriggerExpression = pExpression;
      pExpression->setObjectParent(this);
      mpTriggerExpression->compile();
    }
}

std::string CEvent::getTriggerExpression() const
{
  if (mpTriggerExpression == NULL)
    return "";

  mpTriggerExpression->updateInfix();

  return mpTriggerExpression->getInfix();
}

const CExpression* CEvent::getTriggerExpressionPtr() const
{
  return mpTriggerExpression;
}

CExpression* CEvent::getTriggerExpressionPtr()
{
  return mpTriggerExpression;
}

bool CEvent::setDelayExpression(const std::string & expression)
{
  if (mpDelayExpression == NULL)
    mpDelayExpression = new CExpression("DelayExpression", this);

  return mpDelayExpression->setInfix(expression);
}

void CEvent::setDelayExpressionPtr(CExpression * pExpression)
{
  pdelete(mpDelayExpression);

  if (pExpression)
    {
      mpDelayExpression = pExpression;
      this->mpDelayExpression->setObjectParent(this);
      mpDelayExpression->compile();
    }
}

std::string CEvent::getDelayExpression() const
{
  if (mpDelayExpression == NULL)
    return "";

  mpDelayExpression->updateInfix();

  return mpDelayExpression->getInfix();
}

const CExpression* CEvent::getDelayExpressionPtr() const
{
  return mpDelayExpression;
}

CExpression* CEvent::getDelayExpressionPtr()
{
  return mpDelayExpression;
}

const CCopasiVectorN< CEventAssignment > & CEvent::getAssignments() const
{
  return mAssignments;
}

CCopasiVectorN< CEventAssignment > & CEvent::getAssignments()
{
  return mAssignments;
}

void CEvent::deleteAssignment(const std::string & key)
{
  CEventAssignment * pAssignment =
    dynamic_cast<CEventAssignment *>(CCopasiRootContainer::getKeyFactory()->get(key));

  if (pAssignment != NULL)
    {
      mAssignments.CCopasiVector< CEventAssignment >::remove(pAssignment);
    }
}
