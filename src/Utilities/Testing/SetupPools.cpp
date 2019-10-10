//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2019 QMCPACK developers.
//
// File developed by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//
// File created by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <catch.hpp>
#include "Concurrency/UtilityFunctions.hpp"
#include "Utilities/Testing/SetupPools.h"

namespace qmcplusplus
{
namespace testing
{
SetupPools::SetupPools(RandomNumberControl& random_control) : random_control_(random_control)
{
  OHMMS::Controller->initialize(0, NULL);
  comm = OHMMS::Controller;

  std::cout << "SetupPools::SetupPools()\n";
  Concurrency::OverrideMaxThreads<> override(8);
  
  particle_pool.reset(new ParticleSetPool(mpp(random_control, comm)));
  wavefunction_pool.reset(new WaveFunctionPool(wfp(random_control, comm, particle_pool.get())));
  wavefunction_pool->setPrimary(wavefunction_pool->getWaveFunction("psi0"));
  hamiltonian_pool.reset(new HamiltonianPool(mhp(comm, particle_pool.get(), wavefunction_pool.get())));
}

} // namespace testing
} // namespace qmcplusplus
