// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_JWL__INCLUDED_)
#define MATERIAL_GAS_EOS_JWL__INCLUDED_

#pragma once

#include <utility>
#include "Material/Gas/EoS/EoS.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace JWL
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////

class Data : public Material::Gas::EoS::Data {
  friend class LocalState;
  friend class Energy<0>;
  friend class Energy<1>;
  friend class Energy<2>;
  friend class Jet<0>;
  friend class Jet<1>;

 public:
  Data() {}
  ~Data() {}

  Data(double T0_, double A_, double B_, double R1_,
       double R2_, double w_, double Cv_)
    :T0(T0_), A(A_), B(B_), R1(R1_), R2(R2_), w(w_), Cv(Cv_){}

 private:
  Data(Data &);
  const Data & operator = (const Data &);

 private:
  // Cv: heat capacity per unit volume
  double T0, A, B, R1, R2, w, Cv;
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

	typedef Material::Gas::EoS::JWL::Energy<0> energy_type;
	typedef Material::Gas::EoS::JWL::Jet<0> jet_type;
	typedef double domain_type;

	virtual ~LocalState();
	Material::Gas::EoS::LocalState *Clone() const;
	LocalState(Data*);
	LocalState(Data*, double);
	LocalState(const LocalState &);
	void operator ++ ();
	double GetTemperature() const;
	void Reset(const double &);

private:
	Data * Prop;
	double T;
	const LocalState & operator = (const LocalState &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<0> : public Material::Gas::EoS::Energy<0>
{
public: 

	typedef Material::Gas::EoS::JWL::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:
	LocalState * LS;
	const Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<1> : public Material::Gas::EoS::Energy<1>
{
public: 

	typedef Material::Gas::EoS::JWL::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<1> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:
	LocalState * LS;
	const Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template<> 
class Energy<2> : public Material::Gas::EoS::Energy<2>
{
public: 

	typedef Material::Gas::EoS::JWL::Energy<3> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Gas::EoS::Energy<2> *Clone() const;
	Energy(LocalState * );
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:
	LocalState * LS;
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
	Jet(LocalState *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:
	LocalState * LS;
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
	Jet(LocalState *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:
	LocalState * LS;
	const Jet<1> & operator = (const Jet<1> &);
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_THERMALLK2FK__INCLUDED_)
