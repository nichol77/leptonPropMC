///////////////////////////////////////////////////////////////////////////////
/////   WorldModel.cxx
/////   Is a model of the world, strangely enough
///////////////////////////////////////////////////////////////////////////////
#include "WorldModel.h"

#include <iostream>
#include <fstream>

using namespace std;

ClassImp(WorldModel)



WorldModel::WorldModel(Int_t useBedmap) 
    :fUseBedmap(useBedmap),
     fBedMapBedElevation(0),
     fBedMapWaterDepth(0),
     fBedMapIceThickness(0)
{
    //Default constructor
    defineGeoid();
    loadCrustData();
    if(fUseBedmap) initBedMap();
}

WorldModel::~WorldModel() {
    //Default destructor
    if(fBedMapBedElevation) delete fBedMapBedElevation;
    if(fBedMapWaterDepth) delete fBedMapWaterDepth;
    if(fBedMapIceThickness) delete fBedMapIceThickness;
}

void WorldModel::initBedMap() {
    fBedMapBedElevation = new BedmapTable (RyansCons::kBedElevation);
    fBedMapWaterDepth = new BedmapTable (RyansCons::kWaterDepth);
    fBedMapIceThickness = new BedmapTable (RyansCons::kIceThickness);
}

void WorldModel::defineGeoid() {

  // see monte carlo note #17

  for (int i=0;i<crust2::N_LAT;i++) {
      fGeoid[i]=RyansGeom::getGeoid(i);
  }
}


void WorldModel::loadCrustData() {
    //Taken from ReadCrust in UCLA sim
  // reads in altitudes of 7 layers of crust, ice and water
  // puts data in arrays
  
  fstream infile("geomData/crust2.dat",ios::in);
  if(!infile) {
      std::cerr << "Couldn't open crust2 file: " << endl;
      exit(0);
  }

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
      fElevation[indexlon][indexlat]=(double)atof(selev.c_str());
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
	fWaterThickness[indexlon][indexlat]=(double)atof(sdepth.c_str()); 
      if (layertype.substr(0,3)=="ice") 
	fIceThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,8)=="soft sed") 
	fSoftSedThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,8)=="hard sed") 
	fHardSedThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,11)=="upper crust") 
	fUpperCrustThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,12)=="middle crust") 
	fMiddleCrustThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      if (layertype.substr(0,11)=="lower crust") 
	fLowerCrustThickness[indexlon][indexlat]=(double)atof(sdepth.c_str());
      
      //cout << "indexlon,indexlat are " << indexlon << " "<< indexlat << "\n";

      // region where Ross Ice Shelf was not accounted for in Crust 2.0
      // add it in by hand
      if (indexlat==5 && (indexlon<=5 || indexlon>=176)) // Ross Ice Shelf
	fIceThickness[indexlon][indexlat]=0.5;

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
	fWaterDensity[indexlon][indexlat]=ddensity; 
      if (layertype.substr(0,3)=="ice") 
	  fIceDensity[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,8)=="soft sed") 
	  fSoftSedDensity[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,8)=="hard sed") 
	  fHardSedDensity[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,11)=="upper crust") 
	  fUpperCrustDensity[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,12)=="middle crust")
	  fMiddleCrustDensity[indexlon][indexlat]=ddensity;
      if (layertype.substr(0,11)=="lower crust") 
	  fLowerCrustDensity[indexlon][indexlat]=ddensity;

      //cout << "sdensity is " << sdensity << "\n";
    }

    // adds up total thickness of crust
    fTotalCrustThickness[indexlon][indexlat]=
	fSoftSedThickness[indexlon][indexlat]+
	fHardSedThickness[indexlon][indexlat]+
	fUpperCrustThickness[indexlon][indexlat]+
	fMiddleCrustThickness[indexlon][indexlat]+
	fLowerCrustThickness[indexlon][indexlat];
      
      if (indexlon==179 && indexlat==0)
	  break;
  }  // done reading file
  
  for (int i=0;i<crust2::N_LON;i++) {
      for (int j=0;j<crust2::N_LAT;j++) {	  
	  // array of elevation at each longitude, latitude, including water and ice
	  if (fWaterThickness[i][j] != 0) {
	      fSurface[i][j]=fElevation[i][j]+fWaterThickness[i][j]*1000+fIceThickness[i][j]*1000;
	  }
	  else
	      fSurface[i][j]=fElevation[i][j];
	  if (fabs(fSurface[i][j])<1.E-10) {
	      fSurface[i][j] = 0;
	  }

	  // in units of m
	  fWaterBottom[i][j]=fSurface[i][j]-
	      (fIceThickness[i][j]+fWaterThickness[i][j])*1000;
	  if ((double)fabs(fWaterBottom[i][j])<1.E-10)
	      fWaterBottom[i][j]=0;
	  fIceBottom[i][j]=fWaterBottom[i][j]+
	      fWaterThickness[i][j]*1000;
	  fSoftSedBottom[i][j]=fWaterBottom[i][j]-
	      fSoftSedThickness[i][j]*1000;
	  fHardSedBottom[i][j]=fWaterBottom[i][j]-
	      (fSoftSedThickness[i][j]+
	       fHardSedThickness[i][j])*1000;
	  fUpperCrustBottom[i][j]=fWaterBottom[i][j]-
	      (fSoftSedThickness[i][j]+
	       fHardSedThickness[i][j]+
	       fUpperCrustThickness[i][j])*1000;
	  fMiddleCrustBottom[i][j]=fWaterBottom[i][j]-
	      (fSoftSedThickness[i][j]+
	       fHardSedThickness[i][j]+
	       fUpperCrustThickness[i][j]+
	       fMiddleCrustThickness[i][j])*1000;
	  fLowerCrustBottom[i][j]=fWaterBottom[i][j]-
	      (fSoftSedThickness[i][j]+
	       fHardSedThickness[i][j]+
	       fUpperCrustThickness[i][j]+
	       fMiddleCrustThickness[i][j]+
	       fLowerCrustThickness[i][j])*1000;

      }
  }  
}


