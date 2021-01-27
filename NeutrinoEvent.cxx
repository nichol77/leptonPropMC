///////////////////////////////////////////////////////////////////////////////
/////   NeutrinoEvent.cxx
/////   Contains information about the various interactions (primary and
/////   secondary)
///////////////////////////////////////////////////////////////////////////////
#include "NeutrinoEvent.h"

#include <iostream>

#include "PhysicalConstants.h"

ClassImp(NeutrinoEvent)

TClonesArray *NeutrinoEvent::fgInts = 0;


NeutrinoEvent::NeutrinoEvent()
    :initialEnergy(0),
     nuFlavour(RyansCons::kNothing),fNumInts(0)
{
//Default (zero) constructor 
    if(!fgInts) fgInts= new TClonesArray("InteractionInfo", 1000);
    fInts=fgInts;
}

NeutrinoEvent::NeutrinoEvent(Double_t energy, 
			     RyansCons::ParticleType_t flavour,
			     TVector3 intPos, TVector3 nuDir, 
			     RyansCons::InteractionType_t intType,
			     Double_t elasticity, TVector3 ballonLoc)
    :initialEnergy(energy),
     nuFlavour(flavour),fNumInts(0),
     balloonPosition(ballonLoc)    
{
//Assignment constructor
    fInts = new TClonesArray("InteractionInfo",1000);
    Double_t fEM=0,fHad=0,fPart=0;
    RyansCons::ParticleType_t particle=flavour;
    switch(intType) {
	case RyansCons::kNC:
	    fHad=elasticity;
	    fPart=1-elasticity;
	    break;
	case RyansCons::kCC:
	    fHad=elasticity;
	    fPart=1-elasticity;
	    particle=RyansCons::getCCPartner(particle);
	    if(abs(flavour)==RyansCons::kElectron) {
		fEM=1-elasticity;
		fPart=0;
		particle=RyansCons::kNothing;
	    }
	    break;
	default:
	    std::cerr << "Don't understand interaction type: "
		      << intType << std::endl;
	    break;
    }


    TClonesArray &interactions = *fInts;
    // InteractionInfo *intInfo = 
    new(interactions[fNumInts++]) InteractionInfo(flavour,intPos,0,nuDir,
						  energy,intType,
						  fEM,fHad,fPart,particle);
         
}


NeutrinoEvent::~NeutrinoEvent()
{
//Default destructor 
    //May need to loop over array and delete things
    fInts->Delete();
}


void NeutrinoEvent::AddInteraction(Double_t intEnergy, Double_t afterLength,
			      RyansCons::InteractionType_t intType,
			      Double_t showerEnergy)
{
    
    Double_t fEM=0,fHad=0,fPart=0;
    InteractionInfo *previous=(InteractionInfo*)fInts->Last();
    RyansCons::ParticleType_t intParticle=previous->getFinalParticleType();
    RyansCons::ParticleType_t finalParticle=intParticle;
    TVector3 theDir=previous->getDirection();
    TVector3 intPos=previous->getLocation();
    theDir.SetMag(afterLength);
    intPos+=theDir;
    theDir.SetMag(1);
    Double_t theTime=previous->getIntTime()+(afterLength/c_SI);
    switch(intType) {
	case RyansCons::kBrem:
	    fHad=0;
	    fEM=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    break;
	    
	case RyansCons::kPair:
	    fHad=0;
	    fEM=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    break;
	    
	case RyansCons::kPhoto:
	    fEM=0;
	    fHad=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    break;
	    
	case RyansCons::kKnockOn:
	    fHad=0;
	    fEM=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    break;

	case RyansCons::kCC:
	    fHad=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    finalParticle=RyansCons::getCCPartner(intParticle);
	    break;

	case RyansCons::kDecay:
	    fHad=showerEnergy;
	    fPart=intEnergy-showerEnergy;
	    finalParticle=RyansCons::getCCPartner(intParticle);
	    break;
	default:
	    std::cerr << "Don't understand interaction type: "
		      << intType << std::endl;
	    break;
    }
    fEM/=intEnergy;
    fHad/=intEnergy;
    fPart/=intEnergy;
    
//     std::cerr << "AddInteraction:\t" << intParticle << "\t" << intEnergy
// 	      << "\t" << fEM << "\t" << fHad << "\t" << fPart << std::endl;
    TClonesArray &interactions = *fInts;
    new(interactions[fNumInts++]) InteractionInfo(intParticle,intPos,theTime,
						    theDir,
						    intEnergy,intType,
						    fEM,fHad,fPart,
						    finalParticle);  }

