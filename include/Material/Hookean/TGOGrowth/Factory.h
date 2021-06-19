// Factory.h: Factory for the Hookean::Isotropic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_HOOKEAN_TGO_GROWTH_FACTORY_H__INCLUDED_)
#define MATERIAL_HOOKEAN_TGO_GROWTH_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./TGOGrowth.h"
#include "../../Factory.h"

namespace Material
{
  namespace Hookean
  {
    namespace TGOGrowth
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Material::Factory
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Data data_type;

      Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	  {
	    if (LS != 0)  delete LS;
	    if (W != 0)   delete W;
	    if (DW != 0)  delete DW;
	    if (DDW != 0) delete DDW;
	    if (J != 0)   delete J;
	    if (DJ != 0)  delete DJ;
	  }

      Factory(Material::Hookean::TGOGrowth::Data *Dat_, Clock *T_) : 
	LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_), T(T_) {}

	Material::LocalState * GetLS()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    return LS;
	  }

	Material::Energy<0> * GetW()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    if (W == 0) W = new Material::Hookean::TGOGrowth::Energy<0>(LS);
	    return W;
	  }

	Material::Energy<1> * GetDW()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    if (DW == 0) DW = new Material::Hookean::TGOGrowth::Energy<1>(LS);
	    return DW;
	  }

	Material::Energy<2> * GetDDW()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    if (DDW == 0) DDW = new Material::Hookean::TGOGrowth::Energy<2>(LS);
	    return DDW;
	  }

	Material::Jet<0> * GetJ()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    if (J == 0) J = new Material::Hookean::TGOGrowth::Jet<0>(LS);
	    return J;
	  }

	Material::Jet<1> * GetDJ()
	  {
	    if (LS == 0) LS = new Material::Hookean::TGOGrowth::LocalState(Dat,T);
	    if (DJ == 0) DJ = new Material::Hookean::TGOGrowth::Jet<1>(LS);
	    return DJ;
	  }

      private:

	Clock *T;
	Material::Hookean::TGOGrowth::Data *Dat;
	Material::Hookean::TGOGrowth::LocalState *LS;
	Material::Hookean::TGOGrowth::Energy<0> *W;
	Material::Hookean::TGOGrowth::Energy<1> *DW;
	Material::Hookean::TGOGrowth::Energy<2> *DDW;
	Material::Hookean::TGOGrowth::Jet<0> *J;
	Material::Hookean::TGOGrowth::Jet<1> *DJ;

      private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder : public Material::Builder
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Data data_type;

	Builder() {}
	virtual ~Builder() {}

      Builder(Material::Hookean::TGOGrowth::Data *Dat_, Clock *T_) : 
	Dat(Dat_), T(T_) {}

	Material::Factory * Build() const
	  {
	    return new Material::Hookean::TGOGrowth::Factory(Dat,T);
	  }

      private:

	Clock *T;
	Material::Hookean::TGOGrowth::Data *Dat;
      };

    }

  }

}

#endif // !defined(MATERIAL_HOOKEAN_TGO_GROWTH_FACTORY_H__INCLUDED_
