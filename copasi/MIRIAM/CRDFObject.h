// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/MIRIAM/CRDFObject.h,v $
//   $Revision: 1.5 $
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

#ifndef COPASI_CRDFObject
#define COPASI_CRDFObject

#include <string>

class CRDFLiteral;

class CRDFObject
  {
    // Enumerations
  public:
    /**
     * The three different types possible for objects
     */
    enum eObjectType {RESOURCE = 0, BLANK_NODE, LITERAL};

    // Operations
  public:
    /**
     * Default Constructor
     */
    CRDFObject();

    /**
     * Copy Constructor
     * @param const CRDFObject & src
     */
    CRDFObject(const CRDFObject & src);

    /**
     * Destructor
     */
    virtual ~CRDFObject();

    /**
     * Set the type of the object
     * @param const eObjectType & type
     */
    void setType(const eObjectType & type);

    /**
     * Retrieve the type of the object
     * @return const eObjectType & type
     */
    const eObjectType & getType() const;

    /**
     * Set the resource of the object
     * @param const std::string & resource
     * @param const bool & isLocal
     */
    void setResource(const std::string & resource, const bool & isLocal);

    /**
     * Retrieve the resource of the object
     * @return const std::string & resource
     */
    const std::string & getResource() const;

    /**
     * Check whehter the resource is local
     * @return const bool & isLocal
     */
    const bool & isLocal() const;

    /**
     * Set the ID of the blank node of the object
     * @param const std::string & blankNodeId
     */
    void setBlankNodeId(const std::string & blankNodeId);

    /**
     * Retrieve the ID of the blank node of the object
     * @return const std::string & blankNodeId
     */
    const std::string & getBlankNodeID() const;

    /**
     * Set the literal of the object
     * @param const std::string & lexicalData
     */
    void setLiteral(const CRDFLiteral & literal);

    /**
     * Retrieve the literal of the object
     * @return const CRDFLiteral & literal
     */
    const CRDFLiteral & getLiteral() const;

    /**
     * Reset all Data
     */
    void clearData();

    // Attributes
  private:
    /**
     * The type of the object
     */
    eObjectType mType;

    /**
     * The URI of the resource of type RESOURCE
     */
    std::string mResource;

    /**
     * Stores whether the reources is resolve locally or remotely.
     */
    bool mIsLocalResource;

    /**
     * The Id of the blank node for type BLANK_NODE
     */
    std::string mBlankNodeId;

    /**
     * The pointer to the literal for type LITERAL
     */
    mutable CRDFLiteral * mpLiteral;
  };

#endif // COPASI_CRDFObject
