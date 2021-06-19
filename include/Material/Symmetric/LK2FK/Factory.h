// Factory.h: Factory for the NeoHookean class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_LK2FK_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_LK2FK_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "LK2FK.h"
#include "../Factory.h"

namespace Material
{
namespace Symmetric
{
namespace LK2FK
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	typedef Material::Symmetric::LK2FK::LocalState fk_ls_type;
	typedef Material::Symmetric::LK2FK::Energy<0> fk_e_type;
	typedef Material::Symmetric::LK2FK::Energy<1> fk_de_type;
	typedef Material::Symmetric::LK2FK::Energy<2> fk_dde_type;
	typedef Material::Symmetric::LK2FK::Jet<0> fk_j_type;
	typedef Material::Symmetric::LK2FK::Jet<1> fk_dj_type;

	Factory() : LKFac(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (J       != 0) delete J;
		if (DJ      != 0) delete DJ;
		if (W       != 0) delete W;
		if (DW      != 0) delete DW;
		if (DDW     != 0) delete DDW;
		if (LS      != 0) delete LS;
	}

	Factory(Material::Symmetric::Factory *LKFac_) : LKFac(LKFac_),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		if (LS == 0) LS = new fk_ls_type(LKFac->GetLS());
	}

	Material::Symmetric::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0) W = new fk_e_type(LKFac->GetW());
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS();
		if (DW == 0) DW = new fk_de_type(LKFac->GetDW());
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (LS  == 0) NewLS();
		if (DDW == 0) DDW = new fk_dde_type(LKFac->GetDDW(), LKFac->GetDJ());
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS();
		if (J  == 0) J = new fk_j_type(LKFac->GetJ());
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ == 0) DJ   = new fk_dj_type(LKFac->GetDJ());
		return DJ;
	}

private:

	Material::Symmetric::Factory *LKFac;

	Material::Symmetric::LK2FK::LocalState *LS;
	Material::Symmetric::LK2FK::Energy<0> *W;
	Material::Symmetric::LK2FK::Energy<1> *DW;
	Material::Symmetric::LK2FK::Energy<2> *DDW;
	Material::Symmetric::LK2FK::Jet<0> *J;
	Material::Symmetric::LK2FK::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Symmetric::Builder
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Symmetric::Builder *LKBld_) : LKBld(LKBld_) {}

	Material::Symmetric::Factory * Build() const
	{
	  return new Material::Symmetric::LK2FK::Factory(LKBld->Build());
	}

private:

	Material::Symmetric::Builder *LKBld;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_LK2FK_FACTORY_H__INCLUDED_
