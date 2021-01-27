///////////////////////////////////////////////////////////////////////////////
/////   SubWaveInfo.h
/////   Simple Description of the RF from a shower
//////////////////////////////////////////////////////////////////////////////

#ifndef SUBWAVEINFO_H
#define SUBWAVEINFO_H

#include "TObject.h"
#include "InteractionInfo.h"

#include "RyansConventions.h"


class SubWaveInfo : public TObject {
public:
    SubWaveInfo();
    SubWaveInfo(InteractionInfo &intInfo, Double_t &rayTime, 
		Double_t &iceAtten, Double_t &airAtten, 
		Double_t &surfaceLossPerp, Double_t &surfaceLossPar,
		Double_t &oneOverRLoss);

    InteractionInfo fIntInfo;
    Double_t fRayTravelTime;

    //At some stage these will change and may become frequency dependent
    Double_t fIceAttenuation;
    Double_t fAirAttenuation;
    Double_t fSurfaceLossPerp;
    Double_t fSurfaceLossPar;
    Double_t fOneOverRLoss;

    ClassDef(SubWaveInfo,1);
};


#endif //SUBWAVEINFO_H
