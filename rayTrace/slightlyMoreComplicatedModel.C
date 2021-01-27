#include <iostream>

#include "TCanvas.h"
#include "TF2.h"
#include "TF12.h"
#include "TMarker.h"

const double R_EARTH=6.378140E6;        // radius of Earth in m at bulge
static const double c_SI = 2.99792458e8;


const int NTHETA_ROUGH=100; // bins in theta 
const int NPHI_ROUGH=100;


// parameters of the earth model
const int NLON=180; // number of bins in longitude for crust 2.0
const int NLAT=90;  // number of bins in latitude
const int NPHI=180; // bins in longitude for visible ice in horizon
const double MAXTHETA=180; // maximum value of theta in degrees in polar coordinates
double thetastep; // how big do you step in theta-> always 2deg with Crust 2.0
double phistep; // how big do you step in phi->always 2deg in Crust 2.0
const double COASTLINE=30; // if the rf leaves from beyond this "coastline" (in degrees of latitude relative to south pole) it's not in antarctica.  Real coastline is always closer than this. 
const int ILAT_MAX=(int)((COASTLINE/MAXTHETA)*(double)NLAT); // corresponding latitude bin to "coastline"
double surfacer[NLON][NLAT]; // elevation at the surface (top of ice) compared to geoid (in meters)
double icer[NLON][NLAT];  // elevation at the *bottom* of ice layer (in meters)
double waterr[NLON][NLAT]; // elevation at the bottom of water layer (in meters)
double softsedr[NLON][NLAT]; // elevation at the bottom of soft set layer (in meters)
double hardsedr[NLON][NLAT]; // elev at bottom of hard sed layer (in meters)
double uppercrustr[NLON][NLAT]; // elev at bottom of upper crust layer (in meters)
double middlecrustr[NLON][NLAT]; // elev at bottom of middle crust layer (in meters)
double lowercrustr[NLON][NLAT]; // elev at bottom of lower crust layer (in meters)
double geoid[NLAT];     // realistic shape of earth-radius at each latitude (in meters)
double MIN_ALTITUDE_CRUST; // maximum depth of crust- determines when to start stepping
double MAX_VOL; // maximum volume of ice in a bin in Crust 2.0 - not used
double elevationarray[NLON][NLAT]; // If no water, measures the elevation (relative to geoid, in meters) of the top of the ice or rock (i.e., air interface).  If there is water, measures elevation to bottom of water. (There may or may not be ice on top of the water.) 
double waterthkarray[NLON][NLAT];  // thickness of water layer (in km)
double icethkarray[NLON][NLAT]; // thickness of ice layer (in km)
double softsedthkarray[NLON][NLAT]; // thickness of soft sed layer (in km)
double hardsedthkarray[NLON][NLAT]; // thickness of hard sed layer (in km)
double uppercrustthkarray[NLON][NLAT]; // thickness of upper crust layer (in km)
double middlecrustthkarray[NLON][NLAT]; // thickness of middle crust layer (in km)
double lowercrustthkarray[NLON][NLAT]; // thickness of lower crust layer (in km)
double crustthkarray[NLON][NLAT]; // total thickness of crust (in km)
double waterdensityarray[NLON][NLAT]; // density of water layer bin by bin
double icedensityarray[NLON][NLAT]; // density of ice layer bin by bin
double softseddensityarray[NLON][NLAT]; // density of soft sed layer
double hardseddensityarray[NLON][NLAT]; // density of hard sed layer
double uppercrustdensityarray[NLON][NLAT]; // density of upper crust layer
double middlecrustdensityarray[NLON][NLAT]; // density of middle crust layer
double lowercrustdensityarray[NLON][NLAT]; // density of lower crust layer
double volume=0; // sums the volume of ice over antarctica- calculated once
double area[NLAT]; // area of a bin at a given latitude- calculated once
double average_iceth=0; // average ice thickness over the continent-calculated once
double radii[3] = {1.2e13,(R_EARTH-4.0E4)*(R_EARTH-4.0E4),R_EARTH*R_EARTH}; // average radii of boundaries between earth layers
double densities[3]={14000.,3400.,2900.}; // average density of each earth layer
const int NBNPOSITIONS_MAX=26000;
double volume_inhorizon[NBNPOSITIONS_MAX]; // volume of ice within horizon for each balloon phi position
double volume_inhorizon_average=0; // average volume of ice seen by balloon
//for plotting
int ilon1,ilon2,ilon3;
int ilat1,ilat2,ilat3;
int difflon1,difflon2;
int difflat1,difflat2;



