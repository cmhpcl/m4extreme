// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_SOLVER_RIGIDBODY_H__
#define _M4EXTREME_SOLVER_RIGIDBODY_H__

#include "Set/SetLib.h"

namespace Solver {

  template<unsigned int DIM> 
    class RigidBody {
  public:
    RigidBody() {}
    virtual ~RigidBody() {}
      
  RigidBody(Clock * T_,
	    set<Set::Manifold::Point*> *x_,
	    set<Set::Manifold::Point*> *surface_,
	    const double & M_,
	    const Set::VectorSpace::Hom & I_,
	    const Set::VectorSpace::Vector & C_,
	    const Set::VectorSpace::Vector & V_,
	    const Set::VectorSpace::Vector & A_) 
    : _Chronos(T_), _mass(M_), _xdof(x_), _surfacedof(surface_), 
      _center(C_), _velocity(V_), _angular(DIM), 
      _linear_accel(DIM), _angular_accel(DIM), _gravity_accel(A_),
      _Ic(I_), _Ic_inv(Inverse(I_)),  
      _Rotation(DIM), _dR(DIM) {
      _initialize();
    }
      
  RigidBody(Clock * T_,
	    set<Set::Manifold::Point*> *x_,
	    set<Set::Manifold::Point*> *surface_,
	    const double & M_,
	    const Set::VectorSpace::Hom & I_,
	    const Set::VectorSpace::Vector & C_,
	    const Set::VectorSpace::Vector & V_,
	    const Set::VectorSpace::Vector & A_,
	    const Set::VectorSpace::Vector & W_) 
    : _Chronos(T_), _mass(M_), _xdof(x_), _surfacedof(surface_), 
      _center(C_), _velocity(V_), _angular(W_), 
      _linear_accel(DIM), _angular_accel(DIM), _gravity_accel(A_),
      _Ic(I_), _Ic_inv(Inverse(I_)),  
      _Rotation(DIM), _dR(DIM) {
      _initialize();
    }

    void _initialize() {
      if (DIM == 2) {
	_Rotation[0][0] = _Rotation[1][1] = 1.0;
	_dR[0][0] = _dR[1][1] = 1.0;
      }
      else if (DIM == 3) {
	_Rotation[0][0] = _Rotation[1][1] = _Rotation[2][2] = 1.0;
	_dR[0][0] = _dR[1][1] = _dR[2][2] = 1.0;
      }
      return;
    }

    // predict the new location of the nodes
    void Predictor() {
      const double & dt = _Chronos->DTime();

      // displacement of the center of mass from time t_k to t_k+1
      Set::VectorSpace::Vector cnew = _center + _velocity * dt + 0.5 * _linear_accel * dt * dt;

      // rigid body motion of the nodes
      for ( set<Set::Manifold::Point*>::iterator px = _xdof->begin();
	    px != _xdof->end(); px++ ) {
	int dof_dim = (*px)->size();
	if ( dof_dim != DIM ) {
	  // boundary conditions not supported
	  assert(false);	    
	}
	else {
	  Set::Euclidean::Orthonormal::Point * pxcoord = 
	    dynamic_cast<Set::Euclidean::Orthonormal::Point*>(*px);
	  Set::VectorSpace::Vector yloc = _dR(*pxcoord - _center) + cnew;
	  for ( int k = 0; k < DIM; ++k ) {
	    (*pxcoord)[k] = yloc[k];
	  }
	}
      }

      // update the location of the center
      _center = cnew;

      /*debug begin*/
      //cout << _velocity << "\t" << _linear_accel << "\t" << _center << endl;
      /*debug end*/

      return;
    }
      
