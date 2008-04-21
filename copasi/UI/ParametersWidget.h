// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/ParametersWidget.h,v $
//   $Revision: 1.7.4.1 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2008/01/23 16:52:18 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <qvariant.h>
#include <qdialog.h>

#include "UI/copasiWidget.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QListView;
class QListViewItem;
class QLabel;
class CParameterListItem;

class ParametersWidget : public CopasiWidget
  {
    Q_OBJECT

  public:
    ParametersWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~ParametersWidget();

    virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);
    virtual bool leave();
    virtual bool enter(const std::string & key = "");
    virtual void setFramework(int framework);

  protected:
    bool loadFromModel();
    bool saveToModel();

    std::string objKey;

    QPushButton* commitButton;
    QPushButton* revertButton;
    QListView* listView;
    QLabel* labelTitle;

    QGridLayout* ParametersWidgetLayout;
    QSpacerItem* spacer2;
    QVBoxLayout* layoutLeft;
    QSpacerItem* spacer1;

    CParameterListItem* mTimeItem;
    CParameterListItem* mCompItem;
    CParameterListItem* mReacItem;
    CParameterListItem* mMetabItem;
    CParameterListItem* mParamItem;

  protected slots:
    virtual void commitPressed();
    virtual void revertPressed();

    virtual void editItem(QListViewItem * item, const QPoint & pnt, int c);
    virtual void editItem(QListViewItem * item);
  };

#endif // PARAMETERSWIDGET_H
