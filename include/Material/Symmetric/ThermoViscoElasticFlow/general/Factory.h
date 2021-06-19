// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_THERMOVISCOELASTICFLOW_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_THERMOVISCOELASTICFLOW_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Clock/Clock.h"
#include "ThermoViscoElasticFlow.h"
#include "../Factory.h"

namespace Material
{
  namespace Symmetric
  {
    namespace ThermoViscoElasticFlow
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Material::Symmetric::Factory
	{
	public: 

	  typedef Material::Gas::EoS::Factory eos_fact_type;
	  typedef Material::Gas::EoS::Builder eos_build_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::LocalState lk_ls_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<0> lk_e_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<1> lk_de_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<2> lk_dde_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Jet<0> lk_j_type;
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Jet<1> lk_dj_type;

	Factory() : 
	  LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKDJ(0){}

	  virtual ~Factory() 
	    {	     
	      if (LKDJ    != 0) delete LKDJ;
	      if (LKW     != 0) delete LKW;
	      if (LKDW    != 0) delete LKDW;
	      if (LKDDW   != 0) delete LKDDW;
	      if (LKLS    != 0) delete LKLS;

	      delete EoSFact;
	    }

	Factory(Clock *C_, eos_build_type *EoSB, 
		Material::Symmetric::ThermoViscoElasticFlow::Data *D_) :
	  D(D_), C(C_), EoSFact(EoSB->Build()),
	    LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKJ(0), LKDJ(0){}
	  
	  void NewLS()
	  {
	    if (LKLS == 0) LKLS  = new lk_ls_type(C, D, EoSFact->GetLS());
	  }

	  Material::Symmetric::LocalState * GetLS()
	    {
	      if (LKLS == 0) NewLS();
	      return LKLS;
	    }

	  Material::Symmetric::Energy<0> * GetW()
	    {
	      if (LKLS == 0) NewLS();
	      if (LKW  == 0) LKW   = new lk_e_type(LKLS, EoSFact->GetW());
	      return LKW;
	    }

	  Material::Symmetric::Energy<1> * GetDW()
	    {
	      if (LKLS    == 0) NewLS();
	      if (LKDW  == 0) LKDW   = new lk_de_type(LKLS, EoSFact->GetDW());
	      return LKDW;
	    }

	  Material::Symmetric::Energy<2> * GetDDW()
	    {
	      if (LKLS    == 0) NewLS();
	      if (LKDDW == 0) LKDDW  = new lk_dde_type(LKLS, EoSFact->GetDDW());
	      return LKDDW;
	    }

	  Material::Symmetric::Jet<0> * GetJ()
	    {
	      if (LKLS   == 0) NewLS();
	      return NULL;
	    }

	  Material::Symmetric::Jet<1> * GetDJ()
	    {
	      if (LKLS == 0) NewLS();
	      if (LKDJ  == 0) LKDJ   = new lk_dj_type(LKLS, EoSFact->GetDJ());
	      return LKDJ;
	    }

	private:

	  Clock *C;
	  eos_fact_type *EoSFact;

	  Material::Symmetric::ThermoViscoElasticFlow::Data *D;
	  Material::Symmetric::ThermoViscoElasticFlow::LocalState *LKLS;
	  Material::Symmetric::ThermoViscoElasticFlow::Energy<0> *LKW;
	  Material::Symmetric::ThermoViscoElasticFlow::Energy<1> *LKDW;
	  Material::Symmetric::ThermoViscoElasticFlow::Energy<2> *LKDDW;
	  Material::Symmetric::ThermoViscoElasticFlow::Jet<0> *LKJ;
	  Material::Symmetric::ThermoViscoElasticFlow::Jet<1> *LKDJ;

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

	  Builder(Clock *C_, Material::Gas::EoS::Builder *EoSB_, 
		  Material::Symmetric::ThermoViscoElasticFlow::Data *D_) :
	  D(D_), EoSB(EoSB_), C(C_) {}

	  Material::Symmetric::Factory * Build() const
	    {
	      return new Material::Symmetric::ThermoViscoElasticFlow::Factory(C, EoSB, D);
	    }
	  
	private:
	  
	  Clock *C;
	  Material::Gas::EoS::Builder *EoSB;
	  Material::Symmetric::ThermoViscoElasticFlow::Data *D;
	};

    }

  }

}

#endif // !defined(MATERIAL_SYMMETRIC_THERMOVISCOELASTICFLOW_FACTORY_H__INCLUDED_)
