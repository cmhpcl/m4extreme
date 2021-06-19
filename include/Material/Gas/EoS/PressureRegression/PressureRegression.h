// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_PRESSUREREGRESSION_INCLUDED_)
#define MATERIAL_GAS_EOS_PRESSUREREGRESSION_INCLUDED_

#pragma once

#include <map>
#include <utility>

#include "Material/Gas/EoS/EoS.h"
#include "Set/Indexed/Table/Table.h"
#include "Utils/Regression/Polynomial1d/Polynomial1d.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace PressureRegression
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;
class LinearPolynomialExt;

// Class Data

class Data
{
friend class LocalState;
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public:

    Data();
    virtual ~Data();
    Data(Set::Array *, Set::Array *, Set::Table *, unsigned int, double, double);
    Data(const char *,  const char *, const char *, unsigned int, double, double);
    Data(const Data &);
    Data & operator = (const Data &);

    const Set::Table * GetPData() const;
    Set::Table * GetPData();
    const Set::Array * GetJ() const;
    Set::Array * GetJ();
    const Set::Array * GetT() const;
    Set::Array * GetT();
    const double GetT0() const;
    const double GetAlphaV() const;

private:
    Set::Table *PData;
    Set::Array *J;         // J is the volumetric change (det(F)) domain
    Set::Array *T;         // T is the temperature domain
    double alphaV;         // Volumetric coefficient of thermal expansion
    double T0;             // Reference temperature at which (J,P)=(1,0)
    unsigned int PolyDegree;
    bool fromfile;
};

// Class LocalState

class LocalState : public Material::Gas::EoS::LocalState
{
friend class Energy<0>;
friend class Energy<1>;
friend class Energy<2>;
friend class Jet<0>;
friend class Jet<1>;

public:

        typedef Material::Gas::EoS::PressureRegression::Data data_type;
        typedef Material::Gas::EoS::PressureRegression::Energy<0> energy_type;

        virtual ~LocalState();
        Material::Gas::EoS::LocalState *Clone() const;
        LocalState(Data *);
        LocalState(Data *, double);
        LocalState(LinearPolynomialExt *);
        LocalState(const LocalState &);
        void Reset(const double &);
        void operator ++ ();
	double GetTemperature() const;
	const Data * GetData() const;

private:

    LinearPolynomialExt *f;
    Data *Prop;
    double T;
    bool newf;

private:

        const LocalState & operator = (const LocalState &);
};

// Class Energy<0>

template<>
class Energy<0> : public Material::Gas::EoS::Energy<0>
{
public:

        typedef Material::Gas::EoS::PressureRegression::Energy<1> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<0> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<0> &);
        range_type operator () (const domain_type &) const;
	void print(Set::Array &, Set::Array &, ostream *);

private:

        LocalState *LS;

private:

        const Energy<0> & operator = (const Energy<0> &);
};

// Class Energy<1>

template<>
class Energy<1> : public Material::Gas::EoS::Energy<1>
{
public:

        typedef Material::Gas::EoS::PressureRegression::Energy<2> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<1> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<1> &);
        range_type operator () (const domain_type &) const;
	void print(Set::Array &, Set::Array &, ostream *);

private:

        LocalState *LS;

private:

        const Energy<1> & operator = (const Energy<1> &);
};

// Class Energy<2>

template<>
class Energy<2> : public Material::Gas::EoS::Energy<2>
{
public:

        typedef Material::Gas::EoS::PressureRegression::Energy<3> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<2> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<2> &);
        range_type operator () (const domain_type &) const;
	void print(Set::Array &, Set::Array &, ostream *);

private:

         LocalState *LS;

private:

         const Energy<2> & operator = (const Energy<2> &);
};

// Class Jet<p>

template<unsigned int p> class Jet;

// Class Jet<0>

template<>
class Jet<0> : public Material::Gas::EoS::Jet<0>
{
public:

        typedef Material::Gas::EoS::PressureRegression::Jet<1> tangent_type;
        typedef double domain_type;
        typedef pair<double,double> range_type;

        virtual ~Jet();
        Material::Gas::EoS::Jet<0> *Clone() const;
        Jet(LocalState *);
        Jet(const Jet<0> &);
        range_type operator () (const domain_type &) const;

private:

        LocalState *LS;

private:

        const Jet<0> & operator = (const Jet<0> &);
};

// Class Jet<1>
template <>
class Jet<1> : public Material::Gas::EoS::Jet<1>
{
public:

  typedef Material::Gas::EoS::PressureRegression::Jet<2> tangent_type;
        typedef double domain_type;
        typedef pair<double,double> range_type;

        virtual ~Jet();
        Material::Gas::EoS::Jet<1> *Clone() const;
        Jet(LocalState *);
        Jet(const Jet<1> &);
        range_type operator () (const domain_type &) const;

private:

        LocalState *LS;

private:

        const Jet<1> & operator = (const Jet<1> &);
};

class LinearPolynomialExt
{

public:
    LinearPolynomialExt();
    LinearPolynomialExt(const LinearPolynomialExt &);
    LinearPolynomialExt(Set::Array *, Set::Array *, Set::Table *, unsigned int);
    virtual ~LinearPolynomialExt();
    LinearPolynomialExt *Clone() const;

    double operator () (double, double,
			unsigned int, int, 
			double, double);

private:
    Set::Array *x;        // Array with points where function is known
    Set::Array *y;        // Array with points where function is known
    Set::Table *f;        // Function values at points xi
    Set::Array *dx;       // Array with distances between points in x

    double xmin, xmax; 

    map<unsigned int, Regression1d::Polynomial *> RegressionMap;
    unsigned int PolyDegree;
    bool completebase;

private:
    unsigned int locate(double);
    Regression1d::Polynomial * ProvideRegression(unsigned int);
    LinearPolynomialExt & operator = (const LinearPolynomialExt &);


};


}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_PRESSUREREGRESSION_INCLUDED_)
