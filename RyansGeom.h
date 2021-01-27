#ifndef RYANSGEOM_H
#define RYANSGEOM_H


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// RyansGeom                                                            //
//                                                                      //
// Encapsulate geometry and coordinate related thingies                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include "TMath.h"
#include <iostream>

namespace crust2 {
    const int N_LON=180; // number of bins in longitude for crust 2.0
    const int N_LAT=90;  // number of bins in latitude
    const double MAX_THETA=180; // maximum value of theta in degrees in polar coordinates
}

namespace RyansGeom {

    const double GEOID_MAX=6.378137E6; // parameters of geoid model
    const double GEOID_MIN=6.356752E6;


    
    inline Double_t getTheta(Int_t lat)  { 
	return (((Double_t)lat+0.5)/(Double_t)crust2::N_LAT*crust2::MAX_THETA)*TMath::DegToRad(); 
    }
    inline Double_t getPhi(Int_t lon) { 
	return (double)(-1*((double)lon+0.5)+(double)crust2::N_LON)*2*TMath::Pi()/(double)crust2::N_LON-TMath::Pi()/2;
    }
    inline Int_t getLat(Double_t theta) { 
	return (int)((theta*TMath::RadToDeg())/2.); 
    }

    inline Int_t getLon(Double_t phi){ 
	double phi_deg = phi*TMath::RadToDeg();
	if (phi_deg>270)
	    phi_deg-=360;	
	return (int)((360.*0.75-phi_deg)*180./360.);
    }

    inline Double_t getPhi(Double_t p[3]){    
    // returns phi between 0 and 2pi.
	double pt=0;
	double phi=0;
	pt=sqrt(p[0]*p[0]+p[1]*p[1]);    
	if (pt==0)
	    return 0.;
	else if (pt!=0) {
	    if (p[1]/pt>1 || p[1]/pt<-1) {
		std::cerr << "Error in getPhi. \n";
		return 0;
	    }
	    phi=asin(p[1]/pt);
	}
	if (p[1]<0. && p[0]>0) phi += 2*TMath::Pi();
	else if (phi>0 && p[0]<0.) phi = TMath::Pi() - phi;
	else if (phi<0 && p[0]<0.) phi = -(TMath::Pi()+phi)+2*TMath::Pi();
	return phi;
    }

    inline Double_t getTheta(Double_t p[3]) {
	double pz,pt;
	double tantheta1=0;
	double theta=0;
	
	pz=p[2];
	pt=sqrt(p[0]*p[0]+p[1]*p[1]);
	tantheta1=pt/pz;
	theta=atan(tantheta1);
	
	if (pz<0)
	    theta += TMath::Pi();  
	return theta;  
    }

    inline void getLonLat(Double_t p[3],Int_t& lon,Int_t& lat) {
	lon=RyansGeom::getLon(RyansGeom::getPhi(p));
	lat=RyansGeom::getLat(RyansGeom::getTheta(p));
    }


    inline Double_t getGeoid(Double_t p[3]) {
	return GEOID_MIN*GEOID_MAX/sqrt(pow(GEOID_MIN,2)-(pow(GEOID_MIN,2)-pow(GEOID_MAX,2))*pow(cos(RyansGeom::getTheta(p)),2));    
    }

    inline Double_t getGeoid(Int_t lat) {
	return GEOID_MIN*GEOID_MAX/sqrt(pow(GEOID_MIN,2)-(pow(GEOID_MIN,2)-pow(GEOID_MAX,2))*pow(cos(RyansGeom::getTheta(lat)),2));    
    }
}


#endif //RYANSGEOM_H
