#ifndef OHMMS_NONLINEAR_FITTING_H
#define OHMMS_NONLINEAR_FITTING_H

#include "Numerics/SlaterTypeOrbital.h"
#include "Numerics/OneDimIntegration.h"
#include "Optimize/Minimize.h"

class Any2Slater : public MinimizeFunction {
  
  public:

    typedef OneDimGridFunctor<scalar> SourceType;
    typedef LogGrid<scalar>           GridType;

    Any2Slater(SourceType& in): Source(in),
                                cg_tolerance(1e-6),cg_stepsize(0.001),cg_epsilon(1e-6){ 
      OptParams.resize(1);

      psi_sq.resize(Source.size());
      scalar r2psi2=-10000.0;
      int igMax=-1;
      for(int ig=1; ig<Source.size(); ig++) {
        psi_sq[ig]=Source(ig)*Source(ig);
        scalar r=Source.r(ig);
        scalar t=r*r*psi_sq[ig];
        if(t>r2psi2) {r2psi2=t;igMax=ig;}
      }
      //integrate_RK2_forward(psi_sq,psi_norm);

      igMax+=2;

      Target.N=1;
      Target.Power=0;
      Target.Z=1/Source.r(igMax);

      Target.reset();
      Source.grid().locate(0.2*Source.r(igMax));
      minIndex=Source.grid().Loc;

      cout << "Initial exponent " << Target.Z << " at " << Source.r(igMax) << endl;
      cout << "The index of the cutoff " << minIndex << " at " << Source.r(minIndex) << endl;
      OptParams[0]=Target.Z;
    }

    bool put(xmlNodePtr cur) { mPtr=cur; return true;}

    ///return the number of optimizable parameters
    int NumParams() { return OptParams.size(); }

    ///assign optimization parameter i
    scalar& Params(int i) { return OptParams[i]; }
    ///return optimization parameter i
    scalar Params(int i) const { return OptParams[i]; }

    scalar Cost() {
      Target.Z=OptParams[0];
      //Target.Norm=OptParams[1];
      Target.reset();
      scalar del=0.0;
      for(int ig=minIndex; ig<Source.size(); ig++) {
        //scalar y= Target.f((*myGrid)(ig))-Source[ig];
        scalar r=Source.r(ig);
        scalar y= Target.f(r);
        scalar t=r*r*(y*y*-psi_sq[ig]);
        del += t*t;
      }
      return del;
    }

    void WriteStuff() { 
      cout << "Slater Z = " << Target.Z <<  "    Norm = " << Target.Norm << endl;
    }

    bool optimize() {
      ConjugateGradient CG;
      CG.Tolerance = cg_tolerance;
      CG.StepSize = cg_stepsize;
      CG.epsilon = cg_epsilon;
      CG.Minimize(*this);
      return true;
    }
 
  private:

    SourceType& Source;
    scalar cg_tolerance;
    scalar cg_stepsize;
    scalar cg_epsilon;
    int minIndex;
    xmlNodePtr mPtr;
    GenericSTO<scalar> Target;
    vector<scalar> OptParams;
    Vector<scalar> psi_sq, psi_norm;
  };
#endif
