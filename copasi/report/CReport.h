/****************************************************************************
 * 
 * Created: Tue Aug 11 2003
 *      by: Liang Xu
 *
 *  Created for Copasi by Liang Xu
 ****************************************************************************/

#ifndef COPASI_CReport
#define COPASI_CReport

#include "CCopasiObject.h"
#include <string>
#include <vector>

class CReportDefinition;
class CBody;
class CReportTable;

class CReport : public CCopasiObject
  {
  private:
    CReportDefinition *mpReportDef;
    const std::string mTarget;
    bool mAppend;
    std::string mKey;

  public:
    /**
        *  Default constructor.
        */
    CReport();

    /**
    cleanup
    */
    void cleanup();

    /**
       *  Destructor.
       */
    ~CReport();

    inline void setReportDefinintion(CReportDefinition* pNewReportDefinition)
    {mpReportDef = pNewReportDefinition;}

    /**
    returns the reference of the Report Tag
    */
    CReportDefinition* getDefReference();

    /**
    sets the reference to the report
    */
    void setDefReference(CReportDefinition *reportDef);

    /**
    returns the target of the Report Tag
    */
    const std::string getTarget();

    /**
    sets the reference to the report
    */
    void setTarget(const std::string *target);

    /**
    Returns whether the Report Tag is appended or not
    */
    bool append();

    /**
       sets the append attribute if the report tag
    */
    void setAppend(bool append);

    /**
    to print header
    */
    void printHeader();

    /**
    to print body
    */
    void printBody();

    /**
       to print footer
       */
    void printFooter();
  };

#endif
