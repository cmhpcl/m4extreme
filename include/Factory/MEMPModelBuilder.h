// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_MEMPMODELBUILDER_H__INCLUDED_)
#define M4EXTREME_MEMPMODELBUILDER_H__INCLUDED_

#include "ModelBuilder.h"

namespace m4extreme {
    
  class MEMPModelBuilder;
  
  // global functions
  double normalized_L2_error(int index, MEMPModelBuilder * pModel, const m4extreme::Utils::VectorField & exactsol);
}

namespace m4extreme {
    
    ////////////////////////////////////////////////////////////////////////////
    //  Material Point Model Builder class interfaces
    ////////////////////////////////////////////////////////////////////////////

    class MEMPModelBuilder : public ModelBuilder {
    public:

        typedef std::map<Element::MaterialPoint::LocalState *, int> COST_TYPE;

        enum DATA_TYPE {
            EFFECTIVE_STRESS = 1, EFFECTIVE_STRAIN = 2,  TEMPERATURE = 3, MEAN_STRESS = 4, 
	    VELOCITY = 5, DISPLACEMENT = 6, STATUS = 7, JACOBIAN = 8, J3 = 9, 
	    DEFORMATION_GRADIENT = 10, VOID_FRACTION = 11, ROTATION = 12, ORIENTATION = 13, 
	    GAMMA = 14, WEIGHT = 15, CAUCHY_STRESS = 16, TRUE_STRAIN = 17, ENERGY_RELEASE_RATE = 20,
	    DISSIPATED_ENERGY = 21, VORTICITY = 22, MASS = 23, AV_STRESS = 24, AV_DEFORMATION = 25
        };

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
        enum ENERGY_RELEASE_RATE_TYPE {
	  EIGEN_FRACTURE_TYPE_I = 1, EIGEN_FRACTURE_TYPE_II = 2, EIGEN_FRACTURE_TYPE_III = 3, VAPORIZATION_TYPE = 4, DAMAGE_TYPE = 5
        };
#endif

        MEMPModelBuilder(Clock * Chronos, unsigned int dim, double searchRange, bool isMultiBodies = true, bool adaptive_search = false);
        virtual ~MEMPModelBuilder();

        virtual void CreateModel();
        //virtual void ComputeLumpedMass();
        virtual void Reset();
        void UpdateMaterialPoints();
        void UpdateMaterialPoints(int);
	Geometry::Search<dof_type*> ** GetSearch() { return &_nbs; }
        void SetSearchRange(double searchRange) { _range = searchRange;	}
        double GetSearchRange(){ return _range;	}
	void SetBlackList(int, set<Set::Manifold::Point*> *);
        void SetIncrementalStatus(bool isIncremental) { _isIncremental = isIncremental;	}
	void SetStressTypeStatus(bool isStressType) { _isStressType = isStressType; }
        void SetCriticalMassFactor(double mass_factor) { _critical_mass_factor = mass_factor;}
	void SetCheckRegion(double x0, double x1, double y0, double y1,
			    double z0 = -1.0*std::numeric_limits<double>::max(),
			    double z1 = std::numeric_limits<double>::max()) {
	  _region_x0 = x0; _region_x1 = x1;
	  _region_y0 = y0; _region_y1 = y1;
	  _region_z0 = z0; _region_z1 = z1;
	}

	bool IsAdaptiveSearch() { 
	  return _isAdaptiveSearch;
	}

        virtual void InsertBody(Geometry::CellComplex &,
				std::map<Geometry::Cell*, point_type> &,
				ElementBuilder *,
				int numofQP,
				Material::Builder *,
				std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
				set<Geometry::Cell*> * = NULL,
				const double * = NULL,
				bool = true);

        virtual void InsertBody(size_t k,
				Geometry::CellComplex &,
				std::map<Geometry::Cell*, point_type> &,
				ElementBuilder *,
				int numofQP,
				Material::Builder *,
				std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
				set<Geometry::Cell*> * = NULL,
				const double * = NULL,
				bool = true);

