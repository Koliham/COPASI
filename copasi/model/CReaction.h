/**
 *  CReaction class.
 *  Derived from Gepasi's cstep.cpp. (C) Pedro Mendes 1995-2000.
 *
 *  Converted for Copasi by Stefan Hoops 2001
 */

#ifndef COPASI_CReaction
#define COPASI_CReaction

#include <string>
#include <vector>
#include <utility>


#include "utilities/utilities.h"
#include "function/function.h"
#include "CMetab.h"
#include "CChemEq.h"
#include "CChemEqElement.h"

class CReaction
{
  
  // Attributes
 private:
  class CId2Metab
    {
      friend class CReaction;
      // Attributes
    private:
      /**
       *  The name of the identifier as defined by the called function
       */
      string mIdentifierName;

      /**
       *  The name of the metabolite
       */
      string mMetaboliteName;

      /**
       *  The name of the compartment the metabolite is located in
       */
      string mCompartmentName;

      /**
       *  A pointer to the metabolite
       */
      CMetab *mpMetabolite;

    public:
      // Operations
      /**
       *  Default Constructor
       */
      CId2Metab();

      /**
       *  Destructor
       */
      ~CId2Metab();
    };

  class CId2Param
    {
      friend class CReaction;
      // Attributes
    private:
      /**
       *  The name of the parameter as defined by the called function
       */
      string mIdentifierName;

      /**
       *  The value of the parameter
       */
      C_FLOAT64 mValue;

    public:
      // Operations
      /**
       *  Default Constructor
       */
      CId2Param();

      /**
       *  Destructor
       */
      ~CId2Param();
      /**
       *	Return Identifier Name		Wei Sun		
       */
      string getIdentifierName() const;

      /**
       *	Retun the value of the pararmeter	
       */
      C_FLOAT64 getValue() const;

      /**
       *	Returns the address of mValue
       */
      void * getValueAddr();

    };

  /**
   *  The name of the reaction
   */
  string mName;

  /**
   *  The chemical equation
   */
  CChemEq mChemEq;

  /**
   *  A pointer to the rate function of the reaction
   */
  CBaseFunction * mFunction;

  /**
   *  The flux of the reaction
   */
  C_FLOAT64 mFlux;

  /**
   *  The reversibility of the reaction
   */
  C_INT16 mReversible;

  /**
   *  A vector of links between the substrates of the reaction 
   *  and function parameters
   */
  vector < CId2Metab > * mId2Substrates;

  /**
   *  A vector of links between the products of the reaction 
   *  and function parameters
   */
  vector < CId2Metab > * mId2Products;

  /**
   *  A vector of links between the modifiers of the reaction 
   *  and function parameters
   */
  vector < CId2Metab > * mId2Modifiers;

  /**
   *  A vector of links between the kinetic parameters of the reaction 
   *  and function parameters
   */
  vector < CId2Param > * mId2Parameters;

  /**
   *  A pointer to the  call parameters of the rate function of the reaction
   */
  vector < CCallParameter > * mCallParameters;

  /**
   *
   */
  C_INT32 mFail;

  // Operations
 public:
  /**
   *  Default constructor
   */
  CReaction();

  /**
   *  Named constructor
   *  @param "const string &" name
   */
  CReaction(const string & name);

  /**
   *  Init
   */
  void initialize();
    
  /**
   *  Destructor
   */
  ~CReaction();

  /**
   *  Delete
   */
  void cleanup();
    
  /**
   *  Assignment operator
   */
  CReaction & operator=(const CReaction & rhs);

  /**
   *  Loads an object with data coming from a CReadConfig object.
   *  (CReadConfig object reads an input stream)
   *  @param pconfigbuffer reference to a CReadConfig object.
   *  @return mFail
   *  @see mFail
   */
  C_INT32 load(CReadConfig & configbuffer);

  /**
   *  Saves the contents of the object to a CWriteConfig object.
   *  (Which usually has a file attached but may also have socket)
   *  @param pconfigbuffer reference to a CWriteConfig object.
   *  @return mFail
   *  @see mFail
   */
  C_INT32 save(CWriteConfig & configbuffer);

