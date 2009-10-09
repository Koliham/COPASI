# Begin CVS Header
#   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/UI.pro,v $
#   $Revision: 1.220 $
#   $Name:  $
#   $Author: shoops $
#   $Date: 2009/10/09 21:33:50 $
# End CVS Header

# Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
# Properties, Inc., EML Research, gGmbH, University of Heidelberg,
# and The University of Manchester.
# All rights reserved.

# Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
# Properties, Inc. and EML Research, gGmbH.
# All rights reserved.

######################################################################
# $Revision: 1.220 $ $Author: shoops $ $Date: 2009/10/09 21:33:50 $
######################################################################

LIB = UI
include(../lib.pri)

CONFIG	+= qt
include(../common.pri)

DEPENDPATH += ..
INCLUDEPATH	+= ..
INCLUDEPATH += $${QWT3D_PATH}/include


# Input

### Headers that belong to widgets generated from uic4 forms
HEADERS	+= \
  CQCompartment.h \
  CQCompartmentsWidget.h \
  CQDifferentialEquations.h \
  CQEFMListWidget.h \
  CQEFMResultWidget.h \
  CQEFMWidget.h \
  CQEventsWidget.h \
  CQEventWidget1.h \
  CQExpressionMmlStackedWidget.h \
  CQExpressionMmlWidget.h \
  CQFittingItemWidget.h \
  CQFittingWidget.h \
  CQFunctionsWidget.h \
  CQGlobalQuantitiesWidget.h \
  CQLyapWidget.h \
  CQMCAWidget.h \
  CQModelValue.h \
  CQOptimizationWidget.h \
  CQPlotsWidget.h \
  CQProgressDialog.h \
  CQReactionsWidget.h \
  CQReportDefinition.h \
  CQReportsWidget.h \
  CQSpeciesDetail.h \
  CQSpeciesWidget.h \
  CQTrajectoryWidget.h \
  CQTSSAWidget.h \
  CScanWidgetRandom.h \
  CScanWidgetRepeat.h \
  CScanWidgetScan.h \
  CScanWidgetTask.h \
  SensitivitiesWidget.h \
  SliderSettingsDialog.h \
  StateSubwidget.h \
  SteadyStateWidget.h \
#
### other headers (not belonging to generated widgets)
  AboutDialog.h \
  CColorScale.h \
	CCopasiPlotSelectionDialog.h \
	CCopasiSelectionDialog.h \
	CCopasiSelectionWidget.h \
	CCopasiSimpleSelectionTree.h \
	ChemEqValidator.h \
	CMCAResultWidget.h \
	CReportDefinitionSelect.h \
	CopasiFileDialog.h \
	CopasiSlider.h \
	CopasiTableWidget.h \
	copasiWidget.h \
	copasiui3window.h \
	CProgressBar.h \
	CTabWidget.h \
	CQArrayAnnotationsWidget.h \
	CQBarChart.h \
	CQBaseDataModel.h \
	CQComboDelegate.h \
	CQCompartmentDM.h \
	CQDateTimeEditDelegate.h \
	CQDateTimeEditTableItem.h \
	CQEFMNetReactionDM.h  \
	CQEFMReactionDM.h \
  CQEFMSpeciesDM.h  \
	CQEventDM.h \
	CQExpressionWidget.h \
	CQFluxModeDM.h \
	CQFunctionDM.h \
	CQGlobalQuantityDM.h \
	CQIcons.h \
    CQLayoutsWidget.h \
	CQLyapResultWidget.h \
	CQMathMatrixWidget.h \
    CQMessageBox.h \
    CQPlotDM.h \
	CQReactionDM.h \
  CQReportDM.h \
	CQReportListItem.h \
	CQSBMLFileDialog.h \
	CQSensResultWidget.h \
	CQSortFilterProxyModel.h \
	CQSpecieDM.h \
	CQSpinBoxDelegate.h \
  CQTSSAResultWidget.h \
  CQTSSATimeScaleWidget.h \
	CQUpdatesWidget.h \
	CQValidator.h \
	CScanContainerWidget.h \
	CTimeSeriesTable.h \
	DataModelGUI.h \
	DataModel.txt.h \
	FunctionItemWidget.h \
	FunctionWidget1.h \
	ModelWidget.h \
	MyLineEdit.h \
	listviews.h \
	ObjectBrowserDialog.h \
	ObjectBrowserItem.h \
	ObjectBrowserWidget.h \
	ParametersWidget.h \
	parametertable.h \
	qtUtilities.h \
	ReactionsWidget1.h \
	ScanWidget.h \
	SensWidgetComboBox.h \
	SliderDialog.h \
	StateWidget.h \
	TaskWidget.h \
	TimeSeriesWidget.h \
	Tree.h \
	TSSWidget.h \
