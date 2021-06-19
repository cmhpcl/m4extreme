// Factory.h: Factory for the J2IsotropicAdiabatic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_Thermal_Expansion_J2ISOTROPICADIABATIC_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_Thermal_Expansion_J2ISOTROPICADIABATIC_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Thermal_Expansion.J2IsotropicAdiabatic.h"
#include "Material/Symmetric/FiniteKinematics/FiniteKinematics.h"
#include "Material/Symmetric/LK2FK/LK2FK.h"
#include "Material/Symmetric/Newtonian/Newtonian.h"
#include "Material/Factory.h"
#include "Material/Shear/Factory.h"
#include "Material/Gas/EoS/Factory.h"
#include "Material/Uniaxial/Factory.h"
#include "Clock/Clock.h"

namespace Material
{
namespace Symmetric
{
namespace Thermal_Expansion_J2IsotropicAdiabatic
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
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
        typedef Material::Uniaxial::Factory heat_fact_type;
	typedef Material::Uniaxial::Builder heat_build_type;
        typedef Material::Uniaxial::Factory ts_fact_type;
	typedef Material::Uniaxial::Builder ts_build_type;
        typedef Material::Uniaxial::Factory mu_fact_type;
	typedef Material::Uniaxial::Builder mu_build_type;

	typedef Material::Symmetric::Newtonian::LocalState sym_ls_type;
	typedef Material::Symmetric::Newtonian::Energy<0> sym_e_type;
	typedef Material::Symmetric::Newtonian::Energy<1> sym_de_type;
	//	typedef Material::Symmetric::Newtonian::Energy<2> sym_dde_type;
	//	typedef Material::Symmetric::Newtonian::Jet<0> sym_j_type;
	//	typedef Material::Symmetric::Newtonian::Jet<1> sym_dj_type;

	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::LocalState lk_fact_type;
	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy<0> lk_e_type;
	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy<1> lk_de_type;
	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy<2> lk_dde_type;
	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet<0> lk_j_type;
	typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet<1> lk_dj_type;

	typedef Material::Symmetric::LK2FK::LocalState fk_fact_type;
	typedef Material::Symmetric::LK2FK::Energy<0> fk_e_type;
	typedef Material::Symmetric::LK2FK::Energy<1> fk_de_type;
	typedef Material::Symmetric::LK2FK::Energy<2> fk_dde_type;
	typedef Material::Symmetric::LK2FK::Jet<0> fk_j_type;
	typedef Material::Symmetric::LK2FK::Jet<1> fk_dj_type;

	typedef Material::Symmetric::FiniteKinematics::LocalState j2_fact_type;
	typedef Material::Symmetric::FiniteKinematics::Energy<0> j2_e_type;
	typedef Material::Symmetric::FiniteKinematics::Energy<1> j2_de_type;
	typedef Material::Symmetric::FiniteKinematics::Energy<2> j2_dde_type;
	typedef Material::Symmetric::FiniteKinematics::Jet<0> j2_j_type;
	typedef Material::Symmetric::FiniteKinematics::Jet<1> j2_dj_type;

	Factory() : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0),
		LKLS(0), LKW(0), LKDW(0), LKDJ(0), 
	        FKLS(0), FKW(0), FKDW(0), FKDJ(0),
	        SymLS(0), SymW(0), SymDW(0)/*, SymDDW(0), SymJ(0), SymDJ(0)*/{}

	virtual ~Factory() 
	{
		if (SymW    != 0) delete SymW;
		if (SymDW   != 0) delete SymDW;
		//		if (SymDDW  != 0) delete SymDDW;
		//		if (SymJ    != 0) delete SymJ;
		//		if (SymDJ   != 0) delete SymDJ;
		if (SymLS   != 0) delete SymLS;

		if (FKJ     != 0) delete FKJ;
		if (FKDJ    != 0) delete FKDJ;
		if (FKW     != 0) delete FKW;
		if (FKDW    != 0) delete FKDW;
		if (FKLS    != 0) delete FKLS;

		if (LKJ     != 0) delete LKJ;
		if (LKDJ    != 0) delete LKDJ;
		if (LKW     != 0) delete LKW;
		if (LKDW    != 0) delete LKDW;
		if (LKLS    != 0) delete LKLS;

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
                delete HeatFact;
                delete TSFact;
		delete MuFact;		
	}

