///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
///////////////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_FIELD_MESHEDHALFSPACE_FACTORY_H__INCLUDED_)
#define POTENTIAL_FIELD_MESHEDHALFSPACE_FACTORY_H__INCLUDED_

#pragma once

#include "./MeshedHalfSpace.h"
#include "../Factory.h"

namespace Potential
{
  namespace Field
  {
    namespace MeshedHalfSpace
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Potential::Field::Factory
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Data data_type;

        Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	  
	virtual ~Factory() {
	  if (LS != 0)  delete LS;
	  if (W != 0)   delete W;
	  if (DW != 0)  delete DW;
	  if (DDW != 0) delete DDW;
	  if (J != 0)   delete J;
	  if (DJ != 0)  delete DJ;
	}

        Factory(Potential::Field::MeshedHalfSpace::Data *Dat_, double hmin_) : 
	LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_), hmin(hmin_) {}

	Potential::Field::LocalState * GetLS() {
	  if (LS == 0) LS = new Potential::Field::MeshedHalfSpace::LocalState(Dat, hmin);
	  return LS;
	}

	Potential::Field::Energy<0> * GetW() {
	  GetLS();
	  if (W == 0) W = new Potential::Field::MeshedHalfSpace::Energy<0>(LS);
	  return W;
	}

	Potential::Field::Energy<1> * GetDW() {
	  GetLS();
	  if (DW == 0) DW = new Potential::Field::MeshedHalfSpace::Energy<1>(LS);
	  return DW;
	}

	Potential::Field::Energy<2> * GetDDW() {
	  GetLS();
	  if (DDW == 0) DDW = new Potential::Field::MeshedHalfSpace::Energy<2>(LS);
	  return DDW;
	}

	Potential::Field::Jet<0> * GetJ() {
	  GetLS();
	  if (J == 0) J = new Potential::Field::MeshedHalfSpace::Jet<0>(LS);
	  return J;
	}

	Potential::Field::Jet<1> * GetDJ() {
	  GetLS();
	  if (DJ == 0) DJ = new Potential::Field::MeshedHalfSpace::Jet<1>(LS);
	  return DJ;
	}

      private:
	double hmin;
	Potential::Field::MeshedHalfSpace::Data *Dat;
	Potential::Field::MeshedHalfSpace::LocalState *LS;
	Potential::Field::MeshedHalfSpace::Energy<0> *W;
	Potential::Field::MeshedHalfSpace::Energy<1> *DW;
	Potential::Field::MeshedHalfSpace::Energy<2> *DDW;
	Potential::Field::MeshedHalfSpace::Jet<0> *J;
	Potential::Field::MeshedHalfSpace::Jet<1> *DJ;

      private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder : public Potential::Field::Builder
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Data data_type;

	Builder() {}
	virtual ~Builder() {}

        Builder(Potential::Field::MeshedHalfSpace::Data *Dat_, double hmin_) :
	hmin(hmin_), Dat(Dat_) {}

	Potential::Field::Factory * Build() const {
	  return new Potential::Field::MeshedHalfSpace::Factory(Dat, hmin);
	}

      private:
	double hmin;
	Potential::Field::MeshedHalfSpace::Data *Dat;
      };

    }

  }

}

#endif // !defined(POTENTIAL_FIELD_MESHEDHALFSPACE_FACTORY_H__INCLUDED_