double costheta_int=0; // costheta where the interaction occured wrt earth
double ilat_int=0; // index in latitude where the interaction occured (Crust 2.0 binning)




Double_t simple3DTimeOfFlight(Double_t *x, Double_t *par);
Double_t getMinimumPoint(TF2 *func,Double_t *minXptr, Double_t *minYptr, Double_t xMin=0, Double_t xMax=0, Double_t yMin=0, Double_t yMax=0);
void InitWorldModel();
void ReadCrust(string filename );
void GetGeoid();
double GetPhi(double*);
double dGetPhi(double*);
double GetTheta(double*);
double dGetTheta(double*);
double dGetTheta(int itheta);
double dGetPhi(int ilon);
void GetPhiCostheta(int iphi,int itheta,double& phi,double& costheta);
void GetIPhiITheta(double*,int& iphi,int& itheta);
void GetILonILat(double*,int& ilon,int& ilat);
int GetILat(double);
int GetILon(double);
void GetIPhiIThetaRough(double*,int& iphi,int& itheta);

double getTopOfIce(double point[3]);


void slightlyMoreComplicatedModel() {
//     Double_t nuX=-2000;
//     Double_t nuY=6000000;
//     Double_t ballX=20000;
//     Double_t ballY=6450000;
//     Double_t nIce=1.92;
//     Double_t par[5]={nuX,nuY,ballX,ballY,nIce};
//     TF1 *simple = new TF1("simple",simpleTimeOfFlight,nuX,ballX,5);
//     simple->SetParameters(par);
//     simple->Draw();
    Double_t nuX=1.18094e+06;
    Double_t nuY=-760496;
    Double_t nuZ=6.19992e+06;    
    Double_t ballX=-1.2747e+06;
    Double_t ballY=769556;
    Double_t ballZ=6.2212e+06;
    Double_t nIce=1.76;
    Double_t par[7]={nuX,nuY,nuZ,ballX,ballY,ballZ,nIce};

    TCanvas *can = new TCanvas("can","can");
    TF2 *simple3d = new TF2("simple3d",simple3DTimeOfFlight,-2e6,2e6,-2e6,2e6,7);
    simple3d->SetParameters(par);
    simple3d->Draw("col");

//    simple3d->SetNpx(100);
//    simple3d->SetNpy(100);
    Double_t minimumX,minimumY;
    Double_t minTime=getMinimumPoint(simple3d,&minimumX,&minimumY);
    cout << minimumX << "\t" << minimumY << "\t" << minTime << endl;

    TMarker *marker = new TMarker();
    marker->SetMarkerColor(kBlack);
    marker->SetMarkerSize(2);
    marker->SetMarkerStyle(29);
    marker->DrawMarker(nuX,nuY);    
    marker->SetMarkerStyle(23);
    marker->DrawMarker(ballX,ballY);
    marker->SetMarkerStyle(22);
    marker->SetMarkerColor(kRed);
    marker->DrawMarker(minimumX,minimumY);

    
   //  TCanvas *can2 = new TCanvas("can2","can2");
//     TF2 *simple3dZoom = new TF2("simple3dZoom",simple3DTimeOfFlight,minimumX-100,minimumX+100,minimumY-100,minimumY+100,7);
//     simple3dZoom->SetParameters(par);
// //    simple3dZoom->SetNpx(100);
// //    simple3dZoom->SetNpy(100);
//     simple3dZoom->Draw("col");


//     minTime=getMinimumPoint(simple3dZoom,&minimumX,&minimumY);
//     cout << minimumX << "\t" << minimumY << "\t" << minTime << endl;
    

}



