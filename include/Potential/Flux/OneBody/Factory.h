// Factory.h: interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(FLUX_ONEBODY_FACTORY_H__INCLUDED_)
#define FLUX_ONEBODY_FACTORY_H__INCLUDED_

#pragma once

#include <set>
#include <map>

#include "OneBody.h"
#include "Element/Factory.h"
#include "Set/Manifold/Manifold.h"

namespace Flux
{
  namespace OneBody
  {
    namespace TMCoupling {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Element::Factory
	{
	public: 

	Factory() : LS(0), E(0), DE(0), DDE(0) {}

	  virtual ~Factory() 
	    {
	      if (DDE    != 0) delete DDE;
	      if (DE     != 0) delete DE;
	      if (E      != 0) delete E;
	      if (LS     != 0) delete LS;
	    }

	Factory(Clock * Chronos_,
		double T0_,
		double Text_,
		double wa_,
		double ca_,
		Set::Manifold::Point *x) : 
	  LS(0), E(0), DE(0), DDE(0) 
	    {
	      LS = new Flux::OneBody::LocalState(chronos_, T0_, Text_, wa_, ca_, x);
	    }

	  Element::LocalState * GetLS()
	    {
	      return LS;
	    }

	  Element::Energy<0> * GetE()
	    {
	      return E;
	    }

	  Element::Energy<1> * GetDE()
	    {
	      if (DE == 0) DE = new Flux::OneBody::Energy<1>(LS);
	      return DE;
	    }

	  Element::Energy<2> * GetDDE()
	    {
	      if (DDE == 0) DDE = new Flux::OneBody::Energy<2>(LS);
	      return DDE;
	    }

	  Element::Jet<0> * GetJ()
	    {
	      return 0;
	    }

	  Element::Jet<1> * GetDJ()
	    {
	      return 0;
	    }

	private:
	  Flux::OneBody::LocalState *LS;
	  Flux::OneBody::Energy<0> *E;
	  Flux::OneBody::Energy<1> *DE;
	  Flux::OneBody::Energy<2> *DDE;

	private:

	  Factory(const Factory &);
	  Factory & operator = (const Factory &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder
      {
      public: 

	Builder() {}
	virtual ~Builder() {}

	Flux::OneBody::Factory * Build(Clock * Chronos_,
				       double T0_,
				       double Text_,
				       double wa_,
				       double ca_,
				       Set::Manifold::Point *x) const
	  {
	    return new Flux::OneBody::Factory(Chronos_, T0_, Text_, wa_, ca_, x);
	  }
      };

    }

  }

}

#endif // !defined(FLUX_ONEBODY_FACTORY_H__INCLUDED_
