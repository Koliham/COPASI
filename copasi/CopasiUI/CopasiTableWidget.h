/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/CopasiUI/Attic/CopasiTableWidget.h,v $
   $Revision: 1.1 $
   $Name:  $
   $Author: ssahle $ 
   $Date: 2004/05/19 15:54:00 $
   End CVS Header */

/****************************************************************************
 **  $ CopasiUI/CompartmentsWidget.h               
 **  $ Author  : Mudita Singhal
 **  
 ** This is the header file for the Compartments Widget, i.e the First level 
 ** of Compartments.
 *****************************************************************************/

#ifndef COPASI_TABLE_WIDGET_H
#define COPASI_TABLE_WIDGET_H

#include <qtable.h>
#include "copasi.h"
#include "copasiWidget.h"

class QPushButton;
class QGridLayout;
class QTable;
class MyTable;

class CopasiTableWidget : public CopasiWidget
  {
    Q_OBJECT

  public:
    CopasiTableWidget(QWidget *parent, const char * name = 0, WFlags f = 0);

    virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);
    virtual bool leave();
    virtual bool enter(const std::string & key = "");

  protected slots:
    virtual void slotDoubleClicked(int, int, int, const QPoint &);
    virtual void slotTableSelectionChanged();
    virtual void slotValueChanged(int, int);
    virtual void slotCurrentChanged(int, int);

    virtual void slotBtnOKClicked();
    virtual void slotBtnCancelClicked();
    virtual void slotBtnDeleteClicked();

    //void resizeEvent(QResizeEvent * re);

  protected:
    void fillTable();
    void saveTable();
    //void createNewObject();
    void updateRow(const C_INT32 row);
    QString createNewName(const QString name);
    void resizeTable(const unsigned C_INT32 numRows);

    MyTable* table;
    QPushButton* btnOK;
    QPushButton* btnCancel;
    QPushButton* btnDelete;
    std::vector<std::string> mKeys;

    C_INT32 numCols;
    std::vector<bool> mFlagChanged;
    std::vector<bool> mFlagDelete;
    std::vector<bool> mFlagNew;
    std::vector<bool> mFlagRenamed;

    bool mIgnoreUpdates;

    //These are the methods that need to be implemented by specialized widgets:

    /**
     * This initializes the widget 
     */
    virtual void init();

    /**
     * returns a list of objects that should be displayed
     */
    virtual std::vector<const CCopasiObject*> getObjects() const;

    /**
     * fills one table row with the data from one object
     */
    virtual void tableLineFromObject(const CCopasiObject* obj, unsigned C_INT32 row);

    /**
     * reads the contents of one row of the table and writes it to the object
     */
    virtual void tableLineToObject(unsigned C_INT32 row, CCopasiObject* obj);

    /**
     * creates a new object
     */
    virtual CCopasiObject* createNewObject(const std::string & name);

    /**
     * deletes objects. Performs all additional tasks, like asking the user, ...
     */
    virtual void deleteObjects(const std::vector<std::string> & keys);

    /**
     * this is used to fill a row of the table when a new object is added to the table.
     * it fills only the data columns, not the name.
     */
    virtual void defaultTableLineContent(unsigned C_INT32 row);

    /**
     * the prefix that is used to construct new object names
     */
    virtual QString defaultObjectName() const;
  };

#endif
