######################################################################
# $Revision: 1.67 $ $Author: shoops $ $Date: 2004/11/19 21:26:29 $  
######################################################################

include(../common.pri)

TEMPLATE = app

DEPENDPATH += .. 
INCLUDEPATH += ..


contains(BUILD_OS, WIN32) {
  COPASI_LIBS += ../lib/commandline.lib \
          ../lib/copasiXML.lib \
          ../lib/elementaryFluxModes.lib \
          ../lib/function.lib \
          ../lib/mathmodel.lib \
          ../lib/model.lib \
          ../lib/optimization.lib \
          ../lib/randomGenerator.lib \
          ../lib/report.lib \
          ../lib/sbmlimport.lib \
          ../lib/scan.lib \
          ../lib/steadystate.lib \
          ../lib/trajectory.lib \
          ../lib/utilities.lib \
          ../lib/plot.lib \
          ../lib/wizard.lib

  LIBS += $$COPASI_LIBS
  LIBS += $(QTDIR)/lib/qwt.lib
  LIBS += libsbml.lib
  
  TARGETDEPS += $$COPASI_LIBS

  release {
    distribution.extra = bash ../../admin/mkbuild.sh $${BUILD_OS}
  }
} else {
  # The order of objects is important 
  OBJECTS +=  ../lib/libcommandline.a \
              ../lib/libelementaryFluxModes.a \
              ../lib/libcopasiXML.a \
              ../lib/libmathmodel.a \
              ../lib/libmodel.a \
              ../lib/libfunction.a \
              ../lib/libreport.a \
              ../lib/liboptimization.a \
              ../lib/libsbmlimport.a \
              ../lib/libscan.a \
              ../lib/libsteadystate.a \
              ../lib/libtrajectory.a \
              ../lib/librandomGenerator.a \
              ../lib/libplot.a \
              ../lib/libutilities.a \
              ../lib/libreport.a \
              ../lib/libfunction.a \
              ../lib/libmodel.a \
              ../lib/libwizard.a

  LIBS += -lqwt \
          -lsbml 
  
  release {
    distribution.extra = ../../admin/mkbuild.sh $${BUILD_OS}
  }

  TARGETDEPS += ../lib/libcommandline.a \
                ../lib/libcopasiXML.a \
                ../lib/libelementaryFluxModes.a \
                ../lib/libfunction.a \
                ../lib/libmathmodel.a \
                ../lib/libmodel.a \
                ../lib/liboptimization.a \
#                ../lib/liboutput.a \
                ../lib/librandomGenerator.a \
                ../lib/libreport.a \
                ../lib/libsbmlimport.a \
                ../lib/libscan.a \
                ../lib/libsteadystate.a \
                ../lib/libtrajectory.a \
                ../lib/libutilities.a \
                ../lib/libplot.a \
                ../lib/libwizard.a
}

contains(BUILD_OS, Linux) {   
  LIBS += -Wl,-lqt-mt \
          -Wl,-lXcursor \
          -Wl,-lXft \
          -Wl,-lfontconfig \
          -Wl,-lpthread
}

release {
  contains(BUILD_OS, SunOS) {
    LIBS += -Wl,-lqt-mt \
            -Wl,-lpthread
  }
}

contains(BUILD_OS, SunOS) {
  LIBS += -lICE -ldl
}  

# Input
HEADERS += \
           AboutDialog.h \
           ChemEqValidator.h \
           CMCAWidget.h \
           CMCAResultSubwidget.h \
           CMCAResultWidget.h \
           CReactionInterface.h \
           CReportDefinitionSelect.h \
           CompartmentSymbols.h \
           CompartmentsWidget.h \
           CompartmentsWidget1.h \
           ConstantSymbols.h \
           CopasiDefaultWidget.h \
           CopasiSlider.h \
           CopasiTableWidget.h \
           copasiWidget.h \
           copasiui3window.h \
           CPlotSelectionDialog.h \
           CProgressBar.h \
           CTimeSeriesTable.h \
           DataModel.h \
           DataModelGUI.h \
           DifferentialEquations.h \
           FixedMetaboliteSymbols.h \
           FunctionItemWidget.h \
           FunctionSymbols.h \
           FunctionWidget.h \
           FunctionWidget1.h \
           MetaboliteSymbols.h \
           MetabolitesWidget.h \
           MetabolitesWidget1.h \
           ModelWidget.h \
           ModesWidget.h \
           MoietyWidget.h \
           MoietyWidget1.h \
           MyLineEdit.h \
           listviews.h \
