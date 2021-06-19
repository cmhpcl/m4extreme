// LKMieGruneisen.h: interface for the LKMieGruneisen class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_LKMIEGRUNEISEN__INCLUDED_)
#define MATERIAL_GAS_EOS_LKMIEGRUNEISEN__INCLUDED_

#pragma once

#include <utility>
#include "../EoS.h"
#include "Utils/Fields.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace LKMieGruneisen
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

class Data : public Material::Gas::EoS::Data
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
	Data(const double &,const double &,const double &,
	     const double &,const double &,const double &);
	Data(const Data &);
	Data & operator = (const Data &);

private:
	double T0, Cv;
	double k1, k2, k3;
	double gamma;
};

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Gas::EoS::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Data data_type;
	typedef Material::Gas::EoS::LKMieGruneisen::Energy<0> energy_type;
	typedef Material::Gas::EoS::LKMieGruneisen::Jet<0> jet_type;
	typedef double domain_type;

	virtual ~LocalState();
	Material::Gas::EoS::LocalState *Clone() const;
	LocalState(Data *);
	LocalState(const LocalState &);
	void operator ++ ();
	const double & GetK() const;
	const double & GetThermalExpansion() const;

	virtual void Reset(double);
	virtual double GetTemperature() const;
private:

	Data *Prop;
	double T;
	double K, alpha;

private:

	const LocalState & operator = (const LocalState &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<0> : public Material::Gas::EoS::Energy<0>
{
public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<0> *Clone() const;
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
class Energy<1> : public Material::Gas::EoS::Energy<1>
{
public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<1> *Clone() const;
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
class Energy<2> : public Material::Gas::EoS::Energy<2>
{
public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Energy<3> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<2> *Clone() const;
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
class Jet<0> : public Material::Gas::EoS::Jet<0>
{
public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Jet<1> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Gas::EoS::Jet<0> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	const Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Material::Gas::EoS::Jet<1>
{
public: 

	typedef Material::Gas::EoS::LKMieGruneisen::Jet<2> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Gas::EoS::Jet<1> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;

private:

	const Jet<1> & operator = (const Jet<1> &);
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_LKMIEGRUNEISEN__INCLUDED_)
