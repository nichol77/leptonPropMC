#include <iostream>
#include <fstream>

using namespace std;

//#include "../interactions.h"
#include "../interactions.cxx"
#include "../PhysicalConstants.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TH1.h"
#include "TRandom.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"

void plotBetasForMuons();
void plotBetasForTaus();
void plotInteractionLength();
void plotIonization();
void plotContinuous();
void plotMeanFreePath();
void plotStrangePlot();
void plotLengthHistos();
void plotRanges(); 
void plotCrosssections();
void plotNuPP();
void plotNuBrem();
void plotNuPhoto();
void plotdEdxForMuons();
void plotPairAsFunctionOfNu();
void plotPhotoAsFunctionOfNu();
void softPhotoPlot();
void hardPhotoPlot();
void dumpStrangeNums();


void plotStuff() {
//    dumpStrangeNums();
//    plotPairAsFunctionOfNu();
//    plotPhotoAsFunctionOfNu();
//    softPhotoPlot();
//    hardPhotoPlot();

//    plotBetasForMuons();
//    plotBetasForTaus();
    plotRanges();
//    plotdEdxForMuons();
//    plotInteractionLength();
//     plotIonization();
//     plotContinuous();
     plotMeanFreePath();
//     plotStrangePlot();
//     plotLengthHistos();
//     plotCrosssections();
//     plotNuPP();
//     plotNuBrem();
//     plotNuPhoto();
}


void hardPhotoPlot()
{
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetTitleOffset(0.8,"y");
    Int_t myColours[8]={50,40,30,9,8,38,42,12};
//    Double_t myEnergies[8]={1e9,3e9,1e10,3e10,1e11,3e11,1e12,3e12};
    Double_t myEnergies[8]={1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12};
 
    TCanvas *canHardMuon = new TCanvas("canHardMuon","canHardMuon",600,400);
    TH1F *muFrame=canHardMuon->DrawFrame(1e-6,0,1,300,"'Hard' Photonuclear Cross-section (Muons)");
    muFrame->GetXaxis()->SetTitle("#nu");
    muFrame->GetYaxis()->SetTitle("#nu d#sigma/d#nu (#mub)");
    
    canHardMuon->SetLogx();
    TF1 *plotty = new TF1("plotty",funchardnudSdnu,1e-6,1,3);
    plotty->SetParameters(myEnergies[7],0,22);	
    char label[180];
    TLegend *legMuon = new TLegend(0.16,0.52,0.4,0.88);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);
    for(int i=7;i>=0;i--) {
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameters(myEnergies[i],0,22);	
	TF1 *tempPlot=plotty->DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",myEnergies[i]);
	legMuon->AddEntry(tempPlot,label,"l");
    }
    legMuon->Draw();

   TCanvas *canHardTau = new TCanvas("canHardTau","canHardTau",600,400);
    TH1F *tauFrame=canHardTau->DrawFrame(1e-6,0,1,200,"'Hard' Photonuclear Cross-section (Taus)");
    tauFrame->GetXaxis()->SetTitle("#nu");
    tauFrame->GetYaxis()->SetTitle("#nu d#sigma/d#nu (#mub)");
    canHardTau->SetLogx();
    plotty->SetParameters(myEnergies[7],1,22);
    TLegend *legTau = new TLegend(0.16,0.52,0.4,0.88);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);	
    for(int i=7;i>=0;i--) {
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameters(myEnergies[i],1,22);	
	TF1 *tempPlot=plotty->DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",myEnergies[i]);
	legTau->AddEntry(tempPlot,label,"l");
    }
    legTau->Draw();

    
}


void softPhotoPlot() 
{
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetTitleOffset(0.8,"y");
    Int_t myColours[8]={50,40,30,9,8,38,42,12};
//    Double_t myEnergies[8]={1e9,3e9,1e10,3e10,1e11,3e11,1e12,3e12};
    Double_t myEnergies[8]={1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12};

    TCanvas *canSoftMuon = new TCanvas("canSoftMuon","canSoftMuon",600,400);
    TH1F *muFrame=canSoftMuon->DrawFrame(1e-4,0,1,300,"'Soft' Photonuclear Cross-section (Muons)");
    muFrame->GetXaxis()->SetTitle("#nu");
    muFrame->GetYaxis()->SetTitle("#nu d#sigma/d#nu (#mub)");
    canSoftMuon->SetLogx();
    TF1 *plotty = new TF1("plotty",funcsoftnudSdnu,1e-6,1,4);
    char label[180];
    TLegend *legMuon = new TLegend(0.7,0.52,0.88,0.88);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);	
    for(int i=7;i>=0;i--) {
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameters(myEnergies[i],0,22,11);
	TF1 *tempPlot=plotty->DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",myEnergies[i]);
	legMuon->AddEntry(tempPlot,label,"l");
    }
    legMuon->Draw();
    
    TCanvas *canSoftTau = new TCanvas("canSoftTau","canSoftTau",600,400);
    TH1F *tauFrame=canSoftTau->DrawFrame(1e-4,0,1,200,"'Soft' Photonuclear Cross-section (Taus)");
    tauFrame->GetXaxis()->SetTitle("#nu");
    tauFrame->GetYaxis()->SetTitle("#nu d#sigma/d#nu (#mub)");
    canSoftTau->SetLogx();
    TLegend *legTau = new TLegend(0.7,0.52,0.88,0.88);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);				
    for(int i=7;i>=0;i--) {
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameters(myEnergies[i],1,22,11);
	TF1 *tempPlot=plotty->DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",myEnergies[i]);
	legTau->AddEntry(tempPlot,label,"l");
    }
    legTau->Draw();

}



