// Factory.h: Factory for the Fourier class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_FOURIER_FACTORY_H__INCLUDED_)
#define MATERIAL_FOURIER_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Fourier.h"
#include "Material/Factory.h"

namespace Material
{
  namespace Fourier
  {
    //////////////////////////////////////////////////////////////////////
    // Class Factory
    //////////////////////////////////////////////////////////////////////

    class Factory : public Material::Factory
    {
    public: 

    Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

    virtual ~Factory() 
	{
	  if (DJ     != 0) delete DJ;
	  if (J      != 0) delete J;
	  if (DDW    != 0) delete DDW;
	  if (DW     != 0) delete DW;
	  if (W      != 0) delete W;
	  if (LS     != 0) delete LS;
	}

    Factory(double T_, Material::Fourier::Data * D_) : T(T_), D(D_),
	LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

      void NewLS()
      {
	LS = new Material::Fourier::LocalState(T, D);
      }

      Material::LocalState * GetLS()
	{
	  if (LS == 0) NewLS();
	  return LS;
	}

      Material::Energy<0> * GetW()
	{
	  if (LS == 0) NewLS();
	  if (W == 0) W = new Material::Fourier::Energy<0>(LS);
	  return W;
	}

      Material::Energy<1> * GetDW()
	{
	  if (LS == 0) NewLS();
	  if (DW == 0) DW = new Material::Fourier::Energy<1>(LS);
	  return DW;
	}

      Material::Energy<2> * GetDDW()
	{
	  if (LS == 0) NewLS();
	  if (DDW == 0) DDW = new Material::Fourier::Energy<2>(LS);
	  return DDW;
	}

      Material::Jet<0> * GetJ()
	{
	  if (LS == 0) NewLS();
	  if (J == 0) J = new Material::Fourier::Jet<0>(LS);
	  return J;
	}

      Material::Jet<1> * GetDJ()
	{
	  if (LS == 0) NewLS();
	  if (DJ == 0) DJ = new Material::Fourier::Jet<1>(LS);
	  return DJ;
	}

    private:

      double T;
      Material::Fourier::Data *D;
      Material::Fourier::LocalState *LS;
      Material::Fourier::Energy<0> *W;
      Material::Fourier::Energy<1> *DW;
      Material::Fourier::Energy<2> *DDW;
      Material::Fourier::Jet<0> *J;
      Material::Fourier::Jet<1> *DJ;

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

      Builder() {}
      virtual ~Builder() {}

      Builder(double T_, Material::Fourier::Data * D_) : T(T_), D(D_) {}

      Material::Factory * Build() const
	{
	  return new Material::Fourier::Factory(T, D);
	}

    private:
      double T;
      Material::Fourier::Data *D;
    };

  }

}

#endif // !defined(MATERIAL_FOURIER_FACTORY_H__INCLUDED_