#        CQExpressionMmlWidgetStack.h \
	CQMmlScrollView.h

### Source files that belong to widgets generated from uic4 forms
SOURCES	+= \
  CQCompartment.cpp \
  CQCompartmentsWidget.cpp \
  CQDifferentialEquations.cpp \
  CQEFMListWidget.cpp \
  CQEFMResultWidget.cpp \
  CQEFMWidget.cpp \
  CQEventsWidget.cpp \
  CQEventWidget1.cpp \
  CQExpressionMmlStackedWidget.cpp \
  CQExpressionMmlWidget.cpp \
  CQFittingItemWidget.cpp \
  CQFittingWidget.cpp \
  CQFunctionsWidget.cpp \
  CQGlobalQuantitiesWidget.cpp \
  CQLyapWidget.cpp \
  CQMCAWidget.cpp \
  CQModelValue.cpp \
  CQOptimizationWidget.cpp \
  CQPlotsWidget.cpp \
  CQProgressDialog.cpp \
  CQReactionsWidget.cpp \
  CQReportDefinition.cpp \
  CQReportsWidget.cpp \
  CQSpeciesDetail.cpp \
  CQSpeciesWidget.cpp \
  CQTrajectoryWidget.cpp \
  CQTSSAWidget.cpp \
  CScanWidgetRandom.cpp \
  CScanWidgetRepeat.cpp \
  CScanWidgetScan.cpp \
  CScanWidgetTask.cpp \
  SensitivitiesWidget.cpp \
  SliderSettingsDialog.cpp \
  StateSubwidget.cpp \
  SteadyStateWidget.cpp \
#
### other sources (not belonging to generated widgets)
  AboutDialog.cpp \
  CColorScale.cpp \
  CCopasiPlotSelectionDialog.cpp \
  CCopasiSelectionDialog.cpp \
  CCopasiSelectionWidget.cpp \
  CCopasiSimpleSelectionTree.cpp \
  CMCAResultWidget.cpp \
  CReportDefinitionSelect.cpp \
  CopasiFileDialog.cpp \
  CopasiSlider.cpp \
  CopasiTableWidget.cpp \
  copasiWidget.cpp \
  copasiui3window.cpp \
  CProgressBar.cpp \
  CTabWidget.cpp \
  CQArrayAnnotationsWidget.cpp \
  CQBarChart.cpp \
  CQBaseDataModel.cpp \
  CQComboDelegate.cpp \
  CQCompartmentDM.cpp \
  CQDateTimeEditDelegate.cpp \
  CQDateTimeEditTableItem.cpp \
  CQEFMNetReactionDM.cpp  \
  CQEFMReactionDM.cpp \
  CQEFMSpeciesDM.cpp  \
  CQEventDM.cpp \
  CQExpressionWidget.cpp \
  CQFluxModeDM.cpp \
  CQFunctionDM.cpp \
  CQGlobalQuantityDM.cpp \
    CQLayoutsWidget.cpp \
  CQLyapResultWidget.cpp \
  CQMathMatrixWidget.cpp \
    CQMessageBox.cpp \
    CQPlotDM.cpp \
  CQReactionDM.cpp \
  CQReportDM.cpp \
  CQReportListItem.cpp \
  CQSBMLFileDialog.cpp \
  CQSensResultWidget.cpp \
  CQSortFilterProxyModel.cpp \
  CQSpecieDM.cpp \
  CQSpinBoxDelegate.cpp \
  CQTSSAResultWidget.cpp \
  CQTSSATimeScaleWidget.cpp \
  CQUpdatesWidget.cpp \
  CQValidator.cpp \
  CScanContainerWidget.cpp \
  CTimeSeriesTable.cpp \
  DataModelGUI.cpp \
  FunctionItemWidget.cpp \
  FunctionWidget1.cpp \
  ModelWidget.cpp \
  MyLineEdit.cpp \
  listviews.cpp \
  ObjectBrowserDialog.cpp \
  ObjectBrowserItem.cpp \
  ObjectBrowserWidget.cpp \
  ParametersWidget.cpp \
  parametertable.cpp \
  qtUtilities.cpp \
  ReactionsWidget1.cpp \
  ScanWidget.cpp \
  SensWidgetComboBox.cpp \
  SliderDialog.cpp \
  StateWidget.cpp \
  TaskWidget.cpp \
  TimeSeriesWidget.cpp \
  Tree.cpp \
  TSSWidget.cpp \
