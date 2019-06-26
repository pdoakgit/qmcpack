//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Jeremy McMinnis, jmcminis@gmail.com, University of Illinois at Urbana-Champaign
//                    Raymond Clay III, j.k.rofling@gmail.com, Lawrence Livermore National Laboratory
//
// File created by: Jeremy McMinnis, jmcminis@gmail.com, University of Illinois at Urbana-Champaign
//////////////////////////////////////////////////////////////////////////////////////


#include "QMCDrivers/RMC/RMCFactory.h"
#include "QMCDrivers/RMC/RMC.h"
#include "Message/OpenMP.h"

namespace qmcplusplus
{
    std::unique_ptr<QMCDriver> RMCFactory::create(MCWalkerConfiguration& w,
                              TrialWaveFunction& psi,
                              QMCHamiltonian& h,
                              ParticleSetPool& ptclpool,
                              HamiltonianPool& hpool,
                              WaveFunctionPool& ppool,
                              Communicate* comm)
{
  //(SPACEWARP_MODE,MULTIPE_MODE,UPDATE_MODE)
  std::unique_ptr<QMCDriver> qmc;
#ifdef QMC_CUDA
  APP_ABORT("RMCFactory::create. RMC is not supported on GPU.\n");
#endif
  // This bitset stuff is both unecessary and unwise
  if (RMCMode == 0 || RMCMode == 1) //(0,0,0) (0,0,1) pbyp and all electron
  {
      qmc = std::make_unique<RMC>(w, psi, h, ppool, comm);
  }
  qmc->setUpdateMode(RMCMode & 1);
  return qmc;
}
} // namespace qmcplusplus
