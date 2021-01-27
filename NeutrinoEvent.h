///////////////////////////////////////////////////////////////////////////////
/////   NeutrinoEvent.h
/////   Describes one neutrino event in the ice and contains all its
/////   secondary interactions
//////////////////////////////////////////////////////////////////////////////

#ifndef NEUTRINOEVENT_H
#define NEUTRINOEVENT_H

#include "TObject.h"
#include "TClonesArray.h"

#include "RyansConventions.h"
#include "InteractionInfo.h"
#include "TVector3.h"


class NeutrinoEvent : public TObject {
public:
    NeutrinoEvent();
    NeutrinoEvent(Double_t energy, RyansCons::ParticleType_t flavour,
		  TVector3 intPos, TVector3 nuDir, 
		  RyansCons::InteractionType_t intType,
		  Double_t elasticity, TVector3 ballonLoc);
    ~NeutrinoEvent();


    void AddInteraction(Double_t intEnergy, Double_t afterLength,
			RyansCons::InteractionType_t intType,
			Double_t showerEnergy);

    RyansCons::ParticleType_t getCurrentParticleType(){ 
	InteractionInfo *previous = (InteractionInfo*)fInts->Last();
	return previous->getFinalParticleType();}
    
    Double_t getCurrentParticleEnergy(){ 
	InteractionInfo *previous = (InteractionInfo*)fInts->Last();	
	return previous->getFinalParticleEnergy();}

    
    Double_t initialEnergy;
    RyansCons::ParticleType_t nuFlavour;
    Int_t fNumInts;
    TClonesArray *fInts;
    static TClonesArray *fgInts;
    TVector3 balloonPosition;
		  

    ClassDef(NeutrinoEvent,2);
};


#endif //NEUTRINOEVENT_H
