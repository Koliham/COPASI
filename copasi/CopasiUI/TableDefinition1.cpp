/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/TableDefinition1.cpp,v $
   $Revision: 1.38 $
   $Name:  $
   $Author: ssahle $ 
   $Date: 2004/06/28 15:24:57 $
   End CVS Header */

/****************************************************************************
 ** Form implementation generated from reading ui file '.\TableDefinition1.ui'
 **
 ** Created: Wed Aug 6 22:43:06 2003
 **      by: The User Interface Compiler ($Id: TableDefinition1.cpp,v 1.38 2004/06/28 15:24:57 ssahle Exp $)
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qframe.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qstring.h>
#include <qmessagebox.h>

#include "TableDefinition1.h"
#include "ObjectBrowser.h"
#include "listviews.h"
#include "report/CKeyFactory.h"
#include "report/CReportDefinition.h"
#include "report/CCopasiObjectName.h"
#include "report/CCopasiContainer.h"
#include "report/CCopasiStaticString.h"
#include "report/CReport.h"
#include "ScanWidget.h"
#include "qtUtilities.h"

#include "./icons/scanwidgetbuttonicon.xpm"

/*
 *  Constructs a TableDefinition1 as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
TableDefinition1::TableDefinition1(QWidget* parent, const char* name, WFlags fl)
    : CopasiWidget(parent, name, fl),
    pParent(parent)
{
  QPixmap image0((const char**) image0_data);
  QPixmap image1((const char**) image1_data);
  QPixmap image2((const char**) image2_data);
  QPixmap image3((const char**) image3_data);

  if (!name)
    setName("TableDefinition");
  TableDefinitionLayout = new QGridLayout(this, 1, 1, 11, 6, "TableDefinitionLayout");

  bodyField = new QFrame(this, "bodyField");
  bodyField->setFrameShape(QFrame::HLine);
  bodyField->setFrameShadow(QFrame::Sunken);
  bodyField->setFrameShape(QFrame::HLine);

  TableDefinitionLayout->addMultiCellWidget(bodyField, 2, 2, 0, 1);

  layout14 = new QHBoxLayout(0, 0, 6, "layout14");

  confirmButton = new QPushButton(this, "confirmButton");
  layout14->addWidget(confirmButton);

  cancelButton = new QPushButton(this, "cancelButton");
  layout14->addWidget(cancelButton);

  TableDefinitionLayout->addMultiCellLayout(layout14, 3, 3, 0, 1);

  layout7 = new QHBoxLayout(0, 0, 6, "layout7");

  layout6 = new QVBoxLayout(0, 0, 6, "layout6");

  itemsLabel = new QLabel(this, "itemsLabel");
  layout6->addWidget(itemsLabel);

  layout5 = new QGridLayout(0, 1, 1, 0, 6, "layout5");

  upButton = new QPushButton(this, "upButton");
  upButton->setText(trUtf8(""));
  upButton->setPixmap(image3);
  layout5->addWidget(upButton, 1, 0);

  deleteButton = new QPushButton(this, "deleteButton");
  deleteButton->setText(trUtf8(""));
  deleteButton->setPixmap(image0);
  layout5->addWidget(deleteButton, 0, 1);

  downButton = new QPushButton(this, "downButton");
  downButton->setText(trUtf8(""));
  downButton->setPixmap(image1);
  layout5->addWidget(downButton, 1, 1);

  addButton = new QPushButton(this, "addButton");
  addButton->setText(trUtf8(""));
  addButton->setPixmap(image2);
  layout5->addWidget(addButton, 0, 0);

  layout6->addLayout(layout5);
  QSpacerItem* spacer = new QSpacerItem(90, 262, QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout6->addItem(spacer);
  layout7->addLayout(layout6);

  itemsTable = new QListBox(this);
  layout7->addWidget(itemsTable);

  TableDefinitionLayout->addMultiCellLayout(layout7, 1, 1, 0, 1);

  frame4 = new QFrame(this, "frame4");
  frame4->setFrameShape(QFrame::Box);
  frame4->setFrameShadow(QFrame::Sunken);
  frame4->setLineWidth(1);
  frame4Layout = new QGridLayout(frame4, 1, 1, 11, 6, "frame4Layout");

  tabChecked = new QCheckBox(frame4, "tabChecked");

  frame4Layout->addWidget(tabChecked, 3, 1);

  separatorEdit = new QLineEdit(frame4, "separatorEdit");

  frame4Layout->addWidget(separatorEdit, 2, 1);

  separatorLabel = new QLabel(frame4, "separatorLabel");

  frame4Layout->addWidget(separatorLabel, 2, 0);

  comboTask = new QComboBox(FALSE, frame4, "comboTask");
  comboTask->insertItem("Scan Task");
  comboTask->insertItem("Trajectory Task");
  comboTask->insertItem("SteadyState Task");

  frame4Layout->addWidget(comboTask, 0, 1);

  line4 = new QFrame(frame4, "line4");
  line4->setFrameShape(QFrame::HLine);
  line4->setFrameShadow(QFrame::Sunken);
  line4->setFrameShape(QFrame::HLine);

  frame4Layout->addMultiCellWidget(line4, 1, 1, 0, 1);

  taskLabel = new QLabel(frame4, "taskLabel");

  frame4Layout->addWidget(taskLabel, 0, 0);

  TableDefinitionLayout->addWidget(frame4, 0, 1);

  frame5 = new QFrame(this, "frame5");
  frame5->setFrameShape(QFrame::Box);
  frame5->setFrameShadow(QFrame::Sunken);
  frame5Layout = new QGridLayout(frame5, 1, 1, 11, 6, "frame5Layout");

  commentEdit = new QLineEdit(frame5, "commentEdit");
  commentEdit->setFrameShape(QLineEdit::LineEditPanel);
  commentEdit->setFrameShadow(QLineEdit::Sunken);
  commentEdit->setLineWidth(2);

  frame5Layout->addWidget(commentEdit, 1, 1);

  commentLabel = new QLabel(frame5, "commentLabel");

  frame5Layout->addWidget(commentLabel, 1, 0);

  targetLabel = new QLabel(frame5, "targetLabel");

  frame5Layout->addWidget(targetLabel, 0, 0);

  nameEdit = new QLineEdit(frame5, "nameEdit");
  nameEdit->setFrameShape(QLineEdit::LineEditPanel);
  nameEdit->setFrameShadow(QLineEdit::Sunken);

  frame5Layout->addWidget(nameEdit, 0, 1);

  titleChecked = new QCheckBox(frame5, "titleChecked");

  frame5Layout->addWidget(titleChecked, 2, 1);

  TableDefinitionLayout->addWidget(frame5, 0, 0);
  languageChange();
  clearWState(WState_Polished);

  // tab order
  setTabOrder(nameEdit, commentEdit);
  setTabOrder(commentEdit, titleChecked);
  setTabOrder(titleChecked, comboTask);
  setTabOrder(comboTask, separatorEdit);
  setTabOrder(separatorEdit, tabChecked);
  setTabOrder(tabChecked, addButton);
  setTabOrder(addButton, deleteButton);
  setTabOrder(deleteButton, upButton);
  setTabOrder(upButton, downButton);
  setTabOrder(downButton, itemsTable);
  setTabOrder(itemsTable, confirmButton);
  setTabOrder(confirmButton, cancelButton);

  //connect(nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameTextChanged(const QString&)));
  connect(commentEdit, SIGNAL(textChanged(const QString&)), this, SLOT(commentTextChanged(const QString&)));
  connect(separatorEdit, SIGNAL(textChanged(const QString&)), this, SLOT(separatorTextChanged(const QString&)));

  connect(tabChecked, SIGNAL(clicked()), this, SLOT(tabButtonClicked()));
  connect(titleChecked, SIGNAL(clicked()), this, SLOT(titleButtonClicked()));

  connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
  connect(upButton, SIGNAL(clicked()), this, SLOT(upButtonClicked()));
  connect(downButton, SIGNAL(clicked()), this, SLOT(downButtonClicked()));

  connect(confirmButton, SIGNAL(clicked()), this, SLOT(slotBtnConfirmClicked()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotBtnCancelClicked()));

  connect(comboTask, SIGNAL(activated(const QString &)), this, SLOT(comboTaskChanged(const QString &)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
TableDefinition1::~TableDefinition1()
{}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void TableDefinition1::languageChange()
{
  setCaption(tr("Reports"));
  confirmButton->setText(tr("Commit"));
  cancelButton->setText(tr("Revert"));
  itemsLabel->setText(tr("Items"));
  upButton->setText(QString::null);
  downButton->setText(QString::null);
  deleteButton->setText(QString::null);
  addButton->setText(QString::null);
  tabChecked->setText(tr("Tab"));
  separatorLabel->setText(tr("Separator"));
  taskLabel->setText(tr("Task"));
  commentLabel->setText(tr("Comment"));
  targetLabel->setText(tr("ReportDefinition"));
  titleChecked->setText(tr("Title"));
}

/*This function is to load the model for the table*/
void TableDefinition1::loadTableDefinition1()
{
  CReportDefinition* pReportDefinition =
    dynamic_cast< CReportDefinition * >(GlobalKeys.get(reportKey));

  itemsTable->clear();

  nameEdit->setText(FROM_UTF8(pReportDefinition->getObjectName()));
  commentEdit->setText(FROM_UTF8(pReportDefinition->getComment()));
  titleChecked->setChecked(pReportDefinition->getTitle());

  unsigned C_INT32 i;
  // i+=2; is due to skip to show the separator
  for (i = 0; i < pReportDefinition->getBodyAddr()->size(); i += 2)
    {
      itemsTable->insertItem(FROM_UTF8((*(pReportDefinition->getBodyAddr()))[i]));
    }
  comboTask->setEnabled(true);

  if (pReportDefinition->getSeparator().getStaticString() == "\t")
    {
      separatorEdit->setEnabled(false);
      tabChecked->setChecked(true);
    }
  else
    {
      separatorEdit->setEnabled(true);
      tabChecked->setChecked(false);
      separatorEdit->setText(FROM_UTF8(pReportDefinition->getSeparator().getStaticString()));
    }
  bUpdated = false;
}

