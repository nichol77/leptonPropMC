#define RayTree_cxx
#include "RayTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
using namespace std;

void RayTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L RayTree.C
//      Root > RayTree t
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

   TH1F *histTime = new TH1F("histTime","histTime",1000,0,1e7);
   TH1F *histDeltaT = new TH1F("histDeltaT","histDeltaT",1000,0,1e5);
   
   Long64_t nentries = fChain->GetEntriesFast();

   Int_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
//      cout << jentry << "\t" << rfevent->fNumSubWaves << endl;
      Double_t lastTime=0;
      for(int count=0;count<rfevent->fNumSubWaves;count++) {
//	  cout << "Here\t" << count << "\t" << rfevent->fSubWaves->GetEntries() << endl;
	  
	  SubWaveInfo *swInfo = (SubWaveInfo*) rfevent->fSubWaves->At(count);
	  Double_t rayTravelTime=swInfo->fRayTravelTime;
	  Double_t interactionTime=swInfo->fIntInfo.getIntTime();
//	  cout << rayTravelTime << "\t" << interactionTime << endl;
	  histTime->Fill((rayTravelTime+interactionTime)*1e9);
	  if(lastTime>0) histDeltaT->Fill(((rayTravelTime+interactionTime)*1e9)-lastTime);
	  lastTime=(rayTravelTime+interactionTime)*1e9;
      }

   }
   histDeltaT->Draw();
}
