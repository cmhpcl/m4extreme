// Factory.h: Factory for the NeoHookean class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_HOOKEAN_CUBICDEV_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_HOOKEAN_CUBICDEV_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "CubicDev.h"
#include "../../Factory.h"

namespace Material
{
namespace Symmetric
{
namespace Hookean
{
namespace CubicDev
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	typedef Material::Symmetric::Hookean::CubicDev::Data lk_dat_type;
	typedef Material::Symmetric::Hookean::CubicDev::LocalState lk_ls_type;
	typedef Material::Symmetric::Hookean::CubicDev::Energy<0> lk_e_type;
	typedef Material::Symmetric::Hookean::CubicDev::Energy<1> lk_de_type;
	typedef Material::Symmetric::Hookean::CubicDev::Energy<2> lk_dde_type;
	typedef Material::Symmetric::Hookean::CubicDev::Jet<0> lk_j_type;
	typedef Material::Symmetric::Hookean::CubicDev::Jet<1> lk_dj_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (W       != 0) delete W;
		if (DW      != 0) delete DW;
		if (DDW     != 0) delete DDW;
		if (J       != 0) delete J;
		if (DJ      != 0) delete DJ;
		if (LS      != 0) delete LS;
	}

	Factory(lk_dat_type *Dat_) : Dat(Dat_),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		if (LS == 0) LS  = new lk_ls_type(Dat);
	}

	Material::Symmetric::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0) W   = new lk_e_type(LS);
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (LS    == 0) NewLS();
		if (DW  == 0) DW   = new lk_de_type(LS);
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (LS    == 0) NewLS();
		if (DDW  == 0) DDW   = new lk_dde_type(LS);
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (LS   == 0) NewLS();
		if (J  == 0) J   = new lk_j_type(LS);
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ  == 0) DJ   = new lk_dj_type(LS);
		return DJ;
	}

private:

	Material::Symmetric::Hookean::CubicDev::Data *Dat;
	Material::Symmetric::Hookean::CubicDev::LocalState *LS;
	Material::Symmetric::Hookean::CubicDev::Energy<0> *W;
	Material::Symmetric::Hookean::CubicDev::Energy<1> *DW;
	Material::Symmetric::Hookean::CubicDev::Energy<2> *DDW;
	Material::Symmetric::Hookean::CubicDev::Jet<0> *J;
	Material::Symmetric::Hookean::CubicDev::Jet<1> *DJ;

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

	Builder(Material::Symmetric::Hookean::CubicDev::Data *D_) : D(D_) {}

	Material::Symmetric::Factory * Build() const
	{
		return new Material::Symmetric::Hookean::CubicDev::Factory(D);
	}

private:

	Material::Symmetric::Hookean::CubicDev::Data *D;

};

}

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_HOOKEAN_CUBICDEV_FACTORY_H__INCLUDED_
