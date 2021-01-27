

void sillyTest() {
    
    TF1 *decay = new TF1("decay",decayFormula,0,1000,2);
    decay->SetParameter(0,10);
    decay->SetParameter(1,1);
//    decay->Draw();

    TH1F *partHist = new TH1F("partHist","partHist",200,0,200);
    Int_t numParticles=100000;
    for(int part=0;part<int(numParticles);part++) {
	if(part%1000==0) cerr << "*";
	for(Double_t distance=0;distance<1000;) {
	    Double_t stepSize=gRandom->Exp(0.01);
	    distance+=stepSize;
	    Double_t tempVal=decay->GetRandom();
	    if(tempVal<stepSize ) {
		partHist->Fill(distance);
		break;
	    }
	}
    }
    partHist->Draw();
    decay->SetParameter(1,double(numParticles/10));
    decay->Draw("same");

}

Double_t decayFormula(Double_t *x, Double_t *par) {
    Double_t Reff=par[0];
    Double_t scale=par[1];
   
    return scale*TMath::Exp(-1*x[0]/Reff);
}
