///////////////////////////////////////////////////////////////////////////////
/////   RayTracer.h
/////   Class to do Ray Tracing
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "TObject.h"
#include "TVector3.h"

#include "RyansConventions.h"
#include "WorldModel.h"
#include "RFEvent.h"
#include "NeutrinoEvent.h"

class RayTracer : public TObject {
public:
    RayTracer(WorldModel *wmPtr);
    void doTrace(TVector3 *intPos,TVector3 *balloonPos);
    Double_t getTime()
	{return fMinTime;}
    Double_t getIceDist() {return fIceDist;}
    Double_t getAirDist() {return fAirDist;}
    Double_t getIceAtten() {return fIceAtten;}
    Double_t getAirAtten() {return fAirAtten;}
    Double_t getSurfaceLossPerp() {return fSurfaceLossPerp;}
    Double_t getSurfaceLossPar() {return fSurfaceLossPar;}
    Double_t getOneOverRLoss() {return fOneOverRLoss;}
    RFEvent *makeRFEvent(NeutrinoEvent *nuEvent);
    void fillRFEvent(NeutrinoEvent *nuEvent, RFEvent *rfEvent);

private:
    void getXY(Double_t u, Double_t &x, Double_t &y);
    Double_t getTimeForCrossPoint(Double_t surfPoint);
    void getMinimum(Double_t umin, Double_t umax);
    WorldModel *fWorldModel;
    TVector3 fIntPos;
    TVector3 fBalloonPos;
    
    Double_t fTanTheta;
    Double_t fCosTheta;
    Double_t fSinTheta;
    Double_t fBalloonU;

    Double_t fNIce;
    Double_t fNAir;
    
    Int_t fNpx;
    TVector3 fSurfPos;
    Double_t fMinTime;
    Double_t fIceDist;
    Double_t fAirDist;
    Double_t fIceAtten;
    Double_t fAirAtten;
    Double_t fSurfaceLossPerp;
    Double_t fSurfaceLossPar;
    Double_t fOneOverRLoss;
    
    ClassDef(RayTracer,1);
};


#endif //RAYTRACER_H
