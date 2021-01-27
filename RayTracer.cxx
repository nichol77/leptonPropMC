///////////////////////////////////////////////////////////////////////////////
/////   RayTracer.cxx
/////   Contains information about the various interactions (primary and
/////   secondary)
///////////////////////////////////////////////////////////////////////////////
#include "RayTracer.h"

#include <iostream>
#include "PhysicalConstants.h"

//#include "TF1.h"
//#include "TRandom.h"
#include "TMath.h"

using namespace std;

ClassImp(RayTracer)

RayTracer::RayTracer(WorldModel *fWMPtr)
    :fWorldModel(fWMPtr),fNpx(100)
{
//Default (zero) constructor 
}


void RayTracer::doTrace(TVector3 *intPos,TVector3 *balloonPos)
{
    fBalloonPos=*balloonPos;
    fIntPos=*intPos;
    Double_t x2Prime=fBalloonPos.X()-fIntPos.X();
    Double_t y2Prime=fBalloonPos.Y()-fIntPos.Y();
    
    fTanTheta=y2Prime/x2Prime;
    fSinTheta=TMath::Sin(TMath::ATan(fTanTheta));
    fCosTheta=TMath::Cos(TMath::ATan(fTanTheta));
    fBalloonU=TMath::Sqrt(x2Prime*x2Prime+y2Prime*y2Prime);

    fNIce=1.79; //Will do something better, like getting it from WorldModel
    fNAir=1; 

    this->getMinimum(0,fBalloonU);

    TVector3 icePath=fSurfPos-fIntPos;
    fIceDist=icePath.Mag();
    TVector3 airPath=fBalloonPos-fSurfPos;
    fAirDist=airPath.Mag();

    fIceAtten=TMath::Exp(-1*fIceDist/700); //Will need to do something better
    fAirAtten=1;

    TVector3 surfaceNormal;
    fWorldModel->getSurfaceNormal(fSurfPos.X(),fSurfPos.Y(),&surfaceNormal);
    Double_t incidentAngle=surfaceNormal.Angle(icePath);
    Double_t exitAngle=surfaceNormal.Angle(airPath);

    fSurfaceLossPerp=2*TMath::Sin(exitAngle)*TMath::Cos(incidentAngle)/
	TMath::Sin(incidentAngle+exitAngle);

    fSurfaceLossPerp=2*TMath::Sin(exitAngle)*TMath::Cos(incidentAngle)/
	(TMath::Sin(incidentAngle+exitAngle)*TMath::Cos(incidentAngle-exitAngle));
    fOneOverRLoss=1/(fIceDist+fAirDist); //Is it???

}

void RayTracer::getXY(Double_t u, Double_t &x, Double_t &y)
{
    Double_t xPrime=u*fCosTheta;
    Double_t yPrime=u*fSinTheta;

    x=xPrime+fIntPos.X();
    y=yPrime+fIntPos.Y();
}
 

Double_t RayTracer::getTimeForCrossPoint(Double_t surfPoint) 
{
    //For now we'll just assume we start in the ice
    Double_t x,y;
    getXY(surfPoint,x,y);
    Double_t surfZ=fWorldModel->getSurface(x,y);

    Double_t iceDist=TMath::Sqrt(TMath::Power(surfZ-fIntPos.Z(),2)+
				 TMath::Power(surfPoint-0,2));
    Double_t airDist=TMath::Sqrt(TMath::Power(fBalloonPos.Z()-surfZ,2)+
				 TMath::Power(fBalloonU-surfPoint,2));
				 
    Double_t iceTime=fNIce*iceDist/c_SI;
    Double_t airTime=fNIce*airDist/c_SI;

    return airTime+iceTime;
}

void RayTracer::getMinimum(Double_t umin, Double_t umax)
{
   Double_t u,t;
   if (umin >= umax) {umin = 0; umax = fBalloonU;}
   Double_t du = (umax-umin)/fNpx;
   Double_t uumin = umin;
   Double_t ttmin = this->getTimeForCrossPoint(umin+du);
   for (Int_t i=0;i<fNpx;i++) {
      u = umin + (i+0.5)*du;
      t = this->getTimeForCrossPoint(umin+du);
      if (t < ttmin) {uumin = u; ttmin = t;}
   }
   if (du < 1.e-9*fBalloonU) {
       u=TMath::Min(umax,uumin);
       Double_t x,y,z;
       getXY(u,x,y);
       z=fWorldModel->getSurface(x,y);       
       fSurfPos.SetXYZ(x,y,z);
       fMinTime=ttmin;
   }
   else {
       getMinimum(TMath::Max(umin,uumin-du), TMath::Min(umax,uumin+du));
   }
}

void RayTracer::fillRFEvent(NeutrinoEvent *nuEvent, RFEvent *rfEvent)
{   
    TVector3 balloonPos=nuEvent->balloonPosition;
    for(int count=0;count<nuEvent->fInts->GetEntries();count++) {
	InteractionInfo *intInfo = 
	    (InteractionInfo*) nuEvent->fInts->At(count);
	TVector3 intPos=intInfo->intLocation;	
	doTrace(&intPos,&balloonPos);
	rfEvent->AddSubWave(*intInfo,fMinTime,fIceAtten,fAirAtten,
			    fSurfaceLossPerp,fSurfaceLossPar,fOneOverRLoss);
	
    }
//    cout << rfEvent->fNumSubWaves << "\t" << rfEvent->fSubWaves->GetEntries()
//	 << endl;
}
    

RFEvent *RayTracer::makeRFEvent(NeutrinoEvent *nuEvent)
{
    RFEvent *rfEvent = new RFEvent(*nuEvent);    
    TVector3 balloonPos=nuEvent->balloonPosition;
    for(int count=0;count<nuEvent->fInts->GetEntries();count++) {
	InteractionInfo *intInfo = 
	    (InteractionInfo*) nuEvent->fInts->At(count);
	TVector3 intPos=intInfo->intLocation;	
	doTrace(&intPos,&balloonPos);
	rfEvent->AddSubWave(*intInfo,fMinTime,fIceAtten,fAirAtten,
			    fSurfaceLossPerp,fSurfaceLossPar,fOneOverRLoss);
	
    }
//    cout << rfEvent->fNumSubWaves << "\t" << rfEvent->fSubWaves->GetEntries()
//	 << endl;
    return rfEvent;
}
