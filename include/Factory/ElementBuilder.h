// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_ELEMENTBUILDER_H__INCLUDED_)
#define M4EXTREME_ELEMENTBUILDER_H__INCLUDED_

#include "Set/SetLib.h"
#include "Material/MatLib.h"
#include "Element/ElmLib.h"
#include "Geometry/GeoLib.h"

namespace m4extreme {

    /**
     * Global functions
     */
    void getVertices(Geometry::Cell* e, set<Geometry::Cell*> & vs);
    void getVolumeCells(Geometry::Cell* e, set<Geometry::Cell*> & s);
    void getCoCells(unsigned int d, Geometry::Cell* e, int count, set<Geometry::Cell*> & s);    
    void getSecondRing(Geometry::Cell * e, std::set<Geometry::Cell*> & Nhs);
    void getThirdRing(Geometry::Cell * e, std::set<Geometry::Cell*> & Nhs);

    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Base class for element builders
     * ElementBuilder: abstract class
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class ElementBuilder {
    public:
        ElementBuilder(unsigned int dim);
        virtual ~ElementBuilder();

        virtual void createElement(const vector<Material::LocalState*> &,
                const vector<Material::Energy < 0 > *> &,
                const vector<Material::Energy < 1 > *> &,
                const vector<Material::Energy < 2 > *> &,
                Geometry::Cell *,
                std::map<Geometry::Cell*, Set::Manifold::Point*> &,
                std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
                const double * = NULL) = 0;

	double getElementSize() const {return _size;}
	double getMass() const {return _mass;}

        unsigned int GetDimension() const;
        Element::MaterialPoint::Data* GetData() const;
        Element::LumpedMass* GetMass() const;
        Element::LocalState* GetLS() const;
        Element::Energy < 0 > * GetE() const;
        Element::Energy < 1 > * GetDE() const;
        Element::Energy < 2 > * GetDDE() const;
	void SetContactType(Element::MaterialPoint::CONTACT_TYPE);
	void SetActiveStatus(bool);
	bool GetActiveStatus() const;

    protected:
        unsigned int _dim;
	double _factorial;
	double _size;
	double _mass;
	bool _isActivated;
	
        Element::MaterialPoint::Data * _pEDataloc;
	Element::MaterialPoint::CONTACT_TYPE _contact_type;

        Element::LocalState* _pELSloc;
        Element::LumpedMass* _pELMloc;
        Element::Energy < 0 > * _pEEloc;
        Element::Energy < 1 > * _pEDEloc;
        Element::Energy < 2 > * _pEDDEloc;
    };


    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Finite Element Builder
     * build regular stress elements for FEA
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class FiniteElementBuilder : public ElementBuilder {
    public:

        FiniteElementBuilder(unsigned int dim,
                const Element::Quadrature::Rule &);

        virtual ~FiniteElementBuilder() {
        };

        virtual void createElement(const vector<Material::LocalState*> &,
                const vector<Material::Energy < 0 > *> &,
                const vector<Material::Energy < 1 > *> &,
                const vector<Material::Energy < 2 > *> &,
                Geometry::Cell *,
                std::map<Geometry::Cell*, Set::Manifold::Point*> &,
                std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
		const double * = NULL);

    protected:
        void GetDOF(Geometry::Cell * e,
                std::map<Geometry::Cell*, Set::Manifold::Point*> &,
                std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
                set<Set::Manifold::Point *> &,
                map<Set::Manifold::Point *, Set::VectorSpace::Vector> &) const;

    protected:
        vector<Set::VectorSpace::Vector> _QP;
        vector<double> _QW;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Finite Element source element builder
     * body force finite elements for FEA
     */
    ////////////////////////////////////////////////////////////////////////////////////
    class FESourceElementBuilder : public FiniteElementBuilder {
    public:

        FESourceElementBuilder(unsigned int dim, const Element::Quadrature::Rule & r)
        : FiniteElementBuilder(dim, r) {
        }

        virtual ~FESourceElementBuilder() {
        }

        virtual void createElement(const vector<Material::LocalState*> &,
                const vector<Material::Energy < 0 > *> &,
                const vector<Material::Energy < 1 > *> &,
                const vector<Material::Energy < 2 > *> &,
                Geometry::Cell *,
                std::map<Geometry::Cell*, Set::Manifold::Point*> &,
                std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
		const double * = NULL);
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * membrane element builder
     * stress membrane elements for FEA
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class MembraneElementBuilder : public FiniteElementBuilder {

      typedef set<Material::Membrane::LocalState*> Mat_Membrane_LS;
      typedef set<Material::Membrane::Energy<0> *> Mat_Membrane_E;
      typedef set<Material::Membrane::Energy<1> *> Mat_Membrane_DE;

    public:

        MembraneElementBuilder(unsigned int dim_,
			       const Element::Quadrature::Rule & QR_,
			       unsigned int max_itr_,
			       double tol_) :
        _MAX_ITR(max_itr_), _TOL(tol_), 
	FiniteElementBuilder(dim_, QR_) {
        }

        virtual ~MembraneElementBuilder() {
	  for ( Mat_Membrane_DE::iterator it = _MemDE.begin();
		it != _MemDE.end(); ++it ) {
	    delete *it;
	  }

	  for ( Mat_Membrane_E::iterator it = _MemE.begin();
		it != _MemE.end(); ++it ) {
	    delete *it;
	  }

	  for ( Mat_Membrane_LS::iterator it = _MemLS.begin();
		it != _MemLS.end(); ++it ) {
	    delete *it;
	  }
	}

        virtual void createElement(const vector<Material::LocalState*> &,
                const vector<Material::Energy < 0 > *> &,
                const vector<Material::Energy < 1 > *> &,
                const vector<Material::Energy < 2 > *> &,
                Geometry::Cell *,
                std::map<Geometry::Cell*, Set::Manifold::Point*> &,
                std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
		const double * = NULL);

    private:
	unsigned int _MAX_ITR;
	double _TOL;
	Mat_Membrane_LS _MemLS;
	Mat_Membrane_E  _MemE;
	Mat_Membrane_DE _MemDE;
    };
    
    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Cluster Material Point Element Builder for OTM
     * (1) material point located at the barycenter of the simplicial cell
     * (2) support of the material point is the nth-ring nodal set of the cell
     * (3) using the local maxent approximation scheme
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class MPElementBuilder : public ElementBuilder {
    public:

    MPElementBuilder(unsigned int dim, unsigned int nRing,
		     const Element::Quadrature::Simplicial::Rule & QR,
		     double beta, double cutoff = 1.0e-5, double extension = 2.1,
		     bool updateNeighbor = true,		     
		     Material::Builder * pSourceBuilder = NULL)
      :_nRing(nRing), _QR(QR), _extension(extension), _cutoff(cutoff), _beta(beta), 
	_updateNeighbor(updateNeighbor), _pSourceBuilder(pSourceBuilder),
	_critical_strain(0.25), _critical_numofneighbors(2*(dim+1)), _erosion(true),
	_enforce_meshfree(false), ElementBuilder(dim) {
      }
      
      virtual ~MPElementBuilder() {
	for ( int i = 0; i < _Fact.size(); ++i ) {
	  delete _Fact[i];
	}
      }
      
      void SetEnforceMeshfree(bool);
      void SetCriticalStrain(double);
      void SetCriticalNumofNeighbors(unsigned int);
      void SetErosion(bool);

      virtual void createElement(const vector<Material::LocalState*> &,
				 const vector<Material::Energy < 0 > *> &,
				 const vector<Material::Energy < 1 > *> &,
				 const vector<Material::Energy < 2 > *> &,
				 Geometry::Cell *,
				 std::map<Geometry::Cell*, Set::Manifold::Point*> &,
				 std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> &,
				 const double *);

    protected:
      virtual void _initializeElement(const vector<Material::LocalState*> &,
				      const vector<Material::Energy < 0 > *> &,
				      const vector<Material::Energy < 1 > *> &,
				      const vector<Material::Energy < 2 > *> &,
				      const double *);
      virtual void _createMP(Geometry::Cell * e,
			     std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & P);
      virtual void _getDoF(Geometry::Cell * e,
			   std::map<Geometry::Cell*, Set::Manifold::Point*> & Bind,
			   std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & P);
      virtual void _createMPElementData(const double *);
      virtual void _createMPElementData(Geometry::Cell * e1, Geometry::Cell * e2,
					std::map<Geometry::Cell*, Set::Manifold::Point*> & Bind,
					std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & P) { assert(false); }
      
    protected:
      unsigned int _nRing;
      const Element::Quadrature::Simplicial::Rule & _QR;
      double _extension;
      double _cutoff;
      double _beta;
      bool _updateNeighbor;
      double _critical_strain;
      unsigned int _critical_numofneighbors;
      bool _erosion;
      bool _enforce_meshfree;

      vector<double> _QW;
      vector<Set::VectorSpace::Vector> _xq;
      map<Set::Manifold::Point *, Set::VectorSpace::Vector> _Xloc;
      Material::Builder *_pSourceBuilder;
      vector<Material::Factory*> _Fact;
    };
    
    
    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Cluster Material Point Element Builder for OTM
     * (1) material point located at the barycenter of the simplicial cell
     * (2) support of the material point is the nth-ring nodal set of the cell
     * (3) using the moving least square approximation scheme
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class MPMLSElementBuilder : public MPElementBuilder {
    public:
      
    MPMLSElementBuilder(unsigned int dim, unsigned int nRing,
			const Element::Quadrature::Simplicial::Rule & QR,
			unsigned int m, double beta, double cutoff,
			double extension = 2.1, bool updateNeighbor = true,
			Material::Builder * pSourceBuilder = NULL) 
      : _m(m), MPElementBuilder(dim, nRing, QR, beta, cutoff, extension, updateNeighbor, pSourceBuilder) {
      }

      virtual ~MPMLSElementBuilder() {
      }
      
    protected:
      void _createMPElementData(const double *);
      
    protected:
      unsigned int _m;
    };

   
    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Cluster Material Point Element Builder for OTM
     * (1) material point located at the barycenter of the simplicial cell
     * (2) support of the material point is the nth-ring nodal set of the cell
     * (3) using the blending mixed approximation scheme
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class MPMixedElementBuilder : public MPElementBuilder {
    public:
      
    MPMixedElementBuilder(unsigned int dim, unsigned int nRing,
			  const Element::Quadrature::Simplicial::Rule & QR, 
			  const set<Set::Manifold::Point*> * FEAnodes,
			  const set<Set::Manifold::Point*> * MLSnodes,
			  unsigned int m, double rho, double cutoff,
			  double extension = 2.1, bool updateNeighbor = true,
			  Material::Builder * pSourceBuilder = NULL) 
      : _m(m), _rho(rho), _FEAnodes(FEAnodes), _MLSnodes(MLSnodes),
	MPElementBuilder(dim, nRing, QR, 10.0, cutoff, extension, updateNeighbor, pSourceBuilder) {
      }
      
      virtual ~MPMixedElementBuilder() {
      }
      
    protected:
      virtual void _getDoF(Geometry::Cell * e,
			   std::map<Geometry::Cell*, Set::Manifold::Point*> & Bind,
			   std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & P);
      void _createMPElementData(const double *);
      
    private:
      unsigned int _m;
      double _rho;
      vector<Set::Manifold::Point*> _FirstNeighbor;
      const set<Set::Manifold::Point*> * _FEAnodes;
      const set<Set::Manifold::Point*> * _MLSnodes;
    };
    
    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * Reactive Element Builder for OTM
     */
    /////////////////////////////////////////////////////////////////////////////////////
    class ReactiveElementBuilder : public MPElementBuilder {
    public:
      
    ReactiveElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
			   const Element::Quadrature::Simplicial::Rule & QR,
			   double T0,
			   Material::Builder * pReactedMatBuilder,
			   Element::MaterialPoint::Reactive::Data * Prop,
			   double beta, double cutoff,
			   double extension = 2.1, bool updateNeighbor = true,
			   Material::Builder * pSourceBuilder = NULL) 
      : _Chronos(Chronos), _Prop(Prop), _T0(T0), _pReactedMatBuilder(pReactedMatBuilder), 
	MPElementBuilder(dim, nRing, QR, beta, cutoff, extension, updateNeighbor, pSourceBuilder) {
      }

      virtual ~ReactiveElementBuilder() {
	for ( int i = 0; i < _Fact.size(); ++i ) {
	  delete _Fact[i];
	}
      }
      
    protected:
      virtual void _initializeElement(const vector<Material::LocalState*> &,
				      const vector<Material::Energy < 0 > *> &,
				      const vector<Material::Energy < 1 > *> &,
				      const vector<Material::Energy < 2 > *> &,
				      const double *);
    protected:
      double _T0;
      Clock * _Chronos;
      Element::MaterialPoint::Reactive::Data * _Prop;
      Material::Builder * _pReactedMatBuilder;
      vector<Material::Factory*> _Fact;
   };
}

#endif //M4EXTREME_ELEMENTBUILDER_H__INCLUDED_
