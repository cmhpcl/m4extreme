// MaterialPoint.h: interface for the MaterialPoint class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MODEL_MATERIALPOINT_H__INCLUED_)
#define MODEL_MATERIALPOINT_H__INCLUED_

#pragma once

#include <vector>
#include "../Static/Static.h"

#if defined(_M4EXTREME_THREAD_POOL)
#include "cc++/thread.h"
#include "Threads/ThreadMonitorIface.h"
#endif

namespace Model {
    namespace MaterialPoint {
        class LocalState;
        template <unsigned int> class Energy;
        template <unsigned int> class Jet;

        //////////////////////////////////////////////////////////////////////
        // Class LocalState
        //////////////////////////////////////////////////////////////////////

        class LocalState : public Model::Static::LocalState {
            friend class Energy < 0 >;
            friend class Energy < 1 >;
            friend class Energy < 2 >;
            friend class Jet < 0 >;
            friend class Jet < 1 >;

        public:
            typedef map<Set::Manifold::Point *, Set::Euclidean::Orthonormal::Point> point_type;
            typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;

            LocalState();
            virtual ~LocalState();
            Model::LocalState *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
            LocalState(Clock *, vector<Element::LocalState *> *,
                    map<Set::Manifold::Point *, Set::Manifold::Map *> *,
                    map<Set::Manifold::Point *, Set::Manifold::TMap *> *,
                    bool isMultiBodies = false);
#else
            LocalState(Clock *T_, set<Element::LocalState *> *,
                map<Set::Manifold::Point *, Set::Manifold::Map *> *,
                map<Set::Manifold::Point *, Set::Manifold::TMap *> *,
                bool isMultiBodies = false);
#endif

            LocalState(const LocalState &rhs);
            LocalState & operator =(const LocalState &);
	    virtual void Embed(const set<Set::Manifold::Point *> &,
			       vector_type &);            
            virtual void operator ++ ();
            virtual void Reset(const set<Set::Manifold::Point *> &);

            bool & IsMultiBodies() { return _isMultiBodies;}

        protected:
            bool _isMultiBodies;

#if defined(_M4EXTREME_THREAD_POOL)
            std::vector<Element::LocalState*> * _ELS_mt;      
            static std::vector<int> _Costs, _Costs_new;
            static ost::Mutex _lock;
#endif

	private:
#if defined(_M4EXTREME_THREAD_POOL)
            static void * _embed(void *);
            static void * _reset(void *);
            static void * _self_increment(void *);

        private:

            typedef struct {
                std::vector<Element::LocalState*> * _pELS;
                const vector_type * _nodeset;
            } _eureka_thread_arg;

            typedef struct {
                const set<Set::Manifold::Point *> * _y;
                vector_type * _yemb;
                map<Set::Manifold::Point *, Set::Manifold::Map *> * _Emb;
            } _eureka_thread_emb_arg;
#endif
        };

        //////////////////////////////////////////////////////////////////////
        // Class Energy<p>
        //////////////////////////////////////////////////////////////////////

        template<unsigned int p> class Energy;

        //////////////////////////////////////////////////////////////////////
        // Class Energy<0>
        //////////////////////////////////////////////////////////////////////

        template<> class Energy < 0 > : public Model::Static::Energy < 0 > {
        public:
            typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
            
            Energy();
            virtual ~Energy();
            Model::Energy < 0 > *Clone() const;
#if defined(_M4EXTREME_THREAD_POOL)
	    Energy(LocalState *, vector<Element::Energy < 0 > *> *);
#else
            Energy(LocalState *, set<Element::Energy < 0 > *> *);
#endif

            Energy(const Energy < 0 > &);
            Energy < 0 > & operator =(const Energy < 0 > &);
	    virtual range_type operator () (const domain_type &) const;

	protected:
#if defined(_M4EXTREME_THREAD_POOL)
	    static void * _computeEnergy(void * p);

	    std::vector<Element::Energy<0>*> * _E_mt;

            typedef struct {	      
              const std::vector<Element::Energy < 0 > *> * _pE;
	      const vector_type * _nodeset;
	      std::vector<double> * _energy;
            } _eureka_thread_arg;
#endif
        };

