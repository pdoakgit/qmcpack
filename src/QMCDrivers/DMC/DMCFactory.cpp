//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Ken Esler, kpesler@gmail.com, University of Illinois at Urbana-Champaign
//                    Jeremy McMinnis, jmcminis@gmail.com, University of Illinois at Urbana-Champaign
//                    Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//                    Mark A. Berrill, berrillma@ornl.gov, Oak Ridge National Laboratory
//
// File created by: Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//////////////////////////////////////////////////////////////////////////////////////
    
    


#include "QMCDrivers/DMC/DMCFactory.h"
#include "QMCDrivers/DMC/DMC.h"
#include "Message/OpenMP.h"

#ifdef QMC_CUDA
#include "QMCDrivers/DMC/DMC_CUDA.h"
#endif

//#define PETA_DMC_TEST
namespace qmcplusplus
{

QMCDriver* DMCFactory::create(MCWalkerConfiguration& w, TrialWaveFunction& psi, QMCHamiltonian& h,
                              HamiltonianPool& hpool,WaveFunctionPool& ppool, Communicate* comm)
{
#ifdef QMC_CUDA
  if (GPU)
    return new DMCcuda (w, psi, h, ppool, comm);
#endif
  QMCDriver*  qmc = new DMC(w, psi, h, ppool, comm);
  qmc->setUpdateMode(PbyPUpdate);
  return qmc;
}
}
