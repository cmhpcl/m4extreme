// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FIELD_MOVINGGAUSSIAN__INCLUDED_)
#define FLUX_FIELD_MOVINGGAUSSIAN__INCLUDED_

#pragma once

#include <utility>
#include "Clock/Clock.h"
#include "Set/Algebraic/AlgLib.h"
#include "../Field.h"
#include "./FluxCenter.h"
#include "Utils/Fields.h"

using namespace std;

namespace Flux
{
  namespace Field
  {
    namespace MovingGaussian
    {

      class Data;
      class LocalState;
      template <unsigned int> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Data
      //////////////////////////////////////////////////////////////////////
      
      class Data : public Flux::Field::Data
      {
	typedef Set::VectorSpace::Vector vector_type;
	friend class LocalState;
	friend class Energy<1>;
	friend class Energy<2>;
	
      public:
	
	Data();
	virtual ~Data();
	Data(m4extreme::Utils::ScalarField *, 
	     const double &, const double &, 
	     const double &,
	     Flux::FluxCenter *flux_center);
	Data(const Data &);
	const double & GetPower() const;
	double & GetPower();
	const double & GetRadius2() const;
	double & GetRadius2();
	double & GetOrder();
	const double& GetOrder() const;	
	Data & operator = (const Data &);
		
      private:
	m4extreme::Utils::ScalarField * absorption;
	double power;
	double radius2;
	double order;
	Flux::FluxCenter *flux_center;
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Flux::Field::LocalState
	{

	  friend class Energy<1>;
	  friend class Energy<2>;

	public: 

	  typedef Flux::Field::MovingGaussian::Data data_type;
	  typedef double domain_type;

	  virtual ~LocalState();
	  Flux::Field::LocalState *Clone() const;
	  LocalState(Data *);
	  LocalState(const LocalState &);
	  void operator ++ ();
	  double GetAbsorption() const;

	private:
	  Data *Prop;
	  double ac;

	private:

	  LocalState & operator = (const LocalState &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<1> : public Flux::Field::Energy<1>
	{
	public: 

	  typedef Flux::Field::MovingGaussian::Energy<2> tangent_type;
	  typedef Set::VectorSpace::Vector vector_type;
	  typedef double domain_type;
	  typedef double range_type;

	  virtual ~Energy();
	  Flux::Field::Energy<1> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<1> &);
	  range_type operator () (const vector_type &, const domain_type &) const;

	private:

	  LocalState *LS;

	private:

	  Energy<1> & operator = (const Energy<1> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<2> : public Flux::Field::Energy<2>
	{
	public: 

	  typedef Flux::Field::MovingGaussian::Energy<3> tangent_type;
	  typedef Set::VectorSpace::Vector vector_type;
	  typedef double domain_type;
	  typedef double range_type;

	  virtual ~Energy();
	  Flux::Field::Energy<2> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<2> &);
	  range_type operator () (const vector_type &, const domain_type &) const;

	private:

	  LocalState *LS;

	private:

	  Energy<2> & operator = (const Energy<2> &);
	}; 

      //////////////////////////////////////////////////////////////////////
      // Class Jet<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Jet;

    }
  }
}

#endif // !defined(FLUX_FIELD_MovingGaussian__INCLUDED_)
