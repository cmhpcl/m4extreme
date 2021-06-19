// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_MEMBRANE_H__INCLUDED_)
#define MATERIAL_MEMBRANE_H__INCLUDED_

#pragma once

#include <utility>
#include "../Material.h"

using namespace std;

namespace Material
{
namespace Membrane
{
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Membrane::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	LocalState( 
		const unsigned int &n,
		Material::LocalState *, 
		Material::Energy<1> *, 
		Material::Energy<2> *);
	LocalState( 
		const Set::VectorSpace::Vector &,
		Material::LocalState *, 
		Material::Energy<1> *, 
		Material::Energy<2> *);
	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(const LocalState &);
	virtual void operator ++ ();
	virtual void SetNormal(const Set::VectorSpace::Vector &);
	const Set::VectorSpace::Vector & GetNormal() const;
	Set::VectorSpace::Vector & GetNormal();

	void SetTol(double);
	void SetMaxITR(int);

#if defined(_M4EXTREME_MEMBRANE_VIZ_)
        const Set::VectorSpace::Hom & GetStress() const {return *_pSigma;}
#endif

protected:

	virtual range_type operator () (const domain_type &) const;

protected:

#if defined(_M4EXTREME_MEMBRANE_VIZ_)
	Set::VectorSpace::Hom * _pSigma;
#endif
    
	Set::VectorSpace::Vector Normal;
	Material::LocalState *LS;
	Material::Energy<1> *DW;
	Material::Energy<2> *DDW;

	double TOL;
	int MAX_ITR;

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

template<> class Energy<0> : public Material::Energy<0>
{
public: 

	typedef Membrane::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(LocalState *, Material::Energy<0> *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<0> *W;

private:

	Energy & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <> class Energy<1> : public Material::Energy<1>
{
public: 

	typedef Membrane::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Energy(LocalState *, Material::Energy<1> *);
	Material::Energy<1> *Clone() const;
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<1> *DW;

private:

	Energy & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <> class Energy<2> : public Material::Energy<2>
{
public: 

	typedef Membrane::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Energy(LocalState *, Material::Energy<2> *);
	Material::Energy<2> *Clone() const;
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<2> *DDW;

private:

	Energy & operator = (const Energy<2> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<> class Jet<0> : public Material::Jet<0>
{
public: 

	typedef Membrane::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Jet(LocalState *, Material::Jet<0> *);
	Material::Jet<0> *Clone() const;
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Jet<0> *J;

private:

	Jet & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <> class Jet<1> : public Material::Jet<1>
{
public: 

	typedef Membrane::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Jet(LocalState *, Material::Jet<1> *);
	Material::Jet<1> *Clone() const;
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Jet<1> *DJ;

private:

	Jet & operator = (const Jet<1> &);
};

}

}

#endif // !defined(MATERIAL_MEMBRANE_H__INCLUDED_)