void plotPhotoAsFunctionOfNu() 
{
    Int_t myColours[8]={50,40,30,9,8,38,42,12};
    Double_t myEnergies[8]={1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12};
    
    TCanvas *canPhotoNu = new TCanvas("canPhotoNu","canPhotoNu",600,400);
//    TH1F *muFrame=canMuNuPhoto->DrawFrame(1e-6,1e-30,1,1e-20,"Muon d#sigma/d#nu (Photonuclear)");
    canPhotoNu->SetLogx();
    TF1 *plotty = new TF1("plotty",funcPhotodSdnu,1e-4,1,4);
    plotty->SetTitle("Muon");
    plotty->SetLineColor(myColours[7]);
    plotty->SetParameters(myEnergies[7],0,22,11);
    plotty->DrawCopy();
    plotty->GetXaxis()->SetTitle("#nu");
    plotty->GetYaxis()->SetTitle("#nu d#{sigma}/d#{nu} (#{mu}b)");				
    for(int i=6;i>=0;i--) {
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameter(0,myEnergies[i]);
//	plotty->SetParameter(0,tauMass);
	plotty->DrawCopy("same");
    }
    
   //  TCanvas *canPhotoNu2 = new TCanvas("canPhotoNu2","canPhotoNu2");
//     canPhotoNu2->SetLogx();
//     plotty->SetTitle("Tau");
//     plotty->SetLineColor(myColours[7]);
//     plotty->SetParameter(0,myEnergies[7]);
//     plotty->SetParameter(1,1);
//     plotty->DrawCopy();
//     plotty->GetXaxis()->SetTitle("#nu");
//     plotty->GetYaxis()->SetTitle("#nu d#{sigma}/d#{nu} (#{mu}b)");				
//     for(int i=6;i>=0;i--) {
// 	plotty->SetLineColor(myColours[i]);
// 	plotty->SetParameter(0,myEnergies[i]);
// 	plotty->DrawCopy("same");
//     }

}



