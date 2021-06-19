// Multiphase.h: interface for the Multiphase class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_MULTIPHASE_INCLUDED_)
#define MATERIAL_GAS_EOS_MULTIPHASE_INCLUDED_

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../EoS.h"

using namespace std;

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace Multiphase
{
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

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

	typedef Material::Gas::EoS::Multiphase::Energy<0> energy_type;
	typedef Material::Gas::EoS::Multiphase::Jet<0> jet_type;
	typedef double domain_type;

	LocalState();
	virtual ~LocalState();
	Material::Gas::EoS::LocalState *Clone() const;
	LocalState(const vector<Material::Gas::EoS::LocalState *> &);
	LocalState(const LocalState &);
	void operator ++ ();
	void Reset(const double &);

private:

	vector<Material::Gas::EoS::LocalState *> LS;

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

	typedef Material::Gas::EoS::Multiphase::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<0> *Clone() const;
	Energy(const vector<Material::Gas::EoS::Energy<0> *> &);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Gas::EoS::Energy<0> *> f;

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

	typedef Material::Gas::EoS::Multiphase::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<1> *Clone() const;
	Energy(const vector<Material::Gas::EoS::Energy<0> *> &f,
		   const vector<Material::Gas::EoS::Energy<1> *> &df);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Gas::EoS::Energy<0> *> f;
	vector<Material::Gas::EoS::Energy<1> *> df;

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

	typedef Material::Gas::EoS::Multiphase::Energy<3> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<2> *Clone() const;
	Energy(const vector<Material::Gas::EoS::Energy<0> *> &f,
		   const vector<Material::Gas::EoS::Energy<2> *> &ddf);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Gas::EoS::Energy<0> *> f;
	vector<Material::Gas::EoS::Energy<2> *> ddf;

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

	typedef Material::Gas::EoS::Jet<1> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Gas::EoS::Jet<0> *Clone() const;
	Jet(const vector<Material::Gas::EoS::Energy<0> *> &f,
		const vector<Material::Gas::EoS::Energy<1> *> &df);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Gas::EoS::Energy<0> *> f;
	vector<Material::Gas::EoS::Energy<1> *> df;

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

	typedef Material::Gas::EoS::Jet<2> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	Material::Gas::EoS::Jet<1> *Clone() const;
	Jet(const vector<Material::Gas::EoS::Energy<0> *> &,
		const vector<Material::Gas::EoS::Jet<1> *> &);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	vector<Material::Gas::EoS::Energy<0> *> f;
	vector<Material::Gas::EoS::Jet<1> *> dg;

private:

	const Jet<1> & operator = (const Jet<1> &);
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_MULTIPHASE_INCLUDED_)
