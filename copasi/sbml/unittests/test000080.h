// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/sbml/unittests/test000080.h,v $
//   $Revision: 1.1 $
//   $Name:  $
//   $Author: gauges $
//   $Date: 2009/05/07 10:50:34 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#ifndef TEST_000080_H__
#define TEST_000080_H__

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

class CCopasiDataModel;

class test000080 : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(test000080);

  CPPUNIT_TEST(test_mass_action_recognition_bug);

  CPPUNIT_TEST_SUITE_END();

protected:
  static CCopasiDataModel* pCOPASIDATAMODEL;
  static const char* MODEL_STRING1;

public:
  void setUp();

  void tearDown();

  void test_mass_action_recognition_bug();
};

#endif /* TEST000080_H__ */
