/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/tss/Attic/MMASCIIExporter.cpp,v $
   $Revision: 1.3 $
   $Name:  $
   $Author: nsimus $ 
   $Date: 2005/09/05 09:26:46 $
   End CVS Header */

#include <math.h>
#include "copasi.h"

#include "CopasiDataModel/CCopasiDataModel.h"

#include "MMASCIIExporter.h"

#include "model/CModel.h"
#include "model/CMetab.h"
#include "utilities/CCopasiVector.h"
#include "model/CReaction.h"
#include "model/CMoiety.h"
#include "model/CChemEqElement.h"
#include "function/CFunction.h"
#include "report/CKeyFactory.h"
#include <iostream>
#include <fstream>

/**
 ** Constructor for the exporter.
 */
MMASCIIExporter::MMASCIIExporter()
{}

/**
 ** Destructor for the exporter.
 */
MMASCIIExporter::~MMASCIIExporter()
{}
/**
 **       This method tests if a string only consists of whitespace characters
 **/
bool MMASCIIExporter::isEmptyString(std::string & str)
{
  return (str.find_first_not_of(" \n\t\r") == std::string::npos);
}
/**
 **        This method finds the metab by the name, returns an index
 **/
C_INT32 MMASCIIExporter::findMetabXByName(const CModel* copasiModel, const std::string & Target)
{
  std::string name;
  const CCopasiVector< CMetab > & metabs = copasiModel->getMetabolitesX();
  unsigned C_INT32 id, metabs_size = metabs.size();

  for (id = 0; id < metabs_size; id++)
    {
      name = metabs[id]->getObjectName();
      if (name == Target)
        return id;
    }
  return - 1;
}
/**
 **        This method finds the compartement by the name, returns an index
 **/
C_INT32 MMASCIIExporter::findCompByName(const CModel* copasiModel, const std::string & Target)
{
  std::string name;

  const CCopasiVector< CCompartment > & comps = copasiModel->getCompartments();
  unsigned C_INT32 comps_size = comps.size();
  unsigned C_INT32 id;

  for (id = 0; id < comps_size; id++)
    {
      name = comps[id]->getObjectName();
      if (name == Target)
        return id;
    }
  return - 1;
}
/**
 **        This method finds the global parameter by the name, returns an index   
 **/
C_INT32 MMASCIIExporter::findGlobalParamByName(const CModel* copasiModel, const std::string & Target)
{
  std::string name;
  const CCopasiVector< CModelValue > & modvals = copasiModel->getModelValues();
  unsigned C_INT32 modvals_size = modvals.size();
  unsigned C_INT32 id;

  for (id = 0; id < modvals_size; id++)
    {
      name = modvals[id]->getObjectName();
      if (name == Target)
        return id;
    }
  return - 1;
}
/**
 **        This method finds the local reactions parameter by the name, returns an index  
 **/
C_INT32 MMASCIIExporter::findKinParamByName(const CReaction* reac, const std::string & Target)
{
  std::string tmpname;
  unsigned C_INT32 tmp_k, tmp_size;

  tmp_size = reac->getParameters().size();

  for (tmp_k = 0; tmp_k < tmp_size; tmp_k++)
    {
      tmpname = reac->getParameters().getParameter(tmp_k)->getObjectName();
      if (tmpname == Target)
        return tmp_k;
    }
  return - 1;
}

/**
 ** This method takes some of the copasi CModel objects 
 ** and writes them in the ASCII format in an output file.    
 ** The filename is given as the second
 ** argument to the function. The function return "true" on success and
 ** "false" on failure.
 */

