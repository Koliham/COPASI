// Begin CVS Header 
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/bindings/swig/CCopasiVector.i,v $ 
//   $Revision: 1.17.4.2 $ 
//   $Name:  $ 
//   $Author: gauges $ 
//   $Date: 2008/04/08 15:01:34 $ 
// End CVS Header 

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc. and EML Research, gGmbH. 
// All rights reserved. 

// Copyright © 2005 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.


%{

#include "utilities/CCopasiVector.h"

%}

template < class CType > class CCopasiVector:
        protected std::vector< CType * >, public CCopasiContainer
{

%rename(removeObject) remove(CCopasiObject* pObject);

    public:
      typedef typename std::vector< CType * >::value_type value_type;
      typedef typename std::vector< CType * >::iterator iterator;
      typedef typename std::vector< CType * >::const_iterator const_iterator;

      CCopasiVector(const std::string & name = "NoName",
                    const CCopasiContainer * pParent = NULL,
                    const unsigned C_INT32 &
                    flag = CCopasiObject::Vector);

      CCopasiVector(const CCopasiVector < CType > & src,
                    const CCopasiContainer * pParent = NULL);

      virtual ~CCopasiVector();
      virtual void swap(unsigned C_INT32 indexFrom, unsigned C_INT32 indexTo);
      virtual bool add(CType * src, bool adopt = false);
      virtual void remove(const unsigned C_INT32 & index);
      virtual bool remove(CCopasiObject * pObject);
      virtual unsigned C_INT32 size() const;
      virtual unsigned C_INT32 getIndex(const CCopasiObject * pObject) const;

      %extend
      {
        virtual value_type& get(unsigned C_INT32 index)
        {
            return (*self)[index];
        }

      }
};


template < class CType > class CCopasiVectorN: public CCopasiVector < CType >
{

%rename(removeByName) remove(const std::string& name);
%rename(getIndexByName) getIndex(const std::string& name) const;

    public:
      typedef typename std::vector< CType * >::value_type value_type;
      typedef typename std::vector< CType * >::iterator iterator;
      typedef typename std::vector< CType * >::const_iterator const_iterator;
      CCopasiVectorN(const std::string & name = "NoName",
                     const CCopasiContainer * pParent = NULL);

      CCopasiVectorN(const CCopasiVectorN < CType > & src,
                     const CCopasiContainer * pParent = NULL);

      virtual ~CCopasiVectorN();
      virtual bool add(CType * src, bool adopt = false);
      virtual void remove(const std::string & name);
      virtual unsigned C_INT32 getIndex(const std::string &name) const;

      %extend
      {
        virtual value_type& getByName(const std::string& name)
        {
            return (*self)[name];
        }
      }
};


template < class CType > class CCopasiVectorNS: public CCopasiVectorN < CType >
{
    public:
      typedef typename std::vector< CType * >::value_type value_type;
      typedef typename std::vector< CType * >::iterator iterator;
      typedef typename std::vector< CType * >::const_iterator const_iterator;
      /**
       *  Default constructor
       */
      CCopasiVectorNS(const std::string & name = "NoName",
                      const CCopasiContainer * pParent = NULL);

      /**
       *  Copy constructor
       */
      CCopasiVectorNS(const CCopasiVectorNS < CType > & src,
                      const CCopasiContainer * pParent = NULL) ;

      /**
       *  Destructor
       */
      virtual ~CCopasiVectorNS();

};

%template(TaskStdVector) std::vector<CCopasiTask*>;
%template(TaskVector) CCopasiVector<CCopasiTask>;
%template(TaskVectorN) CCopasiVectorN<CCopasiTask>;

%template(ModelValueStdVector) std::vector<CModelValue*>;
%template(ModelValueVector) CCopasiVector<CModelValue>;
%template(ModelValueVectorN) CCopasiVectorN<CModelValue>;

%template(ReportDefinitionStdVector) std::vector<CReportDefinition*>;
%template(ReportDefinitionVector) CCopasiVector<CReportDefinition>;
%template(ReportDefinitionVectorN) CCopasiVectorN<CReportDefinition>;

%template(MoietyStdVector) std::vector<CMoiety*>;
%template(MoietyVector) CCopasiVector<CMoiety>;

%template(MetabStdVector) std::vector<CMetab*>;
%template(MetabVector) CCopasiVector<CMetab>;
%template(MetabVectorN) CCopasiVectorN<CMetab>;
%template(MetabVectorNS) CCopasiVectorNS<CMetab>;

%template(CompartmentStdVector) std::vector<CCompartment*>;
%template(CompartmentVector) CCopasiVector<CCompartment>;
%template(CompartmentVectorN) CCopasiVectorN<CCompartment>;
%template(CompartmentVectorNS) CCopasiVectorNS<CCompartment>;

%template(ReactionStdVector) std::vector<CReaction*>;
%template(ReactionVector) CCopasiVector<CReaction>;
%template(ReactionVectorN) CCopasiVectorN<CReaction>;
%template(ReactionVectorNS) CCopasiVectorNS<CReaction>;

%template(ReportItemVector) std::vector<CRegisteredObjectName>;

%template(CFunctionStdVector) std::vector<CFunction*>;

%template(CEvaluationTreeStdVector) std::vector<CEvaluationTree*>;
%template(CEvaluationTreeVector) CCopasiVector<CEvaluationTree>;
%template(CEvaluationTreeVectorN) CCopasiVectorN<CEvaluationTree>;

%template(CChemEqElementStdVector) std::vector<CChemEqElement*>;
%template(CChemEqElementVector) CCopasiVector<CChemEqElement>;

%template(ParameterVector) std::vector<CCopasiParameter*>;

%template(IntStdVector) std::vector<C_INT32>;

%template(StringStdVector) std::vector<std::string>;
%template(VectorOfStringVectors) std::vector<std::vector<std::string> >;

%template(FloatStdVector) std::vector<C_FLOAT64>;

%template(OptItemStdVector) std::vector<COptItem*>;

%template(ContainerStdVector) std::vector<CCopasiContainer*>;

typedef CCopasiVectorN<CCopasiTask> TaskVectorN;

typedef CCopasiVectorN<CModelValue> ModelValueVectorN;

typedef CCopasiVector<CMoiety> MoietyVector;

typedef CCopasiVector<CMetab> MetabVector;
typedef CCopasiVectorNS<CMetab> MetabVectorNS;

typedef CCopasiVectorNS<CCompartment> CompartmentVectorNS;

typedef CCopasiVectorNS<CReaction> ReactionVectorNS;

typedef std::vector<CRegisteredObjectName> ReportItemVector;
typedef std::vector<CCopasiParameter*> ParameterVector;

typedef CCopasiVectorN<CEvaluationTree> CEvaluationTreeVectorN;

typedef std::vector<CFunction> CFunctionStdVector;

typedef CCopasiVector<CChemEqElement> CChemEqElementVector;

typedef std::vector<C_INT32> IntStdVector;

typedef std::vector<C_FLOAT64> FloatStdVector;

typedef std::vector<COptItem*> OptItemStdVector;

typedef std::vector<std::string> StringStdVector;
typedef std::vector<std::vector<std::string> > VectorOfStringVectors;

typedef std::vector<CCopasiContainer*> ContainerStdVector;