void plotPairAsFunctionOfNu() 
{
    TStopwatch watchy;
    watchy.Start(1);
    Int_t myColours[8]={50,40,30,9,8,38,42,12};
//    Double_t myEnergies[8]={1e2,1e3,1e4,1e5,1e6,1e9,1e11,1e12};
    Double_t myEnergies[8]={1e9,3e9,1e10,3e10,1e11,3e11,1e12,3e12};

    Double_t A=22;
    Double_t Z=11;
  
    TCanvas *canPairNu = new TCanvas("canPairNu","canPairNu");
    canPairNu->SetLogx();
//    canPairNu->SetLogy();
    TF1 *plotty = new TF1("plotty",funcPairdSdnu,1e-6,1,4);
    plotty->SetTitle("Muon");
    plotty->SetLineColor(myColours[7]);
    Double_t nuMin=getPairNuMin(myEnergies[7]);
    Double_t nuMax=getPairNuMax(myEnergies[7],muonMass,Z);
    plotty->SetParameters(myEnergies[7],0,A,Z);
//    plotty->SetRange(nuMin,nuMax);
    plotty->DrawCopy();
    plotty->GetXaxis()->SetTitle("#nu");
    plotty->GetYaxis()->SetTitle("#nu d#{sigma}/d#{nu} (#{mu}b)");				
    for(int i=6;i>=0;i--) {
	nuMin=getPairNuMin(myEnergies[i]);
	nuMax=getPairNuMax(myEnergies[i],muonMass,Z);
	if(nuMin>1e-6) 
	    plotty->SetRange(nuMin,nuMax);
	else
	    plotty->SetRange(1e-6,nuMax);
	plotty->SetLineColor(myColours[i]);
	plotty->SetParameter(0,myEnergies[i]);
//	plotty->SetParameter(0,tauMass);
	plotty->DrawCopy("same");
	canPairNu->Update();
	canPairNu->Modified();
	watchy.Stop();
	cout << myEnergies[i] << "\t" << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;
	watchy.Start(1);
	
 
    }
    
//     TCanvas *canPairNu2 = new TCanvas("canPairNu2","canPairNu2");
//     canPairNu2->SetLogx();
//     plotty->SetTitle("Tau");
//     plotty->SetLineColor(myColours[7]);
//     plotty->SetParameter(0,myEnergies[7]);
//     plotty->SetParameter(1,1);
//     nuMin=getPairNuMin(myEnergies[7]);
//     nuMax=getPairNuMax(myEnergies[7],tauMass,Z);
// //	plotty->SetRange(nuMin,nuMax);
//     plotty->SetRange(1e-6,nuMax);
//     plotty->DrawCopy();
//     plotty->GetXaxis()->SetTitle("#nu");
//     plotty->GetYaxis()->SetTitle("#nu d#{sigma}/d#{nu} (#{mu}b)");				
//     for(int i=6;i>=0;i--) {
// 	nuMin=getPairNuMin(myEnergies[i]);
// 	nuMax=getPairNuMax(myEnergies[i],tauMass,Z);
// 	if(nuMin>1e-6) 
// 	    plotty->SetRange(nuMin,nuMax);
// 	else
// 	    plotty->SetRange(1e-6,nuMax);
// 	plotty->SetLineColor(myColours[i]);
// 	plotty->SetParameter(0,myEnergies[i]);
// 	plotty->DrawCopy("same");
// 	canPairNu2->Update();
// 	canPairNu2->Modified();
// 	watchy.Stop();
// 	cout << myEnergies[i] << "\t" << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;
// 	watchy.Start(1);
	
 
//     }
    watchy.Stop();
    cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;

}


void plotLengthHistos() 
{
    TCanvas *canLength = new TCanvas("canLength","Lengths (approx)");
    canLength->SetLogy();
    TF1 *freey = new TF1("freey",funcMeanFreePath,1e7,1e12,3);
    freey->SetParameters(0,0,0.001);
    Double_t Lbar=freey->Eval(1e12);
    cout << Lbar << endl;
    TRandom myRandom;
    TH1F *histLength = new TH1F("histLength","histLength",100,0,50000);
    for(int i=0;i<10000;i++) {
	Double_t eta=myRandom.Rndm();
	Double_t Lapprox=-1*TMath::Log(eta)*Lbar;
	histLength->Fill(Lapprox);
    }
    histLength->Draw();

}

