// FaceEdgePair.h: interface for the FaceEdgePair class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_CONTACT_FACEEDGEPAIR_H_INCLUDED_)
#define POTENTIAL_CONTACT_FACEEDGEPAIR_H_INCLUDED_

#pragma once

#include <set>
#include <map>
#include <utility>
#include "../../Set/Algebraic/AlgLib.h"
#include "../../Set/Manifold/Manifold.h"

using namespace std;

namespace Potential
{
namespace Contact
{
namespace FaceEdgePair
{
template <unsigned int> class Intersection;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Intersection<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Intersection;

//////////////////////////////////////////////////////////////////////
// Class Intersection<0>
//////////////////////////////////////////////////////////////////////

template<> class Intersection<0>
{
public: 

	typedef Potential::Contact::FaceEdgePair::Intersection<1> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef Set::VectorSpace::Vector range_type;

	Intersection();
	virtual ~Intersection();
	range_type operator () (const domain_type &, const domain_type &) const;

private:

	Intersection(const Intersection<0> &);
	Intersection<0> & operator = (const Intersection<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Intersection<1>
//////////////////////////////////////////////////////////////////////

template <> class Intersection<1>
{
public: 

	typedef Potential::Contact::FaceEdgePair::Intersection<2> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Hom> range_type;

	Intersection();
	virtual ~Intersection();
	range_type operator () (const domain_type &, const domain_type &) const;

private:

	Intersection(const Intersection<1> &);
	Intersection<1> & operator = (const Intersection<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<> class Jet<0>
{
public: 

	typedef Potential::Contact::FaceEdgePair::Jet<1> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef pair<Set::VectorSpace::Vector,
		map<Set::Manifold::Point *, Set::VectorSpace::Hom> > range_type;

	Jet();
	virtual ~Jet();
	range_type operator () (const domain_type &, const domain_type &) const;

private:

	Jet(const Jet<0> &);
	Jet<0> & operator = (const Jet<0> &);
};

}

}

}

#endif // !defined(POTENTIAL_CONTACT_FACEEDGEPAIR_H_INCLUDED_
