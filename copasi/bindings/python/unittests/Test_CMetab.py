import COPASI
import unittest
from types import *
import math

class Test_CMetab(unittest.TestCase):
  def setUp(self):
    self.model=COPASI.CModel()
    self.compartment=self.model.createCompartment("Comp1")
    self.metab=self.model.createMetabolite("Metab1","Comp1")

  def test_getObjectDisplayName(self):
    s=self.metab.getObjectDisplayName()
    self.assert_(type(s)==StringType)

  def test_getConcentration(self):
    conc=self.metab.getConcentration()
    self.assert_(type(conc)==FloatType)

  def test_setInitialConcentration(self):
    value=5.0
    self.metab.setInitialConcentration(value)
    self.assert_(math.fabs(self.metab.getInitialConcentration()-value/value)<0.001)

  def test_getInitialConcentration(self):
    value=self.metab.getInitialConcentration()
    self.assert_(type(value)==FloatType)

  def test_setInitialValue(self):
    value=5.0
    self.metab.setInitialValue(value)
    self.assert_(math.fabs(self.metab.getInitialValue()-value/value)<0.001)

  def test_getCompartment(self):
    c=self.metab.getCompartment()
    self.assert_(c.__class__==COPASI.CCompartment)
    self.assert_(self.compartment.getKey()==c.getKey())

  def test_getModel(self):
    m=self.metab.getTransitionTime()
    self.assert_(m.__class__==COPASI.CModel)
    self.assert_(self.model.getKey()==m.getKey())

  def test_getTransitionTime(self):
    value=self.metab.getTransitionTime()
    self.assert_(type(value)==FloatType)

  def test_getConcentrationRate(self):
    value=self.metab.getConcentrationRate()
    self.assert_(type(value)==FloatType)

  def test_isDependent(self):
    value=self.metab.isDependent()
    self.assert_(type(value)==BooleanType)

def suite():
  tests=[
          'test_getObjectDisplayName'
         ,'test_getConcentration'
         ,'test_getInitialConcentration'
         ,'test_setInitialConcentration'
         ,'test_setInitialValue'
         ,'test_getCompartment'
         ,'test_getModel'
         ,'test_getTransitionTime'
         ,'test_getConcentrationRate'
         ,'test_isDependent'
        ]
  return unittest.TestSuite(map(Test_CMetab,tests))

if(__name__ == '__main__'):
    unittest.TextTestRunner(verbosity=2).run(suite())


