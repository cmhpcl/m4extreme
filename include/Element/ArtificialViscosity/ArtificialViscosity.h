// ArtificialViscosity.h: interface for the Inelastic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(ELEMENT_ARTIFICIALVISCOSITY_NEWTONIAN_H__INCLUDED_)
#define ELEMENT_ARTIFICIALVISCOSITY_NEWTONIAN_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Element/MaterialPoint/MaterialPoint.h"

namespace Element {

  namespace ArtificialViscosity {

    class LocalState;
    template <unsigned int> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Data
    //////////////////////////////////////////////////////////////////////

    class Data {
    public:

      Data();
      virtual ~Data();
      Data(const double &, const double &, 
	   const double &, const double &);
      Data(const double &, const double &, 
	   const double &, const double &,
	   const double &, const double &);
      Data(const Data &);
      Data & operator =(const Data &);
      const double & GetEta() const;
      double & GetEta();
      const double & GetCI() const;
      double & GetCI();
      const double & GetCS() const;
      double & GetCS();
      const double & GetBI() const;
      double & GetBI();
      const double & GetBS() const;
      double & GetBS();
      const double & GetRho0() const;
      double & GetRho0();
      bool EnableBulkViscosity() const;
      void Randomize();
      

    private:

      double _ci, _cs, _eta, _rho0;
      double _bi, _bs;
      bool _enableBulkViscosity;
    };

    //////////////////////////////////////////////////////////////////////
    // Class LocalState
    //////////////////////////////////////////////////////////////////////

    class LocalState : public Element::LocalState {
      friend class Energy < 0 >;
      friend class Energy < 1 >;
      friend class Energy < 2 >;

    public:
      typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
      typedef vector<Set::VectorSpace::Vector> range_type;

      LocalState(const LocalState &);
      LocalState(Element::MaterialPoint::LocalState *, Data *, Clock *);
      LocalState(Element::MaterialPoint::LocalState *, Data *, Clock *, int);
      virtual ~LocalState();
      Element::LocalState *Clone() const;

      void SetType(int);

      range_type operator () (const domain_type &) const;
      void operator ++();
      set<Set::Manifold::Point *> GetNodes() const;
      const vector<Material::LocalState *> & GetMLS() const;
      Element::MaterialPoint::LocalState * GetELS() const;

      bool isActivated() const;
      bool & isActivated();

      void operator () (const domain_type &, range_type&) const;
      virtual void GetNodes(set<Set::Manifold::Point *> &) const;

      set<pair<Set::Manifold::Point *, Set::Manifold::Point *> > GetNodePairs() const;
      virtual void Reset(const domain_type &);
      virtual void Reset(const map<Set::Manifold::Point *, double> &);

      const Set::VectorSpace::Hom & GetSigmaV() const;
      const Set::VectorSpace::Hom & GetDeformation() const;

    private:            
      int _type;

      vector<double> rhoOld, rho;
      Element::MaterialPoint::LocalState * ELS;
      Data *Properties;
      Clock *Chronos;
      Set::VectorSpace::Hom _sigmav;
      Set::VectorSpace::Hom _D;

    private:
      LocalState & operator =(const LocalState &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy < 0 > : public Element::Energy < 0 > {
    public:

      typedef Element::ArtificialViscosity::Energy < 1 > tangent_type;
      typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
      typedef double range_type;

      Energy();
      virtual ~Energy();
      Element::Energy < 0 > *Clone() const;
      Element::LocalState *GetLocalState() const;
      Element::Energy < 0 > *GetEE() const;
      Energy(LocalState *, Element::Energy < 0 > *);
      Energy(const Energy < 0 > &);            
      virtual range_type operator () (const domain_type &) const;

    private:
      LocalState *LS;
      Element::Energy < 0 > * E;

    private:
      Energy < 0 > & operator =(const Energy < 0 > &);
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template <>
      class Energy < 1 > : public Element::Energy < 1 > {
    public:

      typedef Element::ArtificialViscosity::Energy < 2 > tangent_type;
      typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
      typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> range_type;

      typedef map<Set::Manifold::Point *, double> scalar_type;
      typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
      typedef map<Set::Manifold::Point *, double> scalarset_type;

      Energy();
      virtual ~Energy();
      Element::Energy < 1 > *Clone() const;
      Element::LocalState *GetLocalState() const;
      Element::Energy < 1 > *GetEDE() const;
      Energy(LocalState *, Element::Energy < 1 > *);
      Energy(const Energy < 1 > &);
      virtual range_type operator () (const domain_type &) const;

      virtual void operator () (const domain_type &, range_type &) const;
      virtual void operator () (const vector_type &, const scalar_type &, scalarset_type &) const;
      virtual void operator () (const domain_type &, 
				const vector<Set::VectorSpace::Vector> &,
				range_type &) const;

    private:
      void _addAV_full(const map<Set::Manifold::Point *, Set::VectorSpace::Vector> &,
		       map<Set::Manifold::Point *, Set::VectorSpace::Vector> &) const;
      void _addAV_volumetric(const map<Set::Manifold::Point *, Set::VectorSpace::Vector> &,
			     map<Set::Manifold::Point *, Set::VectorSpace::Vector> &) const;

    private:
      LocalState *LS;
      Element::Energy < 1 > * DE;

    private:
      Energy < 1 > & operator =(const Energy < 1 > &);
    };
  }

}

#endif // !defined(ELEMENT_ARTIFICIALVISCOSITY_NEWTONIAN_H__INCLUDED_)