	Factory(double alpha_, double T0_, double Tm_,
		she_build_type *SheB, har_build_type *HarB, 
		rat_build_type *RatB, eos_build_type *EoSB,
                heat_build_type *HeatB, ts_build_type *TSB, 
		mu_build_type *MuB, Material::Symmetric::Newtonian::Data *D_, Clock *C_) :
	        alpha(alpha_), T0(T0_), Tm(Tm_), SheFact(SheB->Build()), HarFact(HarB->Build()),
		RatFact(RatB->Build()), EoSFact(EoSB->Build()), 
	        MuFact(MuB->Build()), D(D_), C(C_),
                HeatFact(HeatB->Build()), TSFact(TSB->Build()),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0),
		LKLS(0), LKW(0), LKDW(0), LKJ(0), LKDJ(0), 
		FKLS(0), FKW(0), FKDW(0), FKJ(0), FKDJ(0),
	        SymLS(0), SymW(0), SymDW(0)/*, SymDDW(0), SymJ(0), SymDJ(0)*/ {}

	void NewLS()
	{
	        if ( SymLS == 0 ) SymLS = new sym_ls_type(D,C);
		if ( SymW  == 0 ) SymW  = new sym_e_type(SymLS);	       
		if ( LKLS  == 0 ) LKLS  = 
		  new lk_fact_type(C, alpha, T0, Tm,
                        EoSFact->GetLS(), EoSFact->GetDW(), EoSFact->GetDDW(),
                        SheFact->GetG(), SheFact->GetDG(), SheFact->GetDDG(),
                        TSFact->GetW(), TSFact->GetDW(), TSFact->GetDDW(),
                        HeatFact->GetDW(), HeatFact->GetDDW(),
                        HarFact->GetW(), HarFact->GetDW(), HarFact->GetDDW(),
			RatFact->GetW(),RatFact->GetDW(), RatFact->GetDDW(),
			MuFact->GetW(), MuFact->GetDW(), MuFact->GetDDW(), SymW);
		if (FKLS == 0) FKLS  = new fk_fact_type(LKLS);
		if (LS   == 0) LS    = new j2_fact_type(FKLS);
	}

	Material::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (LKW  == 0) LKW   = 
			new lk_e_type(LKLS,EoSFact->GetW(),HeatFact->GetW());
		if (FKW  == 0) FKW   = new fk_e_type(LKW);
		if (W    == 0) W     = new j2_e_type(FKW);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS    == 0) NewLS();
		if (SymDW  == 0) SymDW = new sym_de_type(SymLS);
		if (LKDW  == 0) LKDW   = 
		  new lk_de_type(LKLS,EoSFact->GetDW(),SheFact->GetJ(), SymDW);
		if (FKDW  == 0) FKDW   = new fk_de_type(LKDW);
		if (DW    == 0) DW     = new j2_de_type(FKDW);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS    == 0) NewLS();
		if (LKDJ  == 0) LKDJ   = 
			new lk_dj_type(LKLS,EoSFact->GetDJ(),SheFact->GetJJ(),HarFact->GetDJ(),RatFact->GetDJ());
		if (FKDJ  == 0) FKDJ   = new fk_dj_type(LKDJ);
		if (DDW   == 0) DDW    = new j2_dde_type(FKDJ);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS   == 0) NewLS();
		if (LKJ  == 0) LKJ   = 
			new lk_j_type(LKLS,EoSFact->GetJ(),SheFact->GetJ(),HarFact->GetW(),RatFact->GetW());
		if (FKJ  == 0) FKJ   = new fk_j_type(LKJ);
		if (J    == 0) J     = new j2_j_type(FKJ);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (LKDJ  == 0) LKDJ   = 
			new lk_dj_type(LKLS,EoSFact->GetDJ(),SheFact->GetJJ(),HarFact->GetDJ(),RatFact->GetDJ());
		if (FKDJ  == 0) FKDJ   = new fk_dj_type(LKDJ);
		if (DJ    == 0) DJ     = new j2_dj_type(FKDJ);
		return DJ;
	}

