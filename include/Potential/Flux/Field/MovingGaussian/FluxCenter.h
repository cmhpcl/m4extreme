// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FLUXCENTER_INCLUDED_)
#define FLUX_FLUXCENTER_INCLUDED_

#include "Clock/Clock.h"
#include "Set/Algebraic/AlgLib.h"

namespace Flux
{
  //
  class FluxCenter
  {
  public:
    FluxCenter(Clock *Chronos_, 
	       const Set::VectorSpace::Vector &x_initial_);
    FluxCenter(const Flux::FluxCenter &);

    virtual ~FluxCenter(){};

    const Set::VectorSpace::Vector &GetFluxCenter();
    virtual void Update(){};
    void Reset(const double & reset_time_,
	       const Set::VectorSpace::Vector & x_initial_);

  protected:
    Clock *Chronos;
    double reset_time;
    Set::VectorSpace::Vector x_initial;
    Set::VectorSpace::Vector x_center;
  };

  //
  //
  class UniformLinearFluxCenter : public FluxCenter
  {
  public:
    UniformLinearFluxCenter(Clock *Chronos_, 
			    const Set::VectorSpace::Vector &x_initial_, 
			    const Set::VectorSpace::Vector &velocity_);
    UniformLinearFluxCenter(const Flux::UniformLinearFluxCenter &);

    virtual ~UniformLinearFluxCenter(){};

    virtual void Update();

  protected:
    Set::VectorSpace::Vector velocity;
  };

  //
  //
  class MultipleLinearFluxCenter : public FluxCenter
  {
  public:
    MultipleLinearFluxCenter(Clock *Chronos_, 
			     const Set::VectorSpace::Vector &x_initial_, 
			     double speed_, double tx_, double ty_);
    MultipleLinearFluxCenter(const Flux::MultipleLinearFluxCenter &);

    virtual ~MultipleLinearFluxCenter(){};

    virtual void Update();

  protected:
    double speed;
    double tx;
    double ty;
  };

} // namespace Flux

#endif