void TableDefinition1::slotBtnCancelClicked()
{
  loadTableDefinition1();
}

void TableDefinition1::slotBtnConfirmClicked()
{
  CReportDefinition* pReportDefinition =
    dynamic_cast< CReportDefinition * >(GlobalKeys.get(reportKey));

  if (!pReportDefinition) return;

  if (FROM_UTF8(pReportDefinition->getObjectName()) != (const char*)nameEdit->text().utf8())
    {
      pReportDefinition->setObjectName((const char*)nameEdit->text().utf8());
      ListViews::notify(ListViews::REPORT, ListViews::RENAME, reportKey);
    }

  if (!bUpdated) return;

  pReportDefinition->setComment((const char*)commentEdit->text().utf8());
  pReportDefinition->getHeaderAddr()->clear();
  pReportDefinition->getBodyAddr()->clear();
  pReportDefinition->setTitle(titleChecked->isChecked());

  CCopasiStaticString Separator;
  if (tabChecked->isChecked())
    Separator = "\t";
  else
    Separator = (const char *)separatorEdit->text().utf8();
  pReportDefinition->setSeparator(Separator);

  CCopasiObjectName SeparatorCN(Separator.getCN());
  CCopasiObjectName Title;
  std::vector< CCopasiContainer * > ListOfContainer;
  CCopasiObject* pSelectedObject;
  C_INT32 i;
  for (i = 0; i < itemsTable->numRows(); i++)
    {
      pSelectedObject =
        CCopasiContainer::ObjectFromName(ListOfContainer,
                                         CCopasiObjectName((const char *)itemsTable->text(i).utf8()));
      if (pSelectedObject)
        {
          if (pSelectedObject->getObjectParent())
            {
              Title =
                pSelectedObject->getObjectParent()->getCN();
              Title += ",Reference=Name";
              pReportDefinition->getHeaderAddr()->push_back(Title);

              Title =
                CCopasiStaticString("[" + pSelectedObject->getObjectName() + "]").getCN();
            }
          else
            Title =
              CCopasiStaticString(pSelectedObject->getObjectName()).getCN();

          pReportDefinition->getHeaderAddr()->push_back(Title);
          pReportDefinition->getBodyAddr()->push_back(pSelectedObject->getCN());

          pReportDefinition->getHeaderAddr()->push_back(SeparatorCN);
          pReportDefinition->getBodyAddr()->push_back(SeparatorCN);
        }
    }

  if (pReportDefinition->getHeaderAddr()->size())
    {
      pReportDefinition->getHeaderAddr()->pop_back();
      pReportDefinition->getBodyAddr()->pop_back();
    }

  ListViews::notify(ListViews::REPORT, ListViews::CHANGE, reportKey);
  bUpdated = false;
}

