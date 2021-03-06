// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsEffect>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtCore/QDateTime>
#include <QtGui/QToolBar>

#include <qlayout/CQAnimationWindow.h>
#include <qlayout/CQAnimationSettingsEditor.h>
#include <qlayout/CQCopasiAnimation.h>
#include <qlayout/CQLayoutScene.h>
#include <qlayout/CQCopasiEffect.h>
#include <qlayout/CQEffectDescription.h>

#include <layoutUI/CQSpringLayoutParameterWindow.h>
#include <layoutUI/CQLayoutThread.h>

#include <layout/CLayout.h>

#include <report/CCopasiObjectName.h>
#include <report/CCopasiRootContainer.h>
#include <resourcesUI/CQIconResource.h>

#include <model/CModel.h>
#include <model/CReaction.h>
#include <elementaryFluxModes/CEFMTask.h>
#include <elementaryFluxModes/CEFMProblem.h>
#include <elementaryFluxModes/CFluxMode.h>

class QConservedSpeciesAnimation : public CQCopasiAnimation
{
  virtual void initialize(const CCopasiDataModel &dataModel)
  {
    mpDataModel = &dataModel;
    const CModel& model = *dataModel.getModel();
    const CCopasiVector< CMetab > & metabs = model.getMetabolites();
    CCopasiVector< CMetab >::const_iterator it = metabs.begin();

    while (it != metabs.end())
      {
        mEntries.push_back(new CQEffectDescription((*it)->getCN()));
        ++it;
      }

    // initialize number of steps
    const CCopasiVector< CMoiety > & moieties = model.getMoieties();
    mNumSteps = moieties.size();
  }

  virtual void getScales(std::vector<qreal>& scales, int step)
  {
    if (mpDataModel == NULL) return;

    const CModel& model = *mpDataModel->getModel();
    const CCopasiVector< CMoiety > & moieties = model.getMoieties();
    mNumSteps = moieties.size();

    if (moieties.size() <= (size_t)step) return;

    const CMoiety* moiety = moieties[step];
    const std::vector<std::pair< C_FLOAT64, CMetab * > > &eqn = moiety->getEquation();
    std::map<std::string, double> cnValueMap;
    std::vector<std::pair< C_FLOAT64, CMetab * > >::const_iterator it = eqn.begin();

    while (it != eqn.end())
      {
        cnValueMap[(*it).second->getCN()] = (*it).first;
        ++it;
      }

    for (size_t i = 0; i < mEntries.size(); ++i)
      scales.push_back(cnValueMap[mEntries[i]->getCN()]);
  }
};

/**
 * Animation, that displays one fluxmode per frame
 */
class QFluxModeAnimation : public CQCopasiAnimation
{
public:
  virtual void initialize(const CCopasiDataModel &dataModel)
  {
    mpDataModel = &dataModel;
    const CModel& model = *dataModel.getModel();
    const CCopasiVector< CReaction > & reactions = model.getReactions();
    CCopasiVector< CReaction >::const_iterator it = reactions.begin();
    size_t count = 0;

    while (it != reactions.end())
      {
        mEntries.push_back(new CQEffectDescription((*it)->getCN(), CQEffectDescription::Colorize, Qt::black, Qt::red));
        indexMap[count] = (*it)->getCN();
        ++it;
        ++count;
      }

    // initialize number of steps
    CEFMTask *task = dynamic_cast< CEFMTask * >((*mpDataModel->getTaskList())["Elementary Flux Modes"]);

    if (task == NULL) return;

    const CEFMProblem* problem = dynamic_cast<const CEFMProblem*>(task->getProblem());

    if (problem == NULL) return;

    const std::vector< CFluxMode >& fluxModes = problem->getFluxModes();
    mNumSteps = fluxModes.size();
  }
  virtual void getScales(std::vector<qreal>& scales, int step)
  {
    if (mpDataModel == NULL) return;

    CEFMTask *task = dynamic_cast< CEFMTask * >((*mpDataModel->getTaskList())["Elementary Flux Modes"]);

    if (task == NULL) return;

    const CEFMProblem* problem = dynamic_cast<const CEFMProblem*>(task->getProblem());

    if (problem == NULL) return;

    const std::vector< CFluxMode >& fluxModes = problem->getFluxModes();
    mNumSteps = fluxModes.size();

    if (fluxModes.size() <= (size_t)step) return;

    const CFluxMode& mode = fluxModes[step];

    const std::vector< const CReaction * > &reordered = problem->getReorderedReactions();
    std::map<std::string, double> cnValueMap;

    CFluxMode::const_iterator modeIt = mode.begin();

    while (modeIt  != mode.end())
      {
        const size_t reactionIndex = (*modeIt).first;
        const double coefficient = (*modeIt).second;
        cnValueMap[reordered[reactionIndex]->getCN()] = qAbs(coefficient);
        ++modeIt;
      }

    for (size_t i = 0; i < mEntries.size(); ++i)
      scales.push_back(cnValueMap[mEntries[i]->getCN()]);
  }

protected:
  std::map<size_t, std::string> indexMap;
};