Double_t simpleTimeOfFlight(Double_t *x, Double_t *par) {
    Double_t nuX=par[0];
    Double_t nuY=par[1];
    Double_t ballX=par[2];
    Double_t ballY=par[3];
    Double_t nIce=par[4];

    Double_t surfX=x[0];    
    Double_t surfY=sqrt(R_EARTH*R_EARTH-surfX*surfX);
    
    Double_t iceTime=nIce*sqrt(pow(surfX-nuX,2)+pow(surfY-nuY,2));
    Double_t airTime=sqrt(pow(surfX-ballX,2)+pow(surfY-ballY,2));
    
    return (iceTime+airTime)/c_SI;
}


Double_t simple3DTimeOfFlight(Double_t *x, Double_t *par) {
    static int gotCrust=0;
    if(!gotCrust) {
	InitWorldModel();
	gotCrust=1;
    }
    Double_t nuX=par[0];
    Double_t nuY=par[1];
    Double_t nuZ=par[2];
    Double_t ballX=par[3];
    Double_t ballY=par[4];
    Double_t ballZ=par[5];
    Double_t nIce=par[6];

    Double_t surfX=x[0];
    Double_t surfY=x[1];
    Double_t surfPoint[3]={surfX,surfY,R_EARTH};
    Double_t radius=getTopOfIce(surfPoint);
//    Double_t radius=R_EARTH;
    Double_t surfZ=sqrt(radius*radius-surfX*surfX-surfY*surfY);

//    cout << "Surface:\t" << R_EARTH << "\t" << getTopOfIce(surfPoint) << endl;
    


    Double_t iceTime=nIce*sqrt(pow(surfX-nuX,2)+pow(surfY-nuY,2)+pow(surfZ-nuZ,2));
    Double_t airTime=sqrt(pow(surfX-ballX,2)+pow(surfY-ballY,2)+pow(surfZ-ballZ,2));
    
    return (iceTime+airTime)/c_SI;
}



Double_t getMinimumPoint(TF2 *func,Double_t *minXptr, Double_t *minYptr, Double_t xMin, Double_t xMax, Double_t yMin, Double_t yMax) {

    if(xMin>=xMax) {
	xMin=func->GetXmin();
	xMax=func->GetXmax();
    }
    if(yMin>=yMax) {
	yMin=func->GetYmin();
	yMax=func->GetYmax();
    }
   
//    cout << "x:\t" << xMin << "\t" << xMax << endl;
//    cout << "y:\t" << yMin << "\t" << yMax << endl;
	         
   Double_t x,y,z;
   Double_t dx=(xMax-xMin)/func->GetNpx();
   Double_t dy=(yMax-yMin)/func->GetNpy();
   
   Double_t minX=xMin;
   Double_t minY=yMin;
   Double_t minZ=func->Eval(minX+dx,minY+dy);
   for(int ix=0;ix<func->GetNpx();ix++) {
       for(int iy=0;iy<func->GetNpy();iy++) {
	   x=xMin+(ix+0.5)*dx;
	   y=yMin+(iy+0.5)*dy;
	   z=func->Eval(x,y);
	   if(z<minZ) {
	       minX=x;
	       minY=y;
	       minZ=z;
	   }
       }
   }
   Double_t testDelta=sqrt(dx*dx+dy*dy);
   Double_t otherHalf=sqrt(pow(func->GetXmax()-func->GetXmin(),2)+
			   pow(func->GetYmax()-func->GetYmin(),2));
//   cout << testDelta << "\t" << otherHalf << endl;


   if(testDelta<1e-9*otherHalf) {
       *minXptr=minX;
       *minYptr=minY;
       return minZ;
   }
   else return getMinimumPoint(func,minXptr,minYptr,
			       TMath::Max(xMin,minX-dx),
			       TMath::Min(xMax,minX+dx),
			       TMath::Max(yMin,minY-dy),
			       TMath::Min(yMax,minY+dy));
			    


}


double getTopOfIce(double point[3]) {

    int ilon,ilat;
    GetILonILat(point,ilon,ilat);
    Double_t geo=geoid[ilat];
    Double_t surf=surfacer[ilon][ilat];
    return geo+surf;

}


void InitWorldModel() {
    GetGeoid();
    ReadCrust(string("crust2.dat"));
}