//void TableDefinition1::nameTextChanged(const QString &)
//{}

void TableDefinition1::commentTextChanged(const QString &)
{bUpdated = true;}

void TableDefinition1::separatorTextChanged(const QString &)
{bUpdated = true;}

void TableDefinition1::comboTaskChanged(const QString & C_UNUSED(string))
{bUpdated = true;}

void TableDefinition1::titleButtonClicked()
{bUpdated = true;}

void TableDefinition1::tabButtonClicked()
{
  bUpdated = true;
  if (tabChecked->isChecked())
    {
      separatorEdit->setText("");
    }
  separatorEdit->setEnabled(!tabChecked->isChecked());
}

void TableDefinition1::addButtonClicked()
{
  ObjectBrowser* pSelectedObjects = new ObjectBrowser();
  std::vector<CCopasiObject*>* pSelectedVector = new std::vector<CCopasiObject*>();
  pSelectedObjects->setOutputVector(pSelectedVector);

  if (pSelectedObjects->exec () == QDialog::Rejected)
    {
      pdelete(pSelectedVector);
      return;
    }

  if (pSelectedVector->size() == 0)
    {
      pdelete(pSelectedVector);
      return;
    }

  unsigned C_INT32 i = 0;
  for (; i < pSelectedVector->size(); i++)
    if ((*pSelectedVector)[i])
      if (itemsTable->findItem(FROM_UTF8((*pSelectedVector)[i]->getCN())) == NULL)
        {
          itemsTable->insertItem(FROM_UTF8((*pSelectedVector)[i]->getCN()));
          //      selectedList.push_back((*pSelectedVector)[i]);
          bUpdated = true;
        }

  pdelete(pSelectedVector);
}

