// Factory.h: Factory for the J2Isotropic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_J2ISOTROPIC_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_J2ISOTROPIC_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./J2Isotropic.h"
#include "../Factory.h"
#include "../../Shear/Factory.h"
#include "../../Gas/EoS/Factory.h"
#include "../../Uniaxial/Factory.h"
#include "../../../Clock/Clock.h"

namespace Material
{
namespace Symmetric
{
namespace J2Isotropic
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	typedef Material::Shear::Factory she_fact_type; 
	typedef Material::Shear::Builder she_build_type; 
	typedef Material::Uniaxial::Factory har_fact_type; 
	typedef Material::Uniaxial::Builder har_build_type; 
	typedef Material::Uniaxial::Factory rat_fact_type; 
	typedef Material::Uniaxial::Builder rat_build_type; 
	typedef Material::Gas::EoS::Factory eos_fact_type; 
	typedef Material::Gas::EoS::Builder eos_build_type; 

	typedef Material::Symmetric::J2Isotropic::LocalState lk_fact_type;
	typedef Material::Symmetric::J2Isotropic::Energy<0> lk_e_type;
	typedef Material::Symmetric::J2Isotropic::Energy<1> lk_de_type;
	typedef Material::Symmetric::J2Isotropic::Energy<2> lk_dde_type;
	typedef Material::Symmetric::J2Isotropic::Jet<0> lk_j_type;
	typedef Material::Symmetric::J2Isotropic::Jet<1> lk_dj_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (W       != 0) delete W;
		if (DW      != 0) delete DW;
		if (DDW     != 0) delete DDW;
		if (J       != 0) delete J;
		if (DJ      != 0) delete DJ;
		if (LS      != 0) delete LS;

		delete EoSFact;
		delete RatFact;
		delete HarFact;
		delete SheFact;
	}

	Factory(she_build_type *SheB, har_build_type *HarB, 
		rat_build_type *RatB, eos_build_type *EoSB, Clock *C_) :
		SheFact(SheB->Build()), HarFact(HarB->Build()), 
		RatFact(RatB->Build()), EoSFact(EoSB->Build()), C(C_),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		if (LS == 0) LS = 
			new lk_fact_type(C,SheFact->GetG(),HarFact->GetDW(),
			HarFact->GetDDW(),HarFact->GetDJ(),RatFact->GetDW(),
			RatFact->GetDDW(),RatFact->GetDJ());
	}

	Material::Symmetric::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0) W   = 
			new lk_e_type(LS,EoSFact->GetW(),HarFact->GetW(),RatFact->GetW());
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (LS    == 0) NewLS();
		if (DW  == 0) DW   = 
			new lk_de_type(LS,EoSFact->GetDW(),SheFact->GetJ());
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (LS    == 0) NewLS();
		if (DDW  == 0) DDW   = 
			new lk_dde_type(LS,EoSFact->GetDDW(),HarFact->GetDJ(),RatFact->GetDJ(),SheFact->GetJJ());
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (LS   == 0) NewLS();
		if (J  == 0) J   = 
			new lk_j_type(LS,EoSFact->GetJ(),SheFact->GetJ(),HarFact->GetW(),RatFact->GetW());
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ  == 0) DJ   = 
			new lk_dj_type(LS,EoSFact->GetDJ(),SheFact->GetJJ(),HarFact->GetDJ(),RatFact->GetDJ());
		return DJ;
	}

private:

	Clock *C;

	she_fact_type *SheFact; 
	har_fact_type *HarFact;
	rat_fact_type *RatFact; 
	eos_fact_type *EoSFact; 

	Material::Symmetric::J2Isotropic::LocalState *LS;
	Material::Symmetric::J2Isotropic::Energy<0> *W;
	Material::Symmetric::J2Isotropic::Energy<1> *DW;
	Material::Symmetric::J2Isotropic::Energy<2> *DDW;
	Material::Symmetric::J2Isotropic::Jet<0> *J;
	Material::Symmetric::J2Isotropic::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Symmetric::Builder
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Shear::Builder *SheB_, 
		Material::Uniaxial::Builder *HarB_, 
		Material::Uniaxial::Builder *RatB_, 
		Material::Gas::EoS::Builder *EoSB_, Clock *C_) :
		SheB(SheB_), HarB(HarB_), RatB(RatB_), EoSB(EoSB_), C(C_) {}

	Material::Symmetric::Factory * Build() const
	{
		return new Material::Symmetric::J2Isotropic::Factory(SheB,HarB,RatB,EoSB,C);
	}

private:

	Clock *C;
	Material::Shear::Builder *SheB; 
	Material::Uniaxial::Builder *HarB;
	Material::Uniaxial::Builder *RatB; 
	Material::Gas::EoS::Builder *EoSB; 
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_J2ISOTROPIC_FACTORY_H__INCLUDED_)