void plotNuPP() {
    gStyle->SetPadTopMargin(0.14);
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetTitleOffset(0.85,"y");

    TCanvas *canMuNuPP = new TCanvas("canMuNuPP","dSigma/dnu",600,400);
    canMuNuPP->SetLogy();
    canMuNuPP->SetLogx();
    TH1F *muFrame=canMuNuPP->DrawFrame(1e-6,1e-30,1,1e-20,"Muon d#sigma/d#nu (Pair Production)");

    muFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    muFrame->GetXaxis()->SetTitle("#nu");
    Double_t startEnergy=1e12;
    Double_t isAtau=0;
    Double_t material=2;
    Double_t Z=6.6;
    Double_t A=11.89;
    switch (int(material+0.1)) 
    {	
	case 1: //Rock
	    Z=11;
	    A=22;
	    break;	
	case 2: //Antarctic Ice
	    break;	    
	default:
	    //Stick with water
	    break;
    }


    Double_t energy=startEnergy;  

//     TF1 myBremdSdNu("myBremdSdNu",funcBremdSdnu,0,1,4);
//     myBremdSdNu.SetParameters(energy,isAtau,A,Z);
    TF1 myPairdSdNu("myPairdSdNu",funcSlowPairdSdnu,0,1,4);
    myPairdSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myPhotodSdNu("myPhotodSdNu",funcPhotodSdnu,0,1,4);
//     myPhotodSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myKnockOndSdNu("myKnockOndSdNu",funcKnockOndSdnu,0,1,4);
//     myKnockOndSdNu.SetParameters(energy,isAtau,A,Z);
    Double_t nuMin=1e-6;
    Double_t nuMax=1;

    Int_t colours[6]={50,38,40,30,9,14};
//    Double_t newEnergy=energy;
    isAtau=0;
    TF1 *tempMuon[5];
    TF1 *tempTau[5];
    Double_t energies[5]={1e12,1e11,1e10,1e9,1e8};
    char label[180];
    TLegend *legMuon = new TLegend(0.6,0.5,0.9,0.85);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myPairdSdNu.SetParameter(0,newEnergy);
	myPairdSdNu.SetParameter(1,isAtau);
	myPairdSdNu.SetLineColor(colours[index]);
	nuMin=getPairNuMin(newEnergy);
	if(1e-6>nuMin) nuMin=1e-6;

	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	nuMax=getPairNuMax(newEnergy,leptonMass,Z);
	myPairdSdNu.SetRange(nuMin,nuMax);    
	tempMuon[index]=myPairdSdNu.DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",energies[index]);
	legMuon->AddEntry(tempMuon[index],label,"l");
    }
    legMuon->Draw();


    TCanvas *canTauNuPP = new TCanvas("canTauNuPP","dSigma/dnu",600,400);
    canTauNuPP->SetLogy();
    canTauNuPP->SetLogx();
    TH1F *tauFrame=canTauNuPP->DrawFrame(1e-6,1e-30,1,1e-20,"Tau d#sigma/d#nu (Pair Production)");
    tauFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    tauFrame->GetXaxis()->SetTitle("#nu");
    isAtau=1;

    TLegend *legTau = new TLegend(0.6,0.5,0.9,0.85);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myPairdSdNu.SetParameter(0,newEnergy);
	myPairdSdNu.SetParameter(1,isAtau);
	myPairdSdNu.SetLineColor(colours[index]);
	nuMin=getPairNuMin(newEnergy);
	if(1e-6>nuMin) nuMin=1e-6;
	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	nuMax=getPairNuMax(newEnergy,leptonMass,Z);
	myPairdSdNu.SetRange(nuMin,nuMax);    
	tempTau[index]=myPairdSdNu.DrawCopy("same");

	sprintf(label,"E = %1.0e GeV",energies[index]);
	legTau->AddEntry(tempTau[index],label,"l");
    }
    legTau->Draw();

}

 
void plotNuBrem() {
    gStyle->SetPadTopMargin(0.14);
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetTitleOffset(0.85,"y");

    TCanvas *canMuNuBrem = new TCanvas("canMuNuBrem","dSigma/dnu",600,400);
    canMuNuBrem->SetLogy();
    canMuNuBrem->SetLogx();
    TH1F *muFrame=canMuNuBrem->DrawFrame(1e-6,1e-30,1,1e-20,"Muon d#sigma/d#nu (Bremsstrahlung)");

    muFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    muFrame->GetXaxis()->SetTitle("#nu");
    Double_t startEnergy=1e12;
    Double_t isAtau=0;
    Double_t material=2;
    Double_t Z=6.6;
    Double_t A=11.89;
    switch (int(material+0.1)) 
    {	
	case 1: //Rock
	    Z=11;
	    A=22;
	    break;	
	case 2: //Antarctic Ice
	    break;	    
	default:
	    //Stick with water
	    break;
    }


    Double_t energy=startEnergy;  

    TF1 myBremdSdNu("myBremdSdNu",funcBremdSdnu,0,1,4);
    myBremdSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myPairdSdNu("myPairdSdNu",funcSlowPairdSdnu,0,1,4);
//     myPairdSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myPhotodSdNu("myPhotodSdNu",funcPhotodSdnu,0,1,4);
//     myPhotodSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myKnockOndSdNu("myKnockOndSdNu",funcKnockOndSdnu,0,1,4);
//     myKnockOndSdNu.SetParameters(energy,isAtau,A,Z);
    Double_t nuMin=1e-6;
    Double_t nuMax=1;

    Int_t colours[6]={50,38,40,30,9,14};
//    Double_t newEnergy=energy;
    isAtau=0;
    TF1 *tempMuon[5];
    TF1 *tempTau[5];
    Double_t energies[5]={1e12,1e11,1e10,1e9,1e8};
    char label[180];
    TLegend *legMuon = new TLegend(0.6,0.5,0.9,0.85);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myBremdSdNu.SetParameter(0,newEnergy);
	myBremdSdNu.SetParameter(1,isAtau);
	myBremdSdNu.SetLineColor(colours[index]);
	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	nuMax=getBremNuMax(newEnergy,leptonMass,Z);
	myBremdSdNu.SetRange(nuMin,nuMax);    
	tempMuon[index]=myBremdSdNu.DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",energies[index]);
	legMuon->AddEntry(tempMuon[index],label,"l");
    }
    legMuon->Draw();


    TCanvas *canTauNuBrem = new TCanvas("canTauNuBrem","dSigma/dnu",600,400);
    canTauNuBrem->SetLogy();
    canTauNuBrem->SetLogx();
    TH1F *tauFrame=canTauNuBrem->DrawFrame(1e-6,1e-30,1,1e-20,"Tau d#sigma/d#nu (Bremsstrahlung)");
    tauFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    tauFrame->GetXaxis()->SetTitle("#nu");
    isAtau=1;

    TLegend *legTau = new TLegend(0.6,0.5,0.9,0.85);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myBremdSdNu.SetParameter(0,newEnergy);
	myBremdSdNu.SetParameter(1,isAtau);
	myBremdSdNu.SetLineColor(colours[index]);
	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	nuMax=getBremNuMax(newEnergy,leptonMass,Z);
	myBremdSdNu.SetRange(nuMin,nuMax);    
	tempTau[index]=myBremdSdNu.DrawCopy("same");

	sprintf(label,"E = %1.0e GeV",energies[index]);
	legTau->AddEntry(tempTau[index],label,"l");
    }
    legTau->Draw();

}
 
