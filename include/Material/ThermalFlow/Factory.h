// Factory.h: Factory for the ThermalFlow class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_THERMALFLOW_FACTORY_H__INCLUDED_)
#define MATERIAL_THERMALFLOW_FACTORY_H__INCLUDED_

#pragma once

#include "ThermalFlow.h"
#include "Material/Factory.h"
#include "Material/Uniaxial/Factory.h"

namespace Material
{
  namespace ThermalFlow
  {
    //////////////////////////////////////////////////////////////////////
    // Class Factory
    //////////////////////////////////////////////////////////////////////

    class Factory : public Material::Factory
      {
      public: 
	typedef Material::ThermalFlow::LocalState tf_fact_type;
	typedef Material::ThermalFlow::Energy<0> tf_e_type;
	typedef Material::ThermalFlow::Energy<1> tf_de_type;
	typedef Material::ThermalFlow::Energy<2> tf_dde_type;
	typedef Material::ThermalFlow::Jet<0> tf_j_type;
	typedef Material::ThermalFlow::Jet<1> tf_dj_type;

      Factory() : 
	TFLS(0), TFW(0), TFDW(0), TFDJ(0) {}

	virtual ~Factory() 
	  {
	    if (TFJ     != 0) delete TFJ;
	    if (TFDJ    != 0) delete TFDJ;
	    if (TFW     != 0) delete TFW;
	    if (TFDW    != 0) delete TFDW;
	    if (TFDDW    != 0) delete TFDDW;
	    if (TFLS    != 0) delete TFLS;

	    delete EFact;
	    delete HeatFact;
	  }

      Factory(Material::Builder *EB,
	      Material::Uniaxial::Builder *HeatB) :
	EFact(EB->Build()), HeatFact(HeatB->Build()),
	  TFLS(0), TFW(0), TFDW(0), TFDDW(0), TFJ(0), TFDJ(0) {}

	void NewLS()
	{	  
	  if ( TFLS  == 0 ) {
	    TFLS = new tf_fact_type(EFact->GetLS(), HeatFact->GetLS());
	  }
	}

	Material::LocalState * GetLS()
	  {
	    if (TFLS == 0) NewLS();
	    return TFLS;
	  }

	Material::Energy<0> * GetW()
	  {
	    if (TFLS == 0) NewLS();
	    if (TFW  == 0) 
	      TFW = new tf_e_type(TFLS, EFact->GetW());
	    return TFW;
	  }

	Material::Energy<1> * GetDW()
	  {
	    if (TFLS == 0) NewLS();
	    if (TFDW  == 0) 
	      TFDW = new tf_de_type(TFLS, 
				    EFact->GetDW(),
				    HeatFact->GetDW());
	    return TFDW;
	  }

	Material::Energy<2> * GetDDW()
	  {
	    if (TFLS == 0) NewLS();
	    if (TFDDW  == 0) 
	      TFDDW = new tf_dde_type(TFLS, 
				      EFact->GetDDW(), 
				      HeatFact->GetDDW());
	    return TFDDW;
	  }

	Material::Jet<0> * GetJ()
	  {
	    if (TFLS   == 0) NewLS();
	    if (TFJ  == 0) 
	      TFJ = new tf_j_type(TFLS, EFact->GetJ(), HeatFact->GetJ());
	    return TFJ;
	  }

	Material::Jet<1> * GetDJ()
	  {
	    if (TFLS   == 0) NewLS();
	    if (TFDJ  == 0) 
	      TFDJ = new tf_dj_type(TFLS, EFact->GetDJ(), HeatFact->GetDJ());
	    return TFDJ;
	  }

      private:
	Material::Factory *EFact;
	Material::Uniaxial::Factory *HeatFact;

	Material::ThermalFlow::LocalState *TFLS;
	Material::ThermalFlow::Energy<0> *TFW;
	Material::ThermalFlow::Energy<1> *TFDW;
	Material::ThermalFlow::Energy<2> *TFDDW;
	Material::ThermalFlow::Jet<0> *TFJ;
	Material::ThermalFlow::Jet<1> *TFDJ;

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

	Builder(){}
	virtual ~Builder() {}

        Builder(Material::Builder *EB_,
		Material::Uniaxial::Builder *HeatB_):
	EB(EB_), HeatB(HeatB_) {}
	
	Material::Factory * Build() const {
	  return new Material::ThermalFlow::Factory(EB, HeatB);
	}
	
      private:
	Material::Uniaxial::Builder *HeatB;
	Material::Builder *EB;
      };
    
  }

}


#endif // !defined(MATERIAL_THERMALFLOW_FACTORY_H__INCLUDED_)