#           ObjectBrowser.h \
           ObjectBrowserDialog.h \
           ObjectBrowserItem.h \
           ObjectBrowserWidget.h \
           objectdebug.h \
           OptimizationItemWidget.h \
           OptimizationWidget.h \
           ParamFittingItemWidget.h \
           ParamFittingWidget.h \
           parametertable.h \
           PlotWidget.h \
           qtUtilities.h \
           ReactionsWidget.h \
           ReactionsWidget1.h \
           ScanItemWidget.h \
           ScanWidget.h \
           SliderDialog.h \
           SimpleSelectionDialog.h \
           SimpleSelectionWidget.h \
           SliderSettingsDialog.h \
           StateWidget.h \
           StateSubwidget.h \
           SteadyStateWidget.h \
           TableDefinition.h \
           TableDefinition1.h \
           TimeSeriesWidget.h \
           TimeSeriesSubwidget.h \
           TrajectoryWidget.h \
           Tree.h

# INTERFACES += CompartmentsWidget.ui \
#               FunctionWidget1.ui \
#               MetabolitesWidget1.ui \
#               MoietyWidget1.ui \
#               ObjectBrowser.ui \
#               objectdebug.ui \
#               ReactionsWidget1.ui \
#               SteadyStateWidget.ui \
#               TrajectoryWidget.ui

SOURCES += \
           AboutDialog.cpp \
           CMCAWidget.cpp \
           CMCAResultSubwidget.cpp \
           CMCAResultWidget.cpp \
           CReactionInterface.cpp \
           CReportDefinitionSelect.cpp \
           CompartmentSymbols.cpp \
           CompartmentsWidget.cpp \
           CompartmentsWidget1.cpp \
           ConstantSymbols.cpp \
           CopasiDefaultWidget.cpp \
           CopasiSlider.cpp \
           CopasiTableWidget.cpp \
           copasiWidget.cpp \
           copasiui3window.cpp \
           CPlotSelectionDialog.cpp \
           CProgressBar.cpp \
           CTimeSeriesTable.cpp \
           DataModel.cpp \
           DataModelGUI.cpp \
           DifferentialEquations.cpp \
           FixedMetaboliteSymbols.cpp \
           FunctionItemWidget.cpp \
           FunctionSymbols.cpp \
           FunctionWidget.cpp \
           FunctionWidget1.cpp \
           listviews.cpp \
           main.cpp \
           MetaboliteSymbols.cpp \
           MetabolitesWidget.cpp \
           MetabolitesWidget1.cpp \
           ModelWidget.cpp \
           ModesWidget.cpp \
           MoietyWidget.cpp \
           MoietyWidget1.cpp \
           MyLineEdit.cpp \
#           ObjectBrowser.cpp \
           ObjectBrowserDialog.cpp \
           ObjectBrowserItem.cpp \
           ObjectBrowserWidget.cpp \
           objectdebug.cpp \
           OptimizationItemWidget.cpp \
           OptimizationWidget.cpp \
           ParamFittingItemWidget.cpp \
           ParamFittingWidget.cpp \
           parametertable.cpp \
           PlotWidget.cpp \
           qtUtilities.cpp \
           ReactionsWidget.cpp \
           ReactionsWidget1.cpp \
           ScanItemWidget.cpp \
           ScanWidget.cpp \
           SliderDialog.cpp \
           SimpleSelectionDialog.cpp \
      	   SimpleSelectionWidget.cpp \
           SliderSettingsDialog.cpp \	
           StateWidget.cpp \
           StateSubwidget.cpp \
           SteadyStateWidget.cpp \
           TableDefinition.cpp \
           TableDefinition1.cpp \
           TimeSeriesWidget.cpp \
           TimeSeriesSubwidget.cpp \
           TrajectoryWidget.cpp \
           Tree.cpp


# FORMS +=  TimeSeriesSubwidget.ui
# FORMS +=  StateSubwidget.ui
# FORMS +=  CMCAResultSubwidget.ui
# FORMS +=  SimpleSelectionWidget.ui        
# FORMS +=  SliderSettingsDialog.ui           

release {
  distribution.path = .
  distribution.file = CopasiUI

  INSTALLS += distribution
}
