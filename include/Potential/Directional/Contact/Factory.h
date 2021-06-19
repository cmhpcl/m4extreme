// Factory.h: Factory for the Contact class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////


#if !defined(MATERIAL_DIRECTIONAL_CONTACT_FACTORY_H__INCLUDED_)
#define MATERIAL_DIRECTIONAL_CONTACT_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Contact.h"
#include "Potential/Directional/Factory.h"

namespace Potential
{
  namespace Directional
  {
    namespace Contact
    {
      //////////////////////////////////////////////////////////////////////
      // Class Factory
      //////////////////////////////////////////////////////////////////////

      class Factory : public Potential::Directional::Factory
	{
	public: 

	  typedef Potential::Directional::Contact::Data data_type;

	Factory() : LS(0), E(0), DE(0), DDE(0), J(0), DJ(0) {}
	  virtual ~Factory() 
	    {
	      if (LS != 0)  delete LS;
	      if (E != 0)   delete E;
	      if (DE != 0)  delete DE;
	      if (DDE != 0) delete DDE;
	      if (J != 0)   delete J;
	      if (DJ != 0)  delete DJ;
	    }

	Factory(const Set::VectorSpace::Vector *Normal_,
		Potential::Directional::Contact::Data *Dat_) : 
	  LS(0), E(0), DE(0), DDE(0), J(0), DJ(0), 
	  Dat(Dat_), Normal(Normal_) {}

	  Potential::Directional::LocalState * GetLS()
	    {
	      if (LS == 0) LS = new Potential::Directional::Contact::LocalState(Dat, Normal);
	      return LS;
	    }

	  Potential::Directional::Energy<0> * GetE()
	    {
	      if (LS == 0) GetLS();
	      if (E == 0) E = new Potential::Directional::Contact::Energy<0>(LS);
	      return E;
	    }

	  Potential::Directional::Energy<1> * GetDE()
	    {
	      if (LS == 0) GetLS();
	      if (DE == 0) DE = new Potential::Directional::Contact::Energy<1>(LS);
	      return DE;
	    }

	  Potential::Directional::Energy<2> * GetDDE()
	    {
	      if (LS == 0) GetLS();
	      if (DDE == 0) DDE = new Potential::Directional::Contact::Energy<2>(LS);
	      return DDE;
	    }

	  Potential::Directional::Jet<0> * GetJ()
	    {
	      if (LS == 0) GetLS();
	      if (J == 0) J = new Potential::Directional::Contact::Jet<0>(LS);
	      return J;
	    }

	  Potential::Directional::Jet<1> * GetDJ()
	    {
	      if (LS == 0) GetLS();
	      if (DJ == 0) DJ = new Potential::Directional::Contact::Jet<1>(LS);
	      return DJ;
	    }

	private:
	  const Set::VectorSpace::Vector *Normal;
	  Potential::Directional::Contact::Data *Dat;
	  Potential::Directional::Contact::LocalState *LS;
	  Potential::Directional::Contact::Energy<0> *E;
	  Potential::Directional::Contact::Energy<1> *DE;
	  Potential::Directional::Contact::Energy<2> *DDE;
	  Potential::Directional::Contact::Jet<0> *J;
	  Potential::Directional::Contact::Jet<1> *DJ;

	private:

	  Factory(const Factory &);
	  Factory & operator = (const Factory &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Builder
      //////////////////////////////////////////////////////////////////////

      class Builder : public Potential::Directional::Builder
	{
	public: 

	  typedef Potential::Directional::Contact::Data data_type;

	  Builder() {}
	  virtual ~Builder() {}

	Builder(Potential::Directional::Contact::Data *Dat_) :
	  Normal(NULL), Dat(Dat_) {}

	Builder(const Set::VectorSpace::Vector * Normal_,
		Potential::Directional::Contact::Data *Dat_) :
	  Normal(Normal_), Dat(Dat_) {}

	  Potential::Directional::Factory * Build() const
	    {
	      return new Potential::Directional::Contact::Factory(Normal, Dat);
	    }

	private:
	  const Set::VectorSpace::Vector *Normal;
	  Potential::Directional::Contact::Data *Dat;
	};

    }

  }

}

#endif // !defined(MATERIAL_DIRECTIONAL_CONTACT_FACTORY_H__INCLUDED_
