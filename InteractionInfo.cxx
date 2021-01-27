///////////////////////////////////////////////////////////////////////////////
/////   InteractionInfo.cxx
/////   Contains information about the various interactions (primary and
/////   secondary)
///////////////////////////////////////////////////////////////////////////////
#include "InteractionInfo.h"

#include <iostream>

ClassImp(InteractionInfo)

InteractionInfo::InteractionInfo()
{
//Default (zero) constructor 
  
}

InteractionInfo::InteractionInfo(RyansCons::ParticleType_t ipType,
				 TVector3 &iLoc,
				 Double_t theTime,
				 TVector3 &iDir,
				 Double_t energy,
				 RyansCons::InteractionType_t intKind,
				 Double_t fracEM, Double_t fracHad, 
				 Double_t fracFinal,
				 RyansCons::ParticleType_t fpType)
    :initialParticleType(ipType),
     intLocation(iLoc),
     intTime(theTime),
     intDir(iDir),
     intEnergy(energy),
     intType(intKind),
     fracEMShower(fracEM),
     fracHadShower(fracHad),
     fracFinalParticle(fracFinal),
     finalParticleType(fpType)
{
//     std::cout << "II:\t" << finalParticleType << "\t" << fracEMShower
// 	      << "\t" << fracHadShower << "\t" << fracFinalParticle << std::endl
// 	;
// Assignment Constructor
}