#include <model/CModel.h>
#include <trajectory/CTrajectoryTask.h>
/**
 * Animation that displays the concentrations per time
 */
class QTimeCourseAnimation : public CQCopasiAnimation
{
public:
	
  double getMax(const CTimeSeries* series, size_t index = C_INVALID_INDEX)
  {
    double max = 0;

    if (index != C_INVALID_INDEX)
      {
        for (size_t i = 0; i < series->getRecordedSteps(); ++i)
          {
            max = qMax(max, series->getData(i, index));
          }
      }
    else
      {

        for (size_t i = 0; i < series->getRecordedSteps(); ++i)
          {
            for (size_t j = 0; j < series->getNumVariables(); ++j)
              {
                max = qMax(max, series->getData(i, j));
              }
          }
      }

    return max;
  }

  size_t getIndex(const CTimeSeries*series, const std::string& cn)
  {
    const std::string& key = keyMap[cn];

    for (size_t i = 0; i < series->getNumVariables(); ++i)
      if (series->getKey(i) == key)
        return i;

    return C_INVALID_INDEX;
  }

  double getValue(const CTimeSeries* series, const std::string& cn, int step)
  {
    return series->getData(step, getIndex(series, cn));
  }

  virtual void getScales(std::vector<qreal>& scales, int step, std::vector<qreal>& values = std::vector<qreal>(), std::vector<qreal>& borders = std::vector<qreal>())
  {
    if (mpDataModel == NULL) return;

    CTrajectoryTask *task = dynamic_cast< CTrajectoryTask * >((*mpDataModel->getTaskList())["Time-Course"]);

    if (task == NULL) return;

    const CTimeSeries* series = &task->getTimeSeries();

    if (series == NULL) return;

    mNumSteps = series->getRecordedSteps();

    if (series->getRecordedSteps() < (size_t)step)
      return;

    double max = mMode == CQCopasiAnimation::Global ? getMax(series) : 0;

	//------------------------- Auto Fit Alogorithm-----------------------------

	std::vector<qreal> medians;
	std::vector<qreal> variance;
	std::vector<qreal> average;
	double maxvalue, minvalue;
	size_t maxindex, minindex;
	
	//initializing the min and max medians of all the entries
	maxvalue = 0.0; //these are GLOBAL min and max values
	minvalue = series->getConcentrationData(0,1);
	minindex = 0;
	maxindex = 0;
	if (analyseflag == false) //it checks, if the analysis was done, if not, it does
	{
		for (size_t i = 0; i < mEntries.size(); i++)
		{
			double sum = 0;
			std::vector<qreal> tempvalues;

			//it can happen that the fence is bigger than the spectrum, e.g. the lowest value is 4.0 but the borders allow
			//values from 2.0 which doesnt make sense.
			//So I am saving the max and min for each, these are INDIVIDUAL min/max values for each metabolite
			double tempmin = series->getConcentrationData(0,i+1);
			double tempmax = series->getConcentrationData(0,i+1);

			for (size_t j = 0; j < mNumSteps; j++)
			{
				double actualvalue = series->getConcentrationData(j,i+1);
				tempvalues.push_back(actualvalue);
				sum += series->getConcentrationData(j,i+1);
				//check, if the actual value is higher/lower than tempmin/max
				if (actualvalue > tempmax)
					tempmax = actualvalue;
				if (actualvalue < tempmin)
					tempmin = actualvalue;


			}

			average.push_back ( (double) sum / mNumSteps );
			//sort
			std::sort(tempvalues.begin(),tempvalues.end());
			medians.push_back(tempvalues[mNumSteps/2]);
			//
			if (minvalue > medians[i])
			{
				minvalue = medians[i];
				minindex = i;
			}

			if (maxvalue < medians[i])
			{
				maxvalue = medians[i];
				maxindex = i;
			}

			//calculate the variance
			double tempvar = 0; 
			for (size_t j = 0; j < mNumSteps; j++)
			{
				double conc = series->getConcentrationData(j,i+1);
				tempvar += (conc - average[i]) * (conc - average[i]);
			}
			tempvar = (double) tempvar / mNumSteps;
			tempvar = sqrt(tempvar);
			variance.push_back(tempvar);

			//adding the boundaries for the gauge, if they are individual
			//if (mMode == CQCopasiAnimation::Individual) 
			//{ // I am adding the boundaries for all metabolites, so the first mEntries.size entries are for the individual boundaries
				double lower = medians[i] - (0.5*variance[i]);
				double upper = medians[i] + (0.5*variance[i]);
				if (lower < 0)
					lower = 0.0;
				//check, if boundaries are lower/higher than the min/max value
				if (lower < tempmin)
					lower = tempmin;
				if (upper > tempmax)
					upper = tempmax;

				boundary.push_back(lower);
				boundary.push_back(upper);

			//}


		}
		// I have a vector with the medians now

		// if quotient is higher than 100, it means, there are huge gaps between the lowest and highest value
		// needed for logarithmic values
	
		// and now the global boundaries
		double lower = minvalue - (0.5*variance[minindex]);
		if (lower < 0)
			lower = 0.0;
		double upper = maxvalue + (0.5*variance[maxindex]);
		boundary.push_back(lower); // it must be "lower" here!!!
		boundary.push_back(upper); // and upper
		analyseflag = true;


				if (minvalue == 0.0)
			minvalue = 1.0; //to protect from division by zero

		double quotient = maxvalue / minvalue;
	}
	else
	{
		for (int i = 0; i < boundary.size(); i++)
		{
			borders.push_back(boundary[i]);
		}
		
	}
	///---------------------AutoAdjust ---------------------------

    for (size_t i = 0; i < mEntries.size(); ++i)
      {
        if (mMode == CQCopasiAnimation::Individual)
          max  = getMax(series, getIndex(series, mEntries[i]->getCN()));
		 
        double value = getValue(series, mEntries[i]->getCN(), step);
		double wert = series->getConcentrationData(step, i+1);
		scales.push_back(value / max);
		values.push_back(wert);

				//get the previous value and compare it with the actual value
        if (step != 0)
		{
			double lastvalue = series->getConcentrationData(step-1,i+1);
			if (lastvalue < wert)
				{
				values.push_back(1.0);
				}
			else if (lastvalue > wert)
				{
				values.push_back(-1.0);
				}
			else
				{
				values.push_back(0.0);
				}
		}
		else
		{
			values.push_back(0.0);
		}
	}
  }

