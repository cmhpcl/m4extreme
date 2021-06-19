// LogLaw.h: interface for the LogLaw class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_UNIAXIAL_LOGLAW__INCLUDED_)
#define MATERIAL_UNIAXIAL_LOGLAW__INCLUDED_

#pragma once

#include "Material/Uniaxial/Uniaxial.h"
#include "Utils/Fields.h"

namespace Material
{
namespace Uniaxial
{
namespace LogLaw
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
	Data(const double &);
	Data(const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &, const double &, const double &, const double &);
	Data(const Data &);
	Data & operator = (const Data &);

private:
	bool hasPhaseChange;
        double Csolid, Cliquid, Cgas;
	double Tm, Tb, DT, sigma; // melting, boiling, transition zone
	double Lm, Lb, Ls, Lc; // melting, boiling, solidification, condensation
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

	typedef Material::Uniaxial::LogLaw::Data data_type;
	typedef Material::Uniaxial::LogLaw::Energy<0> energy_type;
	typedef Material::Uniaxial::LogLaw::Jet<0> jet_type;

	virtual ~LocalState();
	Material::Uniaxial::LocalState *Clone() const;
	LocalState(double, Data *);
	LocalState(const LocalState &);
	void operator ++ ();
	void Reset(double);

	const double & GetCv() const;
private:
	double Cv;
	double TOld, T;
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

	typedef Material::Uniaxial::LogLaw::Energy<1> tangent_type;
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

	typedef Material::Uniaxial::LogLaw::Energy<2> tangent_type;
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

	typedef Material::Uniaxial::LogLaw::Energy<3> tangent_type;
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

	typedef Material::Uniaxial::LogLaw::Jet<1> tangent_type;
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

	typedef Material::Uniaxial::LogLaw::Jet<2> tangent_type;
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

#endif // !defined(MATERIAL_UNIAXIAL_LOGLAW__INCLUDED_)
