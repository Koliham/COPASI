// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/MIRIAM/CModelMIRIAMInfo.h,v $
//   $Revision: 1.4 $
//   $Name:  $
//   $Author: aekamal $
//   $Date: 2008/01/29 15:43:44 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/**
 *  CModelMIRIAMInfo: Stores all MIRIAM info for a Model.
 *
 */

#ifndef COPASI_CMODELMIRIAMINFO
#define COPASI_CMODELMIRIAMINFO

#include "utilities/CCopasiVector.h"

#include "CAuthor.h"
#include "CRDFGraph.h"

class CModelMIRIAMInfo
  {
    // Attributes
  private:
    CCopasiVector <CAuthor> mAuthors;
    std::string mAuthorsGraphID;
    CRDFGraph * mpRDFGraph;

    bool fillInfoFromGraph();

    // Operations
  public:
    CModelMIRIAMInfo();
    ~CModelMIRIAMInfo();
    void loadGraph(const std::string& key);
    CRDFGraph * getRDFGraph();
    CCopasiVector <CAuthor> & getAuthors();
    CAuthor* createAuthor(const std::string& objectName);
    bool removeAuthor(const std::string& key);
  };

#endif //COPASI_CMODELMIRIAMINFO
