// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_UTILS_FIELDS_H__INCLUDED_)
#define M4EXTREME_UTILS_FIELDS_H__INCLUDED_

#include "Set/SetLib.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

namespace m4extreme {

  namespace Utils {

    class ScalarField;
    class PolyScalarField;
    class VectorField;

    //  
    // class ScalarField
    //

    class ScalarField {
    public:
      ScalarField() {}
      virtual ~ScalarField() {}
    
      // Note: x, y, z are arbitrary variables, not necessary coordinates
      virtual double operator() (double x, double y=0.0, double z=0.0) const=0;

      virtual double operator() (double t, const Set::VectorSpace::Vector & x) const {
	switch (x.size()) {
	default:
	  return 0.0;
	case 1:
	  return this->operator()(x[0]);
	case 2:
	  return this->operator()(x[0], x[1]);
	case 3:
	  return this->operator()(x[0], x[1], x[2]);
	}
      }

      virtual double operator() (double t, double T, const Set::VectorSpace::Vector & x) const {
	return this->operator()(t, x);
      }
    };

    //
    // class RandomScalarField
    // generating a random number between the given range using a uniform distribution
    //
    class RandomScalarField : public ScalarField {
    public:
    RandomScalarField():
      _lower(0.0), _upper(0.0), _isNormalDistribution(false){}
      ~RandomScalarField(){}

    RandomScalarField(double lower_, double upper_):
      _lower(lower_), _upper(upper_),
      _isNormalDistribution(false) {
	_range = _upper - _lower;
	assert ( _range >= 0.0 );
    }

    RandomScalarField(double lower_, double upper_,
		      double x0_, double sigma_):
      _lower(lower_), _upper(upper_), 
      _x0(x0_), _sigma(sigma_), _isNormalDistribution(true) {
	// sigma * sigma
	_sig2 = _sigma;
	_sig2 *= _sig2;

	// sigma * sqrt(2*PI) = sigma * 2.50662827
	_sigma *= 2.50662827;

	//
	_range = _upper - _lower;
	assert ( _range >= 0.0 );
    }
      
    virtual double operator()(double x, double y=0.0, double z=0.0) const {
      if ( _isNormalDistribution ) {
	double dx2 = x;
	dx2 -= _x0;
	dx2 *= dx2;
	
	double pof = exp(-0.5*dx2/_sig2) / _sigma ;
	double delta = (double)rand()/RAND_MAX;
	if ( delta < pof ) {
	  return _range * (double)rand() / RAND_MAX + _lower;
	}
	else {
	  return 1.0;
	}
      }
      else {
	return _range * (double)rand() / RAND_MAX + _lower;
      }
    }
      
    private:
      double _lower, _upper, _range;
      double _x0, _sigma, _sig2;
      bool _isNormalDistribution;
    };

    //  
    // class PolyScalarField
    // f(x, y, z) = a0 + a1*(x-x0) + a2*(y-y0) + a3*(z-z0) 
    //            + a4*(x-x0)*(x-x0) + a5*(x-x0)*(y-y0) + a6*(x-x0)*(z-z0) 
    //            + a7*(y-y0)*(y-y0) + a8*(y-y0)*(z-z0) + a9*(z-z0)*(z-z0) 
    //            + ...
    //            = (a0, a1, a2, ...) \cdot (1, x-x0, y-y0, ...)
    //
    class PolyScalarField : public ScalarField {
    public:
      PolyScalarField(): pX(NULL){}
      
      PolyScalarField(int dim_, int m_, const Set::VectorSpace::Vector & a_)
	: dim(dim_), m(m_), a(a_) {
	k = GetNumofTerms(dim_, m_);
	assert(dim > 0 && a.size() == k);
	pX = new double[dim];
	for ( int i = 0; i < dim; ++i ) {
	  *(pX+i) = 0.0;
	}
      }

      PolyScalarField(int dim_, int m_, double *px_,
		    const Set::VectorSpace::Vector & a_)
	: dim(dim_), m(m_), a(a_) {
	k = GetNumofTerms(dim_, m_);
	assert(dim > 0 && a.size() == k);
	pX = new double[dim];
	for ( int i = 0; i < dim; ++i ) {
	  *(pX+i) = *(px_+i);
	}
      }

      virtual ~PolyScalarField() {
	if ( pX != NULL ) {
	  delete [] pX;
	}
      }

