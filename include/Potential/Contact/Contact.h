// Contact.h: interface for the Contact class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_CONTACT_H__INCLUDED_)
#define POTENTIAL_CONTACT_H__INCLUDED_

#pragma once

#include <vector>
#include "../../Material/Material.h"
#include "./FaceEdgePair.h"
#include "../../Element/Element.h"
#include "../../Geometry/Algebraic/ChainComplex/ChainComplex.h"
#include "../../Geometry/Algebraic/CellComplex/CellComplex.h"

using namespace std;

namespace Potential
{
namespace Contact
{
class Data;
class LocalState;
template <unsigned int> class Energy;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

        class Data : public Material::Data {
            friend class LocalState;
            friend class Energy < 0 >;
            friend class Energy < 1 >;

public:

	Data();
	virtual ~Data();
	Data(const double &);
	Data(const Data &);
	Data & operator = (const Data &);
	const double & GetStiffness() const;
	double & GetStiffness();
	void Randomize();

private:

	double Stiffness;
};

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Element::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;

public: 

	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef vector<Set::VectorSpace::Vector> range_type;

	LocalState();
	virtual ~LocalState();
	Element::LocalState *Clone() const;
	LocalState(Data *, Geometry::SubComplex *,
		Geometry::Cell *, Geometry::Cell *,
		const map<Geometry::Cell *, Set::Manifold::Point *> &);
	LocalState(const LocalState &);
	LocalState & operator = (const LocalState &);
	void operator ++ ();
	set<Set::Manifold::Point *> GetNodes() const;
	set<pair<Set::Manifold::Point *, Set::Manifold::Point *> > GetNodePairs() const;
	void GetNodes(set<Set::Manifold::Point *> &) const;

private:

	Data *Properties;
	Geometry::SubComplex *S;
	Geometry::Cell *Tet1;
	Geometry::Cell *Tet2;
	map<Geometry::Cell *, Set::Manifold::Point *> Bind;
};

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<0> : public Element::Energy<0>
{
public: 

	typedef Potential::Contact::Energy<1> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<0> *Clone() const;
	Element::LocalState *GetLocalState() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	const Energy<0> & operator = (const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<1> : public Element::Energy<1>
{
public: 

	typedef Potential::Contact::Energy<2> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<1> *Clone() const;
	Element::LocalState *GetLocalState() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	const Energy<1> & operator = (const Energy<1> &);
	range_type operator () (const domain_type &) const;
	void operator()(const domain_type &, range_type &) const;

private:

	LocalState *LS;
};

}

}

#endif // !defined(POTENTIAL_CONTACT_H__INCLUDED_)

