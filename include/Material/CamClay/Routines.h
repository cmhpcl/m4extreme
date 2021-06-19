///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
///////////////////////////////////////////////////////////////////////////////


#if !defined(MATERIAL_LAW_ROUTINES_COMPRESSIBLE_H__INCLUDED_)
#define MATERIAL_LAW_ROUTINES_COMPRESSIBLE_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Material.h"

namespace Material
{
namespace CamClayFiniteDeformation
{



//======== Elasticity Coefficients  ============================

double computef (double Je,int phase ) {
    double a = 25.17e9;
    double b = 1879.7e9;
    double kappa = 33.75e9;
    double f;
    if (phase==0){
        f = kappa/2.0*pow((Je-1),2);
    }
    else {
        f = a/2.0*pow((Je-1.0),2.0) + b/4.0*pow((Je-1.0),4.0);
    } 
    return f;
}

double computedf (double Je,int phase ) {
    double a = 25.17e9;
    double b = 1879.7e9;
    double kappa = 33.75e9;
    double df;
    if (phase==0 ){
        df = kappa*((Je-1.0));
    }
    else {
        df = a*(Je-1.0) + b*pow((Je-1.0),3.0);
    }   
    return df;
}
double computemu (double Je,int phase ) {
    double a0 = 347.15e9;
    double a1 = -745.82e9;
    double a2 = 426.46e9;
    double b1 = 0.2077e9;
    double b2 = -19.498;
    double b3 = 34.6439e9;
    double mu;
    if (phase==0) {
       mu = a0 + a1*Je + a2 * pow(Je,2.0);
    }
    else {
       mu = b1*exp(b2*(Je-1.0)) + b3 ;
    }
    return mu;
}

double computedmu (double Je,int phase ) {
    double a0 = 347.15e9;
    double a1 = -745.82e9;
    double a2 = 426.46e9;
    double b1 = 0.2077e9;
    double b2 = -19.498;
    double b3 = 34.6439e9;
    double dmu;
    if (phase==0) {
       dmu =  a1 + 2.0*a2 * Je;
    }
    else {
       dmu = b2*b1*exp(b2*(Je-1.0));
    } 
    return dmu;
}

//========  Elastic Energy  ============================
double computeWe ( const Set::VectorSpace::Hom &Ce,int phase ) {
   double Je = sqrt(Jacobian(Ce));
   double mu =computemu(Je,phase);
   double f = computef(Je,phase);
   double I1 = Trace(Ce);
   I1 = I1/pow(Je,(2.0/3.0));
   double We =  mu/2.0 * (I1 - 3.0) + f; 
   return We;
} 

 
Set::VectorSpace::Hom computedWe (const Set::VectorSpace::Hom &Ce, int phase ) {
    double Je = sqrt(Jacobian(Ce));
    double dmu = computedmu(Je,phase);
    double df = computedf(Je,phase);
    double mu = computemu(Je,phase);
    double I1 = Trace(Ce);
    I1 = I1/pow(Je,(2.0/3.0));
    //cout << Je<<endl;
    Set::VectorSpace::Hom invCe =Inverse(Ce);
    Set::VectorSpace::Hom eye(3,3);
    Identity(eye);
    Set::VectorSpace::Hom DweDCe = (1.0/2.0)*(dmu/2.0 * (I1 - 3.0)*Je + df*Je -mu*I1/3.0)*(invCe) +1.0/2.0* mu *pow(Je,(-2.0/3.0))*eye;
    return DweDCe;
} 
//======== Stored Energy and Hardening Rule ===============
double computeWp (double Jp ) {

    double Kb = 4.49414e9;
    double kb = 9.123;
    double gamma = 4.12347e9;
    double Wp; 
    if (Jp>1.2)  {
        Wp = Kb/(kb*kb) ;
    }
    else {
        Wp = (Kb/(kb*kb) * pow(Jp,-kb) + (-1.0*gamma + Kb/kb) * log(Jp));
    }
    return Wp;
}
double computedWp (double Jp ) {
   
    double Kb = 4.49414e9;
    double kb = 9.123;
    double gamma = 4.12347e9;
    double dWp;
    if (Jp>1.2)  {
        dWp = 0.0 ;
    }
    else {
        dWp = (-1.0*Kb/(kb) * pow(Jp,-kb-1.0) + (-1.0*gamma + Kb/kb) /(Jp));
    }
    return dWp;
}
double computeddWp (double Jp ) {
   
    double Kb = 4.49414e9;
    double kb = 9.123;
    double gamma = 4.12347e9;
    double ddWp;
    if (Jp>1.2)  {
        ddWp = 0.0 ;
    }
    else {
        ddWp= ((-Kb/kb*(-kb-1.0) * pow(Jp,(-kb-2.0)) - (-1.0*gamma + Kb/kb) * pow(Jp,-2.0)));
    }
    return ddWp;
}

// ======  Routines for variational constitutive update ======================================

double computeqc(double pcin) {
//compute the critical state line
//First switch sign convention
double pc = -1.0*pcin;
double beta = 1.0/2.0;
double B = 1.16752*sqrt(1.0e9);
double pt = -8.5e9;
double qt = 7.40229e9;
double p1 = 12.33716e9;

double pm = (pc+pt)/2.0;

double barp1 = 4.14145e9;
double barp2 =  6.08447e9;
double qc;

if (pm<barp1){
   qc =  (p1-pm)/(p1-pt)*qt;
}
else if (pm < barp2 ) {
    double r = 5.17574e9;
    double s = 7.67441e18;
    qc = sqrt(s+3/4*pow((pm-r),2.0));
}
else {
    qc = B*pow(pm,beta);
}    

return qc;
}



double computetildephistar (const Set::VectorSpace::Hom &dp,const Set::VectorSpace::Hom &F,const Set::VectorSpace::Hom &Fp,double Delt,double pc_hardening,double alpha,double pt ) {
    double Jp = Jacobian(Fp);
    double J = Jacobian(F);
    Set::VectorSpace::Hom eye(3,3);
    Identity(eye);
    double varepsilonp = sqrt(Trace(dp*dp));
    double phistartilde;
    if (varepsilonp > 0.0) {
        Set::VectorSpace::Hom M = dp/varepsilonp;
        Set::VectorSpace::Hom devm = M - Trace(M)/3.0*eye;
        double devmdotdevm = Trace(devm*devm);
        double trM = Trace(M);
        double pc = min(pc_hardening, Jp/J*computedWp(Jp));
        double qc = computeqc(pc);
        alpha = 2.0*qc/(pc-pt); 
        double gfm =sqrt( pow(qc,2.0)/(1.0/(4.0*pow(alpha,2.0))*pow((Trace(M)),2.0) + 1.0/6.0*(devmdotdevm)));

        phistartilde = varepsilonp*(1.0/2.0*trM*(gfm/pow(alpha,2.0)*trM-pc +pt) + 1.0/3.0*(gfm) *(devmdotdevm));
    }
    else {
        phistartilde = 0.0;
    }
    
    return phistartilde;
}


double  computedqcdpc(double pcin) {
//comptute the change in qc with respect to pc.
//compute the critical state line slopt
//First switch sign convention
double pc = -1.0*pcin;
double beta = 1.0/2.0;
double B = 1.16752*sqrt(1.0e9);
double pt = -8.5e9;
double qt = 7.40229e9;
double p1 = 12.33716e9;


double pm = (pc+pt)/2.0;
double barp1 = 4.14145e9;
double barp2 =  6.08447e9;
double dqcdpc;
if (pm < barp1) {
   dqcdpc =  -1.0/(p1-pt)*qt;
}
else if (pm < barp2 ) {
    double r = 5.17574e9;
    double s = 7.67441e18;
    double qc = computeqc(pcin);
    dqcdpc = 3.0/4.0*(pm-r)/qc;
}
else {
    dqcdpc = beta*B*pow(pm,(beta-1.0));
}    

dqcdpc = -1.0*dqcdpc/2.0; // chain rule dpmdpc = 1/2 and additionally reswitch the sign due to the convention change
return dqcdpc;
}


double computedtildephistardpc(const Set::VectorSpace::Hom &dp,const Set::VectorSpace::Hom &F,const Set::VectorSpace::Hom &Fp,double Delt,double pc_hardening,double alpha,double pt ) {
    double Jp = Jacobian(Fp);
    double J = Jacobian(F);
    double varepsilonp = sqrt(Trace(dp*dp));
    double dphistartildedpc;
    if (varepsilonp > 0.0) {
        Set::VectorSpace::Hom eye(3,3);
        Identity(eye);
        Set::VectorSpace::Hom devdp = dp - Trace(dp)/3.0*eye;
        double devdpdotdevdp = Trace(devdp*devdp);
        double trdp = Trace(dp);
        double pc = min(pc_hardening, Jp/J*computedWp(Jp));
        double qc = computeqc( pc );
        alpha = 2.0*qc/(pc-pt);
        double etabar =sqrt((pow(qc,2.0))/(1.0/(4.0*pow(alpha,2.0))*pow((Trace(dp)),2.0) + 1.0/6.0*(devdpdotdevdp)) );
        double xi = 1.0/(4.0*pow(alpha,2.0))*pow((Trace(dp)),2.0) + 1.0/6.0*(devdpdotdevdp);
        double dxidpc = 1.0/8.0*(pc-pt)/(pow(qc,2.0))*(pow(trdp,2.0));
        double detadpcexp = 1.0/(2.0*etabar)*(-1.0*(pow(qc,2.0))/(pow(xi,2.0)))*dxidpc;
        double dxidqc = (pow((pc-pt),2.0))/8.0*(pow(trdp,2.0))*-1.0/(pow(qc,3.0));
        double detadqc = 1.0/etabar*qc/xi-1.0/2.0/etabar*(pow(qc,2.0))/(pow(xi,2.0))*dxidqc;
        
        double dtildephistardpcexp =  1.0/2.0*trdp*(etabar*(pc-pt)/2.0/(pow(qc,2.0))*trdp - 1.0);
        double dtildephistardqcexp = -1.0/2.0*trdp*(etabar/2.0*(pow((pc-pt),2.0))*trdp/pow(qc,3.0));
        double dphistardtildeeta = 1.0/2.0*pow(trdp,2.0)*(pow(((pc-pt)/(2.0*qc)),2.0)) + 1.0/3.0*devdpdotdevdp;
        
        double dqcdpc = computedqcdpc(pc);
        
        dphistartildedpc = dtildephistardpcexp + dtildephistardqcexp *dqcdpc + dphistardtildeeta*(detadpcexp + detadqc*dqcdpc);
    }
    else {
        dphistartildedpc = 0.0;
    } 
    return dphistartildedpc;
}


double computedgddelepsp(double delepsp,const Set::VectorSpace::Hom &M, const Set::VectorSpace::Hom &F, const Set::VectorSpace::Hom &Fpalpha,double Delt,double pc_min,double alpha,int phase,double pt) {
    //compute the derivative of the energy dissipation function per unit undeformed
    //volume.
    //cout<<"flag 1 " << endl; 
    Set::VectorSpace::Hom eye(3,3);
    Identity(eye);
    double J = Jacobian(F);
    double Jpalpha = Jacobian(Fpalpha);
    Set::VectorSpace::Hom Finv = Inverse(F);
    Set::VectorSpace::Hom dp = delepsp*M/Delt;
    //cout<<"flag 2 " << endl; 
    Set::VectorSpace::Hom FinvMF = Inverse(F)*M*F;
    Set::VectorSpace::Hom Fp =Fpalpha*(Inverse(eye - delepsp*FinvMF));
    Set::VectorSpace::Hom Fpinv = Inverse(Fp);
    Set::VectorSpace::Hom Fe = F*(Fpinv);
    double Je = Jacobian(Fe);
    Set::VectorSpace::Hom Ce = Adjoint(Fe)*Fe;
    double I1 = Trace(Ce)/pow(Je,(2.0/3.0));
    double Jp = Jacobian(Fp);
    //cout<<"flag 3 " << endl; 
    Set::VectorSpace::Hom Fpdot=(Fp - Fpalpha)/Delt;    
    double dJpcdJp = 0.0;
    if (Jp/J*computedWp(Jp)<=pc_min) {
        dJpcdJp = (computedWp(Jp) + Jp*computeddWp(Jp));
    }
    else {
        dJpcdJp =0.0;
    }
    //cout<<"flag 4 " << endl; 
    double pc = min(Jp/J*computedWp(Jp),pc_min);
    
    //cout<<"flag 5 " << endl; 
    Set::VectorSpace::Hom DweDCe = computedWe(Ce,phase);
    //cout<<"flag 6 " << endl; 
    double dWpdJp = computedWp(Jp);
    //cout<<"flag 7 " << endl; 
    Set::VectorSpace::Hom g0 =  -2.0* Ce*DweDCe*Adjoint(Fpinv) + Jp*dWpdJp*Adjoint(Fpinv);
    //cout<<"flag 8 " << endl; 
    Set::VectorSpace::Hom dFpddelepsp = Fp*(FinvMF)*Inverse(Fpalpha)*Fp;
    //cout<<"flag 9 " << endl; 
    double dphistarddelepsp= J*computetildephistar(M,F,Fp,Delt,pc_min,alpha,pt)/Delt + computedtildephistardpc(dp,F,Fp,Delt,pc,alpha,pt)*dJpcdJp*Jp*Trace(Adjoint(Fpinv)*dFpddelepsp);
    double dgddelepsp = Trace(g0*dFpddelepsp) +Delt*dphistarddelepsp;

    //cout<<"flag 10 " << endl; 
    return dgddelepsp;
}
 
double computedelepsp (const Set::VectorSpace::Hom &M, const Set::VectorSpace::Hom &F, const Set::VectorSpace::Hom &Fpalpha,double Delt,double pc_min,double alpha,int phase,double pt  ) {
  
    double delepsp = 0.01;
    double lambda = 0.01;
    double tol =0.000001;
    int count =0;
    double error0 = 100.0;
    int  countmax = 1000;
    double delepspold = 1.0;
    double delepspnew = 10.0;
    double dgold = 10.0;
    double dg = computedgddelepsp(delepsp,M,F,Fpalpha,Delt,pc_min,alpha,phase,pt);
    while ((error0 >tol) && (count <countmax)) { 
        delepspnew = delepsp -lambda*dg;
        delepspold = delepsp;
        //error0 = fabs(delepspnew - delepsp)/fabs(delepsp);
        delepsp = delepspnew;
        dgold = dg;
        dg = computedgddelepsp(delepsp,M,F,Fpalpha,Delt,pc_min,alpha,phase,pt);
        lambda = (delepsp-delepspold)/(dg-dgold);
        error0 = fabs(dgold);
        count = count +1;
        
        if (count ==countmax) { 
            //cout <<"The count is too large!!!! Exiting iteration" << endl;
            //cout << "Update of delepsp took too long to converge." << endl;
            //cout << "The error is:  "<< error0<< endl;
            //cout << "lambda is: "<<lambda<<endl;
            delepsp = 0.0;
            //exit (EXIT_FAILURE); 
        }        
    }
    return delepsp;
}
/* 
double alt_computedelepsp (const Set::VectorSpace::Hom &M, const Set::VectorSpace::Hom &F, const Set::VectorSpace::Hom &Fpalpha,double Delt,double pc_min,double alpha,int phase,double pt  ) {
  
    double delepsp = 0.01;
    double lambda = 0.1;
    double tol =0.000001;
    int count =0;
    double error0 = 100.0;
    int  countmax = 1000;
    double delepspnew = 10.0;
    while ((error0 >tol) && (count <countmax)) { 
        //cout << "In the while" <<endl;
        delepspnew = delepsp -lambda*computedgddelepsp(delepsp,M,F,Fpalpha,Delt,pc_min,alpha,phase,pt);
       // cout << count << endl;    
        if (delepspnew<0.0){
            lambda = lambda/2.0;
            //cout <<"This is dgdelepsp: "<< computedgddelepsp(delepsp,M,F,Fpalpha,Delt,pc_min,alpha,phase)<< endl;    
        }
        else   {
            error0 = fabs(delepspnew - delepsp)/fabs(delepsp);
            delepsp = delepspnew;
        }
        count = count +1;
        if (count ==countmax) { 
            //cout <<"The count is too large!!!! Exiting iteration" << endl;
            //cout << "Update of delepsp took too long to converge." << endl;
            //cout << "The error is:  "<< error0<< endl;
            //cout << "lambda is: "<<lambda<<endl;
            delepsp = 0.0;
            //exit (EXIT_FAILURE); 
        }        
    }
    return delepsp;
}
*/
Set::VectorSpace::Hom updateyepre(const Set::VectorSpace::Hom &yepre, double pc, double alpha,double pt) {
    // Find closest point within the elastic region.    
    Set::VectorSpace::Hom eye(3,3);
    Identity(eye);
    double tol =0.000001;
    double lambda = 0.1;
    Set::VectorSpace::Hom yu = yepre;
    Set::VectorSpace::Hom yup = yepre;
    Set::VectorSpace::Hom ytilde = -1.0/2.0*(pc-pt)*eye;
    double qc = computeqc(pc);
    alpha = 2.0*qc/(pc-pt); 
    double error0 = 100.0;
    double pbar = Trace(yepre)/3.0;
    Set::VectorSpace::Hom dev = yepre - pbar*eye;
    double q = sqrt(3.0/2.0*Trace(dev*dev));
        
    double elastic = pow(((2.0*pbar + pc-pt)/(pc-pt)),2.0) + pow((q/qc),2.0);
    if (elastic<1.0) {
        cout <<"In elastic region. This routine should not be called!" << endl;
    }    
    int count = 0;
    int maxcount = 1000;
       
         
    while ((error0 >tol) &&(count<maxcount)) {
        yup = yu*(1.0-lambda) + lambda*ytilde;
        pbar = Trace(yup)/3.0;
        dev = yup - pbar*eye;
        q = sqrt(3.0/2.0*Trace(dev*dev));
        
        //cout << count << endl;    
        elastic = pow(((2.0*pbar + pc-pt)/(pc-pt)),2.0) + pow((q/qc),2.0);
        if (elastic<1.0 ){
            lambda = lambda/2.0;
        }
        else {
            yu = yup;
            error0 = fabs(elastic-1.0);
        }
        count = count+1;
        
        if (count==maxcount) {
            cout<< "Update of yepre took too long to converge."<<endl;
            exit (EXIT_FAILURE); 
        }
    }
    
    //yepre = yup;

    return yup; 
}


// Special Update Fp at the phase transition routine

Set::VectorSpace::Hom updateFpIncremental(const Set::VectorSpace::Hom &F0, double Delt, double pc_min, double alpha,double pt) {
    //Set::VectorSpace::Hom  testit Inverse(F0);
    Set::VectorSpace::Hom eye(3,3);
    Identity(eye);
    Set::VectorSpace::Hom Fpalpha(3,3) ;
    Identity(Fpalpha);
    Set::VectorSpace::Hom Fp = Fpalpha;
    double Jp = Jacobian(Fp);
    double J = Jacobian(F0);
    int phase = 1;
    double Jpalpha = Jacobian(Fpalpha);

    double pc_hardening = min(Jp/J*computedWp(Jp), pc_min);// pc_min;
    //pc_store=zeros(length(lambda),1);

    //tau = zeros(3,3,length(lambda));
    //elastic_store = zeros(length(lambda),1);
    //F_store = zeros(3,3,length(lambda));
    //Fp_store = zeros(3,3,length(lambda));
    //Fe_store = zeros(3,3,length(lambda));
    double delepsp = 0.1;
    Set::VectorSpace::Hom M = eye;
    //    int mn = 1000; //length(lambda);
    Set::VectorSpace::Hom Ffinal = F0; //[lambda(mn) 0 0 ; 0,lambda(mn),0;0,0,lambda(mn)]*[1 gamma(mn) 0 ; 0,1,0;0,0,1];
    Set::VectorSpace::Hom F= F0; //[lambda(mn) 0 0 ; 0,lambda(mn),0;0,0,lambda(mn)]*[1 gamma(mn) 0 ; 0,1,0;0,0,1];
    double JFinal = Jacobian(Ffinal);
    Set::VectorSpace::Hom Fdev = Ffinal /pow((JFinal),(1.0/3.0));
    int numlam = 1000;
    double delJ = (1.0-pow(JFinal,(1.0/3.0)))/(numlam*1.0 -1.0);
    int numgam = 1000; 
    Set::VectorSpace::Vector lam(numlam);
    Set::VectorSpace::Vector gam(numgam+1);
    for  (int jj = 0; jj<numlam;jj++) {
       lam[jj] = 1.0-delJ*jj;
    }
    for  (int jj = 0; jj<numgam+1;jj++) {
       gam[jj] = jj*1.0/numgam;  //100.0;//
    }

    //interp = 0:1/(length(gam)-1):1
    Set::VectorSpace::Vector lambda1(numlam+numgam+1);// = zeros(length(lam)+length(gam),1)
    Set::VectorSpace::Vector gamma1(numlam+numgam+1);// = zeros(length(lam)+length(gam),1)
    //Fdev_store= zeros(3,3,length(lam)+length(gam));
    for (int jj = 0; jj<numlam; jj++) {
        lambda1[jj] = lam[jj];
        gamma1[jj] = 0.0;
    } 
    for (int jj = 0; jj<numgam+1; jj++) {
        lambda1[jj+numlam] = lam[numlam-1];
        gamma1[jj+numlam] = gam[jj];
    } 
    // Create variables 
    Set::VectorSpace::Hom Ce(3,3);
    Set::VectorSpace::Hom Fe(3,3);
    Set::VectorSpace::Hom dWedCe(3,3);
    double Je;
    double I1;
    Set::VectorSpace::Hom yepre(3,3);
    double pbar;
    Set::VectorSpace::Hom dev(3,3);
    //Set::VectorSpace::Hom M(3,3);
    Set::VectorSpace::Hom dp(3,3);
    double q;
    double pc;
    double qc;
    //double J
    double elastic;
    //double delepsp;
    Set::VectorSpace::Hom FinvMF(3,3);
    for (int i = 0; i < numlam + numgam+1; i++) {
        //cout << "This is lamda1[i]: "<<lambda1[i] <<endl;
        //cout << "This is gamma1[i]: "<<gamma1[i] <<endl;
        //cout << "This is [i]: "<<i <<endl;
        
        double interp = gamma1[i];
        F = lambda1[i]*((1.0-interp)*eye+interp*Fdev);
        //for (int k=0; k<3;k++) { 
	//    for (int l=0; l<3;l++) { 
        //	cout << "This is F[k][l]: "<<F[k][l] <<endl;
	//    }
        //}
        Fe = F*Inverse(Fp);
        Je = Jacobian(Fe);
        J = Jacobian(F);
        Ce = Adjoint(Fe)*Fe;
        I1 = Trace(Ce)/pow(Je,(2.0/3.0));
        dWedCe = computedWe(Ce,phase);
        yepre = 1.0/(J)*2.0*Fe* dWedCe*Adjoint(Fe) - pc_hardening*eye;
        pbar = Trace(yepre)/3.0;
        dev = yepre - pbar*eye;
        q = sqrt(3.0/2.0*Trace(dev*dev));
        pc = pc_hardening;
        qc = computeqc(pc);
        alpha = 2.0*qc/(pc_hardening-pt); 
        elastic = pow(((2.0*pbar + pc - pt)/(pc-pt)),2.0) + pow((q/qc),2.0);
        //elastic_store(i) = elastic;
        if (elastic <=1.0) {
           //` cout << "pure elastic"<< endl;
        }
        else {
          
            yepre = updateyepre(yepre,pc,alpha,pt);
            pbar = Trace(yepre)/3.0;
            dev = yepre - pbar*eye;
            M = (1.0/3.0*pow(alpha,2.0)*(2.0*pbar + pc-pt)*eye + 3.0*dev);
            M = M/Trace(M*M);
            delepsp = computedelepsp(M,F,Fpalpha,Delt,pc_min,alpha,phase,pt);
            dp = delepsp/Delt*M;
  
            Set::VectorSpace::Hom FinvMF =Inverse(F)*M*F;
            Fp = Fpalpha*(Inverse(eye - delepsp*FinvMF));
            Fe = F*Inverse(Fp);
            Je = Jacobian(Fe);
	    Jp = Jacobian(Fp);
	    
	    I1 = Trace(Ce)/pow(Je,(2.0/3.0));
	    dWedCe = computedWe(Ce,phase);
	    //Set::VectorSpace::Hom Fpdot = Inverse(Fe) *dp*Fe*Fp; 


            //yepre = 1/(Je*det(Fp))*2*Fe* dWedCe*Fe' - pc_hardening*eye(3);
            //pbar = trace(yepre)/3;
            //dev = yepre - pbar*eye(3);
            //q = sqrt(3/2*trace(dev'*dev));
            //pc = pc_hardening;
            //qc = alpha/2*pc;
            //elastic = ((2*pbar + pc)/pc)^2 + (q/qc)^2;



        }
        Jp = Jacobian(Fp);
  
        //Jpalpha = det(Fpalpha);
        pc_hardening = min(Jp/J*computedWp(Jp), pc_min);
        

        //%compute the Kirchoff stress tensor
        //Fe = F*(Fp^-1);
        //Je = det(Fe);
        //I1 = trace(Fe'*Fe)/(Je^(2/3));
        //dWedCe = computeDweDCe(Fe,I1,phase);
        //tau(:,:,i) = 1/(Je*det(Fp))*2*Fe* dWedCe*Fe';

        //%analyze the yield condition
        //yepre = 1/(Je*det(Fp))*2*Fe* dWedCe*Fe' - pc_hardening*eye(3);
        //pbar = trace(yepre)/3;
        //dev = yepre - pbar*eye(3);
        //q = sqrt(3/2*trace(dev'*dev));
        //pc = pc_hardening;
        //qc = alpha/2*pc;
        //elastic = ((2*pbar + pc)/pc)^2 + (q/qc)^2;
        //%elastic_store(i) = elastic;

        //%q_store(i) = q;
        //%pbar_store(i) = pbar;
        //%F_store(:,:,i) = F;
        //%Fp_store(:,:,i) = Fp;
        //%Fe_store(:,:,i) = Fe;
        //elastic_out = elastic
        Fpalpha = Fp;
    }

    return Fp;
}


}

}

#endif // !defined(MATERIAL_LAW_ROUTINES_COMPRESSIBLE_H__INCLUDED_
