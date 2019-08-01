//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Jeremy McMinnis, jmcminis@gmail.com, University of Illinois at Urbana-Champaign
//                    Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//                    Mark A. Berrill, berrillma@ornl.gov, Oak Ridge National Laboratory
//
// File created by: Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//////////////////////////////////////////////////////////////////////////////////////


#ifndef QMCPLUSPLUS_VMC_H
#define QMCPLUSPLUS_VMC_H
#include "QMCDrivers/QMCDriver.h"
#include "QMCDrivers/CloneManager.h"
namespace qmcplusplus
{
/** @ingroup QMCDrivers  ParticleByParticle
 * @brief Implements a VMC using particle-by-particle move. Threaded execution.
 */
class VMC : public QMCDriver, public CloneManager
{
public:

  struct Repeat
  {
      Repeat() : repeat(false), last_driver_steps(0), last_driver_steps_between_samples(0) {}
      Repeat(int steps, int steps_between_samples) : repeat(true), last_driver_steps(steps), last_driver_steps_between_samples(steps_between_samples) {}
    operator bool() { return repeat; }
      bool repeat;
      int  last_driver_steps;
      int last_driver_steps_between_samples;
  };

  /// Constructor.
  VMC(MCWalkerConfiguration& w, TrialWaveFunction& psi, QMCHamiltonian& h, WaveFunctionPool& ppool, VMC::Repeat repeat, Communicate* comm);
  bool run();
  bool put(xmlNodePtr cur);
  QMCRunType getRunType() { return QMCRunType::VMC; }
  //inline std::vector<RandomGenerator_t*>& getRng() { return Rng;}
private:
  Repeat repeat_;
  int prevSteps;
  int prevStepsBetweenSamples;
  // Flag to preserve QMCPACK behavior for repeated VMC sections
  bool vmc_repeat_;
  ///Ways to set rn constant
  RealType logoffset, logepsilon;
  ///option to enable/disable drift equation or RN for VMC
  std::string UseDrift;
  ///check the run-time environments
  void resetRun();
  ///copy constructor
  VMC(const VMC&) = delete;
  /// Copy operator (disabled).
  VMC& operator=(const VMC&) = delete;
};
} // namespace qmcplusplus

#endif
