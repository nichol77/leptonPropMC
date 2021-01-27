#define interactionTree_cxx
#include "interactionTree.h"
#include "InteractionInfo.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


void interactionTree::Loop(Double_t energyThresh)
{
//   In a ROOT session, you can do:
//      Root > .L interactionTree.C
//      Root > interactionTree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   char histTitle[180];
   TH1F *histNumInts = new TH1F("histNumInts","Number of Interactions",1000,-0.5,999.5);
   TH1F *histShower = new TH1F("histShower","Energy deposited in shower",1000,0,12);
   TH1F *histEMShower = new TH1F("histEMShower","Energy deposited in EM shower",1000,0,12);
   TH1F *histHadShower = new TH1F("histHadShower","Energy deposited in hadronic shower",1000,0,12);
   TH1F *histEMFraction = new TH1F("histEMFraction","Fraction of energy deposited in EM shower",1000,0,1);
   TH1F *histHadFraction = new TH1F("histHadFraction","Fraction of energy deposited in Had shower",1000,0,1);
   TH1F *histTime = new TH1F("histTime","Relative time of Shower",1000,0,1e8);
   sprintf(histTitle,"Interactions above %1.1e GeV",energyThresh);
   TH1F *histNumAbove = new TH1F("histNumAbove",histTitle,100,-0.5,99.5);
   Long64_t nentries = fChain->GetEntriesFast();

   Int_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      Int_t numAboveThresh=0;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      histNumInts->Fill(event->fInts->GetEntries());
      for(int count=0;count<event->fInts->GetEntries();count++) {
	  InteractionInfo *intInfo = (InteractionInfo*) event->fInts->At(count);
	  Double_t emEnergy=intInfo->intEnergy*(intInfo->fracEMShower);
	  Double_t hadEnergy=intInfo->intEnergy*(intInfo->fracHadShower);
	  Double_t theTime=intInfo->intTime*1e9; //Now in nanoseconds
	  histShower->Fill(TMath::Log10(intInfo->intEnergy*(intInfo->fracEMShower+intInfo->fracHadShower)));
	  if(emEnergy>energyThresh) {
	      histEMShower->Fill(TMath::Log10(emEnergy));
	      histTime->Fill(theTime);
	      numAboveThresh++;
	  }
	  if(hadEnergy>energyThresh) {
	      histHadShower->Fill(TMath::Log10(hadEnergy));
	      histTime->Fill(theTime);
	      numAboveThresh++;
	  }	      
	  histEMFraction->Fill(intInfo->fracEMShower);
	  histHadFraction->Fill(intInfo->fracHadShower);

      }
      histNumAbove->Fill(numAboveThresh);
      // if (Cut(ientry) < 0) continue;
   }
//   histNumInts->Draw();

   TCanvas *canShower = new TCanvas("canShower","canShower");
   canShower->Divide(1,3);
   canShower->cd(1);
   histShower->Draw();
   canShower->cd(2);
   histEMShower->Draw();
   canShower->cd(3);
   histHadShower->Draw();

   TCanvas *canFrac = new TCanvas("canFrac","canFrac");
   canFrac->Divide(1,2);
   canFrac->cd(1);
   histEMFraction->Draw();
   canFrac->cd(2);
   histHadFraction->Draw();

   TCanvas *canTime = new TCanvas("canTime","canTime");
   histTime->Draw();
   TCanvas *canNum = new TCanvas("canNum","canNum");
   histNumAbove->Draw();
}