Double_t WorldModel::getCrust2Data(Double_t x, Double_t y, RyansCons::Crust2Type_t type) {
    int lon,lat;
    Double_t p[3]={x,y,6e6};
    RyansGeom::getLonLat(p,lon,lat);
    
    switch(type) {
	case RyansCons::kTopOfIce: return fSurface[lon][lat];
	case RyansCons::kTopOfIceAbsolute: return fSurface[lon][lat] +RyansGeom::getGeoid(p);    
	case RyansCons::kBottomOfIce: return fIceBottom[lon][lat]; 
	case RyansCons::kBottomOfWater: return fWaterBottom[lon][lat]; 
	case RyansCons::kBottomOfSoftSed: return fSoftSedBottom[lon][lat]; 
	case RyansCons::kBottomOfHardSed: return fHardSedBottom[lon][lat]; 
	case RyansCons::kBottomOfUpperCrust: return fUpperCrustBottom[lon][lat]; 
	case RyansCons::kBottomOfMiddleCrust: return fMiddleCrustBottom[lon][lat]; 
	case RyansCons::kBottomOfLowerCrust: return fLowerCrustBottom[lon][lat]; 
	case RyansCons::kGeoid: return RyansGeom::getGeoid(p); 
	case RyansCons::kElevation: return fElevation[lon][lat]; 
	case RyansCons::kWaterDensity: return fWaterDensity[lon][lat]; 
	case RyansCons::kIceDensity: return fIceDensity[lon][lat]; 
	case RyansCons::kSoftSedDensity: return fSoftSedDensity[lon][lat];
	case RyansCons::kHardSedDensity: return fHardSedDensity[lon][lat];
	case RyansCons::kUpperCrustDensity: return fUpperCrustDensity[lon][lat];
	case RyansCons::kMiddleCrustDensity: return fMiddleCrustDensity[lon][lat];
	case RyansCons::kLowerCrustDensity: return fLowerCrustDensity[lon][lat];
	case RyansCons::kThicknessOfIce: return fIceThickness[lon][lat];
	case RyansCons::kThicknessOfWater: return fWaterThickness[lon][lat];
	case RyansCons::kThicknessOfSoftSed: return fSoftSedThickness[lon][lat];
	case RyansCons::kThicknessOfHardSed: return fHardSedThickness[lon][lat];
	case RyansCons::kThicknessOfUpperCrust: return fUpperCrustThickness[lon][lat];
	case RyansCons::kThicknessOfMiddleCrust: return fMiddleCrustThickness[lon][lat];
	case RyansCons::kThicknessOfLowerCrust: return fLowerCrustThickness[lon][lat];
	case RyansCons::kTotalCrustThickness: return fTotalCrustThickness[lon][lat];
	default:
	    std::cerr << "Don't understand type: " << type << endl;
	    return 0;
    }
    
}


