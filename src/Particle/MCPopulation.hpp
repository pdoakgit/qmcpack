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

#ifndef QMCPLUSPLUS_MCPOPULATION_HPP
#define QMCPLUSPLUS_MCPOPULATION_HPP

#include "Particle/ParticleSet.h"
#include "Particle/Walker.h"

namespace qmcplusplus
{

class MCPopulation
{
public:
  using PopulationWalker = Walker<QMCTraits, PtclOnLatticeTraits>;
  using Properties = PopulationWalker::PropertyContainer_t;
private:
  int num_global_walkers_;
  int num_local_walkers_;
  int max_samples_;
  Properties properties_;
  ParticleSet ions_;
  std::vector<int> walker_offsets_;
public:
  const ParticleSet& get_ions() const {return ions_;}
  const int get_max_samples() const { return max_samples_; }
  const Properties& get_properties() const {return properties_; }
  const int get_num_global_walkers() const {return num_global_walkers_; }
  const std::vector<int>& get_walker_offsets() const {return walker_offsets_; }
};


}

#endif
