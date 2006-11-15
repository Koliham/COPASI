%{

#include "model/CModel.h"

%}

class CModel : public CModelEntity
{
    public:
    /**
     * Enum of valid volume units
     */
    enum VolumeUnit {m3 = 0, l, ml, microl, nl, pl, fl};

    /**
     * String representation of valid volume units
     */
    //static const char * VolumeUnitNames[];

    /**
     *  Enum of valid time units
     */
    enum TimeUnit {d = 0, h, min, s, ms, micros, ns, ps, fs};

    /**
     *  Enum of valid quantitye units
     */
    enum QuantityUnit {Mol = 0, mMol, microMol, nMol, pMol, fMol, number};


    /**
     * Enum of valid model types.
     */
    enum ModelType {deterministic = 0, stochastic};


    /**
     *  Default constructor
     */
    CModel();

    /**
     *  Copy construnctor
     *  @param "const CModel &" src
     */
    CModel(const CModel & src);

    /**
     * Destructor
     */
    ~CModel();        

    /**
     *  Build the Stoichiometry Matrix from the chemical equations of the steps
     */
    void buildStoi();

    /**
     *  Build the Reduced Stoichiometry Matrix from the LU decomposition
     */
    void buildRedStoi();

    /**
     *  Build the Moities based on the LU decomposition
     */
    void buildMoieties();

    /**
     * Return the metabolites of this model
     * @return CCopasiVectorN< CMetab > & metabolites
     */
    CCopasiVector< CMetab > & getMetabolites();

    /**
     * Retrieves the vector of metabolites as it is used in the reduced model.
     * @return const CCopasiVectorN< CMetab > &metabolites
     */
    CCopasiVector< CMetab > & getMetabolitesX();

    /**
     *  Get the number of total metabolites
     *  @return C_INT32 totMetab
     */
    unsigned C_INT32 getNumMetabs() const;

    /**
     *  Get the number of variable metabolites
     *  @return unsigned C_INT32 totMetab
     */
    unsigned C_INT32 getNumVariableMetabs() const;

    /**
     *  Get the number of metabolites determinet by ODEs
     *  @return unsigned C_INT32 dimension
     */
    unsigned C_INT32 getNumODEMetabs() const;

    /**
     *  Get the number of independent metabolites determined by reactions
     *  @return unsigned C_INT32 dimension
     */
    unsigned C_INT32 getNumIndependentMetabs() const;

    /**
     *  Get the number of dependent metabolites
     *  @return unsigned C_INT32 dimension
     */
    unsigned C_INT32 getNumDependentMetabs() const;

    /********** additional values *****************************/

    /**
     * Return the non concentration values of this model
     * @return CCopasiVectorN< CModelValue > & values
     */
    CCopasiVectorN< CModelValue > & getModelValues();

    /**
     *  Get the number of non concentration values
     *  @return C_INT32
     */
    unsigned C_INT32 getNumModelValues() const;

    /**
     * Return the vector of reactions
     * @return CCopasiVectorNS <CReaction> & reactions
     */
    CCopasiVectorNS < CReaction > & getReactions();

    /**
     * Get the total steps
     * @return unsigned C_INT32 total steps;
     */
    unsigned C_INT32 getTotSteps() const;

    /**
     * Return the comments of this model Wei Sun
     * @return string
     */
    const std::string & getComments() const;

    /**
     * Return the key of this model
     * @return string key
     */
    const std::string & getKey() const;

    /**
     * Set the title of this model
     * @param "const string &" title title for this model
     */
    bool setTitle(const std::string &title);

    /**
     * Set the title of this model
     * @param "const string &" comments comments for this model
     */
    void setComments(const std::string &comments);

    /**
     * Set the start time for modeling
     * @param const C_FLOAT64 & time
     */
    void setInitialTime(const C_FLOAT64 & time);

    /**
     * Retreive the initial time
     * @return const C_FLOAT64 & time
     */
    const C_FLOAT64 & getInitialTime() const;


    /**
     * Retreive the actual model time
     * @return const C_FLOAT64 & time
     */
    const C_FLOAT64 & getTime() const;

    /************** compartments *********************/

    /**
     * Return the compartments of this model
     * @return CCopasiVectorNS < CCompartment > *
     */
    CCopasiVectorNS < CCompartment > & getCompartments();

    /**
     *  Get the Stoichiometry Matrix of this Model
     */
    const CMatrix < C_FLOAT64 > & getStoi() const;

    /**
     *  Get the Reduced Stoichiometry Matrix of this Model
     */
    const CMatrix < C_FLOAT64 >& getRedStoi() const;

    /**
     *  Get the reordered stoichiometry matrix of this model
     */
    const CMatrix < C_FLOAT64 >& getStoiReordered() const;

    /**
     * Return the mMoieties of this model
     * @return CCopasiVectorN < CMoiety > &
     */
    const CCopasiVector < CMoiety > & getMoieties() const;

    /**
     * Returns the index of the metab
     */
    C_INT32 findMetabByName(const std::string & Target) const;

    /**
     * Returns the index of the Moiety
     */
    C_INT32 findMoiety(std::string &Target) const;

    /**
     * Get the current state of the model, i.e., all current model
     * quantities.
     * @return const CState & initialState
     */
    const CState & getInitialState() const;

