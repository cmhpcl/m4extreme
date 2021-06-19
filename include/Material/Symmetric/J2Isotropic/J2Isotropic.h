// J2Isotropic.h: interface for the J2Isotropic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_J2ISOTROPIC_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_J2ISOTROPIC_H__INCLUDED_

#pragma once

#include <utility>
#include "../Symmetric.h"
#include "../../Shear/Shear.h"
#include "../../Uniaxial/Uniaxial.h"
#include "../../Gas/EoS/EoS.h"
#include "../../../Clock/Clock.h"

namespace Material
{
namespace Symmetric
{
namespace J2Isotropic
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::Symmetric::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public: 

	typedef Material::Symmetric::J2Isotropic::Energy<0> energy_type;
	typedef Set::VectorSpace::Sym domain_type;

	LocalState();
	virtual ~LocalState();
	LocalState(Clock *,
	Material::Shear::Modulus<0> *,
	Material::Uniaxial::Energy<1> *,
	Material::Uniaxial::Energy<2> *,
	Material::Uniaxial::Jet<1> *,
	Material::Uniaxial::Energy<1> *,
	Material::Uniaxial::Energy<2> *,
	Material::Uniaxial::Jet<1> *);
	Material::Symmetric::LocalState *Clone() const;
	LocalState(const LocalState &);
	bool operator () (const domain_type &);
	void operator ++ ();
	const double & GetEpspEff() const;
	double & GetEpspEff();
	const double & GetEpspEffOld() const;
	double & GetEpspEffOld();
	const Set::VectorSpace::Sym & GetEpsp() const;
	Set::VectorSpace::Sym & GetEpsp();
	const Set::VectorSpace::Sym & GetEpspOld() const;
	Set::VectorSpace::Sym & GetEpspOld();

private:

	Clock *Chronos;
	Material::Shear::Modulus<0> *Mu;
	Material::Uniaxial::Energy<1> *DWp;
	Material::Uniaxial::Energy<2> *DDWp;
	Material::Uniaxial::Jet<1> *DJp;
	Material::Uniaxial::Energy<1> *DWv;
	Material::Uniaxial::Energy<2> *DDWv;
	Material::Uniaxial::Jet<1> *DJv;
	double EpspEff, EpspEffOld;
	Set::VectorSpace::Sym Epsp;
	Set::VectorSpace::Sym EpspOld;

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
class Energy<0> : public Material::Symmetric::Energy<0>
{
public: 

	typedef Material::Symmetric::J2Isotropic::Energy<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<0> *Clone() const;
	Energy(LocalState *,
		Material::Gas::EoS::Energy<0> *,
		Material::Uniaxial::Energy<0> *,
		Material::Uniaxial::Energy<0> *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Energy<0> *f;
	Material::Uniaxial::Energy<0> *Wp;
	Material::Uniaxial::Energy<0> *Wv;

private:

	Energy<0> & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1> : public Material::Symmetric::Energy<1>
{
public: 

	typedef Material::Symmetric::J2Isotropic::Energy<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::SymDual range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<1> *Clone() const;
	Energy(LocalState *,
		Material::Gas::EoS::Energy<1> *,
		Material::Shear::Jet<0> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Energy<1> *Df;
	Material::Shear::Jet<0> *JMu;

private:

	Energy<1> & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2> : public Material::Symmetric::Energy<2>
{
public: 

	typedef Material::Symmetric::J2Isotropic::Energy<3> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<2> *Clone() const;
	Energy(LocalState *, 
		Material::Gas::EoS::Energy<2> *,
		Material::Uniaxial::Jet<1> *,
		Material::Uniaxial::Jet<1> *,
		Material::Shear::JetJet<0> *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Energy<2> *DDf;
	Material::Uniaxial::Jet<1> *DJp;
	Material::Uniaxial::Jet<1> *DJv;
	Material::Shear::JetJet<0> *JJMu;

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
class Jet<0> : public Material::Symmetric::Jet<0>
{
public: 

	typedef Material::Symmetric::J2Isotropic::Jet<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<double, Set::VectorSpace::SymDual> range_type;

	Jet();
	virtual ~Jet();
	Material::Symmetric::Jet<0> *Clone() const;
	Jet(LocalState *, 
		Material::Gas::EoS::Jet<0> *, 
		Material::Shear::Jet<0> *,
		Material::Uniaxial::Energy<0> *,
		Material::Uniaxial::Energy<0> *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Jet<0> *g;
	Material::Shear::Jet<0> *JMu;
	Material::Uniaxial::Energy<0> *Wp;
	Material::Uniaxial::Energy<0> *Wv;

private:

	Jet<0> & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1> : public Material::Symmetric::Jet<1>
{
public: 

	typedef Material::Symmetric::J2Isotropic::Jet<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<Set::VectorSpace::SymDual, 
		Set::VectorSpace::Hom> range_type;

	Jet();
	virtual ~Jet();
	Material::Symmetric::Jet<1> *Clone() const;
	Jet(LocalState *,
		Material::Gas::EoS::Jet<1> *,
		Material::Shear::JetJet<0> *,
		Material::Uniaxial::Jet<1> *,
		Material::Uniaxial::Jet<1> *);
		Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Jet<1> *Dg;
	Material::Shear::JetJet<0> *JJMu;
	Material::Uniaxial::Jet<1> *DJp;
	Material::Uniaxial::Jet<1> *DJv;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_J2ISOTROPIC_H__INCLUDED_