    void Corrector(const map<Set::Manifold::Point *, Set::VectorSpace::Vector> & f) {
      set<Set::Manifold::Point*>::iterator px;
      map<Set::Manifold::Point *, Set::VectorSpace::Vector>::const_iterator pF;
	
      Set::VectorSpace::Vector force(DIM);
      Set::VectorSpace::Vector torque(DIM);
      Set::VectorSpace::Vector tloc(DIM);

      // compute the total force and the total torque about the center of mass
      _surface_forces.clear();
      for ( pF = f.begin(); pF != f.end(); pF++ ) {
	if ( (px=_surfacedof->find(pF->first)) != _surfacedof->end() ) {
	  //cout << pF->second << endl;
	  int dof_dim = (*px)->size();
	  if ( dof_dim != DIM ) {
	    // boundary conditions not supported
	    assert(false);	    
	  }
	  else { 
	    force += pF->second;
	    _surface_forces.insert( make_pair(pF->first, pF->second) );

	    Set::Euclidean::Orthonormal::Point * pxcoord = 
	      dynamic_cast<Set::Euclidean::Orthonormal::Point*>(*px);
	    Set::VectorSpace::Vector dloc = (*pxcoord) - _center;
	    _cross_product(dloc, pF->second, tloc);
	    torque += tloc;
	  }
	}	  
      }

      /*debug begin*/
      //cout << force << "\t" << torque << endl;
      //Null(force); Null(torque);
      /*debug end*/

      // calculate the linear acceleration
      Set::VectorSpace::Vector anew = -force / _mass + _gravity_accel;

      // update the linear velocity
      const double & dt = _Chronos->DTime();	
      _velocity +=  0.5 * dt * (_linear_accel + anew);
      _linear_accel = anew;
       
      // update the rotation matrix
      _skew_matrix(_angular, _dR);
      _dR *= dt;
      _dR[0][0] += 1.0;
      if ( DIM > 1 ) {
	_dR[1][1] += 1.0;
      }

      if ( DIM > 2 ) {
	_dR[2][2] += 1.0;
      }

      _Rotation = _dR * _Rotation;
	
      // calculate the angular velocity
      Set::VectorSpace::Hom RT = Adjoint(_Rotation);
      Set::VectorSpace::Hom Inertia = _Rotation * _Ic * RT;
      Set::VectorSpace::Hom Inertia_inv = _Rotation * _Ic_inv * RT;

      /* // explicit method to compute the angular acceleration */
      /* Set::VectorSpace::Vector wIw(DIM); */
      /* _cross_product(_angular, Inertia(_angular), wIw); */
      /* Set::VectorSpace::Vector wdot = Inertia_inv(torque - wIw); */
      /* _angular += 0.5 * dt * (_angular_accel + wdot); */
      /* _angular_accel = wdot; */

      // implicit method to compute the angular acceleration
      int MAX_COUNT=100;

      // suppose initial angular acceleration is zero
      Set::VectorSpace::Vector wnew(DIM);
      Set::VectorSpace::Vector wdot(DIM);
      Set::VectorSpace::Hom alpha(DIM);

      Set::VectorSpace::Vector res(DIM);
      _cross_product(_angular, Inertia(_angular), res);
      res -= torque;

      // trivial update of the angular velocity
      _angular += 0.5 * dt * _angular_accel;

      Set::VectorSpace::Hom W(DIM);
      _skew_matrix(_angular, W);
      Set::VectorSpace::Hom WI = W * Inertia;

      Set::VectorSpace::Hom Iw(DIM);
      _skew_matrix( Inertia(_angular), Iw);
      WI -= Iw;
      WI *= 0.5 * dt;
      WI += Inertia;
	
      int i=0;
      while(i++<MAX_COUNT && Norm(res)>1.0e-8){
	_skew_matrix(wdot, alpha);
	Set::VectorSpace::Hom aI = alpha * Inertia;

	Set::VectorSpace::Hom Ia(DIM);
	_skew_matrix( Inertia(wdot), Ia);
	aI -= Ia;
	aI *= 0.5 * dt * dt;
	aI += WI;
	  
	wdot -= Inverse(aI)*res;
	wnew = _angular + 0.5 * dt * wdot;
	_cross_product(wnew, Inertia(wnew), res);
	res -= torque;
	res += Inertia(wdot);
      };

      _angular = wnew;
      _angular_accel = wdot;

      return;
    }

    const Set::VectorSpace::Vector & GetCenter() const { return _center; }
    const Set::VectorSpace::Vector & GetVelocity() const { return _velocity; }
    const Set::VectorSpace::Vector & GetLinearAcceleration() const { return _linear_accel; }
    const Set::VectorSpace::Hom & GetRotation() const { return _Rotation; }
    const Set::VectorSpace::Hom & GetRotationInc() const { return _dR; }
    const map<Set::Manifold::Point*, Set::VectorSpace::Vector> & GetForces() const { return _surface_forces; }
      
  private:
    void _cross_product(const Set::VectorSpace::Vector & u,
			const Set::VectorSpace::Vector & v,
			Set::VectorSpace::Vector & product) {
      assert( u.size() == 3 && v.size() == 3 );
      product[0] = u[1] * v[2] - u[2] * v[1];
      product[1] = u[2] * v[0] - u[0] * v[2];
      product[2] = u[0] * v[1] - u[1] * v[0];
      return;
    }

    double _cross_product(const Set::VectorSpace::Vector & u,
			  const Set::VectorSpace::Vector & v) {
      assert( u.size() == 2 && v.size() == 2 );
      return u[0] * v[1] - u[1] * v[0];
    }
      
    void _skew_matrix( const Set::VectorSpace::Vector & u,
		       Set::VectorSpace::Hom & A ) {
      assert( u.size() == DIM && A.size1() == DIM && A.size2() == DIM );
      if ( DIM == 2 ) {
	assert(false);
      }
      else if ( DIM == 3 ) {
	A[0][0] = 0.0;  A[0][1] =-u[2]; A[0][2] = u[1];
	A[1][0] = u[2]; A[1][1] = 0.0;  A[1][2] =-u[0];
	A[2][0] =-u[1]; A[2][1] = u[0]; A[2][2] = 0.0;
      }

      return;
    }

  private:
    Clock * _Chronos;
    double _mass;
    set<Set::Manifold::Point*> *_xdof;
    set<Set::Manifold::Point*> *_surfacedof;
    Set::VectorSpace::Vector _center;
    Set::VectorSpace::Vector _velocity;
    Set::VectorSpace::Vector _angular;
    Set::VectorSpace::Vector _linear_accel;
    Set::VectorSpace::Vector _angular_accel;
    Set::VectorSpace::Vector _gravity_accel;
    Set::VectorSpace::Hom _Rotation, _dR;
    Set::VectorSpace::Hom _Ic, _Ic_inv;
    map<Set::Manifold::Point*, Set::VectorSpace::Vector> _surface_forces;
  };
}

#endif