bool MMASCIIExporter::exportMathModel(const CModel* copasiModel, std::string mmasciiFilename, bool overwriteFile)
{
  /* check if the file already exisits.
          If yes, write if overwrite is true, 
          else create an appropriate  CCopasiMessage. */

  std::ifstream testInfile(mmasciiFilename.c_str(), std::ios::in);

  if (testInfile && !overwriteFile)
    {
      // create a CCopasiMessage with the appropriate error
      CCopasiMessage(CCopasiMessage::ERROR, MCDirEntry + 1, mmasciiFilename.c_str());
      return false;
    }

  std::ofstream outFile(mmasciiFilename.c_str(), std::ios::out);

  outFile << "#ifdef METABOLITES" << std::endl;
  //outFile << "//Metabolites:" << std::endl;

  unsigned C_INT32 i, metab_size;
  CMetab* metab;

  metab_size = copasiModel->getMetabolitesX().size();
  for (i = 0; i < metab_size; i++)
    {
      metab = copasiModel->getMetabolitesX()[i];

      outFile << "x[" << i << "]="
      << metab->getInitialConcentration() << ";"
      << '\t' << "//" << metab->getObjectName().c_str() << ","
      << '\t' << CModelEntity::StatusName[metab->getStatus()]
      << std::endl;
    }
  outFile << "#endif // METABOLITES" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef MOIETY" << std::endl;
  //outFile << "//Moiety:" << std::endl;

  const CModel::CLinkMatrixView & L = copasiModel->getL();

  unsigned C_INT32 indep_dep_size, indep_size = copasiModel->getNumIndependentMetabs();
  unsigned C_INT32 j;
  C_FLOAT64 Value;

  indep_dep_size = indep_size + copasiModel->getNumDependentMetabs();

  for (i = indep_size; i < indep_dep_size; i++)
    {
      std::ostringstream Description;
      std::ostringstream comment;

      metab = copasiModel->getMetabolitesX()[i];
      if (metab->getStatus() == CModelEntity::UNUSED) continue;
      if (metab->getStatus() == CModelEntity::FIXED) continue;

      Value = metab->getInitialConcentration();
      comment << metab->getObjectName();

      for (j = 0; j < indep_size; j++)
        if (L(i, j) != 0.0)
          {
            if (L(i, j) < 0.0)
              {
                Description << " - ";
                comment << " + ";
              }
            else
              {
                Description << " + ";
                comment << " - ";
              }
            if (fabs(L(i, j)) != 1.0)
              {
                Description << fabs(L(i, j)) << " * " << "x[" << j << "]";
                comment << fabs(L(i, j)) << " * " << copasiModel->getMetabolitesX()[j]->getObjectName();
              }
            else
              {
                Description << "x[" << j << "]";
                comment << copasiModel->getMetabolitesX()[j]->getObjectName();
              }

            Value -= L(i, j) * copasiModel->getMetabolitesX()[j]->getInitialConcentration();
          }

      outFile << "x[" << i << "] = " << Value << Description.str() << ";" << '\t'
      << "// " << comment.str() << " = const" << std::endl;
    }

  outFile << "#endif // MOIETY" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef COMPARTMENTS" << std::endl;
  //outFile << "//Compartments:" << std::endl;

  const CCopasiVector< CCompartment > & comps = copasiModel->getCompartments();
  unsigned C_INT32 comps_size = comps.size();

  for (i = 0; i < comps_size; i++)
    {
      outFile << "c[" << i << "]="
      << comps[i]->getVolume() << ";"
      << '\t' << "//" << comps[i]->getObjectName().c_str()
      << std::endl;
    }

  outFile << "#endif // COMPARTMENTS" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef GLOBAL_PARAMETERS" << std::endl;
  //outFile << "//Kinetic parameters (global):" << std::endl;

  const CCopasiVector< CModelValue > & modvals = copasiModel->getModelValues();
  unsigned C_INT32 modvals_size = modvals.size();

  for (i = 0; i < modvals_size; i++)
    {
      outFile << "gk[" << i << "]="
      << modvals[i]->getValue() << ";"
      << '\t' << "//" << modvals[i]->getObjectName().c_str()
      << std::endl;
    }

  outFile << "#endif GLOBAL_PARAMETERS" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef KINETIC_PARAMETERS" << std::endl;
  //outFile << "//Kinetic parameters (local reactions parameters):" << std::endl;

  unsigned C_INT32 reacs_size, count = 0;
  const CCopasiVector< CReaction > & reacs = copasiModel->getReactionsX();
  CReaction* reac;

  reacs_size = reacs.size();

  for (i = 0; i < reacs_size; ++i)
    {
      unsigned C_INT32 params_size;
      reac = reacs[i];

      outFile << "// in reaction:" << reac->getObjectName().c_str() << std::endl;

      params_size = reac->getParameters().size();

      for (j = 0; j < params_size; ++j)
        {
          outFile << "k[" << count++ << "]="
          << *reac->getParameters().getParameter(j)->getValue().pDOUBLE << ";"
          << '\t' << "//" << reac->getParameters().getParameter(j)->getObjectName().c_str()
          << std::endl;
        }
    }

  outFile << "#endif // KINETIC_PARAMETERS" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef KINETIC_FUNCTIONS" << std::endl;
  //outFile << "//Kinetic Functions:" << std::endl;

  std::string newFunctionNames[reacs_size];
  bool isNewName[reacs_size];

  for (i = 0; i < reacs_size; ++i)
    if (reacs[i]->getFunction().getType() != CEvaluationTree::MassAction)
      {
        newFunctionNames[i] = reacs[i]->getFunction().getObjectName();
        isNewName[i] = true;
      }
  count = 0;
  for (i = 0; i < reacs_size; ++i)
    {
      std::string tmpName;
      std::ostringstream newName;

      reac = reacs[i];

      if (reac->getFunction().getType() != CEvaluationTree::MassAction)
        if (isNewName[i])
          {
            tmpName = reac->getFunction().getObjectName();
            newName << " Function_" << count << "_";
            newFunctionNames[i] = newName.str();

            for (j = i + 1; j < reacs_size; ++j)
              if (newFunctionNames[j] == tmpName)
                {
                  newFunctionNames[j] = newName.str();
                  isNewName[j] = false;
                }
            count++;
          }
    }

  for (i = 0; i < reacs_size; ++i)
    {
      unsigned C_INT32 params_size;
      std::ostringstream newName;

      reac = reacs[i];
      params_size = reac->getFunctionParameters().size();

      if (reac->getFunction().getType() != CEvaluationTree::MassAction)
        if (isNewName[i])
          {
            outFile << std::endl;
            outFile << "double "
            << newFunctionNames[i]
            //<< reac->getFunction().getObjectName().c_str()
            << "(";

            for (j = 0; j < params_size; ++j)
              {
                outFile << "double " << reac->getFunctionParameters()[j]->getObjectName().c_str();
                if (j != params_size - 1) outFile << ", ";
              }

            outFile << ") ";
            outFile << '\t' << "//" << reac->getFunction().getObjectName().c_str() << std::endl;
            outFile << "{return  " << reac->getFunction().getInfix().c_str() << "; } " << std::endl;
          }
    }

  outFile << "#endif // KINETIC_FUNCTIONS" << std::endl;
  outFile << std::endl;

  outFile << "#ifdef DIFFERENTIAL_EQUATIONS" << std::endl;
  //outFile << "//Differential Equations:" << std::endl;

  const CMatrix< C_FLOAT64 > & redStoi = copasiModel->getRedStoi();
  const CCopasiVector< CMetab > & metabs = copasiModel->getMetabolitesX();

  for (i = 0; i < indep_size; ++i)
    {
      unsigned C_INT32 counter = 0;

      std::ostringstream equation;
      std::string tmpstr;

      unsigned C_INT32 compartment_index;
      const CCompartment* compartment = metabs[i]->getCompartment();

      compartment_index = findCompByName(copasiModel, compartment->getObjectName());

      for (j = 0; j < reacs_size; ++j)
        {
          unsigned C_INT32 k;
          tmpstr = equation.str();

          reac = reacs[j];

          if (fabs(redStoi[i][j]) > 0.0)
            {
              if (redStoi[i][j] < 0.0)
                equation << " - ";
              else
                if (!(isEmptyString(tmpstr))) equation << " + ";

              if (fabs(redStoi[i][j]) != 1.0) equation << fabs(redStoi[i][j]) << " * ";

              if (reac->getFunction().getType() != CEvaluationTree::MassAction)
                {
                  const CFunctionParameters & params = reac->getFunctionParameters();
                  unsigned C_INT32 params_size = params.size();
                  const std::vector<std::vector<std::string> > & keyMap = reac->getParameterMappings();

                  equation
                  << newFunctionNames[j]
                  //<< reac->getFunction().getObjectName().c_str()
                  << "(";

                  for (k = 0; k < params_size; ++k)
                    {
                      C_INT32 index;
                      std::string usage = params[k]->getUsage();
                      std::string name;

                      CCopasiObject * tmp = GlobalKeys.get(keyMap[k][0]);

                      if ((usage == "SUBSTRATE") || (usage == "PRODUCT") || (usage == "MODIFIER"))
                        {
                          CMetab* metab;
                          metab = dynamic_cast< CMetab * >(tmp);
                          name = metab ->getObjectName();

                          index = findMetabXByName(copasiModel, name);
                          equation << "x[" << index << "]";
                        }
                      if (usage == "PARAMETER")
                        if (!(reac->isLocalParameter(k)))
                          {
                            CModelValue* modval;
                            modval = dynamic_cast< CModelValue * >(tmp);
                            name = modval ->getObjectName();
                            index = findGlobalParamByName(copasiModel, name);
                            equation << "gk[" << index << "]";
                          }
                        else
                          {
                            CCopasiParameter* param;
                            param = dynamic_cast< CCopasiParameter * >(tmp);
                            name = param->getObjectName();

                            index = findKinParamByName(reac, name);
                            equation << "k[" << counter + index << "]";
                          }
                      if (usage == "VOLUME")
                        {
                          CCompartment* comp;
                          comp = dynamic_cast< CCompartment * >(tmp);
                          name = comp->getObjectName();

                          index = findCompByName(copasiModel, name);
                          equation << "c[";
                          equation << index << "]";
                        }
                      if (usage == "VARIABLE")
                        {
                          CModelValue* modval;
                          modval = dynamic_cast< CModelValue * >(tmp);

                          outFile << std::endl;
                          outFile << "******* Function parameter" << modval->getObjectName().c_str()
                          << ": this version does not support the parameters usage VARIABLE ******";

                          return false;
                        }

                      if (k != params_size - 1) equation << ", ";
                    }

                  equation << ")";
                }
              else
                {
                  std::ostringstream massaction;
                  std::string name;
                  std::string usage;

                  const CCopasiVector<CChemEqElement> & substrs = reac->getChemEq().getSubstrates();
                  const CCopasiVector<CChemEqElement> & prods = reac->getChemEq().getProducts();
                  const std::vector<std::vector<std::string> > & keyMap = reac->getParameterMappings();
                  CCopasiObject * tmp;

                  unsigned C_INT32 substrs_size = substrs.size(), prods_size = prods.size();
                  unsigned C_INT32 m, mult;
                  C_INT32 index;

                  CChemEqElement* substr;
                  CChemEqElement* prod;

                  const CMassAction cMassAction = static_cast<const CMassAction>(reac->getFunction());

                  massaction << "(";

                  usage = cMassAction.getVariables()[0]->getUsage().c_str();
                  tmp = GlobalKeys.get(keyMap[0][0]);

                  if (!(reac->isLocalParameter(0)))
                    {
                      CModelValue* modval;
                      modval = dynamic_cast< CModelValue * >(tmp);
                      name = modval ->getObjectName();
                      index = findGlobalParamByName(copasiModel, name);
                      massaction << "gk[" << index << "]";
                    }
                  else
                    {
                      CCopasiParameter* param;
                      param = dynamic_cast< CCopasiParameter * >(tmp);
                      name = param->getObjectName();

                      index = findKinParamByName(reac, name);
                      massaction << "k[" << counter + index << "]";
                    }

                  for (k = 0; k < substrs_size; ++k)
                    {
                      substr = substrs[k];
                      mult = substr->getMultiplicity();

                      name = substr->getMetabolite().getObjectName();
                      index = findMetabXByName(copasiModel, name);
                      massaction << " * x[" << index << "]";

                      if (mult > 1)
                        for (m = 1; m < mult; ++m)
                          massaction << " * x[" << index << "]";
                    }

                  if (cMassAction.isReversible() == TriTrue)
                    {
                      massaction << " - ";
                      usage = cMassAction.getVariables()[0]->getUsage().c_str();

                      tmp = GlobalKeys.get(keyMap[2][0]);

                      if (!(reac->isLocalParameter(2)))
                        {
                          CModelValue* modval;
                          modval = dynamic_cast< CModelValue * >(tmp);
                          name = modval ->getObjectName();
                          index = findGlobalParamByName(copasiModel, name);
                          massaction << "gk[" << index << "]";
                        }
                      else
                        {
                          CCopasiParameter* param;
                          param = dynamic_cast< CCopasiParameter * >(tmp);
                          name = param->getObjectName();

                          index = findKinParamByName(reac, name);
                          massaction << "k[" << counter + index << "]";
                        }

                      for (k = 0; k < prods_size; ++k)
                        {
                          prod = prods[k];
                          mult = prod->getMultiplicity();

                          name = prod->getMetabolite().getObjectName();
                          index = findMetabXByName(copasiModel, name);
                          massaction << " * x[" << index << "]";

                          if (mult > 1)
                            for (m = 1; m < mult; ++m)
                              massaction << " * x[" << index << "]";
                        }
                    }

                  massaction << ") ";
                  equation << massaction.str();
                }

              if (reac->getCompartmentNumber() != 1) equation << " / c[" << compartment_index << "]";
            }

          counter += reac->getParameters().size();
        }
      outFile << "dx[" << i << "] = " << equation.str() << ";" << std::endl;
    }
  outFile << "#endif // DIFFERENTIAL_EQUATIONS" << std::endl;

  return true;
}
