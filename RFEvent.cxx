///////////////////////////////////////////////////////////////////////////////
/////   RFEvent.cxx
/////   Contains information about the various interactions (primary and
/////   secondary)
///////////////////////////////////////////////////////////////////////////////
#include "RFEvent.h"

#include <iostream>

#include "PhysicalConstants.h"

ClassImp(RFEvent)

TClonesArray *RFEvent::fgSubWaves = 0;


RFEvent::RFEvent()
    :initialEnergy(0),
     nuFlavour(RyansCons::kNothing),fNumSubWaves(0)
{
//Default (zero) constructor 
    if(!fgSubWaves) fgSubWaves= new TClonesArray("SubWaveInfo", 1000);
    fSubWaves=fgSubWaves;
}

RFEvent::RFEvent(NeutrinoEvent &event)
    :initialEnergy(event.initialEnergy),
     nuFlavour(event.nuFlavour),
     fNumSubWaves(0),
     balloonPosition(event.balloonPosition)    
{
//Assignment constructor
//    if(!fgSubWaves) fgSubWaves= new TClonesArray("SubWaveInfo", 1000);
    fSubWaves=new TClonesArray("SubWaveInfo", 1000);
}


RFEvent::~RFEvent()
{
//Default destructor 
    //May need to loop over array and delete things
    fSubWaves->Delete();
}


void RFEvent::AddSubWave(InteractionInfo &intInfo, Double_t &rayTime, 
			 Double_t &iceAtten, Double_t &airAtten, 
			 Double_t &surfaceLossPerp, Double_t &surfaceLossPar,
			 Double_t &oneOverRLoss) 
{
    TClonesArray &subwave = *fSubWaves;
    new(subwave[fNumSubWaves++]) SubWaveInfo(intInfo,rayTime,
					     iceAtten,airAtten,
					     surfaceLossPerp,surfaceLossPar,
					     oneOverRLoss);

}
