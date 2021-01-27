//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May  2 13:50:20 2005 by ROOT version 4.03/02
// from TTree interactionTree/Tree of Interactions
// found on file: results/theInteractionOutputFile1e19.root
//////////////////////////////////////////////////////////

#ifndef interactionTree_h
#define interactionTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "NeutrinoEvent.h"

class interactionTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   NeutrinoEvent   *event;

   // List of branches
   TBranch        *b_event;   //!

   interactionTree(TTree *tree=0);
   virtual ~interactionTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Double_t energyThresh=1e8);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef interactionTree_cxx
interactionTree::interactionTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("results/theInteractionOutputFile1e19.root");
      if (!f) {
         f = new TFile("results/theInteractionOutputFile1e19.root");
      }
      tree = (TTree*)gDirectory->Get("theTree");

   }
   Init(tree);
}

interactionTree::~interactionTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t interactionTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t interactionTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void interactionTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set object pointer
   event = 0;
   // Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event",&event);
   Notify();
}

Bool_t interactionTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_event = fChain->GetBranch("event");

   return kTRUE;
}

void interactionTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t interactionTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef interactionTree_cxx
