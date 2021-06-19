// Taylor.h: interface for the Taylor class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_TAYLOR_FACTORY_H__INCLUDED_)
#define MATERIAL_TAYLOR_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Taylor.h"
#include "../Factory.h"

namespace Material
{
namespace Taylor
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (DJ     != 0) delete DJ;
		if (J      != 0) delete J;
		if (DDW    != 0) delete DDW;
		if (DW     != 0) delete DW;
		if (W      != 0) delete W;
		if (LS     != 0) delete LS;

		vector<Material::Factory *>::iterator pFloc;
		for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++) delete *pFloc;
	}

	Factory(const vector<Material::Builder *> &Bloc) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) 
	{
		vector<Material::Builder *>::const_iterator pBloc;
		for (pBloc=Bloc.begin(); pBloc!=Bloc.end(); pBloc++)
			Floc.push_back((*pBloc)->Build());
	}

	void NewLS()
	{
		vector<Material::LocalState *> LSloc;
		vector<Material::Factory *>::iterator pFloc;
		for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
			LSloc.push_back((*pFloc)->GetLS());
		LS = new Material::Taylor::LocalState(LSloc);
	}

	Material::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0)
		{
			vector<Material::Energy<0> *> Wloc;
			vector<Material::Factory *>::iterator pFloc;
			for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
				Wloc.push_back((*pFloc)->GetW());
			W = new Material::Taylor::Energy<0>(LS, Wloc);
		}
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS();
		if (DW == 0)
		{
			vector<Material::Energy<1> *> DWloc;
			vector<Material::Factory *>::iterator pFloc;
			for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
				DWloc.push_back((*pFloc)->GetDW());
			DW = new Material::Taylor::Energy<1>(LS, DWloc);
		}
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS  == 0) NewLS();
		if (DDW == 0)
		{
			vector<Material::Energy<2> *> DDWloc;
			vector<Material::Factory *>::iterator pFloc;
			for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
				DDWloc.push_back((*pFloc)->GetDDW());
			DDW = new Material::Taylor::Energy<2>(LS, DDWloc);
		}
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS();
		if (J  == 0)
		{
			vector<Material::Jet<0> *> Jloc;
			vector<Material::Factory *>::iterator pFloc;
			for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
				Jloc.push_back((*pFloc)->GetJ());
			J = new Material::Taylor::Jet<0>(LS, Jloc);
		}
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ == 0)
		{
			vector<Material::Jet<1> *> DJloc;
			vector<Material::Factory *>::iterator pFloc;
			for (pFloc=Floc.begin(); pFloc!=Floc.end(); pFloc++)
				DJloc.push_back((*pFloc)->GetDJ());
			DJ = new Material::Taylor::Jet<1>(LS, DJloc);
		}
		return DJ;
	}

private:

	vector<Material::Factory *> Floc;
	Material::Taylor::LocalState *LS;
	Material::Taylor::Energy<0> *W;
	Material::Taylor::Energy<1> *DW;
	Material::Taylor::Energy<2> *DDW;
	Material::Taylor::Jet<0> *J;
	Material::Taylor::Jet<1> *DJ;

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

	Builder(const vector<Material::Builder *> &Bloc_) : Bloc(Bloc_) {}

	Material::Factory * Build() const
	{
		vector<Material::Factory *> Floc;
		vector<Material::Builder *>::const_iterator pBloc;
		for (pBloc=Bloc.begin(); pBloc!=Bloc.end(); pBloc++)
			Floc.push_back((*pBloc)->Build());
		return new Material::Taylor::Factory(Bloc);
	}

private:

	vector<Material::Builder *> Bloc;
};

}

}

#endif // !defined(MATERIAL_TAYLOR_FACTORY_H__INCLUDED_
