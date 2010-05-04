// Begin CVS Header 
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/bindings/swig/CEvent.i,v $ 
//   $Revision: 1.1.2.2 $ 
//   $Name:  $ 
//   $Author: gauges $ 
//   $Date: 2010/05/04 15:56:42 $ 
// End CVS Header 

// Copyright (C) 2010 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., University of Heidelberg, and The University 
// of Manchester. 
// All rights reserved. 

%{

#include "model/CEvent.h"  
#include <assert.h>  
%}

%ignore CEventAssignment::getExpressionPtr() const; 
%ignore CEvent::getTriggerExpressionPtr() const; 
%ignore CEvent::getDelayExpressionPtr() const; 
%ignore CEvent::getAssignments() const; 

// cleanup does not seem to be implemented
%ignore CEvent::cleanup(); 


%include "model/CEvent.h"



%extend CEvent
{
  CEventAssignment* createAssignment()
  {
    CEventAssignment* pAssignment=new CEventAssignment;
    $self->getAssignments().add(pAssignment,true);
    return pAssignment;
  }
}

