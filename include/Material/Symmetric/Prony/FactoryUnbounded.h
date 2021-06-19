// FactoryUnbounded.h: Factory for the PronyUnbounded class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_PRONYUNBOUNDED_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_PRONYUNBOUNDED_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./PronyUnbounded.h"
#include "../Factory.h"
#include "../../../Clock/Clock.h"

namespace Material
{
namespace Symmetric
{
namespace PronyUnbounded
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	typedef Material::Symmetric::PronyUnbounded::LocalState sym_ls_type;
	typedef Material::Symmetric::PronyUnbounded::Energy<0> sym_e_type;
	typedef Material::Symmetric::PronyUnbounded::Energy<1> sym_de_type;
	typedef Material::Symmetric::PronyUnbounded::Energy<2> sym_dde_type;
	typedef Material::Symmetric::PronyUnbounded::Jet<0> sym_j_type;
	typedef Material::Symmetric::PronyUnbounded::Jet<1> sym_dj_type;

	Factory() : D(0), C(0),LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (W       != 0) delete W;
		if (DW      != 0) delete DW;
		if (DDW     != 0) delete DDW;
		if (J       != 0) delete J;
		if (DJ      != 0) delete DJ;
		if (LS      != 0) delete LS;
	}

	Factory(Material::Symmetric::PronyUnbounded::Data *D_, Clock *C_) : 
		D(D_), C(C_), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0)
	{
		LS = new sym_ls_type(D,C);
	}

	Factory(Material::Symmetric::PronyUnbounded::Data *D_, Clock *C_,
		const vector<Set::VectorSpace::Sym> &EpspOld) : 
		D(D_), C(C_), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0)
	{
		LS = new sym_ls_type(D,C,EpspOld);
	}

	Material::Symmetric::LocalState * GetLS()
	{
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (W  == 0) W = new sym_e_type(LS);
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (DW  == 0) DW = new sym_de_type(LS);
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (DDW  == 0) DDW = new sym_dde_type(LS);
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (J  == 0) J = new sym_j_type(LS);
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (DJ  == 0) DJ = new sym_dj_type(LS);
		return DJ;
	}

private:

	Clock *C;
	Material::Symmetric::PronyUnbounded::Data *D;
	Material::Symmetric::PronyUnbounded::LocalState *LS;
	Material::Symmetric::PronyUnbounded::Energy<0> *W;
	Material::Symmetric::PronyUnbounded::Energy<1> *DW;
	Material::Symmetric::PronyUnbounded::Energy<2> *DDW;
	Material::Symmetric::PronyUnbounded::Jet<0> *J;
	Material::Symmetric::PronyUnbounded::Jet<1> *DJ;

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

	Builder(Material::Symmetric::PronyUnbounded::Data *D_, Clock *C_) : 
		D(D_), C(C_), EpspOld(0) {}

	Builder(Material::Symmetric::PronyUnbounded::Data *D_, Clock *C_,
		vector<Set::VectorSpace::Sym> *EpspOld_) : 
		D(D_), C(C_), EpspOld(EpspOld_) {}

	Material::Symmetric::Factory * Build() const
	{
		if (EpspOld == 0) return new Material::Symmetric::PronyUnbounded::Factory(D,C);
		else return new Material::Symmetric::PronyUnbounded::Factory(D,C,*EpspOld);
	}

private:

	Clock *C;
	Material::Symmetric::PronyUnbounded::Data *D;
	vector<Set::VectorSpace::Sym> *EpspOld;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_PRONYUNBOUNDED_FACTORY_H__INCLUDED_)