	// Values contains now the concentration AND the change from the previous concentration

  virtual void initialize(const CCopasiDataModel &dataModel)
  {
    mpDataModel = &dataModel;
    const CModel& model = *dataModel.getModel();
    const CCopasiVector< CMetab > & metabs = model.getMetabolites();
    CCopasiVector< CMetab >::const_iterator it = metabs.begin();

    while (it != metabs.end())
      {
        mEntries.push_back(new CQEffectDescription((*it)->getCN(), CQEffectDescription::Scale));
        keyMap[(*it)->getCN()] = (*it)->getKey();
        ++it;
      }

    // initialize number of steps
    CTrajectoryTask *task = dynamic_cast< CTrajectoryTask * >((*mpDataModel->getTaskList())["Time-Course"]);

    if (task == NULL) return;

    const CTimeSeries* series = &task->getTimeSeries();

    if (series == NULL) return;

    mNumSteps = series->getRecordedSteps();
	analyseflag = false;
  }
protected:
  std::map<std::string, std::string> keyMap; std::vector<qreal> boundary; bool analyseflag; 
};

CQAnimationWindow::CQAnimationWindow(CLayout* layout, CCopasiDataModel* dataModel)
  : mpScene(NULL)
  , mpModel(NULL)
  , mpWindowMenu(NULL)
  , mAnimation(NULL)
  , mpLayoutThread(NULL)
  , mpCopy(NULL)
{
  init();
  setScene(new CQLayoutScene(layout, dataModel), dataModel);
}

