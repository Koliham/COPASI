/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/StateWidget.cpp,v $
   $Revision: 1.7 $
   $Name:  $
   $Author: stupe $ 
   $Date: 2005/01/27 22:11:44 $
   End CVS Header */

#include <qpushbutton.h>
#include <qlayout.h>
#include "copasi.h"

#include "StateWidget.h"
#include "StateSubwidget.h"
#include "DataModelGUI.h"
#include "report/CKeyFactory.h"
#include "steadystate/CSteadyStateTask.h"
#include "model/CModel.h"

//#include "report/CKeyFactory.h"
#include "qtUtilities.h"

/*
 *  Constructs a StateWidget which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
StateWidget::StateWidget(QWidget* parent, const char* name, WFlags fl)
    : CopasiWidget(parent, name, fl),
    objKey("")
{
  if (!name)
    setName("StateWidget");
  setCaption(trUtf8("StateWidget"));

  setInitialState = new QPushButton(this, "setInitialState");
  setInitialState->setText(trUtf8("SetResultsAsNextInitialState"));
  connect(setInitialState, SIGNAL(clicked()), this, SLOT(runSetInitialState()));

  parentLayout = new QVBoxLayout(this, 0, 0, "parentLayout");
  stateLayout = new QHBoxLayout(0, 0 , 6, "StateLayout");
  mWidgetLayout = new QGridLayout(this, 1, 1, 0, -1, "Layout");

  mCentralWidget = new StateSubwidget(this, "StateSubwidget");
  mWidgetLayout->addWidget(mCentralWidget, 0, 0);

  parentLayout->addLayout(stateLayout);
  parentLayout->addLayout(mWidgetLayout);
  stateLayout->addWidget(setInitialState);

  /*commitChanges = new QPushButton(this, "commitChanges");
  commitChanges->setText(trUtf8("Commit"));
  Layout5->addWidget(commitChanges);

  cancelChanges = new QPushButton(this, "cancelChanges");
  cancelChanges->setText(trUtf8("Revert"));
  Layout5->addWidget(cancelChanges);*/

  // signals and slots connections
  //connect(commitChanges, SIGNAL(clicked()), this, SLOT(slotBtnOKClicked()));
  //connect(cancelChanges, SIGNAL(clicked()), this, SLOT(slotBtnCancelClicked()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
StateWidget::~StateWidget()
{}

/* This function loads the compartments widget when its name is
  clicked in the tree   */
bool StateWidget::loadFromBackend()
{
  mCentralWidget->showUnits();
  if (!dataModel->getSteadyStateTask()) return false;
  if (!dataModel->getSteadyStateTask()->getState()) return false;
  return mCentralWidget->loadAll(dataModel->getSteadyStateTask());
}

bool StateWidget::saveToBackend()
{
  return true;
}

/*void StateWidget::slotBtnCancelClicked()
{
  enter(objKey); // reload
}*/

/*void StateWidget::slotBtnOKClicked()
{
  saveToCompartment();
}*/

bool StateWidget::update(ListViews::ObjectType C_UNUSED(objectType), ListViews::Action

                         C_UNUSED(action), const std::string & C_UNUSED(key))
{
  return true;
}

bool StateWidget::leave()
{
  //return saveToCompartment();
  return true;
}

bool StateWidget::enter(const std::string & C_UNUSED(key))
{
  //objKey = key;
  return loadFromBackend();
  /*CCompartment* comp = dynamic_cast< CCompartment * >(GlobalKeys.get(key));

  if (comp) return loadFromCompartment(comp);
  else return false;*/
}

void StateWidget::runSetInitialState()
{
  CSteadyStateTask* mSteadyStateTask =
    //dynamic_cast<CSteadyStateTask *>(GlobalKeys.get(objKey));
    //I need to remove this hardcoding -- sameer
    dynamic_cast<CSteadyStateTask *>(GlobalKeys.get("Task_2"));
  const CState *currentState = mSteadyStateTask->getState();
  if (currentState)
    dataModel->getModel()->setInitialState(currentState);
}
