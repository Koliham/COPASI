// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/layout/CLRenderCurve.h,v $
//   $Revision: 1.1 $
//   $Name:  $
//   $Author: gauges $
//   $Date: 2010/03/10 12:26:12 $
// End CVS Header

// Copyright (C) 2010 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef CLRenderCurve_H__
#define CLRenderCurve_H__

#include <copasi/layout/CLGraphicalPrimitive1D.h>
#include <copasi/report/CCopasiObject.h>

#include <string>
#include <vector>

#include <sbml/layout/render/RenderCurve.h>

class CLRenderCubicBezier;
class CLRenderPoint;

class CLRenderCurve : public CLGraphicalPrimitive1D, public CCopasiObject
{
private:
  // prevent the compiler from generating an assignment operator
  CLRenderCurve& operator=(const CLRenderCurve& source);

protected:
  std::string mStartHead;
  std::string mEndHead;
  std::vector<CLRenderPoint*> mListOfElements;

  /**
   * key for the object.
   */
  std::string mKey;

public:
  /**
   * Constructor.
   */
  CLRenderCurve(CCopasiContainer* pParent = NULL);

  /**
   * Copy Constructor
   */
  CLRenderCurve(const CLRenderCurve& source, CCopasiContainer* pParent = NULL);

  /**
   * Constructor to generate object from the corresponding SBML object.
   */
  CLRenderCurve(const RenderCurve& source, CCopasiContainer* pParent = NULL);

  /**
   * Destructor
   */
  ~CLRenderCurve();

  /**
   * Sets the id of the start head.
   */
  void setStartHead(const std::string& startHead);

  /**
   * Sets the id of the end head.
   */
  void setEndHead(const std::string& endHead);

  /**
   * Returns the id of the start head.
   */
  const std::string& getStartHead() const;

  /**
   * Returns the id of the end head.
   */
  const std::string& getEndHead() const;

  /**
   * Returns the number of curve segments.
   */
  unsigned int getNumElements() const;

  /**
   * Creates a new bezier element.
   * The element is added to the curve.
   */
  CLRenderCubicBezier* createCubicBezier();

  /**
   * Creates a new point element.
   * The element is added to the curve.
   */
  CLRenderPoint* createPoint();

  /**
   * Returns a const pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   */
  const CLRenderPoint* getCurveElement(unsigned int index) const;

  /**
   * Returns a pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   */
  CLRenderPoint* getCurveElement(unsigned int index);

  /**
   * Adds a copy of the given curve segment to the end of the list of
   *curve segments.
   */
  void addCurveElement(const CLRenderPoint* cs);

  /**
   * Removes the curve segment with the given index.
   */
  void removeCurveElement(unsigned int i);

  /**
   * Returns a const pointer to the list of curve segments.
   */
  const std::vector<CLRenderPoint*>* getListOfCurveElements() const;

  /**
   * Returns a pointer to the list of curve segments.
   */
  std::vector<CLRenderPoint*>* getListOfCurveElements();

  /**
   * Returns true if the start head is set or false otherwise.
   */
  bool isSetStartHead() const;

  /**
   * Returns true if the start head is set or false otherwise.
   */
  bool isSetEndHead() const;

  /**
   * Returns the key string of the object.
   */
  const std::string& getKey() const;

  /**
   * Converts this object to the corresponding SBML object.
   */
  RenderCurve* toSBML() const;

protected:
  friend class Group;
};

#endif /* CLRenderCurve_H__ */