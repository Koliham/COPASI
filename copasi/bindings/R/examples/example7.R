#options(echo = FALSE) # disable echoing of input

#
# This is an example on how to create user defined kinetic functions with the COPASI API
#


# First we load the COPASI package
dyn.load(paste("COPASI", .Platform$dynlib.ext, sep=""))
source("COPASI.R")
# I Don't know exactly what the next line does, but this is what the SWIG
# documentation has to say about it:
# The cacheMetaData(1) will cause R to refresh its object tables. Without it, inheritance of wrapped objects may fail.
cacheMetaData(1)

stopifnot(!is.null(CCopasiRootContainer_getRoot()))
# create a new datamodel
dataModel <- CCopasiRootContainer_addDatamodel()
stopifnot(DataModelVector_size(CCopasiRootContainer_getDatamodelList()) == 1)
# get the model from the datamodel
model <- CCopasiDataModel_getModel(dataModel)
stopifnot(!is.null(model))
# set the units for the model
# we want seconds as the time unit
# microliter as the volume units
# and nanomole as the substance units
invisible(CModel_setTimeUnit(model,"s"))
invisible(CModel_setVolumeUnit(model,"microl"))
invisible(CModel_setQuantityUnit(model,"nMol"))

# we have to keep a set of all the initial values that are changed during
# the model building process
# They are needed after the model has been built to make sure all initial
# values are set to the correct initial value
changedObjects <- ObjectStdVector()

# create a compartment with the name cell and an initial volume of 5.0
# microliter
compartment <- CModel_createCompartment(model,"cell", 5.0)
object <- CCopasiContainer_getObject(compartment,CCopasiObjectName("Reference=InitialVolume"))
stopifnot(!is.null(object))
invisible(ObjectStdVector_push_back(changedObjects,object))
stopifnot(!is.null(compartment))
stopifnot(CompartmentVector_size(CModel_getCompartments(model)) == 1)
# create a new metabolite with the name S and an inital
# concentration of 10 nanomol
# the metabolite belongs to the compartment we created and is is to be
# fixed
S <- CModel_createMetabolite(model,"S", CCopasiObject_getObjectName(compartment), 10.0, "FIXED")
object <- CCopasiContainer_getObject(S,CCopasiObjectName("Reference=InitialConcentration"))
stopifnot(!is.null(object))
invisible(ObjectStdVector_push_back(changedObjects,object))
stopifnot(!is.null(compartment))
stopifnot(!is.null(S))
stopifnot(MetabVector_size(CModel_getMetabolites(model)) == 1)
# create a second metabolite called P with an initial
# concentration of 0. This metabolite is to be changed by reactions
P <- CModel_createMetabolite(model,"P", CCopasiObject_getObjectName(compartment), 0.0, "REACTIONS")
stopifnot(!is.null(P))
object <- CCopasiContainer_getObject(P,CCopasiObjectName("Reference=InitialConcentration"))
stopifnot(!is.null(object))
invisible(ObjectStdVector_push_back(changedObjects,object))
stopifnot(MetabVector_size(CModel_getMetabolites(model)) == 2)

# now we create a reaction
reaction <- CModel_createReaction(model,"reaction")
stopifnot(!is.null(reaction))
stopifnot(ReactionVector_size(CModel_getReactions(model)) == 1)
# reaction converts S to P
# we can set these on the chemical equation of the reaction
chemEq <- CReaction_getChemEq(reaction)
# S is a substrate with stoichiometry 1
invisible(CChemEq_addMetabolite(chemEq,CModelEntity_getKey(S), 1.0, "SUBSTRATE"))
# P is a product with stoichiometry 1
invisible(CChemEq_addMetabolite(chemEq,CModelEntity_getKey(P), 1.0, "PRODUCT"))
stopifnot(CChemEqElementVector_size(CChemEq_getSubstrates(chemEq)) == 1)
stopifnot(CChemEqElementVector_size(CChemEq_getProducts(chemEq)) == 1)
# this reaction is to be irreversible
invisible(CReaction_setReversible(reaction,FALSE))
stopifnot(CReaction_isReversible(reaction) == FALSE)

MV <- CModel_createModelValue(model,"K", 42.0)
# set the status to FIXED
invisible(CModelEntity_setStatus(MV,"FIXED"))
stopifnot(!is.null(MV))
object <- CCopasiContainer_getObject(MV,CCopasiObjectName("Reference=InitialValue"))
stopifnot(!is.null(object))
invisible(ObjectStdVector_push_back(changedObjects,object))
stopifnot(ModelValueVector_size(CModel_getModelValues(model)) == 1)

# now we ned to set a kinetic law on the reaction
# for this we create a user defined function
funDB <- CCopasiRootContainer_getFunctionList()
stopifnot(!is.null(funDB))

func <- CFunctionDB_createFunction(funDB,"My Rate Law","UserDefined")

rateLaw <- CFunctionDB_findFunction(funDB,"My Rate Law")

stopifnot(!is.null(rateLaw))

# now we create the formula for the function and set it on the function
formula <- "(1-0.4/(EXPONENTIALE^(temp-37)))*0.00001448471257*1.4^(temp-37)*substrate"

result <- CExpression_setInfix(func,formula)
stopifnot(result == TRUE)
# make the function irreversible
invisible(CFunction_setReversible(func,"TriFalse"))
# the formula string should have been parsed now
# and COPASI should have determined that the formula string contained 2 parameters (temp and substrate)
variables <- CFunction_getVariables(func)
# per default the usage of those parameters will be set to VARIABLE
index <- CFunction_getVariableIndex(func,"temp")
param <- CFunctionParameters_getParameter(variables,index)
stopifnot(CFunctionParameter_getUsage(param) == "VARIABLE")
# This is correct for temp, but substrate should get the usage SUBSTRATE in order
# for us to use the function with the reaction created above
# So we need to set the usage for "substrate" manually
index <- CFunction_getVariableIndex(func,"substrate")
param <- CFunctionParameters_getParameter(variables,index)
invisible(CFunctionParameter_setUsage(param,"SUBSTRATE"))

# set the rate law for the reaction
invisible(CReaction_setFunction(reaction,func))
stopifnot(!is.null(CReaction_getFunction(reaction)))

# COPASI also needs to know what object it has to assocuiate with the individual function parameters
# In our case we need to tell COPASI that substrate is to be replaced by the substrate of the reaction
# and temp is to be replaced by the global parameter K
invisible(CReaction_setParameterMapping(reaction,"substrate", CModelEntity_getKey(S)))
invisible(CReaction_setParameterMapping(reaction,"temp", CModelENtity_getKey(MV)))

# finally compile the model
# compile needs to be done before updating all initial values for
# the model with the refresh sequence
invisible(CModel_compileIfNecessary(model))

# now that we are done building the model, we have to make sure all
# initial values are updated according to their dependencies
invisible(CModel_updateInitialValues(model,changedObjects))

# save the model to a COPASI file
# we save to a file named example1.cps 
# and we want to overwrite any existing file with the same name
# Default tasks are automatically generated and will always appear in cps
# file unless they are explicitley deleted before saving.
invisible(CCopasiDataModel_saveModel(dataModel,"example7.cps", TRUE))

# export the model to an SBML file
# we save to a file named example1.xml, we want to overwrite any
# existing file with the same name and we want SBML L2V3
tryCatch(CCopasiDataModel_exportSBML(dataModel,"example7.xml", TRUE, 2, 3), error = function(e) {
  write("Error. Exporting the model to SBML failed.", stderr())
  quit(save = "default", status = 1, runLast = TRUE)
} )


