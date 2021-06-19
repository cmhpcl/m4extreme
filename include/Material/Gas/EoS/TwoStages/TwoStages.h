// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////


#if !defined(MATERIAL_GAS_EOS_TWOSTAGES_INCLUDED_)
#define MATERIAL_GAS_EOS_TWOSTAGES_INCLUDED_

#pragma once

#include <map>
#include <utility>

#include "Material/Gas/EoS/EoS.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace TwoStages
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

// Class Data

class Data : public Material::Gas::EoS::Data 
{
  friend class LocalState;
 public:
  Data(){}
  ~Data(){}

  Data(double alpha0_, double T0_): T0(T0_), alpha0(alpha0_){}
  double GetT0() const { return T0;}
  double GetAlpha0() const { return alpha0;}

 private:
  Data(Data &);
  const Data & operator = (const Data &);

 private:
  double T0, alpha0;
};

// Class LocalState

class LocalState : public Material::Gas::EoS::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public:
        LocalState() {}
        virtual ~LocalState(){}
        Material::Gas::EoS::LocalState *Clone() const {
	  return new LocalState(*this);
	}

        LocalState(Data * Prop_, 
		   Material::Gas::EoS::LocalState * LScomp_,  
		   Material::Gas::EoS::LocalState * LSten_):
	  Prop(Prop_), LScomp(LScomp_), LSten(LSten_) {
	  _T = Prop->T0;
	}

        LocalState(Data * Prop_,
		   Material::Gas::EoS::LocalState * LScomp_, 
		   Material::Gas::EoS::LocalState * LSten_, 
		   double T) :
	  Prop(Prop_), LScomp(LScomp_), LSten(LSten_), _T(T) {
	  LScomp->Reset(_T);
	  LSten->Reset(_T);
	}

	LocalState(const LocalState & rhs):
	  Prop(rhs.Prop), LScomp(rhs.LScomp), LSten(rhs.LSten), _T(rhs._T) {}

	double ComputeThermalExpansion() const {
	  if (_T <= Prop->T0) {
	    return 0.0;
	  }
	  else {
	    return Prop->alpha0 * (_T - Prop->T0);
	  }
	}

	double GetTemperature() const { return _T; }
	
        void Reset(const double & T) {
	  _T = T;
	  LScomp->Reset(_T);
	  LSten->Reset(_T);
	}

        void operator ++ () {
	  LScomp->operator++();
	  LSten->operator++();
	}

private:
	Data * Prop;
	double _T;

	Material::Gas::EoS::LocalState * LScomp;
	Material::Gas::EoS::LocalState * LSten;

private:

        const LocalState & operator = (const LocalState &);
};

// Class Energy<0>

template<>
class Energy<0> : public Material::Gas::EoS::Energy<0>
{
public:

        typedef Material::Gas::EoS::TwoStages::Energy<1> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<0> *Clone() const;
        Energy(LocalState *, 
	       Material::Gas::EoS::Energy<0> *, 
	       Material::Gas::EoS::Energy<0> *);
        Energy(const Energy<0> &);
        range_type operator () (const domain_type &) const;

private:

	LocalState * LS;
        Material::Gas::EoS::Energy<0> * Wcomp;
	Material::Gas::EoS::Energy<0> * Wten;

private:

        const Energy<0> & operator = (const Energy<0> &);
};

// Class Energy<1>

template<>
class Energy<1> : public Material::Gas::EoS::Energy<1>
{
public:

        typedef Material::Gas::EoS::TwoStages::Energy<2> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<1> *Clone() const;
        Energy(LocalState *,
	       Material::Gas::EoS::Energy<1> *, 
	       Material::Gas::EoS::Energy<1> *);
        Energy(const Energy<1> &);
        range_type operator () (const domain_type &) const;

private:

	LocalState * LS;
        Material::Gas::EoS::Energy<1> * DWcomp;
	Material::Gas::EoS::Energy<1> * DWten;

private:

        const Energy<1> & operator = (const Energy<1> &);
};

// Class Energy<2>

template<>
class Energy<2> : public Material::Gas::EoS::Energy<2>
{
public:

        typedef Material::Gas::EoS::TwoStages::Energy<3> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<2> *Clone() const;
        Energy(LocalState *,	       
	       Material::Gas::EoS::Energy<2> * ,
	       Material::Gas::EoS::Energy<2> * );
        Energy(const Energy<2> &);
        range_type operator () (const domain_type &) const;

private:

	LocalState * LS;	
	Material::Gas::EoS::Energy<2> * DDWcomp;
	Material::Gas::EoS::Energy<2> * DDWten;

private:

        const Energy<2> & operator = (const Energy<2> &);
};

// Class Jet<p>

template<unsigned int p> class Jet;

// Class Jet<0>

template<>
class Jet<0> : public Material::Gas::EoS::Jet<0>
{
public:

        typedef Material::Gas::EoS::TwoStages::Jet<1> tangent_type;
        typedef double domain_type;
        typedef pair<double,double> range_type;

        virtual ~Jet();
        Material::Gas::EoS::Jet<0> *Clone() const;
        Jet(LocalState *,
	    Material::Gas::EoS::Jet<0> *,	
	    Material::Gas::EoS::Jet<0> *);
        Jet(const Jet<0> &);
        range_type operator () (const domain_type &) const;

private:

	LocalState * LS;
        Material::Gas::EoS::Jet<0> * Jcomp;
	Material::Gas::EoS::Jet<0> * Jten;

private:

        const Jet<0> & operator = (const Jet<0> &);
};

// Class Jet<1>
template <>
class Jet<1> : public Material::Gas::EoS::Jet<1>
{
public:

  typedef Material::Gas::EoS::TwoStages::Jet<2> tangent_type;
        typedef double domain_type;
        typedef pair<double,double> range_type;

        virtual ~Jet();
        Material::Gas::EoS::Jet<1> *Clone() const;
        Jet(LocalState *,
	    Material::Gas::EoS::Jet<1> *,
	    Material::Gas::EoS::Jet<1> *);
        Jet(const Jet<1> &);
        range_type operator () (const domain_type &) const;

private:

	LocalState * LS;
        Material::Gas::EoS::Jet<1> * DJcomp;
	Material::Gas::EoS::Jet<1> * DJten;

private:

        const Jet<1> & operator = (const Jet<1> &);
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_TWOSTAGES_INCLUDED_)
