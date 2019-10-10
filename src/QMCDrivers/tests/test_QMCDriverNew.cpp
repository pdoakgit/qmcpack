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

#include "QMCDrivers/QMCDriverNew.h"
#include "Utilities/Testing/SetupPools.h"
#include "QMCDrivers/tests/QMCDriverNewTestWrapper.h"
#include "QMCDrivers/tests/ValidQMCInputSections.h"
#include "Message/Communicate.h"
#include "QMCDrivers/MCPopulation.h"
#include "Concurrency/Info.hpp"
#include "Concurrency/UtilityFunctions.hpp"

namespace qmcplusplus
{

TEST_CASE("QMCDriverNew tiny case", "[drivers]")
{
  using namespace testing;
  Concurrency::OverrideMaxThreads<> override(8);

  RandomNumberControl random_control(8);
  SetupPools pools(random_control);

  Libxml2Document doc;
  bool okay = doc.parseFromString(valid_vmc_input_sections[valid_vmc_input_vmc_tiny_index]);
  REQUIRE(okay);
  xmlNodePtr node = doc.getRoot();
  QMCDriverInput qmcdriver_input(1);
  qmcdriver_input.readXML(node);

  MCPopulation population(1, pools.particle_pool->getParticleSet("e"), pools.wavefunction_pool->getPrimary(), pools.hamiltonian_pool->getPrimary());
  QMCDriverNewTestWrapper qmcdriver(std::move(qmcdriver_input), population, *(pools.wavefunction_pool->getPrimary()),
                                    *(pools.hamiltonian_pool->getPrimary()), *(pools.wavefunction_pool), random_control, pools.comm);

  // setStatus must be called before process
  std::string root_name{"Test"};
  //For later sections this appears to contain important state.
  std::string prev_config_file{""};

  qmcdriver.setStatus(root_name, prev_config_file, false);
  // We want to express out expectations of the QMCDriver state machine so we catch
  // changes to it over time.
  qmcdriver.set_calc_walker_answers(1, 1);
  REQUIRE(qmcdriver.getBranchEngine() == nullptr);
  qmcdriver.process(node);
  REQUIRE(qmcdriver.getBranchEngine() != nullptr);
  // the walker calculation is done in the derived class, the mock always gives 32.
  REQUIRE(qmcdriver.get_living_walkers() == 1);
  // What else should we expect after process
}


TEST_CASE("QMCDriverNew integration", "[drivers]")
{
  using namespace testing;
  Concurrency::OverrideMaxThreads<> override(8);

  RandomNumberControl random_control(8);
  SetupPools pools(random_control);

  Libxml2Document doc;
  bool okay = doc.parseFromString(valid_vmc_input_sections[valid_vmc_input_vmc_batch_index]);
  REQUIRE(okay);
  xmlNodePtr node = doc.getRoot();
  QMCDriverInput qmcdriver_input(3);
  qmcdriver_input.readXML(node);

  MCPopulation population(4, pools.particle_pool->getParticleSet("e"), pools.wavefunction_pool->getPrimary(), pools.hamiltonian_pool->getPrimary());
  QMCDriverNewTestWrapper qmcdriver(std::move(qmcdriver_input), population, *(pools.wavefunction_pool->getPrimary()),
                                    *(pools.hamiltonian_pool->getPrimary()), *(pools.wavefunction_pool), random_control, pools.comm);

  // setStatus must be called before process
  std::string root_name{"Test"};
  //For later sections this appears to contain important state.
  std::string prev_config_file{""};

  qmcdriver.setStatus(root_name, prev_config_file, false);
  // We want to express out expectations of the QMCDriver state machine so we catch
  // changes to it over time.
  REQUIRE(qmcdriver.getBranchEngine() == nullptr);
  qmcdriver.process(node);
  REQUIRE(qmcdriver.getBranchEngine() != nullptr);
  REQUIRE(qmcdriver.get_living_walkers() == 32);
  // What else should we expect after process
}

} // namespace qmcplusplus
