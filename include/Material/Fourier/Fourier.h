// Fourier.h: interface for the Fourier class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_FOURIER_H_INCLUDED_)
#define MATERIAL_FOURIER_H_INCLUDED_

#pragma once

#include <utility>
#include "Material/Material.h"
#include "Utils/Fields.h"

namespace Material
{
  namespace Fourier
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
	Data(const double &);
	Data(const double &, const double &, const double &,
	     const double &, const double &, const double &);
	Data(const Data &);
	Data & operator = (const Data &);

    private:
	bool hasPhaseChange;
        double Ksolid, Kliquid, Kgas;
	double Tm, Tb, DT, sigma; // melting, boiling, transition zone
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

      typedef Material::Fourier::Energy<0> energy_type;
      typedef Set::VectorSpace::Vector domain_type;

      LocalState(double, Data *);
      virtual ~LocalState();
      Material::LocalState *Clone() const;
      LocalState(const LocalState &);
        
      const double & GetTold() const;
      const double & GetT() const;
      const double & GetK() const;
      void Reset(double T_);        
      void operator ++ ();

    protected:
      double K;
      double Told, T;
      Data * D;

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

      typedef Material::Fourier::Energy<1> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef double range_type;

      Energy(LocalState *);
      virtual ~Energy();
      Material::Energy<0> *Clone() const;
      Energy(const Energy<0> &);
      range_type operator () (const domain_type &) const;

    private:

      LocalState *LS;

    private:

      Energy<0> & operator = (const Energy<0> &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy<1> : public Material::Energy<1>
    {
    public: 

      typedef Material::Fourier::Energy<2> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef Set::VectorSpace::Vector range_type;

      virtual ~Energy();
      Material::Energy<1> *Clone() const;
      Energy(LocalState *);
      Energy(const Energy<1> &);
      range_type operator () (const domain_type &) const;

    private:

      LocalState *LS;
 
    private:

      Energy<1> & operator = (const Energy<1> &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<2>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy<2> : public Material::Energy<2>
    {
    public: 

      typedef Material::Fourier::Energy<3> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef Set::VectorSpace::Hom range_type;

      Energy(LocalState *);
      virtual ~Energy();
      Material::Energy<2> *Clone() const;
      Energy(const Energy<2> &);
      range_type operator () (const domain_type &) const;

    private:

      LocalState *LS;
 
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
      class Jet<0> : public Material::Jet<0>
    {
    public: 

      typedef Material::Fourier::Jet<1> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef pair<double,Set::VectorSpace::Vector> range_type;

      Jet(LocalState *);
      virtual ~Jet();
      Material::Jet<0> *Clone() const;
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
      class Jet<1> : public Material::Jet<1>
    {
    public: 

      typedef Material::Fourier::Jet<2> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

      Jet(LocalState *);
      virtual ~Jet();
      Material::Jet<1> *Clone() const;
      Jet(const Jet<1> &);
      range_type operator () (const domain_type &) const;

    private:

      LocalState *LS;

    private:

      Jet<1> & operator = (const Jet<1> &);
    };

  }

}

#endif // !defined(MATERIAL_FOURIER_H_INCLUDED_
