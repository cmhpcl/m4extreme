// Ogden.h: interface for the Ogden class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_OGDEN_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_OGDEN_H__INCLUDED_

#pragma once

#include <vector>
#include <utility>
#include "../Symmetric.h"
#include "../Gas/Gas.h"
#include "../LK2FK/LK2FK.h"
#include "../../../Utils/LinearAlgebra/EigenSym/EigenSym.h"
#include "../../../Set/Manifold/SymmetricSpace/SymmetricSpace.h"

namespace Material
{
namespace Symmetric
{
namespace Ogden
{
class Data;
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

class Data
{
friend class LocalState;
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public:

	Data();
	virtual ~Data();
	Data(const vector<double> &, const vector<double> &);
	Data(const Data &);
	Data & operator = (const Data &);
	const vector<double> & GetAlpha() const;
	vector<double> & GetAlpha();
	const vector<double> & GetMu() const;
	vector<double> & GetMu();
	void Randomize();

private:

	vector<double> Alpha;
	vector<double> Mu;
};

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Symmetric::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Symmetric::Ogden::Energy<0> energy_type;
	typedef Set::VectorSpace::Sym domain_type;

	LocalState();
	LocalState(Data *, Material::Symmetric::Gas::LocalState *);
	virtual ~LocalState();
	Material::Symmetric::LocalState *Clone() const;
	LocalState(const LocalState &);
	void operator ++ ();

private:

	Data *Properties;
	Material::Symmetric::Gas::LocalState *LS;

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
class Energy<0> : public Material::Symmetric::Energy<0>
{
public: 

	typedef Material::Symmetric::Ogden::Energy<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<0> *Clone() const;
	Energy(LocalState *, Material::Symmetric::Gas::Energy<0> *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Symmetric::Gas::Energy<0> *f;

private:

	Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1> : public Material::Symmetric::Energy<1>
{
public: 

	typedef Material::Symmetric::Ogden::Energy<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::SymDual range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<1> *Clone() const;
	Energy(LocalState *, Material::Symmetric::Gas::Energy<1> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Symmetric::Gas::Energy<1> *Df;

private:

	Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2> : public Material::Symmetric::Energy<2>
{
public: 

	typedef Material::Symmetric::Ogden::Energy<3> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<2> *Clone() const;
	Energy(LocalState *, Material::Symmetric::Gas::Energy<2> *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Symmetric::Gas::Energy<2> *DDf;

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
class Jet<0> : public Material::Symmetric::Jet<0>
{
public: 

	typedef Material::Symmetric::Ogden::Jet<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<double, Set::VectorSpace::SymDual> range_type;

	Jet();
	virtual ~Jet();
	Material::Symmetric::Jet<0> *Clone() const;
	Jet(LocalState *, Material::Symmetric::Gas::Jet<0> *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Symmetric::Gas::Jet<0> *g;

private:

	Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Material::Symmetric::Jet<1>
{
public: 

	typedef Material::Symmetric::Ogden::Jet<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<Set::VectorSpace::SymDual, 
		Set::VectorSpace::Hom> range_type;

	Jet();
	virtual ~Jet();
	Material::Symmetric::Jet<1> *Clone() const;
	Jet(LocalState *, Material::Symmetric::Gas::Jet<1> *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Symmetric::Gas::Jet<1> *Dg;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_OGDEN_H__INCLUDED_
