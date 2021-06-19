// Factory.h: Factory for the NeoHookean class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./LinearizedKinematics.h"
#include "../Factory.h"
#include "../../Factory.h"

namespace Material
{
namespace Symmetric
{
namespace LinearizedKinematics
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Symmetric::LinearizedKinematics::LocalState fk_ls_type;
	typedef Material::Symmetric::LinearizedKinematics::Energy<0> fk_e_type;
	typedef Material::Symmetric::LinearizedKinematics::Energy<1> fk_de_type;
	typedef Material::Symmetric::LinearizedKinematics::Energy<2> fk_dde_type;
	typedef Material::Symmetric::LinearizedKinematics::Jet<0> fk_j_type;
	typedef Material::Symmetric::LinearizedKinematics::Jet<1> fk_dj_type;

	Factory() : Fact0(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (J   != 0) delete J;
		if (DJ  != 0) delete DJ;
		if (W   != 0) delete W;
		if (DW  != 0) delete DW;
		if (DDW != 0) delete DDW;
		if (LS  != 0) delete LS;
	}

	Factory(Material::Symmetric::Factory *Fact0_) : Fact0(Fact0_),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		if (LS == 0) LS = new fk_ls_type(Fact0->GetLS());
	}

	Material::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0) W = new fk_e_type(Fact0->GetW());
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS();
		if (DW == 0) DW = new fk_de_type(Fact0->GetDW());
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS  == 0) NewLS();
		if (DDW == 0) DDW = new fk_dde_type(Fact0->GetDDW());
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS();
		if (J  == 0) J = new fk_j_type(Fact0->GetJ());
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ == 0) DJ = new fk_dj_type(Fact0->GetDJ());
		return DJ;
	}

private:

	Material::Symmetric::Factory *Fact0;
	Material::Symmetric::LinearizedKinematics::LocalState *LS;
	Material::Symmetric::LinearizedKinematics::Energy<0> *W;
	Material::Symmetric::LinearizedKinematics::Energy<1> *DW;
	Material::Symmetric::LinearizedKinematics::Energy<2> *DDW;
	Material::Symmetric::LinearizedKinematics::Jet<0> *J;
	Material::Symmetric::LinearizedKinematics::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Symmetric::Builder *Bld0_) : Bld0(Bld0_) {}

	Material::Factory * Build() const
	{
	  return new Material::Symmetric::LinearizedKinematics::Factory(Bld0->Build());
	}

private:

	Material::Symmetric::Builder *Bld0;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_FACTORY_H__INCLUDED_
