// SurfaceTension.h: interface for the SurfaceTension class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_FIELD_SURFACETENSION_H__INCLUDED_)
#define POTENTIAL_FIELD_SURFACETENSION_H__INCLUDED_

#pragma once

#include <utility>
#include "Clock/Clock.h"
#include "Utils/Fields.h"
#include "Potential/Field/Field.h"
#include "Set/Algebraic/AlgLib.h"

namespace Potential
{
namespace Field
{
namespace SurfaceTension	
{

class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

 
//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Potential::Field::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 
	typedef Potential::Field::SurfaceTension::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Potential::Field::LocalState *Clone() const;
	LocalState(Clock *,
		   const Set::VectorSpace::Vector *,
		   const double *,
		   m4extreme::Utils::ScalarField *,
		   const Set::VectorSpace::Vector &);
	LocalState(Clock *, 
		   const double &,
		   const Set::VectorSpace::Vector *,
		   const double *,
		   m4extreme::Utils::ScalarField *);

	LocalState(const LocalState &);
	virtual void operator ++ ();
	virtual void Reset(double);
	
	const Set::VectorSpace::Vector & GetArea() const;
	const double & GetCurvature() const;
	const double & GetCoefficient() const;

private:
	bool isTemperatureDependent;
	Clock *Chronos;

	// update the neighbor outside of the class
	const Set::VectorSpace::Vector * normed_area;
	const double * curvature;

	// surface tension coefficient function
	m4extreme::Utils::ScalarField *Prop;
	double sigma;

	// current location of the point
	// updated when calling function Energy<p>::operator()(x)
	Set::VectorSpace::Vector xcoord;

	// current temperature of the point
	// updated when calling function Reset(T)
	double T;
private:

	LocalState & operator = (const LocalState &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Energy;

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<>
class Energy<0> : public Potential::Field::Energy<0>
{
public: 

	typedef Potential::Field::SurfaceTension::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Potential::Field::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1> : public Potential::Field::Energy<1>
{
public: 

	typedef Potential::Field::SurfaceTension::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Potential::Field::Energy<1> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2> : public Potential::Field::Energy<2>
{
public: 

	typedef Potential::Field::SurfaceTension::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Potential::Field::Energy<2> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Energy<2> & operator = (const Energy<2> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<>
class Jet<0> : public Potential::Field::Jet<0>
{
public: 

	typedef Potential::Field::SurfaceTension::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Potential::Field::Jet<0> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Potential::Field::Jet<1>
{
public: 

	typedef Potential::Field::SurfaceTension::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Potential::Field::Jet<1> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(POTENTIAL_FIELD_SURFACETENSION_H__INCLUDED_)