void GetGeoid() {

  // see monte carlo note #17

  double GEOID_MAX=6.378137E6; // parameters of geoid model
  double GEOID_MIN=6.356752E6;

  for (int i=0;i<NLAT;i++) {
    geoid[i]=GEOID_MIN*GEOID_MAX/sqrt(pow(GEOID_MIN,2)-(pow(GEOID_MIN,2)-pow(GEOID_MAX,2))*pow(cos(dGetTheta(i)),2));
  }
}


void ReadCrust(string filename ) {

  // reads in altitudes of 7 layers of crust, ice and water
  // puts data in arrays
  
  fstream infile(filename.c_str(),ios::in);

  string thisline; // for reading in file
  string slon; //longitude as a string
  string slat; // latitude as a string
  string selev; // elevation (km relative to geoid)
  string sdepth; // depth (km)
  string sdensity; // density (g/cm^3)
  double dlon,dlat; // longitude, latitude as double
  int endindex; // index along thisline for parsing
  int beginindex; // same

  int indexlon=0; // 180 bins in longitude
  int indexlat=0; // 90 bins in latitude
   
  string layertype; // water, ice, etc.

  while(!infile.eof()) {
    getline(infile,thisline,'\n'); 

    int loc=thisline.find("type, latitude, longitude,"); 
    
    if (loc!=(int)(string::npos)) {      
     
      beginindex=thisline.find_first_not_of(" ",57);
      
      endindex=thisline.find_first_of(" ",61);
      
      slat=thisline.substr(beginindex,endindex-beginindex);
      dlat=(double)atof(slat.c_str());

      beginindex=thisline.find_first_not_of(" ",68);
      endindex=thisline.find_first_of(" ",72);

      slon=thisline.substr(beginindex,endindex-beginindex);
      dlon=(double)atof(slon.c_str());
      
      indexlon=(int)((dlon+180)/2);
      indexlat=(int)((90+dlat)/2);

      beginindex=thisline.find_first_not_of(" ",78);
      endindex=thisline.find_first_of(" ",83);

      selev=thisline.substr(beginindex,endindex-beginindex);
      elevationarray[indexlon][indexlat]=(double)atof(selev.c_str());

      if (indexlon==2 && indexlat==7)
	cout << "dlon,dlat,selev,elevationarry is " << dlon << " " << dlat << " " << selev << " " << elevationarray[indexlon][indexlat] << "\n";

    }
    for (int i=0;i<4;i++) {
      getline(infile,thisline,'\n');
    }
       
    for (int i=0;i<7;i++) {
      getline(infile,thisline,'\n');
      
      //cout << "thisline is " << thisline << "\n";
      endindex=thisline.length()-1;
      beginindex=thisline.find_last_of("0123456789",1000);
      layertype=thisline.substr(beginindex+3,endindex-beginindex);

     
      beginindex=thisline.find_first_not_of(" ",0);
      endindex=thisline.find_first_of(" ",beginindex);
     
      sdepth=thisline.substr(beginindex,endindex-beginindex-1);
      

      // fills arrays of thicknesses of each layer
      if (layertype.substr(0,5)=="water") 
	waterthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str()); 
      if (layertype.substr(0,3)=="ice") 
	icethkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,8)=="soft sed") 
	softsedthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,8)=="hard sed") 
	hardsedthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,11)=="upper crust") 
	uppercrustthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,12)=="middle crust") 
	middlecrustthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,11)=="lower crust") 
	lowercrustthkarray[indexlon][indexlat]=(double)atof(sdepth.c_str());
      
      //cout << "indexlon,indexlat are " << indexlon << " " << indexlat << "\n";
      
      // region where Ross Ice Shelf was not accounted for in Crust 2.0
      // add it in by hand
      if (indexlat==5 && (indexlon<=5 || indexlon>=176)) // Ross Ice Shelf
	icethkarray[indexlon][indexlat]=0.5;

      beginindex=thisline.find_first_not_of(" ",endindex);
      endindex=thisline.find_first_of(" ",beginindex);
      

      beginindex=thisline.find_first_not_of(" ",endindex);
      endindex=thisline.find_first_of(" ",beginindex);
      
      beginindex=thisline.find_first_not_of(" ",endindex);
      endindex=thisline.find_first_of(" ",beginindex);

     
      sdensity=thisline.substr(beginindex,endindex-beginindex);

      double ddensity=(double)atof(sdensity.c_str());
      

      // fills arrays of densities of each layer
      if (layertype.substr(0,5)=="water") 
	waterdensityarray[indexlon][indexlat]=ddensity; 
      if (layertype.substr(0,3)=="ice") 
	icedensityarray[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,8)=="soft sed") 
	softseddensityarray[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,8)=="hard sed") 
	hardseddensityarray[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,11)=="upper crust") 
	uppercrustdensityarray[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,12)=="middle crust")
	middlecrustdensityarray[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,11)=="lower crust") 
	lowercrustdensityarray[indexlon][indexlat]=ddensity;



      //cout << "sdensity is " << sdensity << "\n";
    }

    // adds up total thickness of crust
      crustthkarray[indexlon][indexlat]=softsedthkarray[indexlon][indexlat]+
	hardsedthkarray[indexlon][indexlat]+
	uppercrustthkarray[indexlon][indexlat]+
	middlecrustthkarray[indexlon][indexlat]+
	lowercrustthkarray[indexlon][indexlat];
      
      if (indexlon==179 && indexlat==0)
	  break;
  }  // done reading file
  
  for (int i=0;i<NLON;i++) {
      for (int j=0;j<NLAT;j++) {
	  
	  if (j==3 && (i==179 || i==0 || i==1 || i==2))
	      cout << "ilon, ilat, elevation, waterth, surfacer are " << i << " " << j << " " << elevationarray[i][j] << " " << waterthkarray[i][j] << "\n";
	 
	  
	  // array of elevation at each longitude, latitude, including water and ice
      //surfacer[i][j]=elevationarray[i][j]+waterthkarray[i][j]*1000;
      //surfacer[i][j]=elevationarray[i][j]+waterthkarray[i][j]*1000+icethkarray[i][j]*1000;
      if (waterthkarray[i][j] != 0) {
	surfacer[i][j]=elevationarray[i][j]+waterthkarray[i][j]*1000+icethkarray[i][j]*1000;
	//surfacer[i][j]=elevationarray[i][j]+waterthkarray[i][j]*1000;
	  }
      else
	surfacer[i][j]=elevationarray[i][j];
      if (fabs(surfacer[i][j])<1.E-10) {
	surfacer[i][j] = 0;
	}

      if((j>=4 && j<=6) && (i <=8 ||i >= 172))
	cout<<"ilon, ilat, icethk, waterthk, surfacer : "<<i<<" , "<<j<<" , "<<icethkarray[i][j]<<" , "<<waterthkarray[i][j]<<" , "<<surfacer[i][j]<<endl;

      // reminder- waterr is elevation at *bottom* of water layer, etc. 
      // in units of m
      waterr[i][j]=surfacer[i][j]-(icethkarray[i][j]+waterthkarray[i][j])*1000;
      if ((double)fabs(waterr[i][j])<1.E-10)
	waterr[i][j]=0;
      icer[i][j]=waterr[i][j]+
	 waterthkarray[i][j]*1000;
      softsedr[i][j]=waterr[i][j]-
	softsedthkarray[i][j]*1000;
      hardsedr[i][j]=waterr[i][j]-
	(softsedthkarray[i][j]+
	 hardsedthkarray[i][j])*1000;
      uppercrustr[i][j]=waterr[i][j]-
	(softsedthkarray[i][j]+
	 hardsedthkarray[i][j]+
	 uppercrustthkarray[i][j])*1000;
      middlecrustr[i][j]=waterr[i][j]-
	(softsedthkarray[i][j]+
	 hardsedthkarray[i][j]+
	 uppercrustthkarray[i][j]+
	 middlecrustthkarray[i][j])*1000;
      lowercrustr[i][j]=waterr[i][j]-
	(softsedthkarray[i][j]+
	 hardsedthkarray[i][j]+
	 uppercrustthkarray[i][j]+
	 middlecrustthkarray[i][j]+
	 lowercrustthkarray[i][j])*1000;


    }
  }  
}



