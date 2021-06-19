// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FIELD_RADIATION_FACTORY_H__INCLUDED_)
#define FLUX_FIELD_RADIATION_FACTORY_H__INCLUDED_

#pragma once

#include "./Radiation.h"
#include "../Factory.h"

namespace Flux
{
  namespace Field
  {
    namespace Radiation
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Flux::Field::Factory
	{
	public: 

	  typedef Flux::Field::Radiation::Data data_type;

	Factory() : LS(0), DW(0), DDW(0) {}
	  virtual ~Factory() {
	    if (LS != 0)  delete LS;
	    if (DW != 0)  delete DW;
	    if (DDW != 0) delete DDW;
	  }

	Factory(Flux::Field::Radiation::Data *Dat_) : 
	  LS(0), DW(0), DDW(0), Dat(Dat_) {}

	  Flux::Field::LocalState * GetLS() {
	    if (LS == 0) LS = new Flux::Field::Radiation::LocalState(Dat);
	    return LS;
	  }

	  Flux::Field::Energy<1> * GetDW() {
	    if (LS == 0) GetLS();
	    if (DW == 0) DW = new Flux::Field::Radiation::Energy<1>(LS);
	    return DW;
	  }

	  Flux::Field::Energy<2> * GetDDW() {
	    if (LS == 0) GetLS();
	    if (DDW == 0) DDW = new Flux::Field::Radiation::Energy<2>(LS);
	    return DDW;
	  }

	private:
	
	  Flux::Field::Radiation::Data *Dat;
	  Flux::Field::Radiation::LocalState *LS;
	  Flux::Field::Radiation::Energy<1> *DW;
	  Flux::Field::Radiation::Energy<2> *DDW;
	
	private:
	
	  Factory(const Factory &);
	  Factory & operator = (const Factory &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder : public Flux::Field::Builder {
      public: 
	
	typedef Flux::Field::Radiation::Data data_type;
	
	Builder() {}
	virtual ~Builder() {}
	
      Builder(Flux::Field::Radiation::Data *Dat_) : Dat(Dat_) {}
	
	Flux::Field::Factory * Build() const {
	  return new Flux::Field::Radiation::Factory(Dat);
	}
	
      private:	
	Flux::Field::Radiation::Data *Dat;
      };
      
    }

  }

}

#endif // !defined(FLUX_FIELD_RADIATION_FACTORY_H__INCLUDED_
