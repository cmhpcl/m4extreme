// Factory.h: Factory for the CompressibleNewtonian class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_COMPRESSIBLENEWTONIAN_FACTORY_H__INCLUDED_)
#define MATERIAL_COMPRESSIBLENEWTONIAN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "CompressibleNewtonian.h"
#include "../Factory.h"

namespace Material
{
namespace CompressibleNewtonian
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::CompressibleNewtonian::Data data_type;

	Factory() : LS(0), W(0), DW(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;		
	}

	Factory(Material::CompressibleNewtonian::Data *Dat_, Clock *Ca_) :
	  Ca(Ca_), LS(0), W(0), DW(0), Dat(Dat_) {}

	Factory(const Factory &rhs) : 
	  LS(0), W(0), DW(0), Dat(rhs.Dat), Ca(rhs.Ca) {}

	Material::Factory *Clone() const{return new Factory(*this);}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::CompressibleNewtonian::LocalState(Dat, Ca);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::CompressibleNewtonian::LocalState(Dat, Ca);
		if (W == 0) W = new Material::CompressibleNewtonian::Energy<0>(LS);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::CompressibleNewtonian::LocalState(Dat, Ca);
		if (DW == 0) DW = new Material::CompressibleNewtonian::Energy<1>(LS);
		return DW;
	}

        Material::Energy<2> * GetDDW() {
            return 0;
        }

        Material::Jet<0> * GetJ() {
            return 0;
        }

        Material::Jet<1> * GetDJ() {
            return 0;
        }
        
private:
        Clock *Ca;

	Material::CompressibleNewtonian::Data *Dat;
	Material::CompressibleNewtonian::LocalState *LS;
	Material::CompressibleNewtonian::Energy<0> *W;
	Material::CompressibleNewtonian::Energy<1> *DW;

private:

	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::CompressibleNewtonian::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::CompressibleNewtonian::Data *Dat_,
                Clock *C_) : Dat(Dat_), C(C_) {}

	Material::Factory * Build() const
	{
		return new Material::CompressibleNewtonian::Factory(Dat, C);
	}

private:

	Material::CompressibleNewtonian::Data *Dat;
        Clock *C;
};

}

}

#endif // !defined(MATERIAL_COMPRESSIBLENEWTONIAN_FACTORY_H__INCLUDED_
