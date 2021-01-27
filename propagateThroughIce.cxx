#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "NeutrinoEvent.h"
#include "Propagator.h"
#include "RyansConventions.h"

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"



int main(int argc, char **argv) {

    if(argc<2) {
	std::cerr << "Usage: " << argv[0] << " <uclaSim output file>" << std::endl;
	return 0;
    }
    string uclaFilename=argv[1];
    char garbage[180];
    int numLines=0;    

    ifstream NuFile(uclaFilename.c_str());
    if(!NuFile) {
	std::cout << "Couldn't open file: " << uclaFilename << std::endl;
	return 0;
    }
    
    //File variables
    int nuNumber;
    TVector3 interactionPos;
    double depth;
    TVector3 nuDir;
    TVector3 balloonPos;
    string flavour;
    string currentType;
    double elasticity;
    RyansCons::ParticleType_t nuFlavour; 
    RyansCons::InteractionType_t intType=RyansCons::kCC; 
    Double_t x,y,z;
    Double_t initialEnergy;

    //Output file stuff
    NeutrinoEvent *nuEvent=0;
    TFile *outFile = new TFile("theInteractionOutputFile.root","RECREATE");
    TTree *theTree = new TTree("theTree","Tree of Interactions");
    theTree->Branch("event","NeutrinoEvent",&nuEvent,64000,0);


 
    //Propagation stuff
    Propagator thePropagator(RyansCons::kIce);


    //Dump first three lines
    NuFile >> garbage >> garbage >> garbage >> initialEnergy;
    initialEnergy/=1e9;
    cout << "Energy: " << initialEnergy << endl;
    NuFile.getline(garbage,179);
    NuFile.getline(garbage,179);
    NuFile.getline(garbage,179);

    while(NuFile >> nuNumber >> x >> y >> z) {
	cout << numLines << endl;
	interactionPos.SetXYZ(x,y,z);
	NuFile >> depth >> x >> y >> z;
	nuDir.SetXYZ(x,y,z);
	NuFile >> x >> y >> z;
	balloonPos.SetXYZ(x,y,z);
	NuFile >> flavour >> currentType >> elasticity;

	nuFlavour=RyansCons::kNothing;
	if(flavour==string("nue")) nuFlavour=RyansCons::kNue;
	if(flavour==string("numu")) nuFlavour=RyansCons::kNumu;
	if(flavour==string("nutau")) nuFlavour=RyansCons::kNutau;
	if(currentType==string("cc")) intType=RyansCons::kCC;
	if(currentType==string("nc")) intType=RyansCons::kNC;

	numLines++;
	nuEvent = new NeutrinoEvent(initialEnergy,nuFlavour,interactionPos,
				    nuDir,intType,elasticity,balloonPos);
	
	thePropagator.propagateToThreshold(nuEvent);
	theTree->Fill();
	delete nuEvent;
    }
    cout << numLines << endl;
    outFile->Write();
}
