// Exponential.h: interface for the Exponential class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SOURCE_EXPONENTIAL_H__INCLUDED_)
#define MATERIAL_SOURCE_EXPONENTIAL_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Material.h"
#include "Set/Algebraic/AlgLib.h"

namespace Material
{
namespace Source
{
namespace Exponential
{
class Data;
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

class Data : public Material::Data
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
	Data(const vector<double> &,
	     const vector<double> &,
	     const vector<double> &,
	     const vector<double> &);
	Data(const vector<double> &,
	     const vector<double> &,
	     const vector<double> &,
	     const vector<double> &,
	     const vector<double> &);
	Data(const Data &);
	Data & operator = (const Data &);

private:
	vector<double> _x;
	vector<double> _y;
	vector<double> _z;
	vector<double> _b;
	vector<double> _A;
};

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

	typedef Material::Source::Exponential::Data data_type;
	typedef Material::Source::Exponential::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(Data *);
	LocalState(const LocalState &);
	void operator ++ ();

private:

	Data *Prop;

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

	typedef Material::Source::Exponential::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	Energy<0> & operator = (const Energy<0> &);
};


//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;


}

}

}

#endif // !defined(MATERIAL_SOURCE_EXPONENTIAL_H__INCLUDED_
