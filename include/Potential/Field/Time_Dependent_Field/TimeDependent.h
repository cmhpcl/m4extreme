// TimeDependent.h: interface for the TimeDependent class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(POTENTIAL_FIELD_TIMEDEPENDENT_H__INCLUDED_)
#define POTENTIAL_FIELD_TIMEDEPENDENT_H__INCLUDED_

#pragma once

#include <utility>
#include "Clock/Clock.h"
#include "Potential/Field/Field.h"
#include "Set/Algebraic/AlgLib.h"

namespace Potential
{
namespace Field
{
namespace TimeDependent	
{
class TimeFunction;
class Data;
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Data
//////////////////////////////////////////////////////////////////////
 
// constant time function f(t) = B
 class TimeFunction {
 public:
 TimeFunction(int Dim_, Clock * Chronos_):
   Chronos(Chronos_), B(Dim_) {}

 TimeFunction(Clock * Chronos_, const Set::VectorSpace::Vector & B_):
   Chronos(Chronos_), B(B_) {}
   
   virtual ~TimeFunction() {};
   
   virtual void Update() { return; }

   const Set::VectorSpace::Vector & GetDensity() {
     return B;
   }
   
 protected:
   Clock *Chronos;
   Set::VectorSpace::Vector B;
 };

 // linear time function
 class LinearLoading : public TimeFunction {
 public:

   // without unloading
   // return B_k+1 = B_k + dB * (t_k+1 - t_k)   if time < tl
   // return B_k+1 = Bf                         if time > tl
   // where dB = (Bf - B0)/Dt
 LinearLoading(Clock * Chronos_,
	       double Dt_,
	       const Set::VectorSpace::Vector & B0_,
	       const Set::VectorSpace::Vector & Bf_):
   dB_load(B0_.size()), dB_unload(B0_.size()),
     tl(Chronos->Time() + Dt_), tu(1.0e16), te(1.0e16), unload(false),
     Bf(Bf_), TimeFunction(Chronos_, B0_) {
     
     if (Dt_ > 0.) {
       dB_load = (Bf - B0_) / Dt_;
     }
     else {
       B = Bf;
     }

     max_load = Norm(Bf);
   }
   
   // with unloading
   // return B_k+1 = B_k + dB0 * (t_k+1 - t_k)   if time < tl
   // return B_k+1 = Bf                          if tl <= time < tu
   // return B_k+1 = B_k + dB1 * (t_k+1 - t_k)   if tu <= time < te
   // return B_k+1 = B0                          if te <= time
   // where dB0 = (Bf - B0)/tl
   //       dB1 = (B0 - Bf)/(te-tu)
 LinearLoading(Clock * Chronos_,
	       double tl_, double tu_, double te_,
	       const Set::VectorSpace::Vector & B0_,
	       const Set::VectorSpace::Vector & Bf_):
   dB_load(B0_.size()), dB_unload(B0_.size()),
     tl(tl_), tu(tu_), te(te_), unload(true),
     B0(B0_), Bf(Bf_), TimeFunction(Chronos_, B0_) {
     
     assert(tu >= tl && te > tu);
     
     if ( tl > 0.0 ) {
       dB_load = (Bf - B0_) / tl;
     }
     else {
       B = Bf;
     }
     
     dB_unload = (B0_ - Bf) / (te - tu);

     max_load = Norm(Bf);
   }
   
   virtual ~LinearLoading() {}
   
   void Update() {
     const double & current = Chronos->Time();
     const double & dt = Chronos->DTime();

     if ( current < tl ) {
       if ( current + dt > tl ) {
	 B = Bf;
       }
       else {
	 B += dB_load * dt;
       }
     }
     else if (unload && current > tu) {
       if ( current + dt > te ) {
	 B = B0;
       }
       else {
	 B += dB_unload * dt;
       }
     }

     return;
   }
   