  /**
   *  Retrieves the vector of substrates
   *  @return "vector < CId2Metab > &"
   */
  vector < CId2Metab > &getId2Substrates();

  /**
   *  Retrieves the vector of products
   *  @return "vector < CId2Metab > &"
   */
  vector < CId2Metab > &getId2Products();

  /**
   *  Retrieves the vector of modifiers
   *  @return "vector < CId2Metab > &"
   */
  vector < CId2Metab > &getId2Modifiers();

  /**
   *  Retrieves the vector of parameters
   *  @return "vector < CId2Param > &"
   */
  vector < CId2Param > &getId2Parameters();

  /**
   *  Retrieves the name of the reaction
   *  @return string
   */
  string getName() const;

  /**
   *  Retrieves the chemical equation of the reaction
   *  @return string
   */
  CChemEq & getChemEq();

  /**
   *  Retrieves the chemical structure of the reaction
   *  @return vector < ELEMENT >
   */
  // typedef struct ELEMENT {C_FLOAT64 mValue; string mName;};
  // vector < ELEMENT > getChemStructure() const;

  /**
   *  Retrieves a vector of substrates and their multiplicity
   *  in the chemical reaction.
   *  @return "vector < CChemEqElement * > &" substrates
   */
  vector < CChemEqElement * > & getSubstrates();
  
  /**
   *  Retrieves a vector of products and their multiplicity.
   *  in the chemical reaction.
   *  @return "vector < CChemEqElement * > &" products
   */
  vector < CChemEqElement * > & getProducts();

  /**
   *  Retrieves a vector of metabolites and their total balance.
   *  in the chemical reaction.
   *  @return "vector < CChemEqElement * > &" products
   */
  vector < CChemEqElement * > & getBalances();

  /**
   *  Retrieves the rate function of the reaction
   *  @return "CBaseFunction &"
   */
  CBaseFunction & getFunction();

  /**
   *  Retrieves the flux of the reaction
   *  @return C_FLOAT64
   */
  C_FLOAT64 getFlux() const;

  /**
   *  Retrieves whether the reaction is reversible
   *  @return C_INT16
   */
  C_INT16 isReversible() const;

  /**
   *  Sets the name of the reaction
   *  @param "const string &" name
   */
  void setName(const string & name);

  /**
   *  Sets the chemical equation of the reaction
   *  @param "const string &" chemEq
   */
  void setChemEq(const string & chemEq);

  /**
   *  Sets the rate function of the reaction
   *  @param "const string &" functionName
   */
  void setFunction(const string & functionName);

  /**
   *  Sets the flux of the reaction
   *  @param C_FLOAT64 flux
   */
  void setFlux(C_FLOAT64 flux);

  /**
   *  Sets whether the reaction is reversible
   *  @param C_INT16 reversible
   */
  void setReversible(C_INT16 reversible);

  /**
   *  Compile the reaction, i.e., links the metabolites and parameters with the
   *  rate function.
   *  @param "CCopasiVector < CMetab * > &" metabolites
   */
  void compile(CCopasiVectorN < CCompartment > & compartments);

  /**
   *  Calculate the kinetic function
   *  @return C_FLOAT64 velocity
   */
  C_FLOAT64 calculate();

  /**
   *  Retrieves the Compartment Name for substrates, products, and modifiers
   *  @param "vector < CMetab* > &" Metabolites 
   */
  void old2New(const vector < CMetab* > & metabolites);
    
  /**
   *	Returns the address of mFlux		Wei Sun
   */
  void * getFluxAddr();

  /**
   *	Returns the index of the parameter
   */
  C_INT32 findPara(string &Target);

 private:
  /**
   *
   */
  C_INT32 loadNew(CReadConfig & configbuffer);

  /**
   *
   */
  C_INT32 loadOld(CReadConfig & configbuffer);

  /**
   *
   */
  void initIdentifiers();

  /**
   *
   */
  void setIdentifiers();

  /**
   *
   */
  void checkIdentifiers();
};

#endif // COPASI_CReaction
