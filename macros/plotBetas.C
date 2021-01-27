

void plotBetas() {

//    plotBetasForMuons();
//    plotBetasForTaus();
    plotRanges();
}



void plotdEdxForMuons() {   
    gSystem->CompileMacro("interactions.cxx","k");

    TCanvas *canMuon = new TCanvas("canMuon","Muon Betas",600,400);
    canMuon->SetLogx(1);
    canMuon->SetLogy(1);
    
    TH1F *frame = canMuon->DrawFrame(100,1e-17,1e14,1,"Muon Energy Loss (Water)");
    TF1 *plotty = new TF1("plotty",funcOneOverBeta,100,1e14,2);
    plotty->SetParameter(0,0);
    plotty->SetParameter(1,0.92); //Density
    plotty->Draw("same");    
}

void plotRanges() {   
    gSystem->CompileMacro("interactions.cxx","k");

    TCanvas *canRange = new TCanvas("canRange","Ranges",600,400);
    canRange->SetTopMargin(0.12);
    canRange->SetLeftMargin(0.12);
    canRange->SetLogx(1);
    canRange->SetLogy(1);
    
    TH1F *frame = canRange->DrawFrame(100,1e-4,1e14,1e6,"Range (Ice)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("Range (km)");
    frame->GetYaxis()->SetTitleOffset(0.8);
    
    TF1 *rangey = new TF1("rangey",funcIntegralOneOverBeta,100,1e14,2);
    rangey->SetLineColor(50);
    rangey->SetParameter(0,1);
    rangey->SetParameter(1,0.92); //Density
    TF1 *tauRange=rangey->DrawCopy("same"); 
    rangey->SetLineColor(40);
    rangey->SetParameter(0,0);
    TF1 *muonRange=rangey->DrawCopy("same"); 
 
    TF1 *decayy = new TF1("decayy",funcDecayRange,100,1e14,1);
    decayy->SetLineColor(44);
    decayy->SetParameter(0,1);
    TF1 *tauDecay=decayy->DrawCopy("same"); 
    decayy->SetLineColor(30);
    decayy->SetParameter(0,0);
    TF1 *muonDecay=decayy->DrawCopy("same");       
}  


void plotBetasForMuons() {
    ifstream Input("muonBetaFile.txt");
    Double_t energy[200];
    Double_t bremBeta[200];
    Double_t pairBeta[200];
    Double_t photoBeta[200];
    Double_t combinedBeta[200];
    Int_t numPoints=0;
    while(Input >> energy[numPoints] >> bremBeta[numPoints] 
	  >> pairBeta[numPoints] >> photoBeta[numPoints]) {
	combinedBeta[numPoints]=bremBeta[numPoints]+pairBeta[numPoints]+photoBeta[numPoints];
	numPoints++;
    }
    
    TGraph *grBrem = new TGraph(numPoints,energy,bremBeta);
    TGraph *grPair = new TGraph(numPoints,energy,pairBeta);
    TGraph *grPhoto = new TGraph(numPoints,energy,photoBeta);
    TGraph *grCombined = new TGraph(numPoints,energy,combinedBeta);

    TCanvas *canMuon = new TCanvas("canMuon","Muon Betas",600,400);
    canMuon->SetLeftMargin(0.12);
    canMuon->SetTopMargin(0.12);
    canMuon->SetLogx();
    TH1F *frame = canMuon->DrawFrame(100,0,1e14,12,"Muon Energy Loss (Water)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("E^{-1} dE/dx (10^{-6} g^{-1} cm^{2})");
    frame->GetYaxis()->SetTitleOffset(0.7);


    grBrem->SetLineColor(50);
    grPair->SetLineColor(40);
    grPhoto->SetLineColor(8);
    grCombined->SetLineColor(30);
    grBrem->SetLineStyle(4);
    grPair->SetLineStyle(3);
    grPhoto->SetLineStyle(2);
    grCombined->SetLineStyle(1);
    grBrem->Draw("l");
    grPair->Draw("l");
    grPhoto->Draw("l");
    grCombined->Draw("l");

}


void plotBetasForTaus() {
    ifstream Input("tauBetaFile.txt");
    Double_t energy[200];
    Double_t bremBeta[200];
    Double_t pairBeta[200];
    Double_t photoBeta[200];
    Double_t combinedBeta[200];
    Int_t numPoints=0;
    while(Input >> energy[numPoints] >> bremBeta[numPoints] 
	  >> pairBeta[numPoints] >> photoBeta[numPoints]) {
	combinedBeta[numPoints]=bremBeta[numPoints]+pairBeta[numPoints]+photoBeta[numPoints];
	numPoints++;
    }
    
    TGraph *grBrem = new TGraph(numPoints,energy,bremBeta);
    TGraph *grPair = new TGraph(numPoints,energy,pairBeta);
    TGraph *grPhoto = new TGraph(numPoints,energy,photoBeta);
    TGraph *grCombined = new TGraph(numPoints,energy,combinedBeta);

    TCanvas *canTau = new TCanvas("canTau","Tau Betas",600,400);
    canTau->SetLeftMargin(0.12);
    canTau->SetTopMargin(0.12);
    canTau->SetLogx();
    TH1F *frame = canTau->DrawFrame(100,0,1e14,3,"Tau Energy Loss (Water)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("E^{-1} dE/dx (10^{-6} g^{-1} cm^{2})");
    frame->GetYaxis()->SetTitleOffset(0.7);


    grBrem->SetLineColor(50);
    grPair->SetLineColor(40);
    grPhoto->SetLineColor(8);
    grCombined->SetLineColor(30);
    grBrem->SetLineStyle(4);
    grPair->SetLineStyle(3);
    grPhoto->SetLineStyle(2);
    grCombined->SetLineStyle(1);
    grBrem->Draw("l");
    grPair->Draw("l");
    grPhoto->Draw("l");
    grCombined->Draw("l");

}
