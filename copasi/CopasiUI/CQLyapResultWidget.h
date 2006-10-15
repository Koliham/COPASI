/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/CQLyapResultWidget.h,v $
   $Revision: 1.3 $
   $Name:  $
   $Author: gauges $
   $Date: 2006/10/15 06:57:33 $
   End CVS Header */

// Copyright � 2005 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef CQLYAPRESULT_H
#define CQLYAPRESULT_H

#include "CopasiUI/copasiWidget.h"
//#include "CopasiDataModel/CCopasiDataModel.h"

class QGridLayout;
class QLineEdit;
class QLabel;
class QTable;

class CQLyapResultWidget : public CopasiWidget
  {
    Q_OBJECT

  public:
    CQLyapResultWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~CQLyapResultWidget();

    virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);
    virtual bool leave();
    virtual bool enter(const std::string & key = "");

  protected slots:

  protected:
    bool loadFromBackend();

    QGridLayout* mWidgetLayout;

    QLabel* mLabelExponents;
    QLabel* mLabelSum;
    QLabel* mLabelDivergence;
    QLabel* mLabelComment;

    QTable* mTableExponents;
    QLineEdit* mLineEditSum;
    QLineEdit* mLineEditDivergence;
  };

#endif
