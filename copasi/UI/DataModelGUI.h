/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/DataModelGUI.h,v $
   $Revision: 1.7 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2005/02/18 16:26:50 $
   End CVS Header */

#ifndef DATAMODELGUI_H
#define DATAMODELGUI_H

#include <qobject.h>

#include "Tree.h"
#include "plot/CPlotSpec2Vector.h"

class CMathModel;

class DataModelGUI
  {
  private:
    IndexedTree mTree; // create the  object of the tree

    CMathModel * mpMathModel;
    bool mMathModelUpdateScheduled;

    QApplication *mpApp;

    void linkDataModelToGUI();

    CPlotSpec2Vector mPlotDefinitionList;

  public:
    DataModelGUI();

    void populateData();

    void updateCompartments();
    void updateMetabolites();
    void updateReactions();
    void updateMoieties();
    void updateFunctions();
    void updateReportDefinitions();
    void updatePlots();

    const IndexedNode & getRootNode() const;
    const IndexedNode & getNode(const int & id) const;

    bool loadModel(const char* fileName);
    bool createModel();
    bool saveModel(const char* fileName);

    bool importSBML(const char* fileName);
    bool exportSBML(const char* fileName);

    CPlotSpec2Vector & getPlotDefinitionList();

    CMathModel * getMathModel() {return mpMathModel;}
    bool updateMathModel();
    bool scheduleMathModelUpdate(const bool & update = true);

    void setQApp(QApplication* app);
    QApplication* getQApp() const;
  };

#endif