CQAnimationWindow::CQAnimationWindow()
  : mpScene(NULL)
  , mpModel(NULL)
  , mpWindowMenu(NULL)
  , mAnimation(NULL)
  , mpLayoutThread(NULL)
  , mpCopy(NULL)
{
  init();
}

void CQAnimationWindow::init()
{
  setupUi(this);
  setWindowIcon(CQIconResource::icon(CQIconResource::copasi));
  setUnifiedTitleAndToolBarOnMac(true);

  actionAuto_Layout->setIcon(CQIconResource::icon(CQIconResource::play));
  actionRandomize_Layout->setIcon(CQIconResource::icon(CQIconResource::roll));
  actionExport->setIcon(CQIconResource::icon(CQIconResource::fileExport));

  QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
  graphicsView->fillZoomMenu(viewMenu);

  QToolBar* actionToolbar = this->addToolBar("Actions");
  actionToolbar->addAction(actionExport);
  actionToolbar->addAction(actionAuto_Layout);
  actionToolbar->addAction(actionRandomize_Layout);

  QToolBar* selectToolbar = this->addToolBar("Select");
  graphicsView->fillSelectionToolBar(selectToolbar);

  QToolBar* viewToolBar = this->addToolBar("View");
  graphicsView->fillZoomTooBar(viewToolBar);

  mpWindowMenu = menuBar()->addMenu(tr("&Window"));

  mpLayoutThread = new CQLayoutThread(this);
  connect(mpLayoutThread, SIGNAL(layoutFinished()), this, SLOT(slotStopLayout()));
  connect(mpLayoutThread, SIGNAL(layoutStateChanged(QSharedPointer<CLayoutState>)), this, SLOT(slotLayoutStateChanged(QSharedPointer<CLayoutState>)));

  QDockWidget* pParameterWindow = mpLayoutThread->getParameterWindow();
  addDockWidget(Qt::LeftDockWidgetArea, pParameterWindow);
  viewMenu->addSeparator();
  viewMenu->addAction(pParameterWindow->toggleViewAction());
  toggleUI(false);
}

void CQAnimationWindow::slotExportImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export Image"),
                     "",
                     tr("PDF files (*.pdf);;Images (*.png *.xpm *.jpg);;All files (*.*)"));
  graphicsView->slotSaveToFile(fileName);
}

CQAnimationWindow::~CQAnimationWindow()
{
  mpLayoutThread->terminateLayout();

  if (mpCopy != NULL)
    {
      delete mpCopy;
      mpCopy = NULL;
    }

  if (mAnimation != NULL)
    {
      delete mAnimation;
      mAnimation = NULL;
    }

  removeFromMainWindow();
}

void CQAnimationWindow::setScene(CQLayoutScene* scene, CCopasiDataModel* dataModel)
{
  mpModel = dataModel;
  mpScene = scene;
  this->graphicsView->setScene(mpScene);
  mpScene->recreate();
  graphicsView->setDataModel(dataModel, scene->getCurrentLayout());
  this->graphicsView->invalidateScene();
  //setAnimation(new QConservedSpeciesAnimation(), dataModel);
  //setAnimation(new QFluxModeAnimation(), dataModel);
  setAnimation(new QTimeCourseAnimation(), dataModel);
}

