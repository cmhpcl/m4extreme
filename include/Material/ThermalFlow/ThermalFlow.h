// ThermalFlow.h: interface for the ThermalFlow class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_THERMALFLOW_H__INCLUDED_)
#define MATERIAL_THERMALFLOW_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Uniaxial/Uniaxial.h"
#include "Material/Material.h"

namespace Material {
  namespace ThermalFlow {
    class LocalState;
    template <unsigned int> class Energy;
    template <unsigned int> class Jet;

    //////////////////////////////////////////////////////////////////////
    // Class LocalState
    //////////////////////////////////////////////////////////////////////

    class LocalState : public Material::LocalState {
      friend class Energy < 0 >;
      friend class Energy < 1 >;
      friend class Energy < 2 >;
      friend class Jet < 0 >;
      friend class Jet < 1 >;

    public:

      typedef Material::ThermalFlow::Energy < 0 > energy_type;
      typedef Set::VectorSpace::Vector domain_type;

      LocalState();
      virtual ~LocalState();
      LocalState(const LocalState &);

      LocalState(Material::LocalState * ELS_,	      
		 Material::Uniaxial::LocalState * HLS_);

      Material::LocalState *Clone() const;                
      void operator ++();
      void Reset(double);
      void Relax();
      double GetTemperature() const;
      Material::LocalState * GetMLS();
      Material::Uniaxial::LocalState * GetHLS();
      
    private:
      Material::LocalState * _ELS;                
      Material::Uniaxial::LocalState * _HLS;
      double Eta, EtaOld;

    private:
      LocalState & operator =(const LocalState &);                
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Energy<0>
    //////////////////////////////////////////////////////////////////////

    template<>
      class Energy < 0 > : public Material::Energy < 0 > {
    public:

      typedef Material::ThermalFlow::Energy < 1 > tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef double range_type;

      Energy();
      virtual ~Energy();
      Material::Energy < 0 > *Clone() const;
      Energy(LocalState *,
	     Material::Energy < 0 > *);
      Energy(const Energy < 0 > &);
      virtual range_type operator () (const domain_type &) const;
      virtual range_type operator () (const domain_type &, double) const;
      
    private:
      LocalState *LS;
      Material::Energy < 0 > *We;

    private:
      Energy < 0 > & operator =(const Energy < 0 > &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy < 1 > : public Material::Energy < 1 > {
    public:

      typedef Material::ThermalFlow::Energy < 2 > tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef Set::VectorSpace::Vector range_type;

      Energy();
      virtual ~Energy();
      Material::Energy < 1 > *Clone() const;
      Energy(LocalState *,
	     Material::Energy < 1 > *,
	     Material::Uniaxial::Energy < 1 > *);
      Energy(const Energy < 1 > &);
      virtual range_type operator () (const domain_type &) const;
      virtual double operator () (const domain_type &, double) const;
      Material::Energy < 1 > * GetDW();
      
    private:
      LocalState *LS;
      Material::Energy < 1 > *DWe;
      Material::Uniaxial::Energy < 1 > *DWh;

    private:
      Energy < 1 > & operator =(const Energy < 1 > &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<2>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy < 2 > : public Material::Energy < 2 > {
    public:

      typedef Material::ThermalFlow::Energy < 3 > tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef Set::VectorSpace::Hom range_type;

      Energy();
      virtual ~Energy();
      Material::Energy < 2 > *Clone() const;
      Energy(LocalState *,
	     Material::Energy < 2 > *,
	     Material::Uniaxial::Energy < 2 > *);
      Energy(const Energy < 2 > &);
      virtual range_type operator () (const domain_type &) const;
      virtual double operator () (const domain_type &, double) const;
      
    private:
      LocalState *LS;
      Material::Energy < 2 > *DDWe;
      Material::Uniaxial::Energy < 2 > *DDWh;

    private:
      Energy < 2 > & operator =(const Energy < 2 > &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Jet<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Jet;

    //////////////////////////////////////////////////////////////////////
    // Class Jet<0>
    //////////////////////////////////////////////////////////////////////

    template<>
      class Jet < 0 > : public Material::Jet < 0 > {
    public:

      typedef Material::ThermalFlow::Jet < 1 > tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef pair<double, Set::VectorSpace::Vector> range_type;

      Jet();
      virtual ~Jet();
      Material::Jet < 0 > *Clone() const;
      Jet(LocalState *,
	  Material::Jet < 0 > *,	  
	  Material::Uniaxial::Jet < 0 > *);
      Jet(const Jet < 0 > &);
      range_type operator () (const domain_type &) const;
      pair<double, double> operator () (const domain_type &, double) const;

    private:
      LocalState *LS;
      Material::Jet < 0 > *g;
      Material::Uniaxial::Jet < 0 > *Jh;

    private:
      Jet < 0 > & operator =(const Jet < 0 > &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Jet<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Jet < 1 > : public Material::Jet < 1 > {
    public:

      typedef Material::ThermalFlow::Jet < 2 > tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef pair<Set::VectorSpace::Vector, Set::VectorSpace::Hom> range_type;

      Jet();
      virtual ~Jet();
      Material::Jet < 1 > *Clone() const;
      Jet(LocalState *,
	  Material::Jet < 1 > *,
	  Material::Uniaxial::Jet < 1 > *);
      Jet(const Jet < 1 > &);
      range_type operator () (const domain_type &) const;
      pair<double, double> operator () (const domain_type &, double) const;

    private:
      LocalState *LS;
      Material::Jet < 1 > *Dg;
      Material::Uniaxial::Jet < 1 > *DJh;

    private:
      Jet < 1 > & operator =(const Jet < 1 > &);
    };

  }

}

#endif // !defined(MATERIAL_THERMALFLOW_H__INCLUDED_
