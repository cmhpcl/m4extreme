// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_TMMODELBUILDER_H__INCLUDED_)
#define M4EXTREME_TMMODELBUILDER_H__INCLUDED_

#include "Model/ThermoMechanicalCoupling/TMModel.h"
#include "Factory/MEMPModelBuilder.h"
#include "Potential/Flux/Field/Factory.h"
#include "Potential/Flux/OneBody/OneBody.h"

namespace m4extreme {

  ////////////////////////////////////////////////////////////////////////////
  //  Global interfaces
  ////////////////////////////////////////////////////////////////////////////
  double normalized_L2_error(int index, MEMPModelBuilder * pModel, const m4extreme::Utils::ScalarField & exactsol);

  /////////////////////////////////////////////////////////////////////////////
  //  ThermalMechanical Coupling Material Point Model Builder class interfaces
  /////////////////////////////////////////////////////////////////////////////
  
  class TMModelBuilder : public MEMPModelBuilder {
  public:
    typedef MEMPModelBuilder::point_type point_type;

    TMModelBuilder(Clock * Chronos, unsigned int dim, double searchRange,
		   bool isMultiBodies = true, bool adaptive_search = false) :
    MEMPModelBuilder(Chronos, dim, searchRange, isMultiBodies, adaptive_search) {}

    virtual ~TMModelBuilder() {}
	
    virtual void InitializeDof(std::map<Geometry::Cell*, point_type> &,
			       std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
			       set<Geometry::Cell*> * = NULL);
    
    virtual void CreateModel();
 
    virtual void InsertSurfaceTension(const vector<Geometry::Cell*> &, 
				      const vector<Set::VectorSpace::Vector> &,
				      const vector<double> &,
				      const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> &,
				      m4extreme::Utils::ScalarField *, double *);

    virtual void InsertFlux(double, Flux::Field::Builder *,
			    const vector<double> &,
			    const vector<Geometry::Cell*> &);
    virtual void InsertFlux(int, double, Flux::Field::Builder *,
			    const vector<double> &,
			    const vector<Geometry::Cell*> &);
    virtual void InsertFlux(double, Flux::Field::Builder *,
			    const map<Geometry::Cell*, double> &);
    virtual void InsertFlux(int, double, Flux::Field::Builder *,
			    const map<Geometry::Cell*, double> &);

    const std::map<Geometry::Cell*, std::vector<Flux::OneBody::LocalState*> > & GetFluxLS() const;
    virtual map<Geometry::Cell *, Potential::OneBody::LocalState *> & GetPOBELS(int=0);

  private:
    map<Geometry::Cell*, Potential::OneBody::LocalState*> _POBELS_SurfaceTension;
    std::map<Geometry::Cell*, std::vector<Flux::OneBody::LocalState*> > _fluxLS;
  };
  
}
#endif //M4EXTREME_TMMODELBUILDER_H__INCLUDED_
