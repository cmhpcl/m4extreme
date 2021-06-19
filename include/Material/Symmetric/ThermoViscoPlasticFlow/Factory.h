// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "ThermoViscoPlasticFlow.h"
#include "../Factory.h"
#include "Material/Gas/EoS/Factory.h"
#include "Material/Uniaxial/Factory.h"
#include "Clock/Clock.h"

namespace Material
{
  namespace Symmetric
  {
    namespace ThermoViscoPlasticFlow
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Material::Symmetric::Factory
	{
	public: 

	  typedef Material::Uniaxial::Factory har_fact_type; 
	  typedef Material::Uniaxial::Builder har_build_type; 
	  typedef Material::Uniaxial::Factory rat_fact_type; 
	  typedef Material::Uniaxial::Builder rat_build_type; 
	  typedef Material::Gas::EoS::Factory eos_fact_type; 
	  typedef Material::Gas::EoS::Builder eos_build_type; 

	  typedef Material::Symmetric::ThermoViscoPlasticFlow::LocalState lk_fact_type;
	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<0> lk_e_type;
	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<1> lk_de_type;
	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<2> lk_dde_type;
	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Jet<0> lk_j_type;
	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Jet<1> lk_dj_type;


	Factory() : 	 
	  LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKDJ(0){}

	  virtual ~Factory() 
	    {
	      //if (LKJ     != 0) delete LKJ;
	      //if (LKDJ    != 0) delete LKDJ;
	      if (LKW     != 0) delete LKW;
	      if (LKDW    != 0) delete LKDW;
	      if (LKDDW    != 0) delete LKDDW;
	      if (LKLS    != 0) delete LKLS;

	      delete EoSFact;
	      delete RatFact;
	      delete HarFact;
	    }

        Factory(Clock *C_,
		Material::Symmetric::ThermoViscoPlasticFlow::Data *D_,
		har_build_type *HarB, 
		rat_build_type *RatB, 
		eos_build_type *EoSB)
	  : C(C_), D(D_),
	    HarFact(HarB->Build()), 
	    RatFact(RatB->Build()), EoSFact(EoSB->Build()), 
	    LKLS(0), LKW(0), LKDW(0), LKDDW(0), LKJ(0), LKDJ(0) {}
	
	  void NewLS() {
	    if (LKLS == 0) LKLS = 
			     new lk_fact_type(C, D, EoSFact->GetLS(), 
					      HarFact->GetDW(),  HarFact->GetDDW(), 
					      HarFact->GetDJ(),  RatFact->GetDW(),
					      RatFact->GetDDW(), RatFact->GetDJ());
	  }

	  Material::Symmetric::LocalState * GetLS() {
	    if (LKLS == 0) NewLS();
	    return LKLS;
	  }

	  Material::Symmetric::Energy<0> * GetW() {
	    if (LKLS == 0) NewLS();
	    if (LKW  == 0) LKW   = 
			     new lk_e_type(LKLS, EoSFact->GetW(),
					   HarFact->GetW(), RatFact->GetW());
	    return LKW;
	  }

	  Material::Symmetric::Energy<1> * GetDW() {
	    if (LKLS  == 0) NewLS();
	    if (LKDW  == 0) LKDW   = 
			      new lk_de_type(LKLS, EoSFact->GetDW());
	    return LKDW;
	  }

	  Material::Symmetric::Energy<2> * GetDDW() {
	    if (LKLS    == 0) NewLS();
	    if (LKDDW == 0) LKDDW =
			      new lk_dde_type(LKLS, EoSFact->GetDDW(), 
					      HarFact->GetDJ(), RatFact->GetDJ());
	    return LKDDW;
	  }

	  Material::Symmetric::Jet<0> * GetJ() {
	    return NULL;
	  }

	  Material::Symmetric::Jet<1> * GetDJ() {
	    return NULL;
	  }

	private:
	  Clock *C;	  
	  har_fact_type *HarFact;
	  rat_fact_type *RatFact; 
	  eos_fact_type *EoSFact; 
	  Material::Symmetric::ThermoViscoPlasticFlow::Data *D;
	  Material::Symmetric::ThermoViscoPlasticFlow::LocalState *LKLS;
	  Material::Symmetric::ThermoViscoPlasticFlow::Energy<0> *LKW;
	  Material::Symmetric::ThermoViscoPlasticFlow::Energy<1> *LKDW;
	  Material::Symmetric::ThermoViscoPlasticFlow::Energy<2> *LKDDW;
	  Material::Symmetric::ThermoViscoPlasticFlow::Jet<0> *LKJ;
	  Material::Symmetric::ThermoViscoPlasticFlow::Jet<1> *LKDJ;

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

	Builder(Clock *C_,
		Material::Symmetric::ThermoViscoPlasticFlow::Data *D_,			
		Material::Uniaxial::Builder *HarB_, 
		Material::Uniaxial::Builder *RatB_, 
		Material::Gas::EoS::Builder *EoSB_ ) :
	  D(D_), HarB(HarB_), RatB(RatB_), EoSB(EoSB_), C(C_) {}

	  Material::Symmetric::Factory * Build() const {
	    return new Material::Symmetric::ThermoViscoPlasticFlow::Factory(C, D, HarB, RatB, EoSB);
	  }

	private:
	  Clock *C;	
	  Material::Symmetric::ThermoViscoPlasticFlow::Data *D;	  
	  Material::Uniaxial::Builder *HarB;
	  Material::Uniaxial::Builder *RatB; 
	  Material::Gas::EoS::Builder *EoSB; 
	};

    }

  }

}

#endif //!defined(MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_FACTORY_H__INCLUDED_)