void plotNuPhoto() {
    gStyle->SetPadTopMargin(0.14);
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetTitleOffset(0.85,"y");

    TCanvas *canMuNuPhoto = new TCanvas("canMuNuPhoto","dSigma/dnu",600,400);
    canMuNuPhoto->SetLogy();
    canMuNuPhoto->SetLogx();
    TH1F *muFrame=canMuNuPhoto->DrawFrame(1e-6,1e-30,1,1e-20,"Muon d#sigma/d#nu (Photonuclear)");

    muFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    muFrame->GetXaxis()->SetTitle("#nu");
    Double_t startEnergy=1e12;
    Double_t isAtau=0;
    Double_t material=2;
    Double_t Z=6.6;
    Double_t A=11.89;
    switch (int(material+0.1)) 
    {	
	case 1: //Rock
	    Z=11;
	    A=22;
	    break;	
	case 2: //Antarctic Ice
	    break;	    
	default:
	    //Stick with water
	    break;
    }


    Double_t energy=startEnergy;  

    TF1 myPhotodSdNu("myPhotodSdNu",funcPhotodSdnu,0,1,4);
    myPhotodSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myPairdSdNu("myPairdSdNu",funcSlowPairdSdnu,0,1,4);
//     myPairdSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myPhotodSdNu("myPhotodSdNu",funcPhotodSdnu,0,1,4);
//     myPhotodSdNu.SetParameters(energy,isAtau,A,Z);
//     TF1 myKnockOndSdNu("myKnockOndSdNu",funcKnockOndSdnu,0,1,4);
//     myKnockOndSdNu.SetParameters(energy,isAtau,A,Z);
    Double_t nuMin=1e-6;
    Double_t nuMax=1;

    Int_t colours[6]={50,38,40,30,9,14};
//    Double_t newEnergy=energy;
    isAtau=0;
    TF1 *tempMuon[5];
    TF1 *tempTau[5];
    Double_t energies[5]={1e12,1e11,1e10,1e9,1e8};
    char label[180];
    TLegend *legMuon = new TLegend(0.6,0.5,0.9,0.85);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myPhotodSdNu.SetParameter(0,newEnergy);
	myPhotodSdNu.SetParameter(1,isAtau);
	myPhotodSdNu.SetLineColor(colours[index]);
	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	myPhotodSdNu.SetRange(nuMin,nuMax);    
	tempMuon[index]=myPhotodSdNu.DrawCopy("same");
	sprintf(label,"E = %1.0e GeV",energies[index]);
	legMuon->AddEntry(tempMuon[index],label,"l");
    }
    legMuon->Draw();


    TCanvas *canTauNuPhoto = new TCanvas("canTauNuPhoto","dSigma/dnu",600,400);
    canTauNuPhoto->SetLogy();
    canTauNuPhoto->SetLogx();
    TH1F *tauFrame=canTauNuPhoto->DrawFrame(1e-6,1e-30,1,1e-20,"Tau d#sigma/d#nu (Photonuclear)");
    tauFrame->GetYaxis()->SetTitle("d#sigma/d#nu (cm^{2})");
    tauFrame->GetXaxis()->SetTitle("#nu");
    isAtau=1;

    TLegend *legTau = new TLegend(0.6,0.5,0.9,0.85);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);
    for(Int_t index=0;index<5;index++) {
	Double_t newEnergy=energies[index];
	myPhotodSdNu.SetParameter(0,newEnergy);
	myPhotodSdNu.SetParameter(1,isAtau);
	myPhotodSdNu.SetLineColor(colours[index]);
	Double_t leptonMass=muonMass;
	if(isAtau) leptonMass=tauMass;
	myPhotodSdNu.SetRange(nuMin,nuMax);    
	tempTau[index]=myPhotodSdNu.DrawCopy("same");

	sprintf(label,"E = %1.0e GeV",energies[index]);
	legTau->AddEntry(tempTau[index],label,"l");
    }
    legTau->Draw();

}



