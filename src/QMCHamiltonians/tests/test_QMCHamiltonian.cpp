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


#include "catch.hpp"

#include "type_traits/template_types.hpp"
#include "QMCHamiltonians/QMCHamiltonian.h"
#include "Utilities/Testing/SetupPools.h"

namespace qmcplusplus
{
TEST_CASE("QMCHamiltonian::flex_evaluate", "[hamiltonian]")
{
  using namespace testing;
  OHMMS::Controller->initialize(0, NULL);
  Communicate* comm{OHMMS::Controller};

  SetupPools pools(comm);

  TrialWaveFunction twf(pools.comm);

  std::vector<QMCHamiltonian> hamiltonians;
  hamiltonians.emplace_back(*(pools.hamiltonian_pool->getPrimary()));
  hamiltonians.emplace_back(*(pools.hamiltonian_pool->getPrimary()));

  std::vector<ParticleSet> elecs;
  elecs.emplace_back(*(pools.particle_pool->getParticleSet("e")));
  elecs.emplace_back(*(pools.particle_pool->getParticleSet("e")));

  // TODO: finish initializing the elecs.
  //std::vector<QMCHamiltonian::RealType> local_energies(QMCHamiltonian::flex_evaluate(makeRefVector<QMCHamiltonian>(hamiltonians), makeRefVector<ParticleSet>(elecs)));

  //TODO: Would be nice to check some values but I think the system needs a little more setup
}

} // namespace qmcplusplus
