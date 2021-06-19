// Factory.h: Factory for the pressure interpolation class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_ENERGYINTERPOLATION_FACTORY_H_INCLUDED_)
#define MATERIAL_GAS_EOS_ENERGYINTERPOLATION_FACTORY_H_INCLUDED_

#pragma once

#include <vector>
#include "./EnergyInterpolation.h"
#include "Material/Gas/EoS/Factory.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace EnergyInterpolation
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Gas::EoS::Factory
{
public: 

	typedef Material::Gas::EoS::EnergyInterpolation::Data data_type;

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

	Factory(Material::Gas::EoS::EnergyInterpolation::Data *Dat_, const double x0_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_), x0(x0_) {}

	Material::Gas::EoS::EnergyInterpolation::LocalState * GetLS()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  return LS;
	}

	Material::Gas::EoS::EnergyInterpolation::Energy<0> * GetW()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  if (W == 0) W = new Material::Gas::EoS::EnergyInterpolation::Energy<0>(LS);
	  return W;
	}

	Material::Gas::EoS::EnergyInterpolation::Energy<1> * GetDW()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  if (DW == 0) DW = new Material::Gas::EoS::EnergyInterpolation::Energy<1>(LS);
	  return DW;
	}

	Material::Gas::EoS::EnergyInterpolation::Energy<2> * GetDDW()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  if (DDW == 0) DDW = new Material::Gas::EoS::EnergyInterpolation::Energy<2>(LS);
	  return DDW;
	}

	Material::Gas::EoS::EnergyInterpolation::Jet<0> * GetJ()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  if (J == 0) J = new Material::Gas::EoS::EnergyInterpolation::Jet<0>(LS);
	  return J;
	}

	Material::Gas::EoS::EnergyInterpolation::Jet<1> * GetDJ()
	{
	  if (LS == 0) LS = new Material::Gas::EoS::EnergyInterpolation::LocalState(Dat, x0);
	  if (DJ == 0) DJ = new Material::Gas::EoS::EnergyInterpolation::Jet<1>(LS);
	  return DJ;
	}

private:
	double x0;
	Material::Gas::EoS::EnergyInterpolation::Data *Dat;
	Material::Gas::EoS::EnergyInterpolation::LocalState *LS;
	Material::Gas::EoS::EnergyInterpolation::Energy<0> *W;
	Material::Gas::EoS::EnergyInterpolation::Energy<1> *DW;
	Material::Gas::EoS::EnergyInterpolation::Energy<2> *DDW;
	Material::Gas::EoS::EnergyInterpolation::Jet<0> *J;
	Material::Gas::EoS::EnergyInterpolation::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Gas::EoS::Builder
{
public: 

	typedef Material::Gas::EoS::EnergyInterpolation::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Gas::EoS::EnergyInterpolation::Data *Dat_, const double x0_) : Dat(Dat_), x0(x0_) {}

	Material::Gas::EoS::EnergyInterpolation::Factory * Build() const
	{
	  return new Material::Gas::EoS::EnergyInterpolation::Factory(Dat, x0);
	}

private:
	double x0;
	Material::Gas::EoS::EnergyInterpolation::Data *Dat;
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_ENERGYINTERPOLATION_FACTORY_H_INCLUDED_)