	virtual void InsertTractionBoundary(const vector<Geometry::Cell*> &, 
					    const vector<Set::VectorSpace::Vector> &,
					    const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> &,
					    m4extreme::Utils::ScalarField * pLoad, 
					    double *T0=NULL);

        virtual void InsertTractionBoundary(const set<Geometry::Cell*> &,
					    const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> &,
					    m4extreme::Utils::ScalarField *, 
					    double * T0=NULL);

       
#if defined(_M4EXTREME_THREAD_POOL)       	
	virtual void InsertPotentialField(const set<Geometry::Cell*> &,
					  Potential::Field::Builder *);
	virtual void InsertPotentialTwoBody(dof_type *,
					    dof_type *,
					    Potential::Directional::Builder *,
					    bool=true); 
#endif
        
        const std::set<dof_type *> & GetDetachedNodes() const {
	  return _detached_nodes;
        }

        std::set<dof_type *> & GetDetachedNodes() {
	  return _detached_nodes;
        }
	
	void SetDetachedNodes();

	void SetArtificialViscosity(double ci, double cs, int type = -1, double eta = 0.0) {
	  _ci = ci; _cs = cs;_AV_type=type;_eta = eta;
	}

	void SetArtificialViscosity(double ci, double cs, 
				    double bi, double bs,
				    int type = -1, double eta = 0.0) {
	  _ci = ci; _cs = cs; _bi = bi; _bs = bs; _AV_type=type; _eta = eta;
	}

	void SetHourGlassControl(double hourglass_modulus) {
	  _hourglass_modulus = hourglass_modulus;
	}

#if defined(_M4EXTREME_EIGEN_FRACTURE_)  
	void SetGc(double Gc) {
	  _Gc = Gc;
	  return;
	}

	void SetStretchCriterion(int flag) {
	  _Stretch_Criterion = flag;
	  return;
	}

	void SetCriticalStretchFactor(double val) {
	  _critical_stretch_factor = val;
	  return;
	}

        bool CheckFailureMode_Adaptive(double *, ENERGY_RELEASE_RATE_TYPE *, bool *);
        void ComputeFreeEnergy(bool* useJtotal=NULL);
	void ComputeFreeEnergy(int, bool=true);
        void ComputeFreeEnergy(int, const map<Set::Manifold::Point *, Set::VectorSpace::Vector> &, bool=true);
        bool ComputeEnergyReleaseRate(double *, ENERGY_RELEASE_RATE_TYPE *, ScalarFunc * = NULL);
        bool ComputeEnergyReleaseRate(int, double, ENERGY_RELEASE_RATE_TYPE, ScalarFunc * = NULL);

        std::map<Geometry::Cell*, double> & GetGs() {
            return _Gs;
        }
	
        vector< vector<point_type*> > & GetQPS(){
	    return _qps_point;
	}

	vector< vector<Geometry::Cell*> > & GetQPS_Cell() {
	  return _qps_cell;
	}
		
        vector< vector<double*> > & GetQPS_Weight(){
	    return _qps_weight;
	}

        vector< vector<double> > & GetQPS_E() {
	    return _qps_E;
	}

        vector< vector<double> > & GetQPS_Gc() {
	    return _qps_Gc;
	}

	vector< vector<bool> > & GetQPS_Status() {
	    return _qps_status;
	}

	vector< vector<Element::MaterialPoint::Energy < 0 > *> > & GetQPS_MPE() {
	  return _qps_MPE;
	}

        vector<point_type*> & GetQPS(int i){
	    return _qps_point[i];
	}

	vector<Geometry::Cell*> & GetQPS_Cell(int i) {
	  return _qps_cell[i];
	}
		
        vector<double*> & GetQPS_Weight(int i){
	    return _qps_weight[i];
	}

        vector<double> & GetQPS_E(int i) {
	    return _qps_E[i];
	}

        vector<double> & GetQPS_Gc(int i) {
	    return _qps_Gc[i];
	}

	vector<bool> & GetQPS_Status(int i) {
	    return _qps_status[i];
	}

