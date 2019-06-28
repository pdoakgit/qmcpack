//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2019 developers.
//
// File developed by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//
// File refactored from VMC.h
//////////////////////////////////////////////////////////////////////////////////////


#ifndef QMCPLUSPLUS_VMCBATCHED_H
#define QMCPLUSPLUS_VMCBATCHED_H
#include "QMCDrivers/QMCDriver.h"
namespace qmcplusplus
{
/** @ingroup QMCDrivers  ParticleByParticle
 * @brief Implements a VMC using particle-by-particle move. Threaded execution.
 */
class VMCBatched : public QMCDriver
{
public:
  /// Constructor.
  VMC(MCPopulation& pop, WaveFunctionPool& ppool, Communicate* comm);
  bool run();
  bool put(xmlNodePtr cur);
  //inline std::vector<RandomGenerator_t*>& getRng() { return Rng;}
private:
  int prevSteps;
  int prevStepsBetweenSamples;

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
