// Factory.h: Factory for the ThermoPolymeric class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////


#if !defined(MATERIAL_SYMMETRIC_THERMOPOLYMERIC_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_THERMOPOLYMERIC_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "ThermoPolymeric.h"
#include "Material/Symmetric/FiniteKinematics/FiniteKinematics.h"
#include "Material/Symmetric/LK2FK/LK2FK.h"
#include "Material/Factory.h"


namespace Material
{
  namespace Symmetric
  {
    namespace ThermoPolymeric
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Material::Factory
	{
	public: 

	  typedef Material::Symmetric::ThermoPolymeric::LocalState lk_fact_type;
	  typedef Material::Symmetric::ThermoPolymeric::Energy<0> lk_e_type;
	  typedef Material::Symmetric::ThermoPolymeric::Energy<1> lk_de_type;
	  typedef Material::Symmetric::ThermoPolymeric::Energy<2> lk_dde_type;
	  typedef Material::Symmetric::ThermoPolymeric::Jet<0> lk_j_type;
	  typedef Material::Symmetric::ThermoPolymeric::Jet<1> lk_dj_type;

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
	    LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKDJ(0), 
	    FKLS(0), FKW(0), FKDW(0), FKDDW(0), FKDJ(0) {}

	  virtual ~Factory() 
	    {
	      if (FKJ     != 0) delete FKJ;
	      if (FKDJ    != 0) delete FKDJ;
	      if (FKW     != 0) delete FKW;
	      if (FKDW    != 0) delete FKDW;
	      if (FKDDW   != 0) delete FKDDW;
	      if (FKLS    != 0) delete FKLS;

	      if (LKJ     != 0) delete LKJ;
	      if (LKDJ    != 0) delete LKDJ;
	      if (LKW     != 0) delete LKW;
	      if (LKDW    != 0) delete LKDW;
	      if (LKDDW   != 0) delete LKDDW;
	      if (LKLS    != 0) delete LKLS;

	      if (W       != 0) delete W;
	      if (DW      != 0) delete DW;
	      if (DDW     != 0) delete DDW;
	      if (J       != 0) delete J;
	      if (DJ      != 0) delete DJ;
	      if (LS      != 0) delete LS;
	    }

        Factory(Clock * Chronos_,
		Material::Symmetric::ThermoPolymeric::Data *D_,
		double T0_) :
	  Chronos(Chronos_), D(D_), T0(T0_),
	    LS(0), W(0), DW(0), DDW(0), J(0), DJ(0),
	    LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKJ(0), LKDJ(0), 
	    FKLS(0), FKW(0), FKDW(0), FKDDW(0), FKJ(0), FKDJ(0) {}

	  void NewLS()
	  {
	    if (LKLS == 0) LKLS =  new lk_fact_type(T0, Chronos, D);
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
	      if (LKW  == 0) LKW   = new lk_e_type(LKLS);
	      if (FKW  == 0) FKW   = new fk_e_type(LKW);
	      if (W    == 0) W     = new j2_e_type(FKW);
	      return W;
	    }

	  Material::Energy<1> * GetDW()
	    {
	      if (LS    == 0) NewLS();
	      if (LKDW  == 0) LKDW   = new lk_de_type(LKLS);
	      if (FKDW  == 0) FKDW   = new fk_de_type(LKDW);
	      if (DW    == 0) DW     = new j2_de_type(FKDW);
	      return DW;
	    }

	  Material::Energy<2> * GetDDW()
	    {
	      if (LS    == 0) NewLS();
	      if (LKDDW == 0) LKDDW  = new lk_dde_type(LKLS);
	      if (LKDJ  == 0) LKDJ   = new lk_dj_type(LKLS);
	      if (FKDDW == 0) FKDDW  = new fk_dde_type(LKDDW, LKDJ);
	      if (FKDJ  == 0) FKDJ   = new fk_dj_type(LKDJ);
	      if (DDW   == 0) DDW    = new j2_dde_type(FKDDW, FKDJ);
	      return DDW;
	    }

	  Material::Jet<0> * GetJ()
	    {
	      return J;
	    }

	  Material::Jet<1> * GetDJ()
	    {
	      return DJ;
	    }

	private:
	  Clock *Chronos;
	  double T0;

	  Material::Symmetric::FiniteKinematics::LocalState *LS;
	  Material::Symmetric::FiniteKinematics::Energy<0> *W;
	  Material::Symmetric::FiniteKinematics::Energy<1> *DW;
	  Material::Symmetric::FiniteKinematics::Energy<2> *DDW;
	  Material::Symmetric::FiniteKinematics::Jet<0> *J;
	  Material::Symmetric::FiniteKinematics::Jet<1> *DJ;

	  Material::Symmetric::ThermoPolymeric::Data *D;
	  Material::Symmetric::ThermoPolymeric::LocalState *LKLS;
	  Material::Symmetric::ThermoPolymeric::Energy<0> *LKW;
	  Material::Symmetric::ThermoPolymeric::Energy<1> *LKDW;
	  Material::Symmetric::ThermoPolymeric::Energy<2> *LKDDW;
	  Material::Symmetric::ThermoPolymeric::Jet<0> *LKJ;
	  Material::Symmetric::ThermoPolymeric::Jet<1> *LKDJ;

	  Material::Symmetric::LK2FK::LocalState *FKLS;
	  Material::Symmetric::LK2FK::Energy<0> *FKW;
	  Material::Symmetric::LK2FK::Energy<1> *FKDW;
	  Material::Symmetric::LK2FK::Energy<2> *FKDDW;
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
	
        Builder(Clock *Chronos_,
		Material::Symmetric::ThermoPolymeric::Data *D_,
		double T0_) :
	  Chronos(Chronos_), D(D_), T0(T0_) {}

	  Material::Factory * Build() const
	    {
	      return new Material::Symmetric::ThermoPolymeric::Factory(Chronos, D, T0);
	    }

	private:
	  Clock *Chronos;
	  double T0;
	  Material::Symmetric::ThermoPolymeric::Data *D;
	};

    }

  }

}

#endif // !defined(MATERIAL_SYMMETRIC_THERMOPOLYMERIC_FACTORY_H__INCLUDED_)
