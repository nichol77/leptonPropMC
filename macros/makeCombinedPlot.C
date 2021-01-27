

void makeCombinedPlot() {
    gSystem->CompileMacro("interactions.cxx","k");
    
//    tauPlot();
    muonPlot();
//    dedxPlot();
//    makeTauBetaFile();
//    makeMuonBetaFile();
}

void makeMuonBetaFile() {
//    gSystem->CompileMacro("interactions.cxx","k");

    TF1 *bremy = new TF1("bremy",getBetaForBrem,100,1e14,3);
    TF1 *pairy = new TF1("pairy",getBetaForPair,100,1e14,3);
    TF1 *photoy = new TF1("photoy",getBetaFromFulldSdnu,100,1e14,3);
    TF1 *totally = new TF1("totally",getBetaCombined,100,1e14,3);
    bremy->SetParameters(0,11.89,6.6);
    pairy->SetParameters(0,11.89,6.6);
    photoy->SetParameters(0,11.89,6.6);
    totally->SetParameters(0,11.89,6.6);

    ofstream Output("muonBetaFile.txt");
    Output.precision(8);
    cout.precision(8);
    for(Double_t exponent=100;exponent<=1e14;exponent*=10) {
	for(Double_t multiplier=1;multiplier<10;multiplier++) {
	    Double_t energy=multiplier*exponent;
	    Double_t bremVal=bremy->Eval(energy);
	    Double_t pairVal=pairy->Eval(energy);
	    Double_t photoVal=photoy->Eval(energy);
	    Output << energy << "\t" << bremVal << "\t" << pairVal
		   << "\t" << photoVal << endl;
	    cout << energy << "\t" << bremVal << "\t" << pairVal
		   << "\t" << photoVal << endl;
	}
    }
}


void makeTauBetaFile() {

    TF1 *bremy = new TF1("bremy",getBetaForBrem,100,1e14,3);
    TF1 *pairy = new TF1("pairy",getBetaForPair,100,1e14,3);
    TF1 *photoy = new TF1("photoy",getBetaFromFulldSdnu,100,1e14,3);
    TF1 *totally = new TF1("totally",getBetaCombined,100,1e14,3);
    bremy->SetParameters(1,11.89,6.6);
    pairy->SetParameters(1,11.89,6.6);
    photoy->SetParameters(1,11.89,6.6);
    totally->SetParameters(1,11.89,6.6);

    ofstream Output("tauBetaFile.txt");
    Output.precision(8);
    cout.precision(8);
    for(Double_t exponent=100;exponent<=1e14;exponent*=10) {
	for(Double_t multiplier=1;multiplier<10;multiplier++) {
	    Double_t energy=multiplier*exponent;
	    Double_t bremVal=bremy->Eval(energy);
	    Double_t pairVal=pairy->Eval(energy);
	    Double_t photoVal=photoy->Eval(energy);
	    Output << energy << "\t" << bremVal << "\t" << pairVal
		   << "\t" << photoVal << endl;
	    cout << energy << "\t" << bremVal << "\t" << pairVal
		   << "\t" << photoVal << endl;
	}
    }
}

void dedxPlot() {
//     gSystem->CompileMacro("interactions.cxx","k");
     TF1 *dedxy = new TF1("dedxy",getdEdXCombined,100,1e14,3);
     dedxy->Draw();
}
     

void muonPlot() {
    //  gSystem->CompileMacro("interactions.cxx","k");

    TCanvas *canMuon = new TCanvas("canMuon","Muon Betas",600,400);
    canMuon->SetLeftMargin(0.12);
    canMuon->SetTopMargin(0.12);
    canMuon->SetLogx();
    TH1F *frame = canMuon->DrawFrame(100,0,1e14,12,"Muon Energy Loss (Water)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("E^{-1} dE/dx (10^{-6} g^{-1} cm^{2})");
    frame->GetYaxis()->SetTitleOffset(0.7);
    TF1 *bremy = new TF1("bremy",getBetaForBrem,100,1e14,3);
    TF1 *pairy = new TF1("pairy",getBetaForPair,100,1e14,3);
    TF1 *photoy = new TF1("photoy",getBetaFromFulldSdnu,100,1e14,3);
    TF1 *totally = new TF1("totally",getBetaCombined,100,1e14,3);
    bremy->SetParameters(0,11.89,6.6);
    pairy->SetParameters(0,11.89,6.6);
    photoy->SetParameters(0,11.89,6.6);
    totally->SetParameters(0,11.89,6.6);
    bremy->SetLineColor(50);
    pairy->SetLineColor(40);
    photoy->SetLineColor(8);
    totally->SetLineColor(30);
    bremy->SetLineStyle(4);
    pairy->SetLineStyle(3);
    photoy->SetLineStyle(2);
    totally->SetLineStyle(1);
    bremy->Draw("same");
    pairy->Draw("same");
    photoy->Draw("same");
    totally->Draw("same");

    TLegend *leg = new TLegend(0.15,0.65,0.4,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->AddEntry(totally,"Combined","l");
    leg->AddEntry(bremy,"Bremsstrahlung","l");
    leg->AddEntry(pairy,"Pair Production","l");
    leg->AddEntry(photoy,"Photonuclear","l");
    leg->Draw();
}


void tauPlot() {
//    gSystem->CompileMacro("interactions.cxx","k");

    TCanvas *canTau = new TCanvas("canTau","Tau Betas",600,400);
    canTau->SetLeftMargin(0.12);
    canTau->SetTopMargin(0.12);
    canTau->SetLogx();
    TH1F *frame = canTau->DrawFrame(100,0,1e14,2.5,"Tau Energy Loss (Water)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("E^{-1} dE/dx (10^{-6} g^{-1} cm^{2})");
    frame->GetYaxis()->SetTitleOffset(0.7);
    TF1 *bremy = new TF1("bremy",getBetaForBrem,100,1e14,3);
    TF1 *pairy = new TF1("pairy",getBetaForPair,100,1e14,3);
    TF1 *photoy = new TF1("photoy",getBetaFromFulldSdnu,100,1e14,3);
    TF1 *totally = new TF1("totally",getBetaCombined,100,1e14,3);
    bremy->SetParameters(1,11.89,6.6);
    pairy->SetParameters(1,11.89,6.6);
    photoy->SetParameters(1,11.89,6.6);
    totally->SetParameters(1,11.89,6.6);
    bremy->SetLineColor(50);
    pairy->SetLineColor(40);
    photoy->SetLineColor(8);
    totally->SetLineColor(30);
    bremy->SetLineStyle(4);
    pairy->SetLineStyle(3);
    photoy->SetLineStyle(2);
    totally->SetLineStyle(1);
    bremy->Draw("same");
    pairy->Draw("same");
    photoy->Draw("same");
    totally->Draw("same");

    TLegend *leg = new TLegend(0.15,0.65,0.4,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->AddEntry(totally,"Combined","l");
    leg->AddEntry(bremy,"Bremsstrahlung","l");
    leg->AddEntry(pairy,"Pair Production","l");
    leg->AddEntry(photoy,"Photonuclear","l");
    leg->Draw();
}