private:

	Clock *C;
	double alpha, T0, Tm;

	she_fact_type *SheFact; 
	har_fact_type *HarFact;
	rat_fact_type *RatFact; 
	eos_fact_type *EoSFact;
        heat_fact_type *HeatFact;
        ts_fact_type *TSFact;
	mu_fact_type *MuFact;

	Material::Symmetric::Newtonian::Data *D;
	Material::Symmetric::Newtonian::LocalState *SymLS;
	Material::Symmetric::Newtonian::Energy<0> *SymW;
	Material::Symmetric::Newtonian::Energy<1> *SymDW;
	//	Material::Symmetric::Newtonian::Energy<2> *SymDDW;
	//	Material::Symmetric::Newtonian::Jet<0> *SymJ;
	//	Material::Symmetric::Newtonian::Jet<1> *SymDJ;

	Material::Symmetric::FiniteKinematics::LocalState *LS;
	Material::Symmetric::FiniteKinematics::Energy<0> *W;
	Material::Symmetric::FiniteKinematics::Energy<1> *DW;
	Material::Symmetric::FiniteKinematics::Energy<2> *DDW;
	Material::Symmetric::FiniteKinematics::Jet<0> *J;
	Material::Symmetric::FiniteKinematics::Jet<1> *DJ;

	Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::LocalState *LKLS;
	Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy<0> *LKW;
	Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy<1> *LKDW;
	Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet<0> *LKJ;
	Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet<1> *LKDJ;

	Material::Symmetric::LK2FK::LocalState *FKLS;
	Material::Symmetric::LK2FK::Energy<0> *FKW;
	Material::Symmetric::LK2FK::Energy<1> *FKDW;
	Material::Symmetric::LK2FK::Jet<0> *FKJ;
	Material::Symmetric::LK2FK::Jet<1> *FKDJ;

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

	Builder(double alpha_, double T0_, double Tm_,
		Material::Shear::Builder *SheB_, 
		Material::Uniaxial::Builder *HarB_, 
		Material::Uniaxial::Builder *RatB_, 
		Material::Gas::EoS::Builder *EoSB_,
                Material::Uniaxial::Builder *HeatB_,
                Material::Uniaxial::Builder *TSB_, 
		Material::Uniaxial::Builder *MuB_,
		Material::Symmetric::Newtonian::Data *D_, Clock *C_) :
	  alpha(alpha_), T0(T0_), Tm(Tm_), SheB(SheB_), HarB(HarB_), RatB(RatB_), EoSB(EoSB_),
          HeatB(HeatB_), TSB(TSB_), MuB(MuB_), D(D_), C(C_) {}

	Material::Factory * Build() const
	{
	  return new Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Factory(alpha, T0, Tm, SheB, HarB, RatB, EoSB, HeatB, TSB, MuB, D, C);
	}

private:

	Clock *C;
	Material::Symmetric::Newtonian::Data *D;
        double alpha, T0, Tm;

	Material::Shear::Builder *SheB; 
	Material::Uniaxial::Builder *HarB;
	Material::Uniaxial::Builder *RatB;
        Material::Uniaxial::Builder *HeatB;
        Material::Uniaxial::Builder *TSB;
	Material::Gas::EoS::Builder *EoSB;
	Material::Uniaxial::Builder *MuB;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_J2ISOTROPICADIABATIC_FACTORY_H__INCLUDED_)