#        CQExpressionMmlWidgetStack.cpp \
  CQMmlScrollView.cpp



!contains(DEFINES, COPASI_TSS) {
  HEADERS -= TSSWidget.h
  SOURCES -= TSSWidget.cpp
}

### uic3 forms
FORMS3 += CMCAResultSubwidget.ui
FORMS3 += CQExperimentSelection.ui
#FORMS3 += CQFileDialogBtnGrp.ui
FORMS3 += CQFittingResult.ui
FORMS3 += CQFittingResultTab1.ui
FORMS3 += CQMatrixDialog.ui
FORMS3 += CQMoietiesTaskResult.ui
FORMS3 += CQMoietiesTaskWidget.ui
FORMS3 += CQOptimizationResult.ui
FORMS3 += CQPreferenceDialog.ui
FORMS3 += CQProgressItem.ui
FORMS3 += CQSplashWidget.ui
FORMS3 += CQStartValueReset.ui
FORMS3 += CQTSSAResultSubWidget.ui
FORMS3 += CQTaskBtnWidget.ui
FORMS3 += CQTaskHeaderWidget.ui
#FORMS3 += CQTextDialog.ui
FORMS3 += CUpDownSubwidget.ui
FORMS3 += DefaultplotDialog.ui
FORMS3 += TimeSeriesSubwidget.ui
FORMS3 += objectdebug.ui

### uic3 forms that do not work at the moment
#FORMS3 += CQExperimentData.ui
#FORMS3 += CQProgressItemBar.ui
#FORMS3 += CQProgressItemText.ui

FORMS += CQCompartment.ui
FORMS += CQCompartmentsWidget.ui
FORMS += CQDifferentialEquations.ui
FORMS += CQEFMListWidget.ui
FORMS += CQEFMResultWidget.ui
FORMS += CQEFMWidget.ui
FORMS += CQEventsWidget.ui
FORMS += CQEventWidget1.ui
FORMS += CQExpressionMmlStackedWidget.ui
FORMS += CQExpressionMmlWidget.ui
FORMS += CQFittingItemWidget.ui
FORMS += CQFittingWidget.ui
FORMS += CQFunctionsWidget.ui
FORMS += CQGlobalQuantitiesWidget.ui
FORMS += CQLyapWidget.ui
FORMS += CQMCAWidget.ui
FORMS += CQModelValue.ui
FORMS += CQOptimizationWidget.ui
FORMS += CQPlotsWidget.ui
FORMS += CQProgressDialog.ui
FORMS += CQReactionsWidget.ui
FORMS += CQReportDefinition.ui
FORMS += CQReportsWidget.ui
FORMS += CQSpeciesDetail.ui
FORMS += CQSpeciesWidget.ui
FORMS += CQTrajectoryWidget.ui
FORMS += CQTSSAWidget.ui
FORMS += CScanWidgetRandom.ui
FORMS += CScanWidgetRepeat.ui
FORMS += CScanWidgetScan.ui
FORMS += CScanWidgetTask.ui
FORMS += SensitivitiesWidget.ui
FORMS += SliderSettingsDialog.ui
FORMS += StateSubwidget.ui
FORMS += SteadyStateWidget.ui

