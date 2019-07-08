#include DFCreate.hpp

namespace qmcplusplus
{

template<>
QMCDriverInterface*
DFCreator<QMCDriverFactors::QMCRunType::VMC_BATCH>::operator(MCWalkerConfiguration& w,
			       TrialWaveFunction& psi,
			       QMCHamiltonian& h,
			       ParticleSetPool& ptclpool,
			       HamiltonianPool& hpool,
			       WaveFunctionPool& ppool,
			       Communicate* comm)
{
    int np = omp_get_max_threads();
  //(SPACEWARP_MODE,MULTIPE_MODE,UPDATE_MODE)
  QMCDriverInterface* qmc = nullptr;
  if (VMCMode == 0 || VMCMode == 1) //(0,0,0) (0,0,1)
  {
    qmc = new VMC(w, psi, h, ppool, comm);
  }
  //else if(VMCMode == 2) //(0,1,0)
  //{
  //  qmc = new VMCMultiple(w,psi,h);
  //}
  //else if(VMCMode == 3) //(0,1,1)
  //{
  //  qmc = new VMCPbyPMultiple(w,psi,h);
  //}
  else if (VMCMode == 2 || VMCMode == 3)
  {
    qmc = new CSVMC(w, psi, h, ppool, comm);
  }
  //#if !defined(QMC_COMPLEX)
  //    else if(VMCMode == 6) //(1,1,0)
  //    {
  //      qmc = new VMCMultipleWarp(w,psi,h, ptclpool);
  //    }
  //    else if(VMCMode == 7) //(1,1,1)
  //    {
  //      qmc = new VMCPbyPMultiWarp(w,psi,h, ptclpool);
  //    }
  //#endif
  //     else if(VMCMode == 8) //(only possible for WFMC run)
  //     {
  //       qmc = new WFMCSingleOMP(w,psi,h,hpool,ppool);
  //     }
  qmc->setUpdateMode(VMCMode & 1);
  return qmc;
}

}
