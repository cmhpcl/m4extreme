// Contact.h: interface for the Contact class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_DIRECTIONAL_CONTACT__INCLUDED_)
#define POTENTIAL_DIRECTIONAL_CONTACT__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Potential/Directional/Directional.h"

namespace Potential
{
  namespace Directional
  {
    namespace Contact
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
	virtual ~Data();
	Data(Clock *,
	     const double &, const double &,
	     const double &, const double &);
	Data(const Data &);
	Data & operator = (const Data &);
	void Randomize();

      private:
	Clock *Chronos; //time step
	double kp; //factor
	double RMin; //smoothing distance
	double m0; // average mass
	double C; // average wave speed
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Potential::Directional::LocalState
	{
	  friend class Energy<0>;
	  friend class Energy<1>;
	  friend class Energy<2>;
	  friend class Jet<0>;
	  friend class Jet<1>;

	public: 

	  typedef Potential::Directional::Contact::Data data_type;
	  typedef Potential::Directional::Contact::Energy<0> energy_type;

	  LocalState();
	  virtual ~LocalState();
	  Potential::Directional::LocalState *Clone() const;
	  LocalState(const Data *,
		     const Set::VectorSpace::Vector *);
	  LocalState(const LocalState &);
	  LocalState & operator = (const LocalState &);
	  void operator ++ ();
	  void SetMass(double m_);
	  const double & GetMass() const;

	private:
	  // update the mass outside of the class
	  double mass;

	  // update the neighbor outside of the class
	  const Set::VectorSpace::Vector * slave_normal;

	  // constant properties
	  const Data *Prop;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<0>
      //////////////////////////////////////////////////////////////////////

      template<> 
	class Energy<0> : public Potential::Directional::Energy<0>
	{
	public: 

	  typedef Potential::Directional::Contact::Energy<1> tangent_type;
	  typedef Set::VectorSpace::Vector domain_type;
	  typedef double range_type;

	  Energy();
	  virtual ~Energy();
	  Potential::Directional::Energy<0> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<0> &);
	  const Energy<0> & operator = (const Energy<0> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template<> 
	class Energy<1> : public Potential::Directional::Energy<1>
	{
	public: 

	  typedef Potential::Directional::Contact::Energy<2> tangent_type;
	  typedef Set::VectorSpace::Vector domain_type;
	  typedef Set::VectorSpace::Vector range_type;

	  Energy();
	  virtual ~Energy();
	  Potential::Directional::Energy<1> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<1> &);
	  const Energy<1> & operator = (const Energy<1> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template<> 
	class Energy<2> : public Potential::Directional::Energy<2>
	{
	public: 

	  typedef Potential::Directional::Contact::Energy<3> tangent_type;
	  typedef double domain_type;
	  typedef double range_type;

	  Energy();
	  virtual ~Energy();
	  Potential::Directional::Energy<2> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<2> &);
	  const Energy<2> & operator = (const Energy<2> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Jet;

      //////////////////////////////////////////////////////////////////////
      // Class Jet<0>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Jet<0> : public Potential::Directional::Jet<0>
	{
	public: 

	  typedef Potential::Directional::Jet<1> tangent_type;
	  typedef Set::VectorSpace::Vector domain_type;
	  typedef pair<double, Set::VectorSpace::Vector> range_type;

	  Jet();
	  virtual ~Jet();
	  Potential::Directional::Jet<0> *Clone() const;
	  Jet(LocalState *);
	  Jet(const Jet<0> &);
	  Jet<0> & operator = (const Jet<0> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Jet<1> : public Potential::Directional::Jet<1>
	{
	public: 

	  typedef Potential::Directional::Jet<2> tangent_type;
	  typedef double domain_type;
	  typedef pair<double,double> range_type;

	  Jet();
	  virtual ~Jet();
	  Potential::Directional::Jet<1> *Clone() const;
	  Jet(LocalState *);
	  Jet(const Jet<1> &);
	  Jet<1> & operator = (const Jet<1> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	};

    }

  }

}

#endif // !defined(POTENTIAL_DIRECTIONAL_CONTACT__INCLUDED_)
