// LockingFree.h: interface for the LockingFree class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MODEL_MATERIALPOINT_LOCKINGFREE_H__INCLUED_)
#define MODEL_MATERIALPOINT_LOCKINGFREE_H__INCLUED_

#pragma once

#include "Element/MaterialPoint/MaterialPoint.h"
#include "Element/ArtificialViscosity/ArtificialViscosity.h"
#include "MaterialPoint.h"

namespace Model {

  namespace MaterialPoint {

    namespace LockingFree {

      template <unsigned int> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <> class Energy<1> : public Model::MaterialPoint::Energy<1> {
	typedef set<Set::Manifold::Point *> domain_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> range_type;

      public:
	Energy();
	virtual ~Energy();
	Energy(const Energy<1> &rhs);

#if defined(_M4EXTREME_THREAD_POOL)
	Energy(double, Model::MaterialPoint::LocalState *LS_, 
	       vector<Element::Energy<1> *> *DE_);
#else
	Energy(double, Model::MaterialPoint::LocalState *LS_, 
	       set<Element::Energy<1> *> *DE_);
#endif

	virtual void operator()(const domain_type &, range_type &);

      protected:
	double alpha;
	vector<Element::ArtificialViscosity::LocalState *> mpt_LS;
	vector<Element::LocalState *> non_mpt_LS;
	vector<Element::ArtificialViscosity::Energy<1> *> mpt_DE;
	vector<Element::Energy<1> *> non_mpt_DE; 
      };

    }  // namespace LockingFree
  }  // namespace MaterialPoint
}  // namespace Model

#endif
