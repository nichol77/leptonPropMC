///////////////////////////////////////////////////////////////////////////////
/////   SubWaveInfo.cxx
/////   Simple Description of the RF from a shower
///////////////////////////////////////////////////////////////////////////////
#include "SubWaveInfo.h"


ClassImp(SubWaveInfo)

SubWaveInfo::SubWaveInfo()
{
//Default (zero) constructor 
  
}

SubWaveInfo::SubWaveInfo(InteractionInfo &intInfo, Double_t &rayTime, 
			 Double_t &iceAtten, Double_t &airAtten, 
			 Double_t &surfaceLossPerp, Double_t &surfaceLossPar,
			 Double_t &oneOverRLoss)
    :fIntInfo(intInfo),
     fRayTravelTime(rayTime),
     fIceAttenuation(iceAtten),
     fAirAttenuation(airAtten),
     fSurfaceLossPerp(surfaceLossPerp),
     fSurfaceLossPar(surfaceLossPar),
     fOneOverRLoss(oneOverRLoss)
{
//Assignment constructor

}
