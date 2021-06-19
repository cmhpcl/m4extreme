///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
///////////////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_FIELD_MESHEDHALFSPACE_H__INCLUDED_)
#define POTENTIAL_FIELD_MESHEDHALFSPACE_H__INCLUDED_

#pragma once

#include <utility>
#include "../HalfSpace/HalfSpace.h"
#include "Set/SetLib.h" 
#include "Geometry/GeoLib.h"

namespace Potential
{
  namespace Field
  {
    namespace MeshedHalfSpace	
    {
      class Data;
      class LocalState;
      template <unsigned int> class Energy;
      template <unsigned int> class Jet;

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
	virtual ~Data();

	Data(const map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,	     
	     const vector<Geometry::Cell*> *,
	     const vector<Set::VectorSpace::Vector> &, 
	     const double &);
	
	Data(const Data &);
	Data & operator = (const Data &);
	
	void Reset(const map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
		   const vector<Set::VectorSpace::Vector> &);
    
      private:
	void _get_vertices(Geometry::Cell* e, set<Geometry::Cell*> & vs) const;
	void _halfspace_construct(int, const map<Geometry::Cell*,
				  Set::Euclidean::Orthonormal::Point> &,
				  const vector<Set::VectorSpace::Vector> &,
				  bool);
	void _ngh_construct(int);

      private:
	const vector<Geometry::Cell*> * Surfaces;
	double Stiffness;    
	vector<Potential::Field::HalfSpace::Data*> HD;    
	map<size_t, Set::Euclidean::Orthonormal::Point*> barycenters;    
	Geometry::Search<size_t> * pNeighbors; 
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Potential::Field::LocalState
      {
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;
	friend class Jet<0>;
	friend class Jet<1>;

      public: 

	typedef Potential::Field::MeshedHalfSpace::Data data_type;
	typedef Potential::Field::MeshedHalfSpace::Energy<0> energy_type;
	typedef Potential::Field::MeshedHalfSpace::Jet<0> jet_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Potential::Field::LocalState *Clone() const;
	LocalState(Data *, double);
	LocalState(const LocalState &);
	void operator ++ ();
	void FindNeighbor(const Set::VectorSpace::Vector &, vector<size_t> &);
	
      private:
	double search_range;
	Data *Prop;

      private:    
	vector< Potential::Field::HalfSpace::LocalState* > HLS;
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
	class Energy<0> : public Potential::Field::Energy<0>
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Potential::Field::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:
	vector< Potential::Field::HalfSpace::Energy<0>* > E0;
	Energy<0> & operator = (const Energy<0> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<1> : public Potential::Field::Energy<1>
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Potential::Field::Energy<1> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:
	vector< Potential::Field::HalfSpace::Energy<1>* > DE;
	Energy<1> & operator = (const Energy<1> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<2> : public Potential::Field::Energy<2>
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Potential::Field::Energy<2> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:
	vector< Potential::Field::HalfSpace::Energy<2>* > DDE;
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
	class Jet<0> : public Potential::Field::Jet<0>
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Potential::Field::Jet<0> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:
  	vector< Potential::Field::HalfSpace::Jet<0>* > J;
	Jet<0> & operator = (const Jet<0> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Jet<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Jet<1> : public Potential::Field::Jet<1>
      {
      public: 

	typedef Potential::Field::MeshedHalfSpace::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Potential::Field::Jet<1> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:
	vector< Potential::Field::HalfSpace::Jet<1>* > DJ;
	Jet<1> & operator = (const Jet<1> &);
      };

    }

  }

}

#endif // !defined(POTENTIAL_FIELD_HALFSPACE_H__INCLUDED_
