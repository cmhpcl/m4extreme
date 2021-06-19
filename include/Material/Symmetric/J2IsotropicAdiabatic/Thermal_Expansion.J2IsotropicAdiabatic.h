// J2IsotropicAdiabatic.h: interface for the J2IsotropicAdiabatic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_Thermal_Expansion_J2ISOTROPICADIABATIC_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_Thermal_Expansion_J2ISOTROPICADIABATIC_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Symmetric/Symmetric.h"
#include "Material/Shear/Shear.h"
#include "Material/Uniaxial/Uniaxial.h"
#include "Material/Gas/EoS/EoS.h"
#include "Clock/Clock.h"

namespace Material {
    namespace Symmetric {
        namespace Thermal_Expansion_J2IsotropicAdiabatic {
            class LocalState;
            template <unsigned int> class Energy;
            template <unsigned int> class Jet;

            //////////////////////////////////////////////////////////////////////
            // Class LocalState
            //////////////////////////////////////////////////////////////////////

            class LocalState : public Material::Symmetric::LocalState {
                friend class Energy < 0 >;
                friend class Energy < 1 >;
                friend class Energy < 2 >;
                friend class Jet < 0 >;
                friend class Jet < 1 >;

            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy < 0 > energy_type;
                typedef Set::VectorSpace::Sym domain_type;

                LocalState();
                virtual ~LocalState();
		LocalState(const LocalState &);

                LocalState(Clock *, double, double, double,
                        Material::Gas::EoS::LocalState *,
                        Material::Gas::EoS::Energy < 1 > *,
                        Material::Gas::EoS::Energy < 2 > *,
                        Material::Shear::Modulus < 0 > *,
                        Material::Shear::Modulus < 1 > *,
                        Material::Shear::Modulus < 2 > *,
                        Material::Uniaxial::Energy < 0 > *,
                        Material::Uniaxial::Energy < 1 > *,
                        Material::Uniaxial::Energy < 2 > *,
                        Material::Uniaxial::Energy < 1 > *,
                        Material::Uniaxial::Energy < 2 > *,
                        Material::Uniaxial::Energy < 0 > *,
                        Material::Uniaxial::Energy < 1 > *,
                        Material::Uniaxial::Energy < 2 > *,
			Material::Uniaxial::Energy < 0 > *,
                        Material::Uniaxial::Energy < 1 > *,
			Material::Uniaxial::Energy < 2 > *,
                        Material::Uniaxial::Energy < 0 > *,
                        Material::Uniaxial::Energy < 1 > *,
			Material::Uniaxial::Energy < 2 > *,
			Material::Symmetric::Energy<0> *);

                Material::Symmetric::LocalState *Clone() const;                
                bool operator () (const domain_type &);
                void operator ++();
                const double & GetEpspEff() const;
                double & GetEpspEff();
                const double & GetEpspEffOld() const;
                double & GetEpspEffOld();
                const Set::VectorSpace::Sym & GetEpsp() const;
                Set::VectorSpace::Sym & GetEpsp();
                const Set::VectorSpace::Sym & GetEpspOld() const;
                Set::VectorSpace::Sym & GetEpspOld();

                void Reset(double T_) {
                    T = T_;
                }

                double GetSigEff() const;
                double GetTemperature() const;
                double GetPressure() const;

            private:

                Clock *Chronos;

                Material::Gas::EoS::LocalState * _pEoSLS;
                Material::Gas::EoS::Energy < 1 > *Df;
                Material::Gas::EoS::Energy < 2 > *DDf;

		Material::Shear::Modulus<0> * G;
		Material::Shear::Modulus<1> * DG;
		Material::Shear::Modulus<2> * DDG;

                Material::Uniaxial::Energy < 0 > * TS;
                Material::Uniaxial::Energy < 1 > * DTS;
                Material::Uniaxial::Energy < 2 > * DDTS;

                Material::Uniaxial::Energy < 1 > * DWh;
                Material::Uniaxial::Energy < 2 > * DDWh;

                Material::Uniaxial::Energy < 0 > *Wp;
                Material::Uniaxial::Energy < 1 > *DWp;
                Material::Uniaxial::Energy < 2 > *DDWp;

                Material::Uniaxial::Energy < 0 > *Wv;
                Material::Uniaxial::Energy < 1 > *DWv;
                Material::Uniaxial::Energy < 2 > *DDWv;

		Material::Symmetric::Energy<0> * Phi;
                Material::Uniaxial::Energy<0> *Mu;
                Material::Uniaxial::Energy<1> *DMu;
                Material::Uniaxial::Energy<2> *DDMu;

                double EpspEff, EpspEffOld, SigEff;
                double alpha, T0, Tm, T, TOld;
                double Eta, EtaOld;
                Set::VectorSpace::Sym Epsp;
                Set::VectorSpace::Sym EpspOld;

            private:
                bool _SolveInternalVariable(const domain_type &, double);
		double _GetDW(double, double, const domain_type &);
                double _FalsePosition(const double, const Set::VectorSpace::Sym &,
				      const double, const double, double, double);
                LocalState & operator =(const LocalState &);                
            };

            //////////////////////////////////////////////////////////////////////
            // Class Energy<p>
            //////////////////////////////////////////////////////////////////////

            template<unsigned int p> class Energy;

            //////////////////////////////////////////////////////////////////////
            // Class Energy<0>
            //////////////////////////////////////////////////////////////////////

            template<>
            class Energy < 0 > : public Material::Symmetric::Energy < 0 > {
            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy < 1 > tangent_type;
                typedef Set::VectorSpace::Sym domain_type;
                typedef double range_type;

