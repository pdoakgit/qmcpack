//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2019 developers.
//
// File developed by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//
// File created by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//////////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include <iostream>
#include "Particle/MCWalkerConfiguration.h"
#include "Particle/MCPopulation.hpp"

namespace qmcplusplus
{

TEST_CASE("MCWalkerConfiguration(MCPopulation&)", "[particle]")
{
  MCPopulation pop;
  MCWalkerConfiguration MCWC(pop);
}

void consumeMCW(MCWalkerConfiguration& mcw)
{
  mcw.setLocalEnergy(2.0);
}

}
