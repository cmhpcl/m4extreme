// Factory.h: Factory for the Constant class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FIELD_CONSTANT_FACTORY_H__INCLUDED_)
#define FLUX_FIELD_CONSTANT_FACTORY_H__INCLUDED_

#pragma once

#include "./Constant.h"
#include "../Factory.h"

namespace Flux
{
  namespace Field
  {
    namespace Constant
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Flux::Field::Factory
	{
	public: 

	  typedef Flux::Field::Constant::Data data_type;

	Factory() : LS(0), DW(0), DDW(0) {}
	virtual ~Factory() {
	  if (LS != 0)  delete LS;
	  if (DW != 0)  delete DW;
	  if (DDW != 0) delete DDW;
	}

	Factory(Flux::Field::Constant::Data *Dat_) : 
	LS(0), DW(0), DDW(0), Dat(Dat_) {}

	Flux::Field::LocalState * GetLS() {
	  if (LS == 0) LS = new Flux::Field::Constant::LocalState(Dat);
	  return LS;
	}

	Flux::Field::Energy<1> * GetDW() {
	  if (LS == 0) GetLS();
	  if (DW == 0) DW = new Flux::Field::Constant::Energy<1>(LS);
	  return DW;
	}

	Flux::Field::Energy<2> * GetDDW() {
	  if (LS == 0) GetLS();
	  if (DDW == 0) DDW = new Flux::Field::Constant::Energy<2>(LS);
	  return DDW;
	}

      private:
	
	Flux::Field::Constant::Data *Dat;
	Flux::Field::Constant::LocalState *LS;
	Flux::Field::Constant::Energy<1> *DW;
	Flux::Field::Constant::Energy<2> *DDW;
	
	private:
	
	Factory(const Factory &);
	Factory & operator = (const Factory &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder : public Flux::Field::Builder {
      public: 
	
	typedef Flux::Field::Constant::Data data_type;
	
	Builder() {}
	virtual ~Builder() {}
	
        Builder(Flux::Field::Constant::Data *Dat_) : Dat(Dat_) {}
	
	Flux::Field::Factory * Build() const {
	  return new Flux::Field::Constant::Factory(Dat);
	}
	
      private:	
	Flux::Field::Constant::Data *Dat;
      };
      
    }

  }

}

#endif // !defined(FLUX_FIELD_CONSTANT_FACTORY_H__INCLUDED_
