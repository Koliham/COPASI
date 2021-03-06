// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/elementaryFluxModes/CTableauMatrix.h,v $
//   $Revision: 1.15 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2011/07/08 20:13:34 $
// End CVS Header

// Copyright (C) 2011 - 2010 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/**
 *  CTableauMatrix class.
 *  Used to calculate elementary flux modes
 *
 *  Created for Copasi by Stefan Hoops 2002-05-08
 * (C) Stefan Hoops 2002
 */

#ifndef COPASI_CTableauMatrix
#define COPASI_CTableauMatrix

#include <list>
#include "elementaryFluxModes/CTableauLine.h"

class CTableauMatrix
{
  friend std::ostream & operator << (std::ostream &, const CTableauMatrix &);
  // Attributes

private:
  /**
   *  Vector containing the lines of the Matrix
   *  @associates <{CTableauLine}>
   *  @supplierCardinality 0..*
   */
  std::list< const CTableauLine * > mLine;

  /**
   *  Iterator pointing to the first irreversible reaction
   */
  std::list< const CTableauLine * >::iterator mFirstIrreversible;

  // Operations

public:
  /**
   *  Default constructor
   */
  CTableauMatrix();

  /**
   *  Specific constructor
   *  The first reversibleNumber lines of stoi must be reversible reactions.
   *  Note: stoi is actually the transpose of the stoichiometry matrix
   *  @param const std::vector < std::vector < double > > & stoi
   *  @param const size_t & reversibleNumber
   */
  CTableauMatrix(const std::vector < std::vector < double > > & stoi,
                 const size_t & reversibleNumber);

  /**
   *  Destructor
   */
  ~CTableauMatrix();

  /**
   * Retrieve the number of tableau lines
   * @return size_t size
   */
  size_t size() const;

  /**
   *  Retrieves the first line from the matrix
  *  @return list< const CTableauLine * >::iterator begin
   */
  std::list< const CTableauLine * >::iterator begin();

  /**
   *  Retrieves the first line from the matrix
   *  @return list< const CTableauLine * >::const_iterator begin
   */
  std::list< const CTableauLine * >::const_iterator begin() const;

  /**
   *  Retrieves the last line from the matrix
   *  @return list< const CTableauLine * >::iterator end
   */
  std::list< const CTableauLine * >::iterator end();

  /**
   *  Retrieves the last line from the matrix
   *  @return list< const CTableauLine * >::const_iterator end
   */
  std::list< const CTableauLine * >::const_iterator end() const;

  /**
   *  Add a line to the matrix
   *  @param const CTableauLine * src
   *  @param const bool & check (Default: true)
   */
  void addLine(const CTableauLine * src,
               const bool & check = true);

  /**
   *  Remove the indexed line of the matrix
   *  @param "list< const CTableauLine * >::const_iterator" line
   */
  void removeLine(const std::list< const CTableauLine * >::iterator line);

private:
  /**
   *  Check whether the new line fullfills the constraints
   *  @param "const CTableauLine *" src
   */
  bool isValid(const CTableauLine * src);
};

#endif // COPASI_CTableauMatrix
