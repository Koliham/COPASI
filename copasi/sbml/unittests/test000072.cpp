// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/sbml/unittests/test000072.cpp,v $
//   $Revision: 1.4 $
//   $Name:  $
//   $Author: gauges $
//   $Date: 2009/02/20 10:41:10 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#include "test000072.h"

#include <sstream>
#include <string>
#include "copasi/CopasiDataModel/CCopasiDataModel.h"

#include "copasi/report/CCopasiRootContainer.h"

/**
 * Make sure exporting to Level2 after an export to Level1 does not crash COPASI.
 */
CCopasiDataModel* test000072::pCOPASIDATAMODEL = NULL;

void test000072::setUp()
{
  // Create the root container.
  CCopasiRootContainer::init(false, 0, NULL);
  // Create the global data model.
  pCOPASIDATAMODEL = CCopasiRootContainer::addDatamodel();
}

void test000072::tearDown()
{
  CCopasiRootContainer::destroy();
}

void test000072::test_bug1086()
{
  CCopasiDataModel* pDataModel = pCOPASIDATAMODEL;
  CPPUNIT_ASSERT(pDataModel->importSBMLFromString(MODEL_STRING1));
  // now try to export several times
  std::string s = pDataModel->exportSBMLToString(NULL, 1, 2);
  CPPUNIT_ASSERT(!s.empty());
  s = pDataModel->exportSBMLToString(NULL, 2, 3);
  CPPUNIT_ASSERT(!s.empty());
}

