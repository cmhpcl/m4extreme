// Taylor.h: interface for the Taylor class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_TAYLOR_H__INCLUDED_)
#define MATERIAL_TAYLOR_H__INCLUDED_

#pragma once

#include <vector>
#include <utility>
#include "../Material.h"

namespace Material
{
namespace Taylor
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

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

	typedef Material::Taylor::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	LocalState(const vector<Material::LocalState *> &);
	LocalState(const vector<Material::LocalState *> &, 
		    const vector<double> &);
	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(const LocalState &);
	const vector<double> & GetWt() const;
	vector<double> & GetWt();
	void operator ++ ();

private:

	vector<Material::LocalState *> LS;
	vector<double> Wt;

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
class Energy<0> : public Material::Energy<0>
{
public: 

	typedef Material::Taylor::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	Energy(LocalState *, const vector<Material::Energy<0> *> &);
	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Energy<0> *> f;
	LocalState *LS;

private:

	Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1> : public Material::Energy<1>
{
public: 

	typedef Material::Taylor::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Material::Energy<1> *Clone() const;
	Energy(LocalState *, const vector<Material::Energy<1> *> &);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Energy<1> *> Df;
	LocalState *LS;

private:

	Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2> : public Material::Energy<2>
{
public: 

	typedef Material::Taylor::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Material::Energy<2> *Clone() const;
	Energy(const Energy<2> &);	
	Energy(LocalState *, const vector<Material::Energy<2> *> &);

	range_type operator () (const domain_type &) const;

private:

	vector<Material::Energy<2> *> DDf;
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
class Jet<0> : public Material::Jet<0>
{
public: 

	typedef Material::Taylor::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	Jet(LocalState *, const vector<Material::Jet<0> *> &);
	virtual ~Jet();
	Material::Jet<0> *Clone() const;
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Jet<0> *> g;
	LocalState *LS;

private:

	Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Material::Jet<1>
{
public: 

	typedef Material::Taylor::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	Jet(LocalState *, const vector<Material::Jet<1> *> &);
	virtual ~Jet();
	Material::Jet<1> *Clone() const;
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Jet<1> *> Dg;
	LocalState *LS;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

#endif // !defined(MATERIAL_TAYLOR_H__INCLUDED_
  
