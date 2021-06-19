// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_MEMBRANE_FINITEKINEMATICS_H__INCLUDED_)
#define MATERIAL_MEMBRANE_FINITEKINEMATICS_H__INCLUDED_

#pragma once

#include "../Membrane.h"

using namespace std;

namespace Material
{
namespace Membrane
{
namespace FiniteKinematics
{
class LocalState;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Membrane::LocalState
{
public: 

	typedef Material::Membrane::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	LocalState( 
		const unsigned int &,
		Material::LocalState *, 
		Material::Energy<1> *, 
		Material::Energy<2> *);
	LocalState( 
		const Set::VectorSpace::Vector &,
		Material::LocalState *, 
		Material::Energy<1> *, 
		Material::Energy<2> *);
	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(const LocalState &);
	range_type operator () (const domain_type &) const;

private:

	LocalState & operator = (const LocalState &);
};

}

}

}

#endif // !defined(MATERIAL_MEMBRANE_FINITEKINEMATICS_H__INCLUDED_)