double dSquare(double *p) {
  return p[0]*p[0]+p[1]*p[1]+p[2]*p[2];
}


double GetPhi(double p[3]) {      

  // returns phi between 0 and 2pi.
  double pt=0;
  double phi=0;
  pt=sqrt(p[0]*p[0]+p[1]*p[1]);

  if (pt==0)
    return 0.;
  else if (pt!=0) {
    if (p[1]/pt>1 || p[1]/pt<-1)
      {
	cout << "Error in GetPhi. \n";
	return 0;
      }
    phi=asin(p[1]/pt);
  }
  if (p[1]<0. && p[0]>0) phi += 2*TMath::Pi();
  else if (phi>0 && p[0]<0.) phi = TMath::Pi() - phi;
  else if (phi<0 && p[0]<0.) phi = -(TMath::Pi()+phi)+2*TMath::Pi();
  
  return phi;
}
double dGetPhi(double p[3]) {      

  // returns phi between 0 and 2pi.
  double pt=0;
  double phi=0;
  pt=sqrt(p[0]*p[0]+p[1]*p[1]);

  if (pt==0)
    return 0.;
  else if (pt!=0) {
    if (p[1]/pt>1 || p[1]/pt<-1)
      {
	cout << "Error in GetPhi. \n";
	return 0;
      }
    phi=asin(p[1]/pt);
  }
  if (p[1]<0. && p[0]>0) phi += 2*TMath::Pi();
  else if (phi>0 && p[0]<0.) phi = TMath::Pi() - phi;
  else if (phi<0 && p[0]<0.) phi = -(TMath::Pi()+phi)+2*TMath::Pi();
  else if (p[0]<0.) phi = TMath::Pi() - phi;

  return phi;
}
double GetTheta(double p[3]) {
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
double dGetPhi(int ilon) {
  //double phi=(double)(-1*ilon+NLON)*2*TMath::Pi()/(double)NLON-TMath::Pi()/2;
  double phi=(double)(-1*((double)ilon+0.5)+(double)NLON)*2*TMath::Pi()/(double)NLON-TMath::Pi()/2;
  //if (phi>2*TMath::Pi())
  //phi=phi-2*TMath::Pi();

  return phi;

}
double dGetTheta(int ilat) {
  
  //return ((double)ilat/(double)NLAT*MAXTHETA)*TMath::DegToRad();
  return (((double)ilat+0.5)/(double)NLAT*MAXTHETA)*TMath::DegToRad();

}
double dGetTheta(double p[3]) {
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


void GetPhiCostheta(int iphi,int itheta,double& phi,double& costheta) {
  
  costheta=cos(dGetTheta(itheta));
  phi=phistep*((double)iphi+0.5);


}
void GetIPhiITheta(double *p,int& iphi,int& itheta) {

  iphi=(int)(dGetPhi(p)/phistep);
  itheta=(int)(dGetTheta(p)/thetastep);
  //cout << "p is ";Print(p,3);
  //cout << "itheta is " << itheta << "\n";
}
int GetILat(double theta) {
  
  return (int)((theta*TMath::RadToDeg())/2.);

}
int GetILon(double phi) {
  double phi_deg = phi*TMath::RadToDeg();

  if (phi_deg>270)
    phi_deg=phi_deg-360;

  return (int)((360.*3./4.-phi_deg)*180./360.);
}
void GetILonILat(double *p,int& ilon,int& ilat) {


  
  double phi_deg=dGetPhi(p)*TMath::RadToDeg();
  //cout << "phi_deg is " << phi_deg << "\n";

  if (phi_deg>270)
    phi_deg=phi_deg-360;
  
  ilon=(int)((360.*3./4.-phi_deg)*180./360.);
  
  ilat=(int)((dGetTheta(p)*TMath::RadToDeg())/2.);


}
void GetIPhiIThetaRough(double *p,int& iphi,int& ilat) {

  ilat=(int)(dGetTheta(p)*TMath::RadToDeg())/2;

  double phi_deg=dGetPhi(p)*TMath::RadToDeg();

  if (phi_deg>270)
    phi_deg=phi_deg-360;
  
  iphi=(int)((360.*3./4.-phi_deg)*(double)NPHI_ROUGH/360.);  
    
}