void CQAnimationWindow::slotSwitchAnimation()
{
  QAction *action = dynamic_cast<QAction *>(sender());

  if (action == NULL) return;

  if (action->text() == "View Time Course")
    {
      setAnimation(new QTimeCourseAnimation(), graphicsView->getDataModel());
    }
  else if (action->text() == "View Elementary Modes")
    {
      setAnimation(new QFluxModeAnimation(), graphicsView->getDataModel());
    }
  else if (action->text() == "View Conserved Species")
    {
      setAnimation(new QConservedSpeciesAnimation(), graphicsView->getDataModel());
    }
}

QMenu *CQAnimationWindow::getWindowMenu() const
{
  return mpWindowMenu;
}

void CQAnimationWindow::setAnimation(CQCopasiAnimation* animation, CCopasiDataModel* dataModel)
{
  if (mAnimation != NULL)
    {
      mAnimation->removeFromScene(*mpScene);
      delete mAnimation;
    }

  mAnimation = animation;
  mAnimation->initialize(*dataModel);
  size_t numSteps = mAnimation->getNumSteps();

  if (numSteps > 0)
    {
      mpControls->setVisible(true);
      mpControls->setNumSteps(numSteps);
      slotShowStep(0);
    }
  else
    {
      mpControls->setVisible(false);
      statusBar()->showMessage("No data for the animation! Run task first, and load data.", 1000);
    }
}

void CQAnimationWindow::slotShowStep(int step)
{
  statusBar()->showMessage(QString("Displaying step %1").arg(step + 1), 1000);

  if (mAnimation == NULL) return;

  mAnimation->applyToScene(*mpScene, step, true);
  mpControls->setNumSteps(mAnimation->getNumSteps());
  mpScene->update();
}

void CQAnimationWindow::closeEvent(QCloseEvent * /*closeEvent*/)
{
  // stop the autolayout
  mpLayoutThread->stopLayout();
}

void CQAnimationWindow::slotEditSettings()
{
  CQAnimationSettingsEditor editor;
  editor.initFrom(mAnimation);

  if (editor.exec() == QDialog::Accepted)
    {
      editor.saveTo(mAnimation);
    }
}

void CQAnimationWindow::slotRandomizeLayout()
{
  mpLayoutThread->stopLayout();
  mpLayoutThread->wait();
  mpLayoutThread->randomizeLayout(mpScene->getCurrentLayout());
}

#include <layout/CCopasiSpringLayout.h>
#include <layout/CLayoutEngine.h>

#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QAbstractEventDispatcher>

#include <QtCore/QTimer>

void CQAnimationWindow::slotStopLayout()
{
  if (mpCopy != NULL)
    {
      delete mpCopy;
      mpCopy = NULL;
    }

  actionAuto_Layout->setChecked(false);
  actionAuto_Layout->setText("Run Auto Layout");
  actionAuto_Layout->setIcon(CQIconResource::icon(CQIconResource::play));
}

void CQAnimationWindow::slotLayoutStateChanged(QSharedPointer<CLayoutState> state)
{
  state->applyTo(mpScene->getCurrentLayout());
  mpScene->recreate();
}

void CQAnimationWindow::toggleUI(bool isPlaying)
{
  if (isPlaying)
    {
      actionAuto_Layout->setChecked(true);
      actionAuto_Layout->setText("Stop Auto Layout");
      actionAuto_Layout->setIcon(CQIconResource::icon(CQIconResource::pause));
    }
  else
    {
      mpLayoutThread->stopLayout();
    }
}

/// <summary>
/// Slots the auto layout.
/// </summary>
void CQAnimationWindow::slotAutoLayout()
{
  if (sender() != NULL && !actionAuto_Layout->isChecked())
    {
      mpLayoutThread->stopLayout();
      return;
    }

  toggleUI(true);

  // work on a copy!
  CLayoutState::tagLayout(mpScene->getCurrentLayout());

  if (mpCopy != NULL)
    {
      delete mpCopy;
      mpCopy = NULL;
    }

  mpCopy = new CLayout(*mpScene->getCurrentLayout());
  mpLayoutThread->createSpringLayout(mpCopy, 100000);
}
