// CompressibleNewtonian.h: interface for the Inelastic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_COMPRESSIBLE_NEWTONIAN_H__INCLUDED_)
#define MATERIAL_COMPRESSIBLE_NEWTONIAN_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Material/Material.h"


namespace Material
{
    
  namespace CompressibleNewtonian
  {

    class LocalState;
    template <unsigned int> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Data
    //////////////////////////////////////////////////////////////////////

    class Data : public Material::Data
    {
      friend class LocalState;
      friend class Energy<1>;

    public:

      Data();
      virtual ~Data();
      Data(const double &, const double &);
      Data(const Data &);
      Data & operator = (const Data &);
      const double & GetK() const;
      double & GetK();
      const double & GetEta() const;
      double & GetEta();
      void Randomize();

    private:

      double K;
      double Eta;
    };

    //////////////////////////////////////////////////////////////////////
    // Class LocalState
    //////////////////////////////////////////////////////////////////////

    class LocalState : public Material::LocalState
    {
      friend class Energy<0>;
      friend class Energy<1>;

    public: 
	
      typedef Set::VectorSpace::Vector domain_type;

      LocalState(Data *, Clock *);
      virtual ~LocalState();
      Material::LocalState *Clone() const;	
      const Set::VectorSpace::Hom & GetF() const;
      Set::VectorSpace::Hom & GetF();
      const Set::VectorSpace::Hom & GetFold() const;
      Set::VectorSpace::Hom & GetFold();
      LocalState(const LocalState &);
      void operator ++ ();

    private:

      Data *Properties;
      Clock *Chronos;	
      Set::VectorSpace::Hom F;
      Set::VectorSpace::Hom Fold;

    private:

      LocalState & operator = (const LocalState &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy<0> : public Material::Energy<0>
    {
    public: 

      typedef Material::CompressibleNewtonian::Energy<1> tangent_type;
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

      typedef Material::CompressibleNewtonian::Energy<2> tangent_type;
      typedef Set::VectorSpace::Vector domain_type;
      typedef Set::VectorSpace::Vector range_type;

      Energy(LocalState *);
      virtual ~Energy();
      Material::Energy<1> *Clone() const;
      Energy(const Energy<1> &);
      range_type operator () (const domain_type &) const;

    private:
      LocalState *LS;	

    private:

      Energy<1> & operator = (const Energy<1> &);
    };

  }

}

#endif // !defined(MATERIAL_COMPRESSIBLE_NEWTONIAN_H__INCLUDED_)
