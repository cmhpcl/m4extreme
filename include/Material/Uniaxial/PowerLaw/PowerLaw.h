// PowerLaw.h: interface for the PowerLaw class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_POWERLAW__INCLUDED_)
#define MATERIAL_POWERLAW__INCLUDED_

#pragma once

#include "../Uniaxial.h"

namespace Material
{
namespace Uniaxial
{
namespace PowerLaw
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

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
	Data(
		const double &, 
		const double &, 
		const double &, 
		const double &,
		double=0.0);
	Data(const Data &);
	Data & operator = (const Data &);
	void Randomize();

private:

	double f0;
	double r0;
	double x;
	double xpp;
	double xmm;
	double C0;
	double C1;
	double C2;
	double C3;
	double C4;
};

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Uniaxial::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Uniaxial::PowerLaw::Data data_type;
	typedef Material::Uniaxial::PowerLaw::Energy<0> energy_type;
	typedef Material::Uniaxial::PowerLaw::Jet<0> jet_type;

	virtual ~LocalState();
	Material::Uniaxial::LocalState *Clone() const;
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
class Energy<0> : public Material::Uniaxial::Energy<0>
{
public: 

	typedef Material::Uniaxial::PowerLaw::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Uniaxial::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	const Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<1> : public Material::Uniaxial::Energy<1>
{
public: 

	typedef Material::Uniaxial::PowerLaw::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Uniaxial::Energy<1> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	const Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<2> : public Material::Uniaxial::Energy<2>
{
public: 

	typedef Material::Uniaxial::PowerLaw::Energy<3> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Uniaxial::Energy<2> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	const Energy<2> & operator = (const Energy<2> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<>
class Jet<0> : public Material::Uniaxial::Jet<0>
{
public: 

	typedef Material::Uniaxial::PowerLaw::Jet<1> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Uniaxial::Jet<0> *Clone() const;
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
class Jet<1> : public Material::Uniaxial::Jet<1>
{
public: 

	typedef Material::Uniaxial::PowerLaw::Jet<2> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Uniaxial::Jet<1> *Clone() const;
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

#endif // !defined(MATERIAL_POWERLAW__INCLUDED_)