      virtual double operator() (double x, double y=0.0, double z=0.0) const {

	if ( m == 0 ) return a[0];

	double dx = x - *pX;

	double dy = 0.0; 
	if ( dim > 1 ) {
	  dy = y - *(pX+1);
	}

	double dz = 0.0;
	if ( dim > 2 ) {
	  dz = z - *(pX+2);
	}

	Set::VectorSpace::Vector u(k);
	switch(dim) {
	case 1:
	  {
	    u[0] = 1.0;
	    for ( int i = 1; i < k; ++i ) {
	      u[i] = u[i-1] * dx;
	    }
	  }
	  break;
	case 2:
	  {
	    double tabx[m+1];
	    double taby[m+1];
	  
	    tabx[0] = 1.0;
	    taby[0] = 1.0;
	    for ( int i = 1; i < m+1; ++i ) {
	      tabx[i] = tabx[i-1] * dx;
	      taby[i] = taby[i-1] * dy;
	    }
	  
	    int inc = 0;
	    for ( int i = 0; i < m+1; ++i ) {
	      for ( int j = 0; j < i+1; ++j ) {
		u[inc] = tabx[i-j] * taby[j];
		inc++; 
	      } 
	    } 
	  }
	  break;
	case 3:
	  {
	    if ( m==1 ) {
	      u[0] = 1;
	      u[1] = dx;
	      u[2] = dy;
	      u[3] = dz;
	    }
	    if ( m == 2 ) {
	      u[0] = 1;
	      u[1] = dx;
	      u[2] = dy;
	      u[3] = dz;
	      u[4] = dx * dx;
	      u[5] = dx * dy;
	      u[6] = dy * dy;
	      u[7] = dy * dz;
	      u[8] = dz * dz;
	      u[9] = dx * dz;
	    }
	  }
	  break;
	default:
	  break;
	}      

	return a(u);
      }

      int GetNumofTerms(int dim_, int m_) const {
	switch (dim_ ) {
	case 1:
	  return m_ + 1;
	case 2:
	  return (m_+1) * (m_+2) / 2;
	case 3:
	  return (m_+1) * (m_+2) * (m_+3) / 6;
	default:
	  return 0;
	}      
      }

    private:
      int dim;
      int m;
      int k;
      double *pX;
      Set::VectorSpace::Vector a;
    };

    //  
    // class ExpScalarField
    // f(x, y, z) = alpha * exp[(a0 a1 a2) \ cdot (x-x0 y-y0 z-z0) + b]
    //
    class ExpScalarField : public ScalarField {
    public:
    ExpScalarField(int dim_, double alpha_, double b_,
		   const Set::VectorSpace::Vector & a_,
		   const Set::VectorSpace::Vector & x0_)
      : dim(dim_), alpha(alpha_), b(b_), a(a_), x0(x0_) {
      }

      virtual ~ExpScalarField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	Set::VectorSpace::Vector dx(dim);
	dx[0] = x - x0[0];
	if ( dim > 1 ) dx[1] = y - x0[1];
	if ( dim > 2 ) dx[2] = z - x0[2];
	return alpha * exp(a(dx) + b);
      }

    private:
      int dim;
      double alpha, b;
      Set::VectorSpace::Vector a;
      Set::VectorSpace::Vector x0;
    };

    //  
    // class GaussianScalarField
    // f(x, y, z) = alpha * exp[-0.5 * ( (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ) / (b*b)]
    //
    class GaussianScalarField : public ScalarField {
    public:
    GaussianScalarField(int dim_, double alpha_, 
			const Set::VectorSpace::Vector & b_,
			const Set::VectorSpace::Vector & x0_)
      : dim(dim_), alpha(alpha_), b(b_), x0(x0_) {
	assert(b_.size() == dim && x0_.size() == dim);
      }

      virtual ~GaussianScalarField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	Set::VectorSpace::Vector dx(dim);
	dx[0] = (x - x0[0]) / b[0];
	if ( dim > 1 ) dx[1] = (y - x0[1]) / b[1];
	if ( dim > 2 ) dx[2] = (z - x0[2]) / b[2];
	return alpha * exp(-0.5 * dx(dx));
      }

