// OneBody.h: interface for the OneBody class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(FLUX_ONEBODY_H__INCLUDED_)
#define FLUX_ONEBODY_H__INCLUDED_

#pragma once

#include <vector>
#include <utility>
#include "Clock/Clock.h"
#include "Element/Element.h"
#include "../Field/Field.h"

using namespace std;

namespace Flux
{
  namespace OneBody
  {
    class LocalState;
    template <unsigned int> class Energy;
    template <unsigned int> class Jet;

    //////////////////////////////////////////////////////////////////////
    // Class LocalState
    //////////////////////////////////////////////////////////////////////

    class LocalState : public Element::LocalState
      {
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;
	friend class Jet<0>;
	friend class Jet<1>;

      public: 

	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef vector<Set::VectorSpace::Vector> range_type;

	LocalState();
	virtual ~LocalState();
	Element::LocalState *Clone() const;
	LocalState(Clock * ,
		   Flux::Field::LocalState *,
		   double,
		   double,
		   Set::Manifold::Point *);
	LocalState(Clock *,
		   Flux::Field::LocalState *,
		   int,
		   double,
		   double,
		   Set::Manifold::Point *);
	LocalState(const LocalState &);
	LocalState & operator = (const LocalState &);
	void operator ++ ();

	Flux::Field::LocalState * GetMLS();
	set<Set::Manifold::Point *> GetNodes() const;
	set<pair<Set::Manifold::Point *, Set::Manifold::Point *> > GetNodePairs() const;

	void GetNodes(set<Set::Manifold::Point *>  &) const;
	void GetNodePairs(set<pair<Set::Manifold::Point *, Set::Manifold::Point *> > &) const;
	virtual void Reset(const vector_type &) {return;}
	virtual void Reset(const scalar_type &);

      protected:
	Clock * Chronos;
	int ScalingFactor;
	double TOld, T;
	double wa;
	Set::Manifold::Point *e0;
	Flux::Field::LocalState * MLS;
      };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template<> 
      class Energy<1> : public Element::Energy<1>
      {
      public: 

	typedef Flux::OneBody::Energy<2> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<Set::Manifold::Point *, double> range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<1> *Clone() const;
	Element::LocalState *GetLocalState() const;
	Energy(LocalState *, Flux::Field::Energy<1> *);
	Energy(const Energy<1> &);
	const Energy<1> & operator = (const Energy<1> &);

	virtual vector_type operator () (const vector_type &) const {
	  assert(false);
	}

	virtual void operator () (const vector_type &, vector_type &) const;
	virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;

      private:

	LocalState *LS;
	Flux::Field::Energy<1> *DH;
      };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<2>
    //////////////////////////////////////////////////////////////////////

    template<> 
      class Energy<2> : public Element::Energy<2>
      {
      public: 
	typedef map<pair<Set::Manifold::Point *, 
	  Set::Manifold::Point *>, 
	  Set::VectorSpace::Hom> hom_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<2> *Clone() const;
	Element::LocalState *GetLocalState() const;
	Energy(LocalState *, Flux::Field::Energy<1> *, Flux::Field::Energy<2> *);
	Energy(const Energy<2> &);
	const Energy<2> & operator = (const Energy<2> &);

	virtual hom_type operator () (const vector_type &) const {
	  assert(false);
	}

	virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;

      private:
	LocalState *LS;
	Flux::Field::Energy<1> *DH;
	Flux::Field::Energy<2> *DDH;
      };

  }

}

#endif // !defined(FLUX_ONEBODY_H__INCLUDED_)