   void Reset( double Dt_,
	       const Set::VectorSpace::Vector & B0_,
	       const Set::VectorSpace::Vector & Bf_ ) {
     unload = false;
     B0 = B0_;
     Bf = Bf_;
     max_load = Norm(Bf);
     
     if (Dt_ > 0. ) {
       B  = B0_;
       dB_load = (Bf - B0_) / Dt_;
     }
     else {
       B = Bf;
     }

     tl = Chronos->Time() + Dt_;
     return;
   }
   
   void Reset(double tl_, double tu_, double te_, //loading/unloading period
	      const Set::VectorSpace::Vector & B0_,
	      const Set::VectorSpace::Vector & Bf_) {

     assert(tl >=0.0 && tu >= tl && te > tu);
     
     unload = true;
     B0 = B0_;
     Bf = Bf_;
     max_load = Norm(Bf);

     if ( tl > 0.0 ) {
       B = B0_;
       dB_load = (Bf - B0_) / tl;
     }
     else {
       B = Bf;
     }

     dB_unload = (B0_ - Bf) / (te - tu);

     const double & current = Chronos->Time(); 
     tl = tl_ + current;
     tu = tu_ + current;
     te = te_ + current;
   }

 private:
   bool unload;
   double tl; // time to reach maximum load Bf
   double tu; // time to start unloading
   double te; // time back to initial load B0
   double max_load; // maximum magnitude of the load
   Set::VectorSpace::Vector B0;
   Set::VectorSpace::Vector Bf;
   Set::VectorSpace::Vector dB_load;
   Set::VectorSpace::Vector dB_unload;
 };


 // normal time function
 // return B_k+1 = B0 + Bf/{1+exp[a*(tc-t_k+1)]}
 class NormalLoading : public TimeFunction {
 public:
 NormalLoading(Clock * Chronos_,
	       double alpha_,
	       double tc_,
	       const Set::VectorSpace::Vector & B0_,
	       const Set::VectorSpace::Vector & Bf_):
   alpha(alpha_), tc(tc_), B0(B0_), Bf(Bf_),
     TimeFunction(Chronos_, B0_) {}
   
   virtual ~NormalLoading() {}
   
   void Update() {
     double vloc = 1.0 + exp( alpha * (tc-Chronos->Time()) );
     B = B0 + Bf/vloc;
   }

 private:
   double alpha;
   double tc;
   Set::VectorSpace::Vector B0;
   Set::VectorSpace::Vector Bf;
 };

 //sinusoidal time function
 class SinusoidalLoading : public TimeFunction {
 public:
 SinusoidalLoading(Clock * Chronos_,
	       double w_,
	       double phi_,
	       const Set::VectorSpace::Vector & B0_):
   w(w_), phi(phi_), B0(B0_),
     TimeFunction(Chronos_, B0_) {}
   
   virtual ~SinusoidalLoading() {}
   
   void Update() {
     B = B0;
     B *= sin(w*Chronos->Time() - phi);
   }

 private:
   double w;
   double phi;
   Set::VectorSpace::Vector B0;
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
	typedef Potential::Field::TimeDependent::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Potential::Field::LocalState *Clone() const;
	LocalState(double, TimeFunction *);
	LocalState(const LocalState &);
	void operator ++ ();
	const double & GetQW() const { return QW; }
	double & GetQW() { return QW; }
	const TimeFunction *GetProp() const { return Prop; }
	TimeFunction* GetProp() { return Prop; }
private:
	double QW;
	TimeFunction *Prop;

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
class Energy<0> : public Potential::Field::Energy<0>
{
public: 

	typedef Potential::Field::TimeDependent::Energy<1> tangent_type;
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

	Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1> : public Potential::Field::Energy<1>
{
public: 

	typedef Potential::Field::TimeDependent::Energy<2> tangent_type;
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

	Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2> : public Potential::Field::Energy<2>
{
public: 

	typedef Potential::Field::TimeDependent::Energy<3> tangent_type;
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

	typedef Potential::Field::TimeDependent::Jet<1> tangent_type;
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

	Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Potential::Field::Jet<1>
{
public: 

	typedef Potential::Field::TimeDependent::Jet<2> tangent_type;
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

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(POTENTIAL_FIELD_TIMEDEPENDENT_H__INCLUDED_
