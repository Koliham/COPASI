/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/utilities/COutputHandler.cpp,v $
   $Revision: 1.12 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2006/04/16 21:05:31 $
   End CVS Header */

#include "copasi.h"

#include "COutputHandler.h"
#include "CCopasiTask.h"

COutputHandler::COutputHandler(CCopasiTask * pTask):
    mpTask(pTask),
    mInterfaces(),
    mIsMaster(false)
{}

COutputHandler::~COutputHandler() {};

bool COutputHandler::compile(std::vector< CCopasiContainer * > listOfContainer)
{
  bool success = true;

  std::set< COutputInterface *>::iterator it = mInterfaces.begin();
  std::set< COutputInterface *>::iterator end = mInterfaces.end();

  std::set< CCopasiObject * >::const_iterator itObj;
  std::set< CCopasiObject * >::const_iterator endObj;

  if (mpTask) listOfContainer.push_back(mpTask);

  for (; it != end; ++it)
    {
      success &= (*it)->compile(listOfContainer);

      // Collect the list of objects
      const std::set< CCopasiObject * > & Objects = (*it)->getObjects();
      for (itObj = Objects.begin(), endObj = Objects.end(); itObj != endObj; ++itObj)
        mObjects.insert(*itObj);
    }

  if (mIsMaster) success &= compileRefresh();

  return success;
}

void COutputHandler::output(const Activity & activity)
{
  if (mIsMaster) refresh();

  std::set< COutputInterface *>::iterator it = mInterfaces.begin();
  std::set< COutputInterface *>::iterator end = mInterfaces.end();

  for (; it != end; ++it)
    (*it)->output(activity);

  return;
}

void COutputHandler::separate(const Activity & activity)
{
  std::set< COutputInterface *>::iterator it = mInterfaces.begin();
  std::set< COutputInterface *>::iterator end = mInterfaces.end();

  for (; it != end; ++it)
    (*it)->separate(activity);

  return;
}

void COutputHandler::finish()
{
  std::set< COutputInterface *>::iterator it = mInterfaces.begin();
  std::set< COutputInterface *>::iterator end = mInterfaces.end();

  for (; it != end; ++it)
    (*it)->finish();

  return;
}

void COutputHandler::addInterface(COutputInterface * pInterface)
{mInterfaces.insert(pInterface);}

void COutputHandler::removeInterface(COutputInterface * pInterface)
{mInterfaces.erase(pInterface);}

void COutputHandler::setMaster(const bool & isMaster)
{mIsMaster = isMaster;}

const bool & COutputHandler::isMaster() const
  {return mIsMaster;}

void COutputHandler::refresh()
{
  std::vector< Refresh * >::iterator it = mObjectRefreshes.begin();
  std::vector< Refresh * >::iterator end = mObjectRefreshes.end();

  for (;it != end; ++it) (**it)();
}

bool COutputHandler::compileRefresh()
{
  mObjectRefreshes.clear();

  std::set< CCopasiObject * >::const_iterator it = mObjects.begin();
  std::set< CCopasiObject * >::const_iterator end = mObjects.end();

  for (; it != end; ++it)
    if ((*it)->getRefresh())
      mObjectRefreshes.push_back((*it)->getRefresh());

  return true;
}