Double_t WorldModel::getSurface(Double_t x, Double_t y) {
    if(!fUseBedmap) return getCrust2Data(x,y,RyansCons::kTopOfIceAbsolute);

    Double_t point[3]={x,y,6e6};
    Int_t goodFlag;
    Double_t bedElev = fBedMapBedElevation->getValue(x,y,goodFlag);
    if(!goodFlag) return getCrust2Data(x,y,RyansCons::kTopOfIceAbsolute);
    
    Double_t waterDepth = fBedMapWaterDepth->getValue(x,y,goodFlag);
    if(!goodFlag) waterDepth=0;
    
    Double_t iceThickness = fBedMapIceThickness->getValue(x,y,goodFlag);
    if(!goodFlag) iceThickness=0;

//    Double_t geoid = getCrust2Data(x,y,RyansCons::kGeoid);
    Double_t geoid = RyansGeom::getGeoid(point);

    Double_t relHeight=bedElev+waterDepth+iceThickness;
    if(relHeight<0) return geoid;
    return relHeight+geoid;
}

Double_t WorldModel::getIceThickness(Double_t x, Double_t y) {
    if(!fUseBedmap) return getCrust2Data(x,y,RyansCons::kThicknessOfIce);
    Int_t goodFlag;
    Double_t iceThickness = fBedMapIceThickness->getValue(x,y,goodFlag);
    if(!goodFlag) iceThickness=0;//return getCrust2Data(x,y,RyansCons::kThicknessOfIce);
    return iceThickness;   
}

void WorldModel::getSurfaceNormal(Double_t x, Double_t y, TVector3 *normal) {
    Double_t delta=1000; 
    //Need to get function of form Z=Z0+f(x)+g(y)
    //f(x)=ax^2+bx+c  g(x)=dy^2+ey+f;

    Double_t a,b,c,d,e,f;

    Double_t z0=getSurface(x,y);
    Double_t zx1=getSurface(x+delta,y);
    Double_t zx2=getSurface(x-delta,y);
    Double_t zy1=getSurface(x,y+delta);
    Double_t zy2=getSurface(x,y-delta);

    {
	Double_t f1=zx1-z0;
	Double_t f2=0;
	Double_t f3=zx2-z0;
	Double_t x1=x-delta;
	Double_t x2=x;
	Double_t x3=x+delta;

	a=((f2-f1)*(x3-x1)-(f3-f1)*(x2-x1))/
	    ((x2*x2-x1*x1)*(x3-x1)-(x3*x3-x1*x1)*(x2-x1));
	b=((f3-f1)-a*(x3*x3-x1*x1))/(x3-x1);
	c=f1-a*x1*x1-b*x1;	
    }

    {
	Double_t f1=zy1-z0;
	Double_t f2=0;
	Double_t f3=zy2-z0;
	Double_t y1=y-delta;
	Double_t y2=y;
	Double_t y3=y+delta;

	d=((f2-f1)*(y3-y1)-(f3-f1)*(y2-y1))/
	    ((y2*y2-y1*y1)*(y3-y1)-(y3*y3-y1*y1)*(y2-y1));
	e=((f3-f1)-a*(y3*y3-y1*y1))/(y3-y1);
	f=f1-a*y1*y1-b*y1;	
    }
    TVector3 bigNormal(2*a*x+b,2*d*y+e,-1);
    *normal=bigNormal.Unit();
}