void plotStrangePlot() 
{
    TCanvas *canStrange = new TCanvas("canStrange","Strange Thing",600,400);
    canStrange->SetTopMargin(0.14);
    canStrange->SetLeftMargin(0.12);
    canStrange->SetLogy();
    canStrange->SetLogx();

    TStopwatch watchy;
//    watchy.Start(1);
//     Double_t newEnergy=getNewEnergy(1e12,1e5,0,2,0.001);
//     watchy.Stop();
//     cout << "New Energy: "<< newEnergy << endl;
//     cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;

    watchy.Start(1);
//    TF1 *strangey = new TF1("strangey",funcOneOverCEL,1e8,1e12,3);
    TF1 *strangey = new TF1("strangey",newFuncOneOverCEL,1e8,1e12,3);
    strangey->SetLineColor(50);
    strangey->SetParameters(1,2,0.001); // isAtau, material, nu_cut
    strangey->Draw();
//    TGraph *grIntegral=computeRightIntegral(strangey, 1e12, 9.8e11, 100) ;
//    TGraph *grIntegral=getRightIntegral(strangey,1e12,1e5,100);
//    grIntegral->Draw("ap");
    canStrange->Update();
    canStrange->Modified();
    watchy.Stop();
    cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;
}

void dumpStrangeNums()
{
    Double_t theEnergies[1000];
    Double_t theMuonVals[1000];
    Double_t theTauVals[1000];
    Int_t numPoints=0;

    TF1 strangey("strangey",funcOneOverCEL,1e8,1e12,3);
    strangey.SetParameters(0,2,0.001); // isAtau, material, nu_cut
    TF1 strangey2("strangey2",funcOneOverCEL,1e8,1e12,3);
    strangey2.SetParameters(1,2,0.001); // isAtau, material, nu_cut

    for(Double_t exponent=1e7;exponent<=1e12;exponent*=10) {
	for(Double_t multiplier=1;multiplier<10;multiplier*=1.1) {
	    Double_t energy=multiplier*exponent;
	    Double_t value=strangey.Eval(energy);
	    Double_t value2=strangey2.Eval(energy);
//	    cout << energy << "\t" << strangey.Eval(energy) << endl;
	    theEnergies[numPoints]=energy;
	    theMuonVals[numPoints]=value;
	    theTauVals[numPoints]=value2;
	    numPoints++;	    
	}
    }

    cout << "Double_t fOneOverCELEnergy[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << theEnergies[i] << ",";    
    cout << theEnergies[numPoints-1] << "};" << endl;
    cout << endl << endl;
    
    cout << "Double_t fOneOverCELMuon[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << theMuonVals[i] << ",";    
    cout << theMuonVals[numPoints-1] << "};" << endl;
    cout << endl << endl;
    
    cout << "Double_t fOneOverCELTau[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << theTauVals[i] << ",";    
    cout << theTauVals[numPoints-1] << "};" << endl;
    cout << endl << endl;
    
}

void plotMeanFreePath()
{
    TCanvas *canFree = new TCanvas("canFree","Mean Free Path",600,400);
    canFree->SetTopMargin(0.14);
    canFree->SetLeftMargin(0.12);
    canFree->SetLogy();
    canFree->SetLogx();
    TH1F *framey=canFree->DrawFrame(1e8,1e-1,1e12,1e7,"Mean Free Path");
    framey->GetXaxis()->SetTitle("Energy (GeV)");
    TStopwatch watchy;
    watchy.Start(1);
    TF1 *freey = new TF1("freey",funcMeanFreePath,1e8,1e12,3);
    freey->SetLineColor(50);
    freey->SetParameters(0,2,0.001); // isAtau, material, nu_cut
    freey->DrawCopy("same");
    freey->SetLineColor(30);
    freey->SetParameters(1,2,0.001); // isAtau, material, nu_cut
    freey->DrawCopy("same");

    canFree->Update();
    canFree->Modified();
    watchy.Stop();
    cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;

}


