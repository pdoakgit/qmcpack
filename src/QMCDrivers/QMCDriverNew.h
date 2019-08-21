//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2019 QMCPACK developers.
//
// File developed by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//
// File refactored from: QMCDriver.h
//////////////////////////////////////////////////////////////////////////////////////


/**
 * @file
 * Declaration of QMCDriverNew
 *
 * This will replace QMCDriver once unified drivers are finished
 * the general documentation from QMCDriver.h must be moved before then
 */
#ifndef QMCPLUSPLUS_QMCDRIVERNEW_H
#define QMCPLUSPLUS_QMCDRIVERNEW_H

#include <type_traits>

#include "Configuration.h"
#include "Utilities/PooledData.h"
#include "Utilities/NewTimer.h"
#include "QMCWaveFunctions/TrialWaveFunction.h"
#include "QMCApp/WaveFunctionPool.h"
#include "QMCHamiltonians/QMCHamiltonian.h"
#include "Estimators/EstimatorManagerBase.h"
#include "Particle/MCPopulation.h"
#include "QMCDrivers/Crowd.h"
#include "QMCDrivers/QMCDriverInterface.h"
#include "QMCDrivers/GreenFunctionModifiers/DriftModifierBase.h"
#include "QMCDrivers/SimpleFixedNodeBranch.h"
#include "QMCDrivers/BranchIO.h"
#include "QMCDrivers/QMCDriverInput.h"
class Communicate;

namespace qmcplusplus
{
//forward declarations: Do not include headers if not needed
class HDFWalkerOutput;
class TraceManager;

/** @ingroup QMCDrivers
 * @{
 * @brief QMCDriverNew Base class for Unified Drivers
 *
 * General Principals
 * Parameters unchanged from input by driver are not copied into class state
 * The driver state machine should be simple.
 *
 */
class QMCDriverNew : public QMCDriverInterface, public MPIObjectBase
{
public:
  using RealType              = QMCTraits::RealType;
  using IndexType             = QMCTraits::IndexType;
  using FullPrecisionRealType = QMCTraits::FullPrecRealType;

  /** separate but similar to QMCModeEnum
   *  
   *  a code smell
   */
  enum
  {
    QMC_UPDATE_MODE,
    QMC_MULTIPLE,
    QMC_OPTIMIZE,
    QMC_WARMUP
  };

  using MCPWalker = MCPopulation::MCPWalker;
  using Buffer    = MCPWalker::Buffer_t;

  /** bits to classify QMCDriver
   *
   * - qmc_driver_mode[QMC_UPDATE_MODE]? particle-by-particle: walker-by-walker
   * - qmc_driver_mode[QMC_MULTIPLE]? multiple H/Psi : single H/Psi
   * - qmc_driver_mode[QMC_OPTIMIZE]? optimization : vmc/dmc/rmc
   */
  std::bitset<QMC_MODE_MAX> qmc_driver_mode;

  /// whether to allow traces
  bool allow_traces;
  /// traces xml
  xmlNodePtr traces_xml;

  /// Constructor.
  QMCDriverNew(QMCDriverInput&& input,
               MCPopulation& population,
               TrialWaveFunction& psi,
               QMCHamiltonian& h,
               WaveFunctionPool& ppool,
               Communicate* comm);

  virtual ~QMCDriverNew();

  ///return current step
  inline IndexType current() const { return current_step_; }

  /** Set the status of the QMCDriver
   * @param aname the root file name
   * @param h5name root name of the master hdf5 file containing previous qmcrun
   * @param append if ture, the run is a continuation of the previous qmc
   *
   * All output files will be of
   * the form "aname.s00X.suffix", where "X" is number
   * of previous QMC runs for the simulation and "suffix"
   * is the suffix for the output file.
   */
  void setStatus(const std::string& aname, const std::string& h5name, bool append);

  /** add QMCHamiltonian/TrialWaveFunction pair for multiple
   * @param h QMCHamiltonian
   * @param psi TrialWaveFunction
   *
   * *Multiple* drivers use multiple H/Psi pairs to perform correlated sampling
   * for energy difference evaluations.
   */
  void add_H_and_Psi(QMCHamiltonian* h, TrialWaveFunction* psi);

  void setupWalkers();

  void putWalkers(std::vector<xmlNodePtr>& wset);

  ///set the BranchEngineType
  void setBranchEngine(SimpleFixedNodeBranch* be) { branchEngine = be; }

  ///return BranchEngineType*
  SimpleFixedNodeBranch* getBranchEngine() { return branchEngine; }

  virtual IndexType calc_default_local_walkers() = 0;

  int addObservable(const std::string& aname);

  RealType getObservable(int i);

  ///set global offsets of the walkers
  void setWalkerOffsets();

  //virtual std::vector<RandomGenerator_t*>& getRng() {}

  ///return the random generators
  inline std::vector<RandomGenerator_t*>& getRng() { return Rng; }