	vector<Element::MaterialPoint::Energy < 0 > *> & GetQPS_MPE(int i) {
	  return _qps_MPE[i];
	}

        int GetNumofErodedMpts(int index) const {
            return _eroded_mpts[index];
        }

	int GetNumofNghs() const {
	    return _average_nghs;
	}

	virtual double GetDissipatedEnergy() const {
	  double etotal = 0.0;
	  for (int i = 0; i < _MEMPLS.size(); ++i ) {
	    etotal += GetDissipatedEnergy(i);
	  }

	  return etotal;
	}
		
	virtual double GetDissipatedEnergy(int k) const {
	  double etotal = 0.0;
	  const vector<Element::MaterialPoint::LocalState*> & ELSloc = _MEMPLS[k];
	  for ( int i = 0; i < ELSloc.size(); ++i ) {
	    etotal += _dissipated_energy.find(ELSloc[i])->second;
	  }

	  return etotal;
	}
#endif

	double GetMass() const;
	double GetMass(int) const;
        double GetMinMass() const;
        void   GetMinSize(std::vector<double> &) const;
	double GetMinSize(int) const;
        double GetMinimalElementSize(std::vector<double> &) const;
	double GetMaximalElementSize(std::vector<double> &) const;
        const std::vector<double> & GetElementSizes(int) const;
        const std::vector< std::vector<double> > & GetElementSizes() const;
        const vector<Element::MaterialPoint::LocalState *> & GetMEMPLS(int) const;
        const vector< vector<Element::MaterialPoint::LocalState *> > & GetMEMPLS() const;
	const vector< vector<Element::MaterialPoint::LumpedMass *> > & GetMEMPMass() const;
        const vector<Geometry::Cell *> & GetMEMPQP(int) const;
        const vector< vector<Geometry::Cell *> > & GetMEMPQP() const;
        void GetQP(int, std::vector<Set::VectorSpace::Vector> &, std::vector<double> &, std::vector<int> &) const;
        void GetQPData(map<Geometry::Cell *, double> &,
                DATA_TYPE) const;
        void GetQPData(map<Geometry::Cell *, Set::VectorSpace::Vector> &,
                DATA_TYPE) const;
        void GetQPData(map<Geometry::Cell *, Set::VectorSpace::Hom> &,
                DATA_TYPE) const;
        void GetNodeData(map<Geometry::Cell*, double> &, DATA_TYPE) const;
        void GetNodeData(map<Geometry::Cell*, Set::VectorSpace::Hom> &, DATA_TYPE) const;

        void GetQPData(int k,
                map<Geometry::Cell *, double> &,
                DATA_TYPE) const;
        void GetQPData(int k,
                map<Geometry::Cell *, Set::VectorSpace::Vector> &,
                DATA_TYPE) const;
        void GetQPData(int k,
                map<Geometry::Cell *, Set::VectorSpace::Hom> &,
                DATA_TYPE) const;
        Geometry::Cell* GetNodeData(int k, map<Geometry::Cell*, double> &, DATA_TYPE) const;
        void GetNodeData(int k, map<Geometry::Cell*, Set::VectorSpace::Hom> &, DATA_TYPE) const;
        void GetNodeData(map<Geometry::Cell*, Set::VectorSpace::Vector> &, DATA_TYPE) const;
	double GetComputationalCost(vector<COST_TYPE> &) const;	
	void write(ostream &, const std::map<dof_type*, int> &) const;
	void read(istream &, int, int, const std::map<int, dof_type*> &);

	virtual double GetTotalMass() {
	  double mass = 0.0;
	  for ( int i = 0; i < _MEMPLS.size(); ++i ) {
	    mass += GetTotalMass(i);
	  }
	  return mass;
	}
	
	virtual double GetTotalMass(int k) const {
	  const vector<double> & mloc = _element_mass[k];
	  return std::accumulate(mloc.begin(), mloc.end(), 0.0);
	}
	