void plotContinuous()
{
    TCanvas *can = new TCanvas("can","Continuous",600,400);
    can->SetTopMargin(0.14);
    can->SetLeftMargin(0.12);
    can->SetLogy();
    can->SetLogx();
    TH1F *framey=can->DrawFrame(1e8,1e-1,1e12,1e7,"Continuous Energy Loss");
    framey->GetXaxis()->SetTitle("Energy (GeV)");
    framey->GetYaxis()->SetTitle("dE/dx (GeV cm^{-1})");
    framey->GetYaxis()->SetTitleOffset(0.8);
    TStopwatch watchy;
    watchy.Start(1);
    TF1 *celly = new TF1("celly",funcContinuousEnergyLoss,1e8,1e12,3);
    celly->SetLineColor(50);
    celly->SetParameters(0,2,0.001); // isAtau, material, nu_cut
    celly->DrawCopy("same");
    celly->SetLineColor(30);
    celly->SetParameters(1,2,0.001); // isAtau, material, nu_cut
    celly->DrawCopy("same");
 
    can->Update();
    can->Modified();
    watchy.Stop();
    cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;

}


void plotIonization()
{
    TF1 *iony = new TF1("iony",funcIonizationdEdx,1e7,1e14,2);
    iony->SetParameter(0,0); //isATau
    iony->SetParameter(1,2); //material == ice
    iony->Draw();
}


void plotInteractionLength()
{
//    gSystem->CompileMacro("interactions.cxx","k");
    TCanvas *canNeutrino = new TCanvas("canNeutrino","Neutrino Betas",600,400);
    canNeutrino->SetLogx(1);
    canNeutrino->SetLogy(1);
//     TF1 *neuy = new TF1("neuy",funcNuTotInteractionLength,1e7,1e14,2);
//     neuy->SetParameter(0,0);
//     neuy->SetParameter(1,0.92); // Density
//     neuy->Draw();
}


void plotdEdxForMuons() {   
    //  gSystem->CompileMacro("interactions.cxx","k");

    TCanvas *canMuon = new TCanvas("canMuon","Muon Betas",600,400);
    canMuon->SetLogx(1);
    canMuon->SetLogy(1);
    
    TH1F *frame = canMuon->DrawFrame(100,1e-17,1e14,1,"Muon Energy Loss (Water)");
    frame->GetXaxis()->SetTitle("Energy (GeV)");
    TF1 *plotty = new TF1("plotty",funcOneOverBeta,100,1e14,2);
    plotty->SetParameter(0,0);
    plotty->SetParameter(1,2); //Material
    plotty->Draw("same");    
}