  ///return the i-th random generator
  inline RandomGenerator_t& getRng(int i) { return (*Rng[i]); }

  std::string getEngineName() { return QMCType; }
  unsigned long getDriverMode() { return qmc_driver_mode.to_ulong(); }
  IndexType get_walkers_per_crowd() const { return walkers_per_crowd_; }

  /** @ingroup Legacy interface to be dropped
   *  @{
   */
  bool put(xmlNodePtr cur) { return false; };

  /** QMCDriverNew driver will eventuall ignore cur
   *
   *  This is the shared entry point
   *  from QMCMain so cannot be updated yet
   */
  void process(xmlNodePtr cur);

  /** should be set in input don't see a reason to set individually
   * @param pbyp if true, use particle-by-particle update
   */
  inline void setUpdateMode(bool pbyp) { qmc_driver_mode[QMC_UPDATE_MODE] = pbyp; }

  void putTraces(xmlNodePtr txml) {}
  void requestTraces(bool allow_traces) {}
  /** }@ */

protected:
  QMCDriverInput qmcdriver_input_;

  std::vector<Crowd> crowds_;
  IndexType walkers_per_crowd_;

  ///branch engine
  SimpleFixedNodeBranch* branchEngine;
  ///drift modifer
  DriftModifierBase* DriftModifier;

  ///the number to delay updates by
  int k_delay;

  /** period of recording walker configurations
   *
   * Default is 0 indicating that only the last configuration will be saved.
   */
  int walker_dump_period;


  IndexType current_step_;

  ///counter for number of moves accepted
  IndexType nAccept;

  ///counter for number of moves /rejected
  IndexType nReject;


  ///maximum cpu in secs
  RealType MaxCPUSecs;

  ///Time-step factor \f$ 1/(2\tau)\f$
  RealType m_oneover2tau;
  ///Time-step factor \f$ \sqrt{\tau}\f$
  RealType m_sqrttau;

  ///type of qmc: assigned by subclasses
  std::string QMCType;
  ///the root of h5File
  std::string h5FileRoot;
  ///root of all the output files
  std::string RootName;


  ///the entire (or on node) walker population
  MCPopulation& population_;

  ///trial function
  TrialWaveFunction& Psi;

  ///Hamiltonian
  QMCHamiltonian& hamiltonian_;

  WaveFunctionPool& psiPool;

  ///Observables manager
  EstimatorManager* Estimators;

  ///record engine for walkers
  HDFWalkerOutput* wOut;


  ///a list of TrialWaveFunctions for multiple method
  std::vector<TrialWaveFunction*> Psi1;

  ///a list of QMCHamiltonians for multiple method
  std::vector<QMCHamiltonian*> H1;

  ///Random number generators
  std::vector<RandomGenerator_t*> Rng;

  ///a list of mcwalkerset element
  std::vector<xmlNodePtr> mcwalkerNodePtr;

  ///a list of timers
  std::vector<NewTimer*> myTimers;

  ///temporary storage for drift
  ParticleSet::ParticlePos_t drift;

  ///temporary storage for random displacement
  ParticleSet::ParticlePos_t deltaR;

  /** @ingroup Driver mutable input values
   *
   *  they should be limited to values that can be changed from input
   *  or are live state.
   *  @{
   */
  int num_crowds_;

  ///the number of saved samples
  IndexType target_samples_;

  /// the number of blocks between recomptePsi
  IndexType nBlocksBetweenRecompute;

  // ///alternate method of setting QMC run parameters
  // IndexType nStepsBetweenSamples;
  // ///samples per thread
  // IndexType nSamplesPerThread;

  //  TODO: restart
  //  /** period of dumping walker configurations and everything else for restart
  //  *
  //  * The unit is a block.
  //  */
  // int check_point_period_;

  /** }@ */
public:
  ///Copy Constructor (disabled).
  QMCDriverNew(const QMCDriverNew&) = delete;
  ///Copy operator (disabled).
  QMCDriverNew& operator=(const QMCDriverNew&) = delete;

  bool putQMCInfo(xmlNodePtr cur);

  void addWalkers(int nwalkers, const ParticleAttrib<TinyVector<QMCTraits::RealType,3>>& positions);

  int get_num_crowds() { return num_crowds_; }
  /** record the state of the block
   * @param block current block
   *
   * virtual function with a default implementation
   */
  virtual void recordBlock(int block);

  /** finalize a qmc section
   * @param block current block
   * @param dumpwalkers if true, dump walkers
   *
   * Accumulate energy and weight is written to a hdf5 file.
   * Finialize the estimators
   */
  bool finalize(int block, bool dumpwalkers = true);

  int rotation;
  std::string getRotationName(std::string RootName);
  std::string getLastRotationName(std::string RootName);

  NewTimer* checkpointTimer;

private:
  friend std::ostream& operator<<(std::ostream& o_stream, const QMCDriverNew& qmcd);
};
/**@}*/
} // namespace qmcplusplus

#endif