    private:
      int dim;
      double alpha;
      Set::VectorSpace::Vector b;
      Set::VectorSpace::Vector x0;
    };

    //  
    // class PowerScalarField
    // f(x, y, z) = a + b * (|(x, y, z) - (x0, y0, z0)|/r)^alpha
    //
    class PowerScalarField : public ScalarField {
    public:
    PowerScalarField(int dim_,  
		     double a_, double b_, 
		     double alpha_, double r_,
		     const Set::VectorSpace::Vector & x0_)
      : dim(dim_), alpha(alpha_), a(a_), b(b_), r(r_), x0(x0_) {
	assert( r != 0.0 );
      }

      virtual ~PowerScalarField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {

	if ( fabs(alpha) < 1.0e-16 ) return a + b;

	Set::VectorSpace::Vector dx(dim);
	dx[0] = x - x0[0];
	if ( dim > 1 ) dx[1] = y - x0[1];
	if ( dim > 2 ) dx[2] = z - x0[2];
	double norm_dx = fabs(dx[0]);
	if ( dim > 1 ) norm_dx = sqrt(dx[0] * dx[0] + dx[1] * dx[1]);
	if ( dim > 2 ) norm_dx = sqrt(dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2]);
	return a + b * pow(norm_dx/r, alpha);
      }

    private:
      int dim;
      double alpha, a, b, r;
      Set::VectorSpace::Vector x0;
    };

    //
    // class CosineScalarField
    // f(theta) = sum_{i=0}^m a_i * cos(i*theta)
    //
    class CosineScalarField : public ScalarField {
    public:
    CosineScalarField(const std::vector<double> & a_)
      : a(a_) {
	assert(!a.empty());
      }
      
      virtual ~CosineScalarField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	double floc = 0.0;
	for ( int i = 0; i < a.size(); ++i ) {
	  floc += a[i] * cos(i*x);
	}

	return floc;
      }

    private:
      std::vector<double> a;
    };

    //  
    // class PiecewiseConstantField
    // only implemented for one-dimensional function
    // f(x) = f1 if x < xm-dx
    //      = (1-a(x))f1 + a(x)f2 xm-dx<= x <= xm+dx (transition zone)
    //      = f2 if xm+dx < x < xb-dx
    //      = (1-b(x))f2 + b(x)f3 xb-dx<= x <= xb+dx (transition zone)
    //      = f3 if xb+dx < x
    // a(x)={ 1 + erf[3(x-xm)/sqrt(2)/dx] } / 2;
    // b(x)={ 1 + erf[3(x-xb)/sqrt(2)/dx)] } / 2;
    // 
    class PiecewiseConstantField : public ScalarField {
    public:
    PiecewiseConstantField( const vector<double> & xa_, 
			    const vector<double> & f_,
			    double dx_)
      : xa(xa_), f(f_), dx(dx_) {
	int numofx = xa.size();
	assert(f.size() > numofx && numofx != 0);
	for ( int i = 0; i < numofx-1; ++i ) {
	  assert(xa[i] < xa[i+1]);
	}
      }

      virtual ~PiecewiseConstantField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	if ( x <= xa[0]-dx ) {
	  return f[0];
	}
	else if ( x <= xa[0] ) {
	  double aT = _Na(x, xa[0]);
	  return (1.0-aT)*f[0] + aT*f[1];
	}
	else if ( x >= xa.back()+dx ) {
	  return f[xa.size()];
	}
	else if ( x >= xa.back() ) {
	  double aT = _Na(x, xa.back());
	  return (1.0-aT)*f[xa.size()-1] + aT*f[xa.size()];	  
	}
	else {
	  vector<double>::const_iterator low = std::lower_bound(xa.begin(), xa.end(), x);
	  int i = low - xa.begin();

	  if ( x < xa[i]+dx ) {
	    double aT = _Na(x, xa[i]);
	    return (1.0-aT)*f[i] + aT*f[i+1];
	  }
	  else if ( x <= xa[i+1]-dx ) {
	    return f[i+1];
	  }
	  else {
	    double aT = _Na(x, xa[i+1]);
	    return (1.0-aT)*f[i+1] + aT*f[i+2];
	  }
	}
      }

    private:
      double _Na(const double & x, 
		 const double & xc) const {
	double xloc = 3.0*(x-xc)/dx/1.4142135623731; //standard variation=dx/3
	double aT = erf(fabs(xloc));
	if ( xloc < 0 ) {
	  aT *= -1.0;
	}
	aT += 1.0;
	aT *= 0.5;
	return aT;
      }

    private:
      std::vector<double> xa, f;
      double dx;
    };

    //
    // class PiecewiseLinearField
    //
    class PiecewiseLinearField : public ScalarField {
    public:
    PiecewiseLinearField( const map<double, double> & data_)
      : data(data_) {
	assert( !data.empty() );
      }

      virtual ~PiecewiseLinearField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	map<double, double>::const_iterator pD = data.begin();
	while ( pD != data.end() ) {
	  if (x < pD->first ) {
	    break;
	  }
	  pD++;
	}
	
	if ( pD == data.begin() ) {
	  return pD->second;
	}
	else if ( pD == data.end() ) {
	  return data.rbegin()->second;
	}
	else {
	  double x1 = pD->first;
	  double f1 = pD->second;
	  pD--;
	  double x0 = pD->first;
	  double f0 = pD->second;

	  return ((x1-x)*f0 + (x-x0)*f1)/(x1-x0);
	}
      }

    private:
      map<double, double> data;
    };

    //  
    // class PiecewiseScalarField
    // 1. only implemented for one-dimensional function
    // 2. the domain is divided into n+1 subdomains from 
    //    {-inf, xm[0], xm[1], ..., xm[n-1], inf}
    // 3. given the function in each subdomain:
    //    fs[0] for x < xm[0]
    //    fs[1] for xm[0] <= x < xm[1]
    //    ...
    //    fs[n-1] for xm[n-2] to xm[n-1]
    //    fs[n-1] for xm[n-1] <= x (extrapolation of the last function)
    // Thus,
    // f(x) = fs[0](x)   if x < xm[0]
    //      = fs[i](x)   if xm[i] <= x < xm[i+1] i=0..n-1
    //      = fs[n-1](x)   if xm[n-2] <= x
    // assure fs[i](xm[i]) = fs[i+1](xm[i]) for C0 continuity
    // 
    class PiecewiseScalarField : public ScalarField {
    public:
    PiecewiseScalarField( const vector<double> & xm_, 
			  const vector<ScalarField *> & fs_)
      : xm(xm_), fs(fs_) {

	int n = xm.size();
	assert( n != 0 && n == fs.size() && fs[0] != NULL);

	for ( int i = 0; i < n-1; ++i ) {
	  double xloc = xm[i];
	  assert(xloc < xm[i+1]);
	  assert(fs[i] != NULL);
	  double f0 = (*fs[i])(xloc);
	  double f1 = (*fs[i+1])(xloc);

	  if ( fabs(f0 - f1) > 1.0e-8 ) {
	    cerr << "\nPiecewiseScalarField: discontinuity (" 
		 << f0 << "!=" << f1 <<" at x=" << xloc << endl;
	    assert(false);
	  }
	}
      }

      virtual ~PiecewiseScalarField() {}

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	if ( x < xm[0] ) {
	  return (*fs[0])(x);
	}
	else {
	  for ( int i = 0; i < xm.size()-1; ++i ) {
	    if ( xm[i] <= x && x < xm[i+1] ) {
	      return (*fs[i+1])(x);
	    }
	  }

	  return (*fs.back())(x);
	}
      }

    private:
      vector<double> xm;
      vector<ScalarField *> fs;
    };


    //
    // ComposeScalarField
    // f(x, y, z) = f_0(x) * f_1(y) * f_2(z)
    // f(t, x) = f_0(t) * f_1(x[0], x[1], x[2])
    //
    class ComposeScalarField : public ScalarField {
    public:
    ComposeScalarField( const vector<ScalarField *> & fs_ )
      : fs(fs_) {
	assert(!fs.empty());
      }
      
      virtual ~ComposeScalarField() {}
      
      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	double floc = (*fs[0])(x);
	if ( fs.size() > 1 ) {
	  floc *= (*fs[1])(y);
	}
	
	if ( fs.size() > 2 ) {
	  floc *= (*fs[2])(z);
	}
	
	return floc;
      }

      virtual double operator() (double t, const Set::VectorSpace::Vector & x) const {
	double floc = (*fs[0])(t);
	
	if ( fs.size() > 1 && x.size() > 1 ) {
	  if ( x.size() < 2 ) {
	    floc *= (*fs[1])(x[0]);
	  }
	  else if ( x.size() < 3 ) {
	    floc *= (*fs[1])(x[0], x[1]);
	  }
	  else {
	    floc *= (*fs[1])(x[0], x[1], x[2]);
	  }
	}
	
	return floc;
      }

    protected:
      vector<ScalarField *> fs;
    };

    //
    // cylindrical coordinates: (r, theta, z) only defined in 3D
    // f(x, y, z) = f0(r) * f1(theta) * f2(z)
    // f(t, x, y z) = f0(t) * f1(r) * f2(theta) * f3(z);
    //
    class CylindricalComposeField : public ComposeScalarField {
    public:
    CylindricalComposeField(const Set::VectorSpace::Vector & x0_, 
			    const vector<ScalarField *> & fs_ )
      : center(x0_), ComposeScalarField(fs_) {
	assert(center.size()>1);
      }
      
      virtual ~CylindricalComposeField() {}
      
      virtual double operator() (double x, double y=0.0, double z=0.0) const {

	double dx = x - center[0];
	double dy = y - center[1];
	double theta = atan2(dy, dx);
	if ( dy < 0.0 ) theta += 6.28318530717958;

	dx *= dx;
	dy *= dy;
	double radius = sqrt(dx+dy);

	double floc = (*fs[0])(radius);
	
	if ( fs.size() > 1 ) {
	  floc *= (*fs[1])(theta);
	}
	
	if ( fs.size() > 2 ) {
	  floc *= (*fs[2])(z);
	}
	
	return floc;
      }

      virtual double operator() (double t, const Set::VectorSpace::Vector & x) const {
	
	assert(x.size() == 3);

	double floc = (*fs[0])(t);	
	
	double dx = x[0] - center[0];
	double dy = x[1] - center[1];
	double theta = atan2(dy, dx);
	if ( dy < 0.0 ) theta += 6.28318530717958;

	dx *= dx;
	dy *= dy;
	double radius = sqrt(dx+dy);


	floc *= (*fs[1])(radius);
	
	if ( fs.size() > 1 ) {
	  floc *= (*fs[2])(theta);
	}
	
	if ( fs.size() > 2 ) {
	  floc *= (*fs[3])(x[2]);
	}
	
	return floc;
      }

    private:
      Set::VectorSpace::Vector center;
    };

    //
    // class NormalScalarField
    // only implemented for one-dimensional function
    // f(x) = f0 + ft/{1 + exp[a*(x-xt)]}
    // 
    class NormalScalarField : public ScalarField {
    public:
      NormalScalarField( double f0_, double ft_, 
			 double a_,  double x0_,
			 double xt_)
	: f0(f0_), ft(ft_), a(a_), x0(x0_), xt(xt_) {
	assert(x0 < xt);
      }
      
      virtual ~NormalScalarField() {}
      
      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	if ( x < x0 ) {
	  return f0;
	}
	else {
	  return f0 + ft / ( 1 + exp(a*(x-xt)) );
	}
      }
      
    private:
      double f0, ft, a, x0, xt;
    };

    //
    // class ApparentHeatCapacity
    // 
    class ApparentHeatCapacity : public ScalarField {
    public:
      ApparentHeatCapacity(double Tm_,     double Tb_, double DT_, 
			   double sigma_,  double Cs_, double Cl_, 
			   double Cg_,     double Lm_, double Lb_)
	: Tm(Tm_), Tb(Tb_), DT(DT_), sigma(sigma_), 
	Cs(Cs_), Cl(Cl_), Cg(Cg_), Lm(Lm_), Lb(Lb_) {
      }
      
      virtual ~ApparentHeatCapacity() {}
      
      virtual double operator() (double T, double y=0.0, double z=0.0) const {
	// reset the specific heat capacity using the apparent heat capacity method
	
	double Cv = 0.0;

	if ( T < Tm - DT ) { // solid state
	  Cv = Cs;
	}
	else if ( T < Tm + DT ) { // solid to liquid transition state
	  double x = (T - Tm)/sigma/1.4142135623731;
	  if ( x < 0.0 ) { 
	    x = -x;
	  }
	  double aT = erf(x);
	  
	  if ( T < Tm ) {
	    aT *= -1.0;
	  }
	  
	  aT += 1.0;
	  aT *= 0.5;
	  Cv = Cs * (1.0 - aT);
	  Cv += Cl * aT;
	  
	  double dadT = exp(-x*x);
	  dadT /= sigma / 2.506628274631;
	  Cv += Lm * dadT;
	}
	else if ( T < Tb - DT ) { // liquid state
	  Cv = Cl;
	} 
	else if ( T < Tb + DT ) { // liquid to gas transition state
	  double x = (T - Tb)/sigma/1.4142135623731;
	  if ( x < 0.0 ) { 
	    x = -x;
	  }
	  double aT = erf(x);
      
	  if ( T < Tb ) {
	    aT *= -1.0;
	  }
	  
	  aT += 1.0;
	  aT *= 0.5;
	  Cv = Cl * (1.0 - aT);
	  Cv += Cg * aT;
      
	  double dadT = exp(-x*x);
	  dadT /= sigma / 2.506628274631;
	  Cv += Lb * dadT;
	}
	else { // gas state
	  Cv = Cg;
	}

	return Cv;
      }
      
      void SetMelting(double Tm_) {
	Tm = Tm_;
      }

      void SetBoiling(double Tb_) {
	Tb = Tb_;
      }

    private:
      double Tm, Tb, DT, sigma, Cs, Cl, Cg, Lm, Lb;
    };

    //
    // special class for the calculation of recoil pressure
    //
    class RecoilPressure_I : public ScalarField {
    public:
    RecoilPressure_I(double R_, double H_, double Tb_, double P0_):
      R(R_), H(H_), P0(P0_), Tb(Tb_){
    	assert( R_ != 0.0 && Tb_ > 0.0 );
      }

      virtual ~RecoilPressure_I() {}

      // t is time, T is temperature
      virtual double operator() (double t, double T, double z=0.0) const {
    	if ( T > Tb ) {
    	  return P0 * exp(H * (1.0/Tb - 1.0/T) / R);
    	}
    	else {
    	  return 0.0;
    	}
      }

      virtual double operator() (double t, double T, const Set::VectorSpace::Vector & x) const {
	return this->operator()(t, T);
      }

    private:
      double R, H, Tb, P0;
    };

    //
    // Lindemann model for the melting and boiling temperature
    //
    class Lindemann : public ScalarField {
    public:
      Lindemann(double Tp_, double aT_, double gamma_):
      gamma(2.0*(1.0/3.0 + aT_ - gamma_)), Tp(Tp_), aT(aT_){
      }
      
      virtual ~Lindemann() {}

      // J is the Jacobian
      virtual double operator() (double J, double y=0.0, double z=0.0) const {
	return Tp * exp(2.0*aT*(1.0-J)) * pow(J, gamma);
      }

    private:
      double Tp, aT, gamma;
    };


    //
    // special class for the calculation of recoil pressure
    //
    class RecoilPressure_II : public ScalarField {
    public:
      RecoilPressure_II( double f0_, double ft_, 
			 double a_,  double T0_,
			 double Tt_)
	: f0(f0_), ft(ft_), a(a_), T0(T0_), Tt(Tt_) {
	assert(T0 < Tt);
      }
      
      virtual ~RecoilPressure_II() {}
      
      // t is time, T is temperature
      virtual double operator() (double t, double T, double z=0.0) const {
	if ( T < T0 ) {
	  return f0;
	}
	else {
	  return f0 + ft / ( 1 + exp(a*(T-Tt)) );
	}
      }
      
      virtual double operator() (double t, double T, const Set::VectorSpace::Vector & x) const {
	return this->operator()(t, T);
      }

    private:
      double f0, ft, a, T0, Tt;
    };

    //
    // special class for the calculation of surface tension coefficient
    // Sahoo et. al., Metall. Mater. Trans. B, 19(3), 1988
    //
    class SurfaceTensionCoeff : public ScalarField {
    public:
      SurfaceTensionCoeff(double sig0_,  double A_, 
			  double Tm_,    double R_, 
			  double Gamma_, double k_, 
			  double DH_,    double as_):
      sig0(sig0_), A(A_), Tm(Tm_), 
      c0(R_*Gamma_), c1(k_*as_), c2(-DH_/R_) {
      }
      
      ~SurfaceTensionCoeff(){}

      // t is time, T is temperature
      virtual double operator() (double t, double T, double z=0.0) const {
	if ( T < Tm ) { // ignore surface tension is solid state
	  return 0.0;
	}
	else {
	  if ( c1 == 0.0 ) {
	    double sig = sig0 - A*(T-Tm);
	    if ( sig < 0.0 ) {
	      return 0.0;
	    }
	    else {
	      return sig;
	    }
	  }
	  else {
	    double sig = sig0 - A*(T-Tm) - c0*T*log(1+c1*exp(c2/T));
	    if ( sig < 0.0 ) {
	      return 0.0;
	    }
	    else {
	      return sig;
	    }
	  }
	}
      }
      
    private:
      double sig0, A, Tm, c0, c1, c2;
    };

    //
    // special class for the calculation of the derivative of 
    // the surface tension coefficient w.r.t. temperature 
    // Sahoo et. al., Metall. Mater. Trans. B, 19(3), 1988
    //
    class DSurfaceTensionCoeff : public ScalarField {
    public:
      DSurfaceTensionCoeff(double A_,     double Tm_, double R_, 
			   double Gamma_, double k_, 
			   double DH_,    double as_):
      A(A_), Tm(Tm_), c0(R_*Gamma_), c1(k_*as_), c2(-DH_/R_){
      }
      
      ~DSurfaceTensionCoeff(){}

      // t is time, T is temperature
      virtual double operator() (double t, double T, double z=0.0) const {
	if ( T < Tm ) { // ignore surface tension derivative is solid state
	  return 0.0;
	}
	else {
	  if ( c1 == 0.0 ) {
	    return -A;
	  }
	  else {
	    double ev = exp(c2/T);
	    ev *= c1;
	    double evpp = ev + 1.0;
	    return -A - c0 * log(evpp) + c0*ev*c2 / T / evpp;
	  }
	}
      }
      
    private:
      double A, Tm, c0, c1, c2;
    };

    //
    // special class for the newtonian viscosity model as a function of temeprature
    // f(T) = f0 * exp(-a0 + a1/T)
    //
    class AssaelViscosityModel : public ScalarField {
    public:
      AssaelViscosityModel( double f0_, double a0_, double a1_ ) 
	: f0(f0_), a0(a0_), a1(a1_) {
      }
     
      virtual ~AssaelViscosityModel() {
      }

      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	assert(x > 0.0);
	return f0 * exp(a1 / x - a0);
      }
      
    private:
      double f0, a0, a1;
    };

    //
    //* Friedlander equation for blast wave:
    //* P(t) = P0 + Pm(r) * (C0(r) + C1(r) * t) * exp(C2 * t) 
    //* Pm(r) = Pm * exp(-f*r/RI);
    //* C0(r) = (Tp+Tmax * r/RI) / (Tp-Tmax*r/RI);
    //* C1(r) = -t / (Tp-Tmax*r/RI);
    //* C2 = -C / Tp
    //* where P0 is the pressure before shock;
    //*       Pm is the peak pressure after shock;
    //*       t is current time;
    //*       C0, C1, C2 are coefficients fit to experimental data;
    //*       C2 < 0.
    //
    class FriedlanderBlastWave : public ScalarField {
    public:
      FriedlanderBlastWave(const Set::VectorSpace::Vector & Origin_,
			   double Pm_, double P0_, double f_, 
			   double Tp_, double Tmax_,
			   double RI_, double C_)	
	: _Origin(Origin_), _Pm(Pm_), _P0(P0_), _f(-1.0*f_/RI_), 
	_Tp(Tp_), _Tmax(Tmax_/RI_), _C(-1.0*C_/Tp_){
	assert(C_ > 0.0 && RI_ > 0.0 && f_ > 0.0);
      }
      
      virtual ~FriedlanderBlastWave(){
      }
      
      virtual double operator() (double x, double y=0.0, double z=0.0) const {
	return 0.0;
      }
      
      virtual double operator() (double t, const Set::VectorSpace::Vector & x) const {
	assert( x.size() == _Origin.size() );
	
	Set::VectorSpace::Vector xa = x - _Origin;
	double r = Norm(xa);
	
	double t0 = _Tmax;
	t0 *= r;

	double P = _P0;

	if ( t > t0) {
	  double Tpn = _Tp;
	  Tpn += t0;
	  Tpn -= t;

	  double Tpd = _Tp;
	  Tpd -= t0;

	  Tpn /= Tpd;
	  Tpn *= exp(_C*t);

	  double Pm = _Pm;
	  Pm *= Tpn;
	  Pm *= exp(_f*r);
	  P += Pm;
	}

	return P;
      }

    private:
      Set::VectorSpace::Vector _Origin;
      double _Pm, _P0, _f, _Tp, _Tmax, _C;
    };

    //
    // Cylindrical coordinate interpolation (Bi-linear interpolation)
    //
    //
    class CylindricalInterpolation : public ScalarField {
    public:
      //
      // zcoords and angles must be pre-sorted in ascend order in the input file
      //
      CylindricalInterpolation(const Set::VectorSpace::Vector & x0_, 
			       const string & filename_)
      : center(x0_) {
	assert(center.size()>1);
	char delim = ',';
	ifstream ifs(filename_.c_str());
	if ( ifs.is_open() ) {
	  string str, item;
	  char substr[2048];
	  double zloc;
	  int MAX_COUNT=1e4;
	  
	  getline(ifs, str);

	  std::istringstream iss(str);
	  while( std::getline(iss, item, delim) ) {
	    *std::back_inserter(_angles)++ = atof(item.c_str())*3.14159265358979/180.0;
	  }

	  int ncount=0;
	  while ( !ifs.eof() && ncount++ < MAX_COUNT ) {
	    getline(ifs, str);
	    if ( str.empty() ) continue;
	    sscanf(str.c_str(), "%lf, %[^\n]", &zloc, substr);
	    _zcoords.push_back(zloc);

	    vector<double> vloc;
	    std::istringstream issloc(substr);
	    while( std::getline(issloc, item, delim) ) {
	      *std::back_inserter(vloc)++ = atof(item.c_str());
	    }

	    if ( vloc.size() >= _angles.size() ) {
	      _data.push_back(vloc);
	    }
	    else {
	      cerr << "bad data at line #" << ncount
		   << " in the file " << filename_.c_str() 
		   << ". Abort!" << endl;
	      assert(false);
	    }
	  }
	}
	else {
	  cerr << "cannot fine the data file " << filename_.c_str() << ". Abort!" << endl;
	  assert(false);
	}
      }
      
      virtual ~CylindricalInterpolation() {}
      
      virtual double operator() (double x, double y=0.0, double z=0.0) const {

	double dx = x - center[0];
	double dy = y - center[1];
	double theta = atan2(dy, dx); // atan2 returns value from -pi to pi
	if ( dy < 0.0 ) theta += 6.28318530717958;

	std::vector<double>::const_iterator low_angle = std::lower_bound(_angles.begin(), _angles.end(), theta);
	int col = low_angle - _angles.begin();	
	if ( low_angle == _angles.end() ) {
	  --col;
	}

	std::vector<double>::const_iterator low_z = std::lower_bound(_zcoords.begin(), _zcoords.end(), z);
	int row =  low_z - _zcoords.begin();
	if ( low_z == _zcoords.end() ) {
	  --row;
	}

	double floc = 0.0;
	if ( low_z == _zcoords.begin() ||
	     low_z == _zcoords.end() ) {
	  if ( col == 0 ) {
	    floc = _data[row][col];
	  }
	  else {
	    if ( low_angle == _angles.end() ){
	      double t0 = _angles[col];
	      double t1 = _angles[0]+6.28318530717958;
	      double Delta_t = t1 - t0;
	      assert(Delta_t > 0.0 );
	      floc = ( (theta-t0)*_data[row][0] + (t1-theta)*_data[row][col] ) / Delta_t;
	    }
	    else {
	      double t0 = _angles[col-1];
	      double t1 = _angles[col];
	      double Delta_t = t1 - t0;
	      assert(Delta_t > 0.0 );
	      floc = ( (theta-t0)*_data[row][col] + (t1-theta)*_data[row][col-1] ) / Delta_t;
	    }
	  }
	}
	else {
	  if ( col == 0 ) {
	    double z0 = _zcoords[row-1];
	    double z1 = _zcoords[row];
	    double Delta_z = z1 - z0;
	    assert(Delta_z > 0.0);
	    floc = ((z-z0)*_data[row][col]+(z1-z)*_data[row-1][col])/Delta_z;
	  }
	  else {
	    double z0 = _zcoords[row-1];
	    double z1 = _zcoords[row];
	    double Delta_z = z1 - z0;

	    if ( low_angle == _angles.end() ){
	      double t0 = _angles[col];
	      double t1 = _angles[0]+6.28318530717958;
	      double Delta_t = t1 - t0;

	      assert(Delta_t > 0.0 && Delta_z > 0.0);

	      double v0 = ((z-z0)*_data[row][col]+(z1-z)*_data[row-1][col])/Delta_z;
	      double v1 = ((z-z0)*_data[row][0]+(z1-z)*_data[row-1][0])/Delta_z;
	      floc = ( (theta-t0)*v1 + (t1-theta)*v0 ) / Delta_t;
	    }
	    else {
	      double t0 = _angles[col-1];
	      double t1 = _angles[col];
	      double Delta_t = t1 - t0;

	      assert(Delta_t > 0.0 && Delta_z > 0.0);
	      double v0 = ((z-z0)*_data[row][col-1]+(z1-z)*_data[row-1][col-1])/Delta_z;
	      double v1 = ((z-z0)*_data[row][col]+(z1-z)*_data[row-1][col])/Delta_z;
	      floc = ( (theta-t0)*v1 + (t1-theta)*v0 ) / Delta_t;
	    }
	  }
	}
	
	return floc;
      }

    private:
      Set::VectorSpace::Vector center;
      vector<double> _zcoords;
      vector<double> _angles;
      vector< vector<double> > _data;
    };

    //  
    // class VectorField
    //

    class VectorField {
    public:
      VectorField() {}
      virtual ~VectorField() {}
    
      virtual Set::VectorSpace::Vector operator() (double x, double y, double z) const = 0;
    };

  }
}
#endif //M4EXTREME_UTILS_FIELDS_H__INCLUDED_
