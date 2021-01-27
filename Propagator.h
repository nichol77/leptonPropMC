///////////////////////////////////////////////////////////////////////////////
/////   Propagator.h
/////   Describes one neutrino event in the ice and contains all its
/////   secondary interactions
//////////////////////////////////////////////////////////////////////////////

#ifndef PROPAGATOR_H
#define PROPAGATOR_H

#include "TObject.h"
#include "TRandom.h"
#include "TF1.h"

#include "RyansConventions.h"
#include "NeutrinoEvent.h"


class Propagator : public TObject {
public:
    Propagator();
    Propagator(RyansCons::MaterialType_t medium, 
	       Double_t nuCut=0.001, Double_t eThresh=1e8);
    ~Propagator();

    Double_t oneStep(NeutrinoEvent *nuEvent);
    Int_t propagateToThreshold(NeutrinoEvent *nuEvent);



    RyansCons::MaterialType_t fMedium;
    Double_t fNuCut; //0.001 is default value
    Double_t fEThresh; // in GeV (1e8 is default)
		  

private:
    TRandom *fRandom;
    TF1 *fMeanFree;
    TF1 *fBremdSdNu;
    TF1 *fPairdSdNu;
    TF1 *fPhotodSdNu;
    TF1 *fKnockOndSdNu;
    TF1 *fDecayRange;
    TF1 *fWeakCCLength;
//    TF1 *fWeakNCLength; //Need to include
    
    

    ClassDef(Propagator,1);
};


#endif //PROPAGATOR_H