	virtual double GetTotalVolume() const {
	  double volume = 0.0;
	  for ( int i = 0; i < _MEMPLS.size(); ++i ) {
	    volume += GetTotalMass(i);
	  }
	  return volume;
	}
	
	virtual double GetTotalVolume(int k) const {
	  const vector<Element::MaterialPoint::LocalState*> & ELSloc = _MEMPLS[k];
	  double volume = 0.0;
	  for ( int i = 0; i < ELSloc.size(); ++i ) {
	    volume += ELSloc[i]->GetVolume();
	  }

	  return volume;
	}
	
	virtual double GetKineticEnergy() const {
	  double ke = 0.0;
	  for ( int i = 0; i < _MEMPLS.size(); ++i ) {
	    ke += GetKineticEnergy(i);
	  }
	  return ke;
	}

        virtual double GetKineticEnergy(int k) const {
	  double kloc = 0.0;

	  // map the velocity to material points for the calculation of local
	  // kinetic energy (!!!must do this way if the program is running in MPI mode)
	  map <Set::Manifold::Point *, Set::VectorSpace::Vector> vemb;
	  dynamic_cast<Model::Static::LocalState*> (_pLS)->Embed(_v, vemb);

	  map<Set::Manifold::Point*, double>::const_iterator pN;
	  const vector<double> & mloc = _element_mass[k];
	  const vector<Element::MaterialPoint::LocalState*> & ELSloc = _MEMPLS[k];

	  for (int i = 0; i < ELSloc.size(); i++) {
	    const vector<map<Set::Manifold::Point*, double> > & Nloc = ELSloc[i]->GetN();
	    const vector<double> & qw = ELSloc[i]->GetQW();
	    double wloc = ELSloc[i]->GetVolume();

	    for ( int q = 0; q < Nloc.size(); q++ ) {
	      Set::VectorSpace::Vector vp(_DIM);
	      for ( pN = Nloc[q].begin(); pN != Nloc[q].end(); pN++ ) {
		// avoid crazy detached nodes
		if ( _detached_nodes.find(pN->first) == _detached_nodes.end() ) {
		  vp += vemb.find(pN->first)->second * pN->second;
		}
	      }

	      kloc += 0.5 * mloc[i] * vp(vp) * qw[q] / wloc;
	    }
	  }

	  return kloc;
	}

        virtual void GetMomentum(int k,
				 Set::VectorSpace::Vector & mnt) const {
	  assert(mnt.size() == _DIM);	  
	  Null(mnt);
	  
	  // map the velocity to material points for the calculation of local
	  // linear momentum (!!!must do this way if the program is running in MPI mode)
	  map <Set::Manifold::Point *, Set::VectorSpace::Vector> vemb;
	  dynamic_cast<Model::Static::LocalState*> (_pLS)->Embed(_v, vemb);
	  
	  map<Set::Manifold::Point*, double>::const_iterator pN;
	  const vector<double> & mloc = _element_mass[k];
	  const vector<Element::MaterialPoint::LocalState*> & ELSloc = _MEMPLS[k];

	  for (int i = 0; i < ELSloc.size(); i++) {
	    const vector<map<Set::Manifold::Point*, double> > & Nloc = ELSloc[i]->GetN();
	    const vector<double> & qw = ELSloc[i]->GetQW();
	    double wloc = ELSloc[i]->GetVolume();

	    for ( int q = 0; q < Nloc.size(); q++ ) {
	      Set::VectorSpace::Vector vp(_DIM);
	      for ( pN = Nloc[q].begin(); pN != Nloc[q].end(); pN++ ) {
		// avoid crazy detached nodes
		if ( _detached_nodes.find(pN->first) == _detached_nodes.end() ) {
		  vp += vemb.find(pN->first)->second * pN->second;
		}
	      }

	      mnt += (mloc[i] * qw[q] / wloc) * vp;
	    }
	  }

	  return;
	}

	virtual double GetStrainEnergy() const {
	  double etotal = 0.0;
	  for (int i = 0; i < _MEMPEE.size(); ++i ) {
	    etotal += GetStrainEnergy(i);
	  }

	  return etotal;	  
	}