void TableDefinition1::deleteButtonClicked()
{
  QListBoxItem* selectedItem = itemsTable->selectedItem ();
  UINT32 selectedIndex = itemsTable->index(selectedItem);
  if (selectedItem)
    {
      itemsTable->removeItem(selectedIndex);
      bUpdated = true;
    }
}

void TableDefinition1::upButtonClicked()
{
  QListBoxItem* selectedItem = itemsTable->selectedItem ();
  UINT32 selectedIndex = itemsTable->index(selectedItem);
  if ((selectedItem) && (selectedIndex != 0))
    {
      //swap in selectedList
      //      CCopasiObject* pDownObject = selectedList[selectedIndex];
      // check for valid of the update object pointer array
      // QString pDownItemStr1(pDownObject->getObjectUniqueName().);
      //     CCopasiObject* pUpperObject = selectedList[selectedIndex - 1];
      //      selectedList[selectedIndex] = pUpperObject;
      //      selectedList[selectedIndex - 1] = pDownObject;

      //swap in ListBox
      QString pDownItemStr(itemsTable->item(selectedIndex)->text());
      QString pUpperItemStr(itemsTable->item(selectedIndex - 1)->text());
      itemsTable->changeItem (pUpperItemStr, selectedIndex);
      itemsTable->changeItem (pDownItemStr, selectedIndex - 1);
      bUpdated = true;
    }
}

void TableDefinition1::downButtonClicked()
{
  QListBoxItem* selectedItem = itemsTable->selectedItem ();
  UINT32 selectedIndex = itemsTable->index(selectedItem);
  if ((selectedItem) && (itemsTable->item(selectedIndex + 1)))
    {
      //swap in selectedList
      //      CCopasiObject* pDownObject = selectedList[selectedIndex + 1];
      // check for valid of the update object pointer array
      // QString pDownItemStr1(pDownObject->getObjectUniqueName().);
      //      CCopasiObject* pUpperObject = selectedList[selectedIndex];
      //      selectedList[selectedIndex + 1] = pUpperObject;
      //      selectedList[selectedIndex] = pDownObject;

      //swap in ListBox
      QString pDownItemStr(itemsTable->item(selectedIndex + 1)->text());
      QString pUpperItemStr(itemsTable->item(selectedIndex)->text());
      itemsTable->changeItem (pDownItemStr, selectedIndex);
      itemsTable->changeItem (pUpperItemStr, selectedIndex + 1);
      bUpdated = true;
    }
}

bool TableDefinition1::enter(const std::string & key)
{
  if (!dynamic_cast< CReportDefinition * >(GlobalKeys.get(key))) return false;

  reportKey = key;

  loadTableDefinition1();

  return true;
}

bool TableDefinition1::leave()
{
  slotBtnConfirmClicked();
  return true;
}

bool TableDefinition1::update(ListViews::ObjectType objectType, ListViews::Action C_UNUSED(action), const std::string & C_UNUSED(key))
{
  switch (objectType)
    {
    case ListViews::MODEL:
      break;

    default:
      break;
    }
  return true;
}
