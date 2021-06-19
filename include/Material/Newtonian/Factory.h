// Factory.h: Factory for the Newtonian class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_NEWTONIAN_FACTORY_H__INCLUDED_)
#define MATERIAL_NEWTONIAN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Newtonian.h"
#include "../Factory.h"
#include "../Incremental/Incremental.h"
#include "../../Clock/Clock.h"

namespace Material
{
namespace Newtonian
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Newtonian::Data data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0),
		NewLS(0), NewW(0), NewDW(0), NewDDW(0), NewJ(0), NewDJ(0) {}

	virtual ~Factory() 
	{
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
		if (LS != 0)  delete LS;
		if (NewW != 0)   delete NewW;
		if (NewDW != 0)  delete NewDW;
		if (NewDDW != 0) delete NewDDW;
		if (NewJ != 0)   delete NewJ;
		if (NewDJ != 0)  delete NewDJ;
		if (NewLS != 0)  delete NewLS;
	}

	Factory(Material::Newtonian::Data *NewDat_, Clock *C_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0),
		NewLS(0), NewW(0), NewDW(0), NewDDW(0), NewJ(0), NewDJ(0),
		NewDat(NewDat_), C(C_) {}

	Material::LocalState * GetLS()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		if (NewW == 0) NewW  = new Material::Newtonian::Energy<0>(NewLS); 
		if (W    == 0) W     = new Material::Incremental::Energy<0>(LS,NewW);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		if (NewDW == 0) NewDW  = new Material::Newtonian::Energy<1>(NewLS); 
		if (DW    == 0) DW     = new Material::Incremental::Energy<1>(LS,NewDW);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		if (NewDDW == 0) NewDDW  = new Material::Newtonian::Energy<2>(NewLS); 
		if (DDW    == 0) DDW     = new Material::Incremental::Energy<2>(LS,NewDDW);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		if (NewJ == 0) NewJ  = new Material::Newtonian::Jet<0>(NewLS); 
		if (J    == 0) J     = new Material::Incremental::Jet<0>(LS,NewJ);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) 
		{
			NewLS  = new Material::Newtonian::LocalState(NewDat,C);
			LS     = new Material::Incremental::LocalState(NewLS);
		}
		if (NewDJ == 0) NewDJ  = new Material::Newtonian::Jet<1>(NewLS); 
		if (DJ    == 0) DJ     = new Material::Incremental::Jet<1>(LS,NewDJ);
		return DJ;
	}

private:

	Clock *C;
	Material::Newtonian::Data *NewDat;
	Material::Incremental::LocalState *LS;
	Material::Incremental::Energy<0> *W;
	Material::Incremental::Energy<1> *DW;
	Material::Incremental::Energy<2> *DDW;
	Material::Incremental::Jet<0> *J;
	Material::Incremental::Jet<1> *DJ;
	Material::Newtonian::LocalState *NewLS;
	Material::Newtonian::Energy<0> *NewW;
	Material::Newtonian::Energy<1> *NewDW;
	Material::Newtonian::Energy<2> *NewDDW;
	Material::Newtonian::Jet<0> *NewJ;
	Material::Newtonian::Jet<1> *NewDJ;

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

	typedef Material::Newtonian::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Newtonian::Data *Dat_, Clock *C_) : 
		Dat(Dat_), C(C_) {}

	Material::Factory * Build() const
	{
		return new Material::Newtonian::Factory(Dat,C);
	}

private:

	Clock *C;
	Material::Newtonian::Data *Dat;
};

}

}

#endif // !defined(MATERIAL_NEWTONIAN_FACTORY_H__INCLUDED_