const char* test000072::MODEL_STRING1 =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
  "<sbml xmlns=\"http://www.sbml.org/sbml/level2/version3\" level=\"2\" version=\"3\">\n"
  "  <model id=\"Model_1\" name=\"New Model\">\n"
  "    <notes>\n"
  "      <body xmlns=\"http://www.w3.org/1999/xhtml\">\n"
  "      </body>\n"
  "    </notes>\n"
  "    <listOfFunctionDefinitions>\n"
  "      <functionDefinition id=\"function_1\" name=\"function_1\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> k </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <times/>\n"
  "              <cn> 3.0 </cn>\n"
  "              <ci> k </ci>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "      <functionDefinition id=\"function_2\" name=\"function_2\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> A </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> B </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <plus/>\n"
  "              <ci> B </ci>\n"
  "              <ci> A </ci>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "      <functionDefinition id=\"function_3\" name=\"function_3\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> k </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> C </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <minus/>\n"
  "              <ci> C </ci>\n"
  "              <apply>\n"
  "                <times/>\n"
  "                <ci> k </ci>\n"
  "                <cn> 1.3 </cn>\n"
  "              </apply>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "      <functionDefinition id=\"function_4\" name=\"function_4\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> x </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> y </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <divide/>\n"
  "                <apply>\n"
  "                  <ci> function_2 </ci>\n"
  "                  <ci> x </ci>\n"
  "                  <ci> y </ci>\n"
  "                </apply>\n"
  "                <cn> 2.0 </cn>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "      <functionDefinition id=\"function_5\" name=\"function_5\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> a </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> b </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> c </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <plus/>\n"
  "              <apply>\n"
  "                <ci> function_3 </ci>\n"
  "                <ci> c </ci>\n"
  "                <ci> a </ci>\n"
  "              </apply>\n"
  "              <apply>\n"
  "                <minus/>\n"
  "                <apply>\n"
  "                  <ci> function_1 </ci>\n"
  "                  <ci> b </ci>\n"
  "                </apply>\n"
  "                <cn> 5.23 </cn>\n"
  "              </apply>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "      <functionDefinition id=\"function_6\" name=\"function_6\">\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <lambda>\n"
  "            <bvar>\n"
  "              <ci> k1 </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> k2 </ci>\n"
  "            </bvar>\n"
  "            <bvar>\n"
  "              <ci> k3 </ci>\n"
  "            </bvar>\n"
  "            <apply>\n"
  "              <ci> function_5 </ci>\n"
  "              <ci> k1 </ci>\n"
  "              <ci> k2 </ci>\n"
  "              <ci> k3 </ci>\n"
  "            </apply>\n"
  "          </lambda>\n"
  "        </math>\n"
  "      </functionDefinition>\n"
  "    </listOfFunctionDefinitions>\n"
  "    <listOfCompartments>\n"
  "      <compartment id=\"compartment_1\" name=\"compartment\" size=\"1\"/>\n"
  "    </listOfCompartments>\n"
  "    <listOfSpecies>\n"
  "      <species id=\"species_1\" name=\"A\" compartment=\"compartment_1\" initialConcentration=\"1\"/>\n"
  "      <species id=\"species_2\" name=\"B\" compartment=\"compartment_1\" initialConcentration=\"1\"/>\n"
  "      <species id=\"species_3\" name=\"C\" compartment=\"compartment_1\" initialConcentration=\"1\" constant=\"false\"/>\n"
  "      <species id=\"species_4\" name=\"D\" compartment=\"compartment_1\" initialConcentration=\"1\" constant=\"false\"/>\n"
  "      <species id=\"species_5\" name=\"E\" compartment=\"compartment_1\" initialConcentration=\"1\" constant=\"false\"/>\n"
  "      <species id=\"species_6\" name=\"F\" compartment=\"compartment_1\" initialConcentration=\"1\" constant=\"false\"/>\n"
  "    </listOfSpecies>\n"
  "    <listOfParameters>\n"
  "      <parameter id=\"parameter_1\" name=\"K1\" value=\"1.1\" constant=\"false\"/>\n"
  "      <parameter id=\"parameter_2\" name=\"K2\" value=\"1.2\"/>\n"
  "      <parameter id=\"parameter_3\" name=\"K3\" value=\"1.3\" constant=\"false\"/>\n"
  "    </listOfParameters>\n"
  "    <listOfRules>\n"
  "     <assignmentRule variable=\"parameter_1\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_1 </ci>\n"
  "          <cn> 4.5 </cn>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </assignmentRule>\n"
  "     <assignmentRule variable=\"parameter_3\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_3 </ci>\n"
  "          <cn> 2.0 </cn>\n"
  "          <ci> parameter_1 </ci>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </assignmentRule>\n"
  "     <assignmentRule variable=\"species_3\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_5 </ci>\n"
  "          <ci> parameter_1 </ci>\n"
  "          <cn> 2.4 </cn>\n"
  "          <cn> 3.5 </cn>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </assignmentRule>\n"
  "     <rateRule variable=\"species_4\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_2 </ci>\n"
  "          <ci> parameter_1 </ci>\n"
  "          <cn> 3.4 </cn>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </rateRule>\n"
  "     <rateRule variable=\"species_5\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_4 </ci>\n"
  "          <cn> 1.4 </cn>\n"
  "          <ci> parameter_2 </ci>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </rateRule>\n"
  "     <rateRule variable=\"species_6\">\n"
  "       <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "        <apply>\n"
  "          <ci> function_6 </ci>\n"
  "          <ci> parameter_3 </ci>\n"
  "          <ci> parameter_1 </ci>\n"
  "          <cn> 3.4 </cn>\n"
  "        </apply>\n"
  "       </math>\n"
  "     </rateRule>\n"
  "    </listOfRules>\n"
  "  <listOfReactions>\n"
  "    <reaction id=\"reaction1\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_1 </ci>\n"
  "              <ci> parameter_1 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "    <reaction id=\"reaction2\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_2 </ci>\n"
  "              <ci> parameter_1 </ci>\n"
  "              <ci> parameter_2 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "    <reaction id=\"reaction3\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_3 </ci>\n"
  "              <ci> parameter_1 </ci>\n"
  "              <ci> species_2 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "    <reaction id=\"reaction4\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_4 </ci>\n"
  "              <ci> species_1 </ci>\n"
  "              <ci> species_2 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "    <reaction id=\"reaction5\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_5 </ci>\n"
  "              <ci> parameter_1 </ci>\n"
  "              <ci> parameter_3 </ci>\n"
  "              <ci> species_2 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "    <reaction id=\"reaction6\" reversible=\"true\">\n"
  "      <listOfReactants>\n"
  "        <speciesReference species=\"species_1\"/>\n"
  "      </listOfReactants>\n"
  "      <listOfProducts>\n"
  "        <speciesReference species=\"species_2\"/>\n"
  "      </listOfProducts>\n"
  "      <kineticLaw>\n"
  "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
  "          <apply>\n"
  "            <times/>\n"
  "            <ci> compartment_1 </ci>\n"
  "            <apply>\n"
  "              <ci> function_6 </ci>\n"
  "              <ci> parameter_1 </ci>\n"
  "              <ci> parameter_2 </ci>\n"
  "              <ci> parameter_3 </ci>\n"
  "            </apply>\n"
  "          </apply>\n"
  "        </math>\n"
  "      </kineticLaw>\n"
  "    </reaction>\n"
  "  </listOfReactions>\n"
  "  </model>\n"
  "</sbml>\n"
;