                Energy();
                virtual ~Energy();
                Material::Symmetric::Energy < 0 > *Clone() const;
                Energy(LocalState *,
                        Material::Gas::EoS::Energy < 0 > *,
                        Material::Uniaxial::Energy < 0 > *);
                Energy(const Energy < 0 > &);
                range_type operator () (const domain_type &) const;

            private:

                LocalState *LS;
                Material::Gas::EoS::Energy < 0 > *f;
                Material::Uniaxial::Energy < 0 > * Wh;

            private:
                Energy < 0 > & operator =(const Energy < 0 > &);
            };

            //////////////////////////////////////////////////////////////////////
            // Class Energy<1>
            //////////////////////////////////////////////////////////////////////

            template <>
            class Energy < 1 > : public Material::Symmetric::Energy < 1 > {
            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy < 2 > tangent_type;
                typedef Set::VectorSpace::Sym domain_type;
                typedef Set::VectorSpace::SymDual range_type;

                Energy();
                virtual ~Energy();
                Material::Symmetric::Energy < 1 > *Clone() const;
                Energy(LocalState *,
		       Material::Gas::EoS::Energy < 1 > *,
		       Material::Shear::Jet < 0 > *,
		       Material::Symmetric::Energy<1> *);
                Energy(const Energy < 1 > &);
                range_type operator () (const domain_type &) const;

            private:

                LocalState *LS;
                Material::Gas::EoS::Energy < 1 > *Df;
                Material::Shear::Jet < 0 > *JG;
		Material::Symmetric::Energy<1> *DPhi;

            private:

                Energy < 1 > & operator =(const Energy < 1 > &);
            };

            //////////////////////////////////////////////////////////////////////
            // Class Energy<2>
            //////////////////////////////////////////////////////////////////////

            template <>
            class Energy < 2 > : public Material::Symmetric::Energy < 2 > {
            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Energy < 3 > tangent_type;
                typedef Set::VectorSpace::Sym domain_type;
                typedef Set::VectorSpace::Hom range_type;

                Energy();
                virtual ~Energy();
                Material::Symmetric::Energy < 2 > *Clone() const;
                Energy(LocalState *,
                        Material::Gas::EoS::Energy < 2 > *,
                        Material::Uniaxial::Jet < 1 > *,
                        Material::Uniaxial::Jet < 1 > *,
                        Material::Shear::JetJet < 0 > *);
                Energy(const Energy < 2 > &);
                range_type operator () (const domain_type &) const;

            private:

                LocalState *LS;
                Material::Gas::EoS::Energy < 2 > *DDf;
                Material::Uniaxial::Jet < 1 > *DJp;
                Material::Uniaxial::Jet < 1 > *DJv;
                Material::Shear::JetJet < 0 > *JJG;

            private:

                Energy < 2 > & operator =(const Energy < 2 > &);
            };

            //////////////////////////////////////////////////////////////////////
            // Class Jet<p>
            //////////////////////////////////////////////////////////////////////

            template<unsigned int p> class Jet;

            //////////////////////////////////////////////////////////////////////
            // Class Jet<0>
            //////////////////////////////////////////////////////////////////////

            template<>
            class Jet < 0 > : public Material::Symmetric::Jet < 0 > {
            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet < 1 > tangent_type;
                typedef Set::VectorSpace::Sym domain_type;
                typedef pair<double, Set::VectorSpace::SymDual> range_type;

                Jet();
                virtual ~Jet();
                Material::Symmetric::Jet < 0 > *Clone() const;
                Jet(LocalState *,
                        Material::Gas::EoS::Jet < 0 > *,
                        Material::Shear::Jet < 0 > *,
                        Material::Uniaxial::Energy < 0 > *,
                        Material::Uniaxial::Energy < 0 > *);
                Jet(const Jet < 0 > &);
                range_type operator () (const domain_type &) const;

            private:

                LocalState *LS;
                Material::Gas::EoS::Jet < 0 > *g;
                Material::Shear::Jet < 0 > *JG;
                Material::Uniaxial::Energy < 0 > *Wp;
                Material::Uniaxial::Energy < 0 > *Wv;

            private:

                Jet < 0 > & operator =(const Jet < 0 > &);
            };

            //////////////////////////////////////////////////////////////////////
            // Class Jet<1>
            //////////////////////////////////////////////////////////////////////

            template <>
            class Jet < 1 > : public Material::Symmetric::Jet < 1 > {
            public:

                typedef Material::Symmetric::Thermal_Expansion_J2IsotropicAdiabatic::Jet < 2 > tangent_type;
                typedef Set::VectorSpace::Sym domain_type;
                typedef pair<Set::VectorSpace::SymDual,
                Set::VectorSpace::Hom> range_type;

                Jet();
                virtual ~Jet();
                Material::Symmetric::Jet < 1 > *Clone() const;
                Jet(LocalState *,
                        Material::Gas::EoS::Jet < 1 > *,
                        Material::Shear::JetJet < 0 > *,
                        Material::Uniaxial::Jet < 1 > *,
                        Material::Uniaxial::Jet < 1 > *);
                Jet(const Jet < 1 > &);
                range_type operator () (const domain_type &) const;

            private:

                LocalState *LS;
                Material::Gas::EoS::Jet < 1 > *Dg;
                Material::Shear::JetJet < 0 > *JJG;
                Material::Uniaxial::Jet < 1 > *DJp;
                Material::Uniaxial::Jet < 1 > *DJv;

            private:

                Jet < 1 > & operator =(const Jet < 1 > &);
            };

        }      
    }
}

#endif // !defined(MATERIAL_SYMMETRIC_Thermal_Expansion_J2ISOTROPICADIABATIC_H__INCLUDED_
