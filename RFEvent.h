///////////////////////////////////////////////////////////////////////////////
/////   RFEvent.h
/////   Describes the RF for one neutrino event
//////////////////////////////////////////////////////////////////////////////

#ifndef RFEVENT_H
#define RFEVENT_H

#include "TObject.h"
#include "TClonesArray.h"

#include "RyansConventions.h"
#include "SubWaveInfo.h"
#include "InteractionInfo.h"
#include "NeutrinoEvent.h"
#include "TVector3.h"


class RFEvent : public TObject {
public:
    RFEvent();
    RFEvent(NeutrinoEvent &nuEvent);
    ~RFEvent();

    void AddSubWave(InteractionInfo &intInfo, Double_t &rayTime, 
		    Double_t &iceAtten, Double_t &airAtten, 
		    Double_t &surfaceLossPerp, Double_t &surfaceLossPar,
		    Double_t &oneOverRLoss);
    
    Double_t initialEnergy;
    RyansCons::ParticleType_t nuFlavour;
    Int_t fNumSubWaves;
    TClonesArray *fSubWaves;
    static TClonesArray *fgSubWaves;
    TVector3 balloonPosition;


    ClassDef(RFEvent,2);
};


#endif //RFEVENT_H
