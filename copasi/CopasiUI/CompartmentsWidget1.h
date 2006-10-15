/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/CompartmentsWidget1.h,v $
   $Revision: 1.28 $
   $Name:  $
   $Author: gauges $
   $Date: 2006/10/15 06:57:33 $
   End CVS Header */

// Copyright � 2005 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/****************************************************************************
 **  $ CopasiUI/CompartmentsWidget1.h
 **  $ Author  : Mudita Singhal
 **
 ** This is the header file for the Compartments Widget1, i.e the second level
 ** of Compartments.
 *****************************************************************************/
#ifndef COMPARTMENTSWIDGET1_H
#define COMPARTMENTSWIDGET1_H

#include "CopasiUI/copasiWidget.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFrame;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QPushButton;

class CCompartment;

class CompartmentsWidget1 : public CopasiWidget
  {
    Q_OBJECT

  public:
    CompartmentsWidget1(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~CompartmentsWidget1();

    virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);
    virtual bool leave();
    virtual bool enter(const std::string & key = "");

  protected slots:
    virtual void slotBtnCancelClicked();
    virtual void slotBtnOKClicked();
    virtual void slotBtnNewClicked();
    virtual void slotBtnDeleteClicked();
    virtual void slotListBoxCurrentChanged(const QString &);

  protected:
    bool loadFromCompartment(const CCompartment *);
    bool saveToCompartment();
    std::string objKey;

    QString volumeSave;

    QGridLayout* CompartmentsWidget1Layout;
    QHBoxLayout* Layout5;
    QLineEdit* LineEdit4;
    QLineEdit* LineEdit1;
    QLabel* TextLabel2;
    QLabel* TextLabel2_2;
    QLabel* TextLabel1;
    QFrame* Line4;
    QFrame* Line4_2;
    QListBox* ListBox1;
    QLabel* TextLabel3;
    QLineEdit* LineEdit3;
    QPushButton* commitChanges;
    QPushButton* cancelChanges;
    QPushButton* newCompartmentBtn;
    QPushButton* deleteCompartmentBtn;
    QFrame* Line4_3;
    const CCompartment *currentCompt;
  };

#endif // COMPARTMENTSWIDGET1_H
