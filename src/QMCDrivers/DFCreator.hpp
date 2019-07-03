#ifndef QMCPLUSPLUS_DFCREATOR_HPP
#define QMCPLUSPLUS_DFCREATOR_HPP

namespace qmcplusplus
{
template<QMCDriverFactory::QMCRunType RT>
class DFCreator
{
  
  xmlNodePtr myNode;
  QMCDriverInterface* operator(MCWalkerConfiguration& w,
			       TrialWaveFunction& psi,
			       QMCHamiltonian& h,
			       ParticleSetPool& ptclpool,
			       HamiltonianPool& hpool,
			       WaveFunctionPool& ppool,
			       Communicate* comm);
}

}

#endif