        //////////////////////////////////////////////////////////////////////
        // Class Energy<1>
        //////////////////////////////////////////////////////////////////////

        template<> class Energy < 1 > : public Model::Static::Energy < 1 > {
            friend class Jet < 1 >;

        public:
            typedef set<Set::Manifold::Point *> domain_type;
            typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
            typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> force_type;

            Energy();
            virtual ~Energy();
            Model::Energy < 1 > *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
            Energy(LocalState *, vector<Element::Energy < 1 > *> *);
#else
            Energy(LocalState *, set<Element::Energy < 1 > *> *);
#endif
            Energy(const Energy < 1 > &);
            Energy < 1 > & operator =(const Energy < 1 > &);

	    const std::map<Element::LocalState*, int> & GetComputationalCost() const {
	      return _computational_cost;
	    }

#if defined(_M4EXTREME_THREAD_POOL)

            void operator () (const domain_type &, range_type &);
            range_type operator () (const domain_type &) const;

        protected:
            std::vector<Element::Energy<1>*> * _DE_mt;

        private:
            static void * _computeForce(void *);

            static std::vector<int> _Costs, _Costs_new;

#if defined(_M4EXTREME_THREAD_EFFICIENT_LOCK_)
            typedef struct {
              const std::vector<Element::Energy < 1 > *> * _pDE;
	      const vector_type * _nodeset;
            } _eureka_thread_arg;

            static force_type _femb;
            static ost::Mutex _lock;
#else
            typedef struct {	      
              const std::vector<Element::Energy < 1 > *> * _pDE;
	      const vector_type * _nodeset;
	      std::vector<force_type> * _forces;
            } _eureka_thread_arg;
#endif

#endif
        };

        //////////////////////////////////////////////////////////////////////
        // Class Energy<2>
        //////////////////////////////////////////////////////////////////////

        template<> class Energy < 2 > : public Model::Static::Energy < 2 > {
        public:

            Energy();
            virtual ~Energy();
            Model::Energy < 2 > *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
            Energy(LocalState *, vector<Element::Energy < 2 > *> *);
#else
            Energy(LocalState *, set<Element::Energy < 2 > *> *);
#endif
            Energy(const Energy < 2 > &);
            Energy < 2 > & operator =(const Energy < 2 > &);

	protected:
#if defined(_M4EXTREME_THREAD_POOL)
	    std::vector<Element::Energy<2>*> * _DDE_mt;
#endif	    
        };

        //////////////////////////////////////////////////////////////////////
        // Class Jet<p>
        //////////////////////////////////////////////////////////////////////

        template<unsigned int p> class Jet;

        //////////////////////////////////////////////////////////////////////
        // Class Jet<0>
        //////////////////////////////////////////////////////////////////////

        template<> class Jet < 0 > : public Model::Static::Jet < 0 > {
        public:

            Jet();
            virtual ~Jet();
            Model::Jet < 0 > *Clone() const;
            Jet(LocalState *, set<Element::Jet < 0 > *> *);
            Jet(const Jet < 0 > &);
            Jet < 0 > & operator =(const Jet < 0 > &);
        };

        //////////////////////////////////////////////////////////////////////
        // Class Jet<1>
        //////////////////////////////////////////////////////////////////////

        template<> class Jet < 1 > : public Model::Static::Jet < 1 > {
        public:

            Jet();
            virtual ~Jet();
            Model::Jet < 1 > *Clone() const;
            Jet(LocalState *, set<Element::Jet < 1 > *> *);
            Jet(const Jet < 1 > &);
            Jet < 1 > & operator =(const Jet < 1 > &);
        };

    }

}

#endif // !defined(MODEL_MATERIALPOINT_H__INCLUED_
