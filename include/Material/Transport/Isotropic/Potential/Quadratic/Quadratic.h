// Quadratic.h: interface for the Quadratic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL_QUADRATIC__INCLUDED_)
#define MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL_QUADRATIC__INCLUDED_

#pragma once

#include <utility>
#include "../Potential.h"

namespace Material
{
namespace Transport
{
namespace Isotropic
{
namespace Potential
{
namespace Quadratic
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
	Data(const double &);
	Data(const Data &);
	virtual ~Data();
	Data & operator = (const Data &);
	const double GetC() const;
	double & GetC();
	void Randomize();

private:

	double C;
};

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Transport::Isotropic::Potential::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Transport::Isotropic::Potential::LocalState base_type;
	typedef Material::Transport::Isotropic::Potential::Quadratic::Data data_type;
	typedef Material::Transport::Isotropic::Potential::Quadratic::Energy<0> energy_type;
	typedef double domain_type;

	virtual ~LocalState();
	base_type *Clone() const;
	LocalState(Data *);
	LocalState(const LocalState &);
	void operator ++ ();

private:

	Data *Prop;

private:

	LocalState & operator = (const LocalState &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<0> : public Material::Transport::Isotropic::Potential::Energy<0>
{
public: 

	typedef Material::Transport::Isotropic::Potential::Energy<0> base_type;
	typedef Material::Transport::Isotropic::Potential::Quadratic::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	base_type *Clone() const;
	Energy(const Energy<0> &);
	Energy(LocalState *);
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
class Energy<1> : public Material::Transport::Isotropic::Potential::Energy<1>
{
public: 

	typedef Material::Transport::Isotropic::Potential::Energy<1> base_type;
	typedef Material::Transport::Isotropic::Potential::Quadratic::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	base_type *Clone() const;
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
class Energy<2> : public Material::Transport::Isotropic::Potential::Energy<2>
{
public: 

	typedef Material::Transport::Isotropic::Potential::Energy<2> base_type;
	typedef Material::Transport::Isotropic::Potential::Quadratic::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Energy(LocalState *);
	Energy(const Energy<2> &);
	base_type *Clone() const;
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
class Jet<0> : public Material::Transport::Isotropic::Potential::Jet<0>
{
public: 

	typedef Material::Transport::Isotropic::Potential::Jet<0> base_type;
	typedef Material::Transport::Isotropic::Potential::Jet<1> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	base_type *Clone() const;
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
class Jet<1> : public Material::Transport::Isotropic::Potential::Jet<1>
{
public: 

	typedef Material::Transport::Isotropic::Potential::Jet<1> base_type;
	typedef Material::Transport::Isotropic::Potential::Jet<2> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	virtual ~Jet();
	base_type *Clone() const;
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

}

}

#endif // !defined(MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL_QUADRATIC__INCLUDED_)
