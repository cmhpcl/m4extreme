// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_THERMALLK2FK__INCLUDED_)
#define MATERIAL_GAS_EOS_THERMALLK2FK__INCLUDED_

#pragma once

#include <utility>
#include "Material/Gas/EoS/EoS.h"

namespace Material
{
  namespace Gas
  {
    namespace EoS
    {
      namespace ThermalLK2FK
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
	public:
	  Data() {}
	  ~Data() {}

	Data(double alpha0_, double T0_, double alpha1_=0.0) : 
	  T0(T0_), alpha0(alpha0_), alpha1(alpha1_){}
	  double GetT0() const { return T0;}
	  double GetAlpha0() const { return alpha0;}
	  double GetAlpha1() const { return alpha1;}
	private:
	  Data(Data &);
	  const Data & operator = (const Data &);

	private:
	  double T0, alpha0, alpha1;
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

	    typedef Material::Gas::EoS::ThermalLK2FK::Energy<0> energy_type;
	    typedef Material::Gas::EoS::ThermalLK2FK::Jet<0> jet_type;
	    typedef double domain_type;

	    virtual ~LocalState();
	    Material::Gas::EoS::LocalState *Clone() const;
	    LocalState(Data*, Material::Gas::EoS::LocalState *);
	    LocalState(Data*, double, Material::Gas::EoS::LocalState *);
	    LocalState(const LocalState &);
	    void operator ++ ();
	    double ComputeThermalExpansion() const;
	    double GetTemperature() const;
	    void Reset(const double &);

	  private:
	    Data * Prop;
	    double _T;
	    Material::Gas::EoS::LocalState *LS;

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

	    typedef Material::Gas::EoS::ThermalLK2FK::Energy<1> tangent_type;
	    typedef double domain_type;
	    typedef double range_type;

	    virtual ~Energy();
	    Material::Gas::EoS::Energy<0> *Clone() const;
	    Energy(LocalState * LS_, Material::Gas::EoS::Energy<0> *);
	    Energy(const Energy<0> &);
	    range_type operator () (const domain_type &) const;

	  private:
	    LocalState * LS;
	    Material::Gas::EoS::Energy<0> *f;

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

	    typedef Material::Gas::EoS::ThermalLK2FK::Energy<2> tangent_type;
	    typedef double domain_type;
	    typedef double range_type;

	    virtual ~Energy();
	    Material::Gas::EoS::Energy<1> *Clone() const;
	    Energy(LocalState * LS_, Material::Gas::EoS::Energy<1> *);
	    Energy(const Energy<1> &);
	    range_type operator () (const domain_type &) const;

	  private:

	    Material::Gas::EoS::Energy<1> *Df;

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

	    typedef Material::Gas::EoS::ThermalLK2FK::Energy<3> tangent_type;
	    typedef double domain_type;
	    typedef double range_type;

	    virtual ~Energy();
	    Material::Gas::EoS::Energy<2> *Clone() const;
	    Energy(LocalState * LS_, Material::Gas::EoS::Jet<1> *);
	    Energy(const Energy<2> &);
	    range_type operator () (const domain_type &) const;

	  private:

	    Material::Gas::EoS::Jet<1> *Dg;

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
	    Jet(LocalState * LS_, Material::Gas::EoS::Jet<0> *);
	    Jet(const Jet<0> &);
	    range_type operator () (const domain_type &) const;

	  private:
	    LocalState * LS;
	    Material::Gas::EoS::Jet<0> *g;

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
	    Jet(LocalState * LS_, Material::Gas::EoS::Jet<1> *);
	    Jet(const Jet<1> &);
	    range_type operator () (const domain_type &) const;

	  private:
	    LocalState * LS;
	    Material::Gas::EoS::Jet<1> *Dg;

	  private:

	    const Jet<1> & operator = (const Jet<1> &);
	  };

      }

    }

  }

}

#endif // !defined(MATERIAL_GAS_EOS_THERMALLK2FK__INCLUDED_)