	// Note: do not use the _qps_E to compute the totoal strain energy, since
	// (1) it only contains values when EigenErosion is enabled and called,
	// (2) strain energy in _qps_E is nonzero only at those material points in stretch
        virtual double GetStrainEnergy(int k) const {
	  double etotal = 0.0;

#if defined(_M4EXTREME_EIGEN_FRACTURE_) && defined(_M4EXTREME_FRACTURE_CONSERVATIVE_I_)
	  const set<Element::MaterialPoint::Energy<0>*> & DMPEloc = _dissipated_MPE[k];
#endif

	  map<Set::Manifold::Point *, Set::VectorSpace::Vector> yemb;
	  static_cast<Model::Static::LocalState*>(_pLS)->Embed(_x, yemb);

	  const vector<Element::MaterialPoint::Energy<0>*> & EEloc = _MEMPEE[k];
	  for ( int i = 0; i < EEloc.size(); ++i ) {
	    const Element::LocalState *LSLoc = EEloc[i]->GetLocalState();      

#if defined(_M4EXTREME_EIGEN_FRACTURE_) 
	    if ( LSLoc->isActivated() ) {
#endif
	      set <Set::Manifold::Point*> N;
	      LSLoc->GetNodes(N);
	      map<Set::Manifold::Point *, Set::VectorSpace::Vector> yloc;
	      set<Set::Manifold::Point *>::iterator pN;
	      for (pN = N.begin(); pN != N.end(); pN++) {
		yloc.insert( make_pair(*pN, yemb.find(*pN)->second) );
	      }
	      
	      etotal += EEloc[i]->operator()(yloc);  
#if defined(_M4EXTREME_EIGEN_FRACTURE_)
	    }
#endif
	  }

	  return etotal;	  
	}	

    protected:
        virtual void _initializeMaterial();
        virtual void _initializeSingleElement(Geometry::Cell *,
                std::map<Geometry::Cell*, point_type> &,
                ElementBuilder *,
                const double * = NULL);
        virtual void _initializeSingleElement(size_t k,
		Geometry::Cell *,
                std::map<Geometry::Cell*, point_type> &,
                ElementBuilder *,
                const double * = NULL);

        void _updateSearch();
	void _initializeAuxiliaryData(const set<Geometry::Cell *> & ,
				      ElementBuilder * ,
				      Material::Builder *,
				      const double * );

#if defined(_M4EXTREME_EIGEN_FRACTURE_) && defined(_M4EXTREME_THREAD_POOL)
        static void * _computeEnergy(void *);
        static void * _computeEnergyReleaseRate(void *);
#endif

    private:
        MEMPModelBuilder(const MEMPModelBuilder &);
        MEMPModelBuilder & operator =(const MEMPModelBuilder &);

    public:
	point_type _lowerCorner;
        point_type _upperCorner;
        std::vector<dofset_type *> _carriers;
	std::map<dof_type*, int>   _node_carrier_id;

    protected:
	double _range, _critical_mass_factor, _min_mass;
	double _region_x0, _region_x1, _region_y0, _region_y1, _region_z0, _region_z1;
	double _Gc;
	int _Stretch_Criterion;
	double _critical_stretch_factor;
        bool _isMultiBodies;
        bool _isIncremental;
	bool _isStressType;
	bool _isAdaptiveSearch;

        Geometry::Search<dof_type*> * _nbs;
        std::set<Element::MaterialPoint::Data *> _ED;
        std::set<dof_type*> _detached_nodes;
	std::vector<Potential::Field::TimeDependent::TimeFunction*> _PFD;
	
#if defined(_M4EXTREME_THREAD_POOL)
        std::vector<Element::LocalState *> _ELS_mt;
        std::vector<Element::Energy < 0 > *> _EE_mt;
        std::vector<Element::Energy < 1 > *> _EDE_mt;
        std::vector<Element::Energy < 2 > *> _EDDE_mt;
        std::vector<Element::LumpedMass *> _ELM_mt;
#endif

