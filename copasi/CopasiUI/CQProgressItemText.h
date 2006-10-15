/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/CQProgressItemText.h,v $
   $Revision: 1.5 $
   $Name:  $
   $Author: gauges $
   $Date: 2006/10/15 06:57:33 $
   End CVS Header */

// Copyright � 2005 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/****************************************************************************
 ** Form interface generated from reading ui file 'CQProgressItemText.ui'
 **
 ** Created: Tue Jul 5 17:04:25 2005
 **      by: The User Interface Compiler ($Id: CQProgressItemText.h,v 1.5 2006/10/15 06:57:33 gauges Exp $)
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/

#ifndef CQPROGRESSITEMTEXT_H
#define CQPROGRESSITEMTEXT_H

#include <qvariant.h>
#include <qpixmap.h>
#include "CopasiUI/CQProgressItem.h"
#include "utilities/CVector.h"
#include "utilities/CProcessReport.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;

class CQProgressItemText : public CQProgressItem
  {
    Q_OBJECT

  public:
    CQProgressItemText(QWidget* parent = 0, const char* name = 0);
    ~CQProgressItemText();

    QLabel* mItemName;
    QLineEdit* mValue;

    virtual bool initFromProcessReportItem(CProcessReportItem * pItem);
    virtual bool process();
    virtual bool reset();

  protected:
    QHBoxLayout* mLabel;
    QSpacerItem* mSpacer;

  protected slots:
    virtual void languageChange();

  private:
    void (CQProgressItemText::*mpSetValue)();
    CCopasiParameter::Value mParameterValue;

    QPixmap image0;

    virtual void setValueFromDOUBLE();
    virtual void setValueFromINT();
    virtual void setValueFromUINT();
    virtual void setValueFromSTRING();
  };

#endif // CQPROGRESSITEMTEXT_H