void plotRanges() {   
//    gSystem->CompileMacro("interactions.cxx","k");
    TStopwatch watchy;
    watchy.Start(1);
    TCanvas *canRange = new TCanvas("canRange","Ranges",600,400);
    canRange->SetTopMargin(0.14);
    canRange->SetLeftMargin(0.12);
    canRange->SetLogx(1);
    canRange->SetLogy(1);
    
    TH1F *frame = canRange->DrawFrame(1e8,1,1e12,1e5,"Typical Interaction Scales (Ice)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("Range (km)");
    frame->GetYaxis()->SetTitleOffset(0.8);
    
    TF1 *rangey = new TF1("rangey",funcIntegralOneOverBeta,1e8,1e12,3);
    rangey->SetLineStyle(3);
    rangey->SetLineColor(50);
    rangey->SetParameter(0,1);
//    rangey->SetParameter(1,2); //Material
    rangey->SetParameter(1,0.92); //Density
    rangey->SetParameter(2,1e8); //Minimum Energy
    TF1 *tauRange=rangey->DrawCopy("same");
    
    rangey->SetLineStyle(1);
    rangey->SetLineColor(40);
    rangey->SetParameter(0,0);
    TF1 *muonRange=rangey->DrawCopy("same"); 
    cout << tauRange->Eval(1e9) << "\t" << muonRange->Eval(1e9) << endl;

    TF1 *decayy = new TF1("decayy",funcDecayRange,1e8,1e12,1);
    decayy->SetLineColor(44);
    decayy->SetParameter(0,1);
    TF1 *tauDecay=decayy->DrawCopy("same"); 
    decayy->SetLineColor(30);
    decayy->SetParameter(0,0);
//    TF1 *muonDecay=decayy->DrawCopy("same");  

    TF1 *neuy = new TF1("neuy",funcNuCCInteractionLength,1e8,1e12,2);
    neuy->SetParameter(0,0);
    neuy->SetParameter(1,0.92); // Density
    neuy->SetLineColor(8);
    neuy->SetLineStyle(4);

    neuy->Draw("same");


    TLegend *leg = new TLegend(0.2,0.65,0.5,0.85);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->AddEntry(tauRange,"Tau Range (E_{cut} = 10^{8} GeV)","l");
    leg->AddEntry(tauDecay,"Tau Decay","l");
    leg->AddEntry(muonRange,"Muon Range (E_{cut} = 10^{8} GeV)","l");
    leg->AddEntry(neuy,"Weak [L_{int}(#nu_{CC})]","l");
    leg->Draw();
    canRange->Update();
    canRange->Modified();
    watchy.Stop();
    cout << "Times: " << watchy.CpuTime() << "\t" << watchy.RealTime() << endl;

}  




void plotCrosssections() {   
//    gSystem->CompileMacro("interactions.cxx","k");
    Double_t nu_cut=0.00;
//    Double_t nu_cut=0.1;
    TCanvas *canCrossMuon = new TCanvas("canCrossMuon","Muon Cross-sections",600,400);
    canCrossMuon->SetTopMargin(0.14);
    canCrossMuon->SetLeftMargin(0.12);
    canCrossMuon->SetLogx(1);
    canCrossMuon->SetLogy(1);
    
    TH1F *frame = canCrossMuon->DrawFrame(1e8,100,1e12,1e4,"Muon Cross-sections (Ice)");
    frame->GetXaxis()->SetTitle("E (GeV)");
    frame->GetYaxis()->SetTitle("#sigma (#mub)");
    frame->GetYaxis()->SetTitleOffset(0.8);
    
    TF1 *crossy = new TF1("crossy",funcCrossAboveNuCut,1e8,1e12,4);
    crossy->SetLineStyle(3);
    crossy->SetLineColor(50);
    crossy->SetParameters(0,0,2,nu_cut); // interaction, isATau, material nu_cut
    TF1 *muonBremCross=crossy->DrawCopy("same"); 
    crossy->SetLineStyle(1);
    crossy->SetLineColor(40);
    crossy->SetParameter(0,1);
    TF1 *muonPairCross=crossy->DrawCopy("same");  
    crossy->SetLineStyle(2);
    crossy->SetLineColor(44);
    crossy->SetParameter(0,2);
    TF1 *muonPhotoCross=crossy->DrawCopy("same");  
    crossy->SetLineStyle(3);
    crossy->SetLineColor(8);
    crossy->SetParameter(0,3);
//    TF1 *muonKnockOnCross=crossy->DrawCopy("same"); 
    
    TLegend *legMuon = new TLegend(0.13,0.68,0.43,0.85);
    legMuon->SetBorderSize(0);
    legMuon->SetFillColor(0);
    legMuon->SetFillStyle(0);
    legMuon->AddEntry(muonBremCross,"Bremsstrahlung","l");
    legMuon->AddEntry(muonPairCross,"Pair Production","l");
    legMuon->AddEntry(muonPhotoCross,"Photonuclear","l");
//    legMuon->AddEntry(muonKnockOnCross,"Knock-on Cross","l");
    legMuon->Draw();

    TCanvas *canCrossTau = new TCanvas("canCrossTau","Cross-sections",600,400);
    canCrossTau->SetTopMargin(0.14);
    canCrossTau->SetLeftMargin(0.12);
    canCrossTau->SetLogx(1);
    canCrossTau->SetLogy(1);
    
    TH1F *frame2 = canCrossTau->DrawFrame(1e8,1e-1,1e12,1e4,"Tau Cross-sections (Ice)");
    frame2->GetXaxis()->SetTitle("E (GeV)");
    frame2->GetYaxis()->SetTitle("#sigma (#mub)");
    frame2->GetYaxis()->SetTitleOffset(0.8);
    
    crossy->SetLineStyle(3);
    crossy->SetLineColor(50);
    crossy->SetParameters(0,1,2,nu_cut);// interaction, isATau, material nu_cut

    TF1 *tauBremCross=crossy->DrawCopy("same"); 
    crossy->SetLineStyle(1);
    crossy->SetLineColor(40);
    crossy->SetParameter(0,1);
    TF1 *tauPairCross=crossy->DrawCopy("same");  
    crossy->SetLineStyle(2);
    crossy->SetLineColor(44);
    crossy->SetParameter(0,2);
    TF1 *tauPhotoCross=crossy->DrawCopy("same");  
    crossy->SetLineStyle(3);
    crossy->SetLineColor(8);
    crossy->SetParameter(0,3);
//    TF1 *tauKnockOnCross=crossy->DrawCopy("same"); 
    
    TLegend *legTau = new TLegend(0.13,0.68,0.43,0.85);
    legTau->SetBorderSize(0);
    legTau->SetFillColor(0);
    legTau->SetFillStyle(0);
    legTau->AddEntry(tauBremCross,"Bremsstrahlung","l");
    legTau->AddEntry(tauPairCross,"Pair Production","l");
    legTau->AddEntry(tauPhotoCross,"Photonuclear","l");
//    legTau->AddEntry(tauKnockOnCross,"Knock-on Cross","l");
    legTau->Draw();
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
