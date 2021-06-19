#if !defined(MATERIAL_GAS_EOS_ENERGYINTERPOLATION_INCLUDED_)
#define MATERIAL_GAS_EOS_ENERGYINTERPOLATION_INCLUDED_

#pragma once

#include <map>
#include <utility>

#include "Material/Gas/EoS/EoS.h"
#include "Set/Indexed/Table/Table.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace EnergyInterpolation
{
class Data;
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;
class BilinearInterpolation;

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
    Data(Set::Array *, Set::Array *, Set::Table *);
    Data(const char *,  const char *, const char *);
    Data(const Data &);
    Data & operator = (const Data &);

    const Set::Table * GetWData() const;
    Set::Table * GetWData();
    const Set::Array * GetJ() const;
    Set::Array * GetJ();
    const Set::Array * GetT() const;
    Set::Array * GetT();

private:
    Set::Table *WData;
    Set::Array *J;         // J is the volumetric change (det(F)) domain
    Set::Array *T;         // T is the temperature domain
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

        typedef Material::Gas::EoS::EnergyInterpolation::Data data_type;
        typedef Material::Gas::EoS::EnergyInterpolation::Energy<0> energy_type;

        virtual ~LocalState();
        Material::Gas::EoS::LocalState *Clone() const;
        LocalState(Data *);
        LocalState(Data *, double);
        LocalState(BilinearInterpolation *);
        LocalState(const LocalState &);
        void Reset(const double &);
        void operator ++ ();
	const Data * GetData() const;

private:

    BilinearInterpolation *f;
    Data *Prop;
    double Jac;
    bool newf;

private:

        const LocalState & operator = (const LocalState &);
};

// Class Energy<0>

template<>
class Energy<0> : public Material::Gas::EoS::Energy<0>
{
public:

        typedef Material::Gas::EoS::EnergyInterpolation::Energy<1> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<0> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<0> &);
        range_type operator () (const domain_type &) const;

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

        typedef Material::Gas::EoS::EnergyInterpolation::Energy<2> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<1> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<1> &);
        range_type operator () (const domain_type &) const;

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

        typedef Material::Gas::EoS::EnergyInterpolation::Energy<3> tangent_type;
        typedef double domain_type;
        typedef double range_type;

        virtual ~Energy();
        Material::Gas::EoS::Energy<2> *Clone() const;
        Energy(LocalState *);
        Energy(const Energy<2> &);
        range_type operator () (const domain_type &) const;
	
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

        typedef Material::Gas::EoS::EnergyInterpolation::Jet<1> tangent_type;
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

        typedef Material::Gas::EoS::EnergyInterpolation::Jet<2> tangent_type;
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

class BilinearInterpolation
{

public:
    BilinearInterpolation();
    BilinearInterpolation(const BilinearInterpolation &);
    BilinearInterpolation(Set::Array *, Set::Array *, Set::Table *);
    virtual ~BilinearInterpolation();
    BilinearInterpolation *Clone() const;

    double operator () (double, double,
			int, int);

private:
    Set::Array *x;        // Array with points where function is known
    Set::Array *y;        // Array with points where function is known
    Set::Table *f;        // Function values at points xi
    double dx;            // Constant spacing in x
    double dy;            // Constant spacing in y

    double xmin, xmax, ymin, ymax; 

private:
    unsigned int locateX(double);
    unsigned int locateY(double);
    BilinearInterpolation & operator = (const BilinearInterpolation &);


};


}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_ENERGYINTERPOLATION_INCLUDED_)
