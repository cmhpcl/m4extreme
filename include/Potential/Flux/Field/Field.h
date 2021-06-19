// Field.h: interface for the Field class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FIELD__INCLUDED_)
#define FLUX__FIELD__INCLUDED_

#pragma once

#include <utility>
#include "Set/Algebraic/AlgLib.h"

using namespace std;

namespace Flux
{
  namespace Field
  {
    //////////////////////////////////////////////////////////////////////
    // Class LocalState
    //////////////////////////////////////////////////////////////////////
    class Data {
    public:
      Data() {}
      virtual ~Data() {}
    };

    class LocalState
    {
    public: 

      LocalState() {}
      virtual ~LocalState() {}
      virtual LocalState *Clone() const=0;
      virtual void operator ++ ()=0;
      virtual void Reset(double) {return;}
    };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int> class Energy;

    //////////////////////////////////////////////////////////////////////
    // Class Energy<1>
    //////////////////////////////////////////////////////////////////////

    template<> class Energy<1>
      {
      public: 

	typedef Flux::Field::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector vector_type;
	typedef double domain_type;
	typedef double range_type;

	Energy() {}
	virtual ~Energy() {}
	virtual Energy<1> *Clone() const=0;
	virtual range_type operator () (const vector_type &, const domain_type &) const=0;
      };

    //////////////////////////////////////////////////////////////////////
    // Class Energy<2>
    //////////////////////////////////////////////////////////////////////

    template<> class Energy<2>
      {
      public: 

	typedef Flux::Field::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector vector_type;
	typedef double domain_type;
	typedef double range_type;

	Energy() {}
	virtual ~Energy() {}
	virtual Energy<2> *Clone() const=0;
	virtual range_type operator () (const vector_type &, const domain_type &) const=0;
      };

    //////////////////////////////////////////////////////////////////////
    // Class Jet<p>
    //////////////////////////////////////////////////////////////////////

    template<unsigned int p> class Jet;

  }

}

#endif // !defined(FLUX_FIELD__INCLUDED_)
