//
// prototype for element set up with factory pattern
// Bo Li
//

#if !defined(M4EXTREME_TMELEMENTBUILDER_H__INCLUDED_)
#define M4EXTREME_TMELEMENTBUILDER_H__INCLUDED_

#include "Factory/ElementBuilder.h"
#include "Element/ThermoMechanicalCoupling/TMElement.h"
#include "Element/ThermoMechanicalCoupling/ReactiveTMElement.h"
#include "Element/ThermoMechanicalCoupling/PorousTMElement.h"

namespace m4extreme {

  /////////////////////////////////////////////////////////////////////////////////////
  /**
   * TMElement builder within OTM framework using LME shape functions
   * (1) material point located at the barycenter of a simplicial cell
   * (2) support for the material point includes the vertices of the simplicial cell
   * (3) using the local maxent approximation scheme
   */
  /////////////////////////////////////////////////////////////////////////////////////
  class TMElementBuilder : public MPElementBuilder {
  public:
  TMElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
		   const Element::Quadrature::Simplicial::Rule & QR,
		   unsigned int TimeFactor,
		   Material::Builder * pHeatBuilder,
		   Material::Builder * pSourceBuilder,
		   double beta, double cutoff,
		   double extension = 2.1,
		   bool updateNeighbor = true,
		   Material::Builder * pBodyForceBuilder = NULL) 
    : _Chronos(Chronos), _TimeFactor(TimeFactor), _Tm(1.0e16), _Tb(1.0e16),
      _pHeatBuilder(pHeatBuilder), _pSourceBuilder(pSourceBuilder),
      MPElementBuilder(dim, nRing, QR, beta, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }

  TMElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
		   const Element::Quadrature::Simplicial::Rule & QR,
		   unsigned int TimeFactor,
		   double Tm, double Tb,
		   Material::Builder * pHeatBuilder,
		   Material::Builder * pSourceBuilder,
		   double beta, double cutoff,
		   double extension = 2.1,
		   bool updateNeighbor = true,
		   Material::Builder * pBodyForceBuilder = NULL) 
    : _Chronos(Chronos), _TimeFactor(TimeFactor), _Tm(Tm), _Tb(Tb),
      _pHeatBuilder(pHeatBuilder), _pSourceBuilder(pSourceBuilder),
      MPElementBuilder(dim, nRing, QR, beta, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }

    virtual ~TMElementBuilder() {
      for ( int i = 0; i < _Fact.size(); ++i ) {
	delete _Fact[i];
      }
    }

  protected:
    virtual void _createMP(Geometry::Cell * e,				      
			   std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & PExt);
    virtual void _getDoF(Geometry::Cell * e,
			 std::map<Geometry::Cell*, Set::Manifold::Point*> & Bind,
			 std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & PExt);
    virtual void _initializeElement(const vector<Material::LocalState*> &,
				    const vector<Material::Energy < 0 > *> &,
				    const vector<Material::Energy < 1 > *> &,
				    const vector<Material::Energy < 2 > *> &,
				    const double *);
  protected:
    Clock *_Chronos;
    unsigned int _TimeFactor;
    double _Tm;
    double _Tb;
    vector<double> _T0;
    Material::Builder *_pHeatBuilder;
    Material::Builder *_pSourceBuilder;
    vector<Material::Factory*> _Fact;
  };
 
  /////////////////////////////////////////////////////////////////////////////////////
  /**
   * TMMLSElement builder within OTM framework using MLS shape functions
   * (1) material point located at the barycenter of the simplicial cell
   * (2) support of the material point is the nodal set of the cell
   * (3) using the moving least square approximation scheme
   */
  /////////////////////////////////////////////////////////////////////////////////////
  class TMMLSElementBuilder : public TMElementBuilder {
  public:

  TMMLSElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
		      const Element::Quadrature::Simplicial::Rule & QR,
		      unsigned int TimeFactor,
		      Material::Builder * pHeatBuilder,
		      Material::Builder * pSourceBuilder,
		      unsigned int m, double beta, double cutoff,
		      double extension = 2.1,
		      bool updateNeighbor = true,
		      Material::Builder * pBodyForceBuilder = NULL) 
    : _m(m),
      TMElementBuilder(Chronos, dim, nRing, QR, TimeFactor, pHeatBuilder, pSourceBuilder, 
		       beta, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }

    virtual ~TMMLSElementBuilder() {
    }

  protected:
    void _createMPElementData(const double *);

  private:
    unsigned int _m;
  };

   
  /////////////////////////////////////////////////////////////////////////////////////
  /**
   * Mixed Material Point Element Builder for OTM
   * (1) material point located at the barycenter of the simplicial cell
   * (2) support of the material point is the nodal set of the cell
   * (3) using the blending mixed approximation scheme
   */
  /////////////////////////////////////////////////////////////////////////////////////
  class TMMixedElementBuilder : public TMElementBuilder {
  public:
    TMMixedElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
			  const Element::Quadrature::Simplicial::Rule & QR,
			  unsigned int TimeFactor,
			  Material::Builder * pHeatBuilder,
			  Material::Builder * pSourceBuilder,			  
			  const set<Set::Manifold::Point*> * FEAnodes,
			  const set<Set::Manifold::Point*> * MLSnodes,
			  unsigned int m, double rho, double cutoff,
			  double extension = 2.1,
			  bool updateNeighbor = true,
			  Material::Builder * pBodyForceBuilder = NULL)   
      : _m(m), _rho(rho), _FEAnodes(FEAnodes), _MLSnodes(MLSnodes),
      TMElementBuilder(Chronos, dim, nRing, QR, TimeFactor, pHeatBuilder, pSourceBuilder, 
		       10.0, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }
    
    virtual ~TMMixedElementBuilder() {
    }
    
  protected:
    void _getDoF(Geometry::Cell * e,
		 std::map<Geometry::Cell*, Set::Manifold::Point*> & Bind,
		 std::map<Geometry::Cell*, Set::Euclidean::Orthonormal::Point> & PExt);
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
   * Porous Material Point Element Builder for OTM
   * (1) material point located at the barycenter of the simplicial cell
   * (2) support of the material point is the nodal set of the cell
   * (3) using the LME approximation scheme
   */
  /////////////////////////////////////////////////////////////////////////////////////  
  class PorousTMElementBuilder : public TMElementBuilder {
  public:

  PorousTMElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
			 const Element::Quadrature::Simplicial::Rule & QR,
			 unsigned int TimeFactor,
			 Material::Builder * pHeatBuilder,
			 Material::Builder * pSourceBuilder,
			 double Rc_, double Phif_, double Phif0_, double Rhof_,
			 map<Set::Manifold::Point*, Set::VectorSpace::Vector> * Flux_,
			 double beta, double cutoff,
			 double extension = 2.1,
			 bool updateNeighbor = true,
			 Material::Builder * pBodyForceBuilder = NULL) 
    : Rc(Rc_), Phif(Phif_), Phif0(Phif0_), Rhof(Rhof_), Flux(Flux_),
      TMElementBuilder(Chronos, dim, nRing, QR, TimeFactor, pHeatBuilder, pSourceBuilder, 
		       10.0, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }

    virtual ~PorousTMElementBuilder() {}

  protected:
    virtual void _initializeElement(const vector<Material::LocalState*> &,
				    const vector<Material::Energy < 0 > *> &,
				    const vector<Material::Energy < 1 > *> &,
				    const vector<Material::Energy < 2 > *> &,
				    const double *);
  private:
    double Rc, Phif, Phif0, Rhof;
    map<Set::Manifold::Point*, Set::VectorSpace::Vector> * Flux;
  };

  /////////////////////////////////////////////////////////////////////////////////////
  /**
   * Moist Porous Material Point Element Builder for OTM
   * (1) material point located at the barycenter of the simplicial cell
   * (2) support of the material point is the nodal set of the cell
   * (3) using the LME approximation scheme
   */
  /////////////////////////////////////////////////////////////////////////////////////  
  class MoistPorousTMElementBuilder : public TMElementBuilder {
  public:

  MoistPorousTMElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
			      const Element::Quadrature::Simplicial::Rule & QR,
			      unsigned int TimeFactor, 
			      unsigned int UnitFactor_,
			      Material::Builder * pHeatBuilder,
			      Material::Builder * pSourceBuilder,
			      double Tb_, double Ks_, double Phif_, double Phif0_, double Moist_,
			      map<Set::Manifold::Point*, Set::VectorSpace::Vector> * Flux_,
			      double beta, double cutoff,
			      double extension = 2.1,
			      bool updateNeighbor = true,
			      Material::Builder * pBodyForceBuilder = NULL) 
    : UnitFactor(UnitFactor_), Ks(Ks_), Tb(Tb_), Phif(Phif_), Phif0(Phif0_), Moist(Moist_), Flux(Flux_),
      TMElementBuilder(Chronos, dim, nRing, QR, TimeFactor, pHeatBuilder, pSourceBuilder, 
		       10.0, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
    }

    virtual ~MoistPorousTMElementBuilder() {}

  protected:
    virtual void _initializeElement(const vector<Material::LocalState*> &,
				    const vector<Material::Energy < 0 > *> &,
				    const vector<Material::Energy < 1 > *> &,
				    const vector<Material::Energy < 2 > *> &,
				    const double *);
  private:
    double UnitFactor, Ks, Tb, Phif, Phif0, Moist;
    map<Set::Manifold::Point*, Set::VectorSpace::Vector> * Flux;
  };

  /////////////////////////////////////////////////////////////////////////////////////
  /**
   * Reactive Element Builder for HOTM
   */
  /////////////////////////////////////////////////////////////////////////////////////
  class ReactiveTMElementBuilder : public TMElementBuilder {
    public:
      
    ReactiveTMElementBuilder(Clock * Chronos, unsigned int dim, unsigned int nRing,
			     const Element::Quadrature::Simplicial::Rule & QR,
			     Element::MaterialPoint::ReactiveTMCoupling::Data * Prop,
			     Material::Gas::EoS::JWL::Builder * pReactedMatBuilder,
			     unsigned int TimeFactor,
			     Material::Builder * pHeatBuilder,
			     Material::Builder * pSourceBuilder,
			     Material::Source::Reaction::Builder * pReactionSrcBuilder,
			     double beta, double cutoff,
			     double extension = 2.1,
			     bool updateNeighbor = true,
			     Material::Builder * pBodyForceBuilder = NULL)
      : _Prop(Prop), _pReactedMatBuilder(pReactedMatBuilder), 
        _pReactionSrcBuilder(pReactionSrcBuilder),
	TMElementBuilder(Chronos, dim, nRing, QR, TimeFactor, pHeatBuilder, pSourceBuilder, 
			 beta, cutoff, extension, updateNeighbor, pBodyForceBuilder) {
      }

      virtual ~ReactiveTMElementBuilder() {
	for ( int i = 0; i < _EoSFact.size(); ++i ) {
	  delete _EoSFact[i];
	}
      }
      
    protected:
      virtual void _initializeElement(const vector<Material::LocalState*> &,
				      const vector<Material::Energy < 0 > *> &,
				      const vector<Material::Energy < 1 > *> &,
				      const vector<Material::Energy < 2 > *> &,
				      const double *);

    protected:
      Element::MaterialPoint::ReactiveTMCoupling::Data * _Prop;
      Material::Gas::EoS::JWL::Builder * _pReactedMatBuilder;
      Material::Source::Reaction::Builder * _pReactionSrcBuilder;
      vector<Material::Gas::EoS::Factory*> _EoSFact;
   };

}

#endif //M4EXTREME_TMELEMENTBUILDER_H__INCLUDED_