    /**
     * Get the current state of the model, i.e., all current model
     * quantities.
     * @return const CState & currentState
     */
    const CState & getState() const;

    /**
     * Set all initial model quantities to the one given by the state and
     * updates moieties and metabolite concentrations.
     * @param const CState & state
     */
    void setInitialState(const CState & state);

    /**
     * Set the unit for volumes. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const std::string & name
     * @return bool success
     */
    bool setVolumeUnit(const std::string & name);

    /**
     * Set the unit for volumes. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const CModel::VolumeUnit & unit
     * @return bool success
     */
    bool setVolumeUnit(const CModel::VolumeUnit & unit);

    /**
     * Get the unit for volumes
     * @return std::string volumeUnit
     */
    std::string getVolumeUnitName() const;

    /**
     * Get the unit for volumes
     * @return CModel::VolumeUnit volumeUnit
     */
    CModel::VolumeUnit getVolumeUnitEnum() const;

    /**
     * Set the unit for time. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const std::string & name
     * @return bool success
     */
    bool setTimeUnit(const std::string & name);

    /**
     * Set the unit for time. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const const CModel::TimeUnit & unit
     * @return bool success
     */
    bool setTimeUnit(const CModel::TimeUnit & unit);

    /**
     * Get the unit for time
     * @return std::string timeUnit
     */
    std::string getTimeUnitName() const;

    /**
     * Get the unit for time
     * @return CModel::TimeUnit timeUnit
     */
    CModel::TimeUnit getTimeUnitEnum() const;

    /**
     * Set the unit for quantities. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const std::string & name
     * @return bool success
     */
    bool setQuantityUnit(const std::string & name);

    /**
     * Set the unit for quantities. If copasi recognises
     * the unit the conversion factors are set accordingly
     * and true is returned.
     * @param const CModel::QuantityUnit & unit
     * @return bool success
     */
    bool setQuantityUnit(const CModel::QuantityUnit & unit);

    /**
     * Get the unit for quantities
     * @return std::string quantityUnit
     */
    std::string getQuantityUnitName() const;

    /**
     * Get the unit for quantities
     * @return CModel::QuantityUnit quantityUnit
     */
    CModel::QuantityUnit getQuantityUnitEnum() const;

    /**
     * Set the type of the model
     * @param const ModelType & modelType
     */
    void setModelType(const ModelType & modelType);

    /**
     * Retreive the type of the model.
     * @return const ModelType & modelType
     */
    const ModelType & getModelType() const;

    /**
     *  Get the conversion factor quantity -> number
     */
    const C_FLOAT64 & getQuantity2NumberFactor() const;

    /**
     *  Get the conversion factor number -> quantity
     */
    const C_FLOAT64 & getNumber2QuantityFactor() const;

    /* convenience methods: */
    std::string getConcentrationUnitName() const;
    std::string getConcentrationRateUnitName() const;
    std::string getQuantityRateUnitName() const;

    /************  create/remove model entities *******************/

    /**
     * Add a metabolite to the model
     * @param const std::string & name
     * @param const std::string & compartment
     * @param const C_FLOAT64 & iconc (default 1.0)
     * @param const CMetab::Status & status (default CMetab::METAB_VARIABL)
     * @return bool success (false if failed)
     * @see CMetab for more information
     */
    CMetab* createMetabolite(const std::string & name,
                             const std::string & compartment,
                             const C_FLOAT64 & iconc = 1.0,
                             const CModelEntity::Status & status = CModelEntity::REACTIONS);

    /* Remove a metabolite from the model */
    bool removeMetabolite(const std::string & key,
                          const bool & recursive = true);


    /**
     * Add a compartment to the model
     * @param const std::string &name
     * @param const C_FLOAT64 & volume (default 1.0)
     * @return bool success (false if failed)
     */
    CCompartment* createCompartment(const std::string & name,
                                    const C_FLOAT64 & volume = 1.0);

    /* Remove a Compartment from the model */
    bool removeCompartment(const std::string & key,
                           const bool & recursive = true);

    /**
     * Add a new rection to the model
     * @param const std::string &name
     * @return bool success (false if failed)
     */
    CReaction* createReaction(const std::string &name);


    /* Remove a reaction from the model*/
    bool removeReaction(const std::string & key,
                        const bool & recursive = true);

    /**
     * Add a non concentration value to the model
     * @param const std::string &name
     * @param const C_FLOAT64 & value (default 0.0)
     */
    CModelValue* createModelValue(const std::string & name,
                                  const C_FLOAT64 & value = 0.0);

    bool removeModelValue(const std::string & key,
                          const bool & recursive = true);

   /**
     * Check whether the model contains reversible reactions
     * @return bool hasReversibleReaction
     */
    bool hasReversibleReaction() const;

    /**
     * check if the model is suitable for stochastic simulation
     **/
    std::string suitableForStochasticSimulation() const;

    /**
     * Check whether the model is autonomous
     * @return bool isAutonomous
     */
    bool isAutonomous() const;

%extend
{
    /**
     *  Get the number of compartments 
     *  @return C_INT32 getCompartments().size()
     */
    unsigned C_INT32 getNumCompartments() const
    {
    	return self->getCompartments().size();
    }

    /**
     *  Get the number of reactions
     *  @return C_INT32 getReactions().size()
     */
    unsigned C_INT32 getNumReactions() const
    {
    	return self->getReactions().size();
    }

		
}


};


