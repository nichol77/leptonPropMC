///////////////////////////////////////////////////////////////////////////////
/////   InteractionInfo.h
/////   Simple description of a stochastic energy loss interaction
//////////////////////////////////////////////////////////////////////////////

#ifndef INTERACTIONINFO_H
#define INTERACTIONINFO_H

#include "TObject.h"
#include "TVector3.h"

#include "RyansConventions.h"


class InteractionInfo : public TObject {
public:
    InteractionInfo();
    InteractionInfo(RyansCons::ParticleType_t ipType,
		    TVector3 &iLoc,
		    Double_t theTime,
		    TVector3 &iDir,
		    Double_t energy,
		    RyansCons::InteractionType_t intKind,
		    Double_t fracEM, Double_t fracHad, Double_t fracFinal,
		    RyansCons::ParticleType_t fpType);

    TVector3 getDirection() 
	{return intDir;}
    TVector3 getLocation() 
	{return intLocation;} 
    RyansCons::ParticleType_t getFinalParticleType()
	{ return finalParticleType;}
    Double_t getIntTime()
	{return intTime;}
    Double_t getIntEnergy() 
	{return intEnergy;}
    Double_t getFinalParticleEnergy() 
	{return intEnergy*fracFinalParticle;}


    RyansCons::ParticleType_t initialParticleType;
    TVector3 intLocation;
    Double_t intTime; // t=0 is the time of neutrino interaction
    TVector3 intDir;
    Double_t intEnergy;
    RyansCons::InteractionType_t intType;

    Double_t fracEMShower;
    Double_t fracHadShower;
    Double_t fracFinalParticle;  // The three fracs should add to unity   
    
    RyansCons::ParticleType_t finalParticleType;

    ClassDef(InteractionInfo,1);
};


#endif //INTERACTIONINFO_H
