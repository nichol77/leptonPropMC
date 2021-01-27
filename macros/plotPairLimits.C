#include "PhysicalConstants.h"

void plotPairLimits() {

 //    TF1 *minny = new TF1("minny",funcPairNuMin,1e8,1e12,1);
//     minny->SetParameter(0,1); //Nothing
//     minny->Draw();

//     TF1 *maxy = new TF1("maxy",funcPairNuMax,1e8,1e12,2);
//     maxy->SetParameters(tauMass,6.6);
//     maxy->Draw();


    TF1 *maxy = new TF1("maxy",funcBremNuMax,1e8,1e12,2);
    maxy->SetParameters(tauMass,6.6);
    maxy->Draw();

}




Double_t funcPairNuMin(Double_t *energy, Double_t *par) {

    return getPairNuMin(energy[0]);


}



Double_t funcPairNuMax(Double_t *energy, Double_t *par) {

    Double_t leptonMass=par[0];
    Double_t Z=par[1];
    return getPairNuMax(energy[0],leptonMass,Z);

}


Double_t funcBremNuMax(Double_t *energy, Double_t *par) {

    Double_t leptonMass=par[0];
    Double_t Z=par[1];
    return getBremNuMax(energy[0],leptonMass,Z);

}



