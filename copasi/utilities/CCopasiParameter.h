// Copyright (C) 2010 - 2013 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., University of Heidelberg, and The University 
// of Manchester. 
// All rights reserved. 

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

// Copyright (C) 2003 - 2007 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc. and EML Research, gGmbH. 
// All rights reserved. 

#ifndef COPASI_CCopasiParameter
#define COPASI_CCopasiParameter

#include <string>
#include <vector>

#include "copasi/report/CCopasiContainer.h"
#include "copasi/report/CCopasiObjectName.h"

class CCopasiParameterGroup;

class CReadConfig;

/**
 *  CCopasiParameter class.
 *  This class is used to describe parameters. This class is intended
 *  to be used with integration or optimization methods or reactions.
 *
 *  Created for COPASI by Stefan Hoops 2002
 */
class CCopasiParameter: public CCopasiContainer
{
  // Attributes
public:
  enum Type
  {
    DOUBLE = 0,
    UDOUBLE,
    INT,
    UINT,
    BOOL,
    GROUP,
    STRING,
    CN,
    KEY,
    FILE,
    EXPRESSION,
    INVALID
  };

  union Value
  {
    C_FLOAT64 * pDOUBLE;
    C_FLOAT64 * pUDOUBLE;
    C_INT32 * pINT;
    unsigned C_INT32 * pUINT;
    bool * pBOOL;
    std::vector< CCopasiParameter * > * pGROUP;
    std::string * pSTRING;
    CRegisteredObjectName * pCN;
    std::string * pKEY;
    std::string * pFILE;
    std::string * pEXPRESSION;
    void * pVOID;
  };

  /**
   * String literals for the GUI to display type names of parameters known
   * to COPASI.
   */
  static const std::string TypeName[];

  /**
   * XML type names of parameters known to COPASI.
   */
  static const char* XMLType[];

protected:
  /**
   * The key of the parameter.
   */
  std::string mKey;

private:
  /**
   * The type of the parameter
   */
  CCopasiParameter::Type mType;

protected:
  /**
   *  The size allocated for the value of the parameter.
   */
  size_t mSize;

  /**
   *  A pointer to the value of the parameter.
   */
  Value mValue;

  /**
   * A pointer to the object reference for  the value
   */
  mutable CCopasiObject * mpValueReference;

  // Operations

private:
  /**
   * Default constructor
   */
  CCopasiParameter();

public:
  /**
   * Copy constructor
   * @param const CCopasiParameter & src
   * @param const CCopasiContainer * pParent (default: NULL)
   */
  CCopasiParameter(const CCopasiParameter & src,
                   const CCopasiContainer * pParent = NULL);

  /**
   * Specific constructor
   * @param const string & name
   * @param const CCopasiParameter::Type & type
   * @param const void * pValue (default: NULL)
   * @param const CCopasiContainer * pParent (default: NULL)
   * @param const std::string & objectType (default: "Parameter")
   */
  CCopasiParameter(const std::string & name,
                   const Type & type,
                   const void * pValue = NULL,
                   const CCopasiContainer * pParent = NULL,
                   const std::string & objectType = "Parameter");

  /**
   * Destructor
   */
  virtual ~CCopasiParameter();

  /**
   * Assignment operator
   * @param const CCopasiParameter & rhs
   * @return CCopasiParameter & lhs
   */
  CCopasiParameter & operator = (const CCopasiParameter & rhs);

  /**
   * Return the key of this model
   * @return string key
   */
  virtual const std::string & getKey() const;

  /**
   * Set the value of the parameter
   * @param const const CType & value
   * @return bool isValidValue
   */
  template <class CType> bool setValue(const CType & value)
  {
    if (!isValidValue(value)) return false;

    *(CType *) mValue.pVOID = value;
    return true;
  }

  /**
   * Set the value of the parameter when the origin is a parameter group.
   * This function is currently not implemented and creates a fatalError
   * when called.
   * @param const CCopasiParameter::parameterGroup& value
   * @return bool false
   */
  bool setValue(const std::vector< CCopasiParameter * > & value);

  /**
   * Retrieve the private value of the parameter.
   * @return const CCopasiParameter::Value & Value
   */
  const Value & getValue() const;

  /**
   * Retrieve the private value of the parameter.
   * @return CCopasiParameter::Value & Value
   */
  Value & getValue();

  /**
   * Retrieve the object which represents the value of the parameter
   * @return CCopasiObject * pValueReference
   */
  CCopasiObject * getValueReference() const;

  /**
   * Retrieve the type of the parameter.
   * @return CCopasiParameter::Type & type
   */
  const CCopasiParameter::Type & getType() const;

  /**
   * Check whether the value corresponds to the type
   * @param const C_FLOAT64 & value
   * @return bool isValidValue
   */
  bool isValidValue(const C_FLOAT64 & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const C_INT32 & value
   * @return bool isValidValue
   */
  bool isValidValue(const C_INT32 & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const unsigned C_INT32 & value
   * @return bool isValidValue
   */
  bool isValidValue(const unsigned C_INT32 & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const bool & value
   * @return bool isValidValue
   */
  bool isValidValue(const bool & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const std::string & value
   * @return bool isValidValue
   */
  bool isValidValue(const std::string & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const CRegisteredObjectName & value
   * @return bool isValidValue
   */
  bool isValidValue(const CCopasiObjectName & value) const;

  /**
   * Check whether the value corresponds to the type
   * @param const CCopasiParameterGroup::parameterGroup & value
   * @return bool isValidValue
   */
  bool isValidValue(const std::vector< CCopasiParameter * > & value) const;

  /**
   * This is the output method for any object. The default implementation
   * provided with CCopasiObject uses the ostream operator<< of the object
   * to print the object.To override this default behavior one needs to
   * reimplement the virtual print function.
   * @param std::ostream * ostream
   */
  virtual void print(std::ostream * ostream) const;

  /**
   * Output stream operator
   * @param ostream & os
   * @param const CCopasiParameter & A
   * @return ostream & os
   */
  friend std::ostream &operator<<(std::ostream &os, const CCopasiParameter & o);

  /**
   * Comparison operator
   * @param const CCopasiParameter & lhs
   * @param const CCopasiParameter & rhs
   * @return bool equal
   */
  friend bool operator==(const CCopasiParameter & lhs, const CCopasiParameter & rhs);

  /**
   * Retrieve the CN of the math container
   * The math container provides values for the numerical values of model objects.
   * For the CN mechanism to work properly it has to pretend to be the model.
   * @return CCopasiObjectName
   */
  virtual CCopasiObjectName getCN() const;

  virtual void * getValuePointer() const;

  /**
   *  Overload display name. Special treatment for reaction parameters
   *  to provide a shorter display.
   */
  virtual std::string getObjectDisplayName(bool regular = true, bool richtext = false) const;

private:
  /**
   * Create or copy the value
   * @param const CCopasiParameter::Value & value
   * @return CCopasiParameter::Value Value
   */
  Value createValue(const Value & value);

  /**
   * Delete the value
   */
  void deleteValue();
};

#endif // COPASI_CCopasiParameter