# headers generated from .ui files
HEADERS += \
#    CMCAResultSubwidget.h \
    CQExperimentData.h \
#    CQExperimentSelection.h \
    CQFileDialogBtnGrp.h \
#    CQFittingResult.h \
#    CQFittingResultTab1.h \
#    CQMatrixDialog.h \
#    CQMoietiesTaskResult.h \
#    CQMoietiesTaskWidget.h \
##    CQOptimizationResult.h \
#    CQPreferenceDialog.h \
#    CQProgressItem.h \
    CQProgressItemBar.h \
    CQProgressItemText.h \
#    CQSplashWidget.h \
#    CQStartValueReset.h \
#    #CQTSSAResultSubWidget.h \
#    CQTaskBtnWidget.h \
#    CQTaskHeaderWidget.h \
    CQTextDialog.h \
#    CUpDownSubwidget.h \
#    DefaultplotDialog.h \
#    TimeSeriesSubwidget.h \
#    objectdebug.h \


# sources generated from .ui files
SOURCES += \
 #   CMCAResultSubwidget.cpp \
    CQExperimentData.cpp \
 #   CQExperimentSelection.cpp \
    CQFileDialogBtnGrp.cpp \
 #   CQFittingResult.cpp \
 #   CQFittingResultTab1.cpp \
 #   CQMatrixDialog.cpp \
 #   CQMoietiesTaskResult.cpp \
 #   CQMoietiesTaskWidget.cpp \
 #   #CQOptimizationResult.cpp \
 #   CQPreferenceDialog.cpp \
 #   CQProgressItem.cpp \
    CQProgressItemBar.cpp \
    CQProgressItemText.cpp \
 #   CQSplashWidget.cpp \
 #   CQStartValueReset.cpp \
 #   #CQTSSAResultSubWidget.cpp \
 #   CQTaskBtnWidget.cpp \
 #   CQTaskHeaderWidget.cpp \
    CQTextDialog.cpp \
 #   CUpDownSubwidget.cpp \
 #   DefaultplotDialog.cpp \
 #   TimeSeriesSubwidget.cpp \
 #   objectdebug.cpp \


!contains(DEFINES, HAVE_MML) {
  HEADERS -= CQDifferentialEquations.h
#  HEADERS -= CQDifferentialEquations.ui.h
  SOURCES -= CQDifferentialEquations.cpp
}


#ifdef COPASI_LICENSE_COM
contains(USE_LICENSE, COM) {
# FORMS3 += CQRegistrationDialog.ui

  HEADERS += CQRegistrationDialog.h \
             CQRegistrationDialog.ui.h

  SOURCES += CQRegistrationDialog.cpp
}
#endif // COPAS_LICENSE_COM

DISTFILES += UI.vcproj \
             icons/Copasi.ico \
             icons/Copasi??-Alpha.xpm \
             icons/CopasiDoc.ico \
             icons/CopasiDoc??-Alpha.xpm \
             icons/closeSlider.xpm \
             icons/checkModel.xpm \
             icons/copasi_beta_background.xpm \
             icons/copasi_rc.xpm \
             icons/editSlider.xpm \
             icons/filenew.xpm \
             icons/fileopen.xpm \
             icons/fileprint.xpm \
             icons/filesave.xpm \
             icons/istos.xpm \
             icons/locked.xpm \
             icons/modifier.xpm \
             icons/objectAll.xpm \
             icons/objectNone.xpm \
             icons/objectParts.xpm \
             icons/product.xpm \
             icons/scanwidgetbuttonicon.xpm \
             icons/showSliders.xpm \
             icons/stois.xpm \
             icons/substrate.xpm \
             icons/unlocked.xpm \
             icons/MIRIAM.xpm \
             icons/saveIcon.xpm \
             icons/edit_Icon.xpm \
             icons/photo.xpm
             



#The following line was inserted by qt3to4
QT += xml  opengl qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

