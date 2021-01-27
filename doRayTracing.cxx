#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "NeutrinoEvent.h"
#include "WorldModel.h"
#include "RayTracer.h"
#include "RFEvent.h"

#include "RyansConventions.h"

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"



int main(int argc, char **argv) {

    if(argc<2) {
	std::cerr << "Usage: " << argv[0] << " <energy (eg 1e19)>" << std::endl;
	return 0;
    }
    string fileSuffix=argv[1];
    string inFilename=
	"results/theInteractionOutputFile"+fileSuffix+".root";
    string outFilename=
	"results/theRFOutputFile"+fileSuffix+".root";
    //Input file stuff
    NeutrinoEvent *nuEvent=0;
    TFile *inFile = new TFile(inFilename.c_str(),"READ");
    TTree *nuTree = (TTree*) inFile->Get("theTree");
    nuTree->SetBranchAddress("event",&nuEvent);

    //Output file stuff
    RFEvent *rfEvent=0;
    TFile *outFile = new TFile(outFilename.c_str(),"RECREATE");
    TTree *rfTree = new TTree("rfTree","Tree of rays");
    rfTree->Branch("rfevent","RFEvent",&rfEvent,64000,0); //Need 64000,0 Why??
    
    WorldModel *wmPtr = new WorldModel();
    RayTracer *rtPtr = new RayTracer(wmPtr);

    int numEvents=nuTree->GetEntries();
    for(int entry=0;entry<numEvents;entry++) {
	if(entry%100==0) {
	    cout.precision(4);
	    cout << entry << "\t(" << float(100*entry)/float(numEvents) << "%)\n"; 
	}
	nuTree->GetEvent(entry);
//	rfEvent = new RFEvent(*nuEvent);
//	rtPtr->fillRFEvent(nuEvent,rfEvent);
	rfEvent = rtPtr->makeRFEvent(nuEvent);
	rfTree->Fill();
	delete rfEvent;
//	if(entry>10) break;
    }

    outFile->Write();
    inFile->Close();
    outFile->Close();
}