	// coefficients in artificial viscosity
	double _ci, _cs, _bi, _bs, _eta, _AV_type;
	std::vector<Element::ArtificialViscosity::Data *> _EAVD;
	std::vector< std::vector<Element::ArtificialViscosity::LocalState *> > _EAVLS;

	// coefficient in hour glass modes
	double _hourglass_modulus;
        
	std::vector< Material::Builder * > _MatBuilder;
	std::vector< m4extreme::ElementBuilder * > _ElmBuilder;
	std::vector<const double*> _Rho;
        std::vector< std::vector<Element::MaterialPoint::LumpedMass *> > _MEMPMass;
        std::vector< std::vector<Element::MaterialPoint::LocalState *> > _MEMPLS;
	std::vector< std::vector<Element::MaterialPoint::Energy<0> *> > _MEMPEE;
        std::vector< std::vector<Geometry::Cell*> > _MEMPQP;
	std::vector< std::map<Geometry::Cell*, Element::MaterialPoint::LocalState*> > _MEMPQP_LS;
        std::vector< std::vector<double> > _element_sizes, _element_mass;	

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
	std::vector< std::vector<Geometry::Cell*> > _qps_cell;
        std::vector< std::vector<point_type*> > _qps_point;
	std::vector< std::vector<double*> > _qps_weight;
        std::vector< std::vector<bool> > _qps_status;
        std::vector< std::vector<double> > _qps_E;
	std::vector< std::vector<double> > _qps_Gc;
	std::vector< std::set<Element::MaterialPoint::Energy<0>*> > _dissipated_MPE;
        std::vector< std::vector<Element::MaterialPoint::Energy < 0 > *> > _qps_MPE;
        std::vector<int> _eroded_mpts;
	int _average_nghs;
	std::vector<COST_TYPE> _computational_cost_E, _computational_cost_G;

#if defined(_M4EXTREME_THREAD_POOL)  
	static std::map<Element::MaterialPoint::LocalState *, double> _dissipated_energy;
        static std::map<Geometry::Cell*, double> _Gs;       
        std::vector< std::vector<int> > _CostsE, _CostsE_new, _CostsG, _CostsG_new;

        typedef struct {
            std::vector<int> * _pCosts;
            std::vector<int> * _pCosts_new;
	    std::vector<point_type*> * _pQps;
	    std::vector<Geometry::Cell*> * _pQps_cell;
	    std::vector<double*> * _pQps_weight;
            std::vector<Element::MaterialPoint::Energy < 0 > *> * _pMPE;
	    std::set<Element::MaterialPoint::Energy<0>*> * _pDMPE;
            std::vector<double> * _pQps_E;
	    std::vector<double> * _pQps_Gc;
            std::vector<bool> * _pQps_status;
            const std::map<dof_type *, vector_type> * _pNodes;
	    const std::vector<double> * _pQW0;
            Geometry::Search<size_t> * _pNbs;
	    ScalarFunc * _pCED;
	    int _factorial;            
	    int _stretch_criterion;
	    double _stretch_factor;
            double _factor;
            ENERGY_RELEASE_RATE_TYPE _type;
            std::vector<int> * _pCounter;
        } _eureka_thread_arg;

#else
	std::map<Element::MaterialPoint::LocalState *, double> _dissipated_energy;
        std::map<Geometry::Cell*, double> _Gs;
#endif

#endif
    };

    ////////////////////////////////////////////////////////////////////////////
    //  LockingFree Material Point Model Builder class interfaces
    ////////////////////////////////////////////////////////////////////////////
    
    class LockingFreeModelBuilder : public MEMPModelBuilder {
    public:
      LockingFreeModelBuilder(double, Clock * Chronos, unsigned int dim, double searchRange, bool isMultiBodies = false, bool adaptive_search = false);
      virtual ~LockingFreeModelBuilder();
      
      virtual void CreateModel();
      
    private:
      double alpha;
    };
}

#endif //M4EXTREME_MEMPMODELBUILDER_H__INCLUDED_
