///////////////////////////////////////////////////////////////////////////////
/////   Propagator.cxx
/////   Contains information about the various interactions (primary and
/////   secondary)
///////////////////////////////////////////////////////////////////////////////
#include "Propagator.h"

#include <iostream>
#include "interactions.h"

//#include "TF1.h"
//#include "TRandom.h"
//#include "TMath.h"

using namespace std;

ClassImp(Propagator)

Propagator::Propagator()
    :fMedium(RyansCons::kIce),
     fNuCut(0.001),
     fEThresh(1e8)
{
//Default (zero) constructor 
}


Propagator::Propagator(RyansCons::MaterialType_t medium, 
		       Double_t nuCut, Double_t eThresh)
    :fMedium(medium),fNuCut(nuCut),fEThresh(eThresh)
{
    //Assignment constructor
    fRandom=new TRandom(); //Will add seed at some point
    fMeanFree=0;
    fBremdSdNu=0;
    fPairdSdNu=0;
    fPhotodSdNu=0;
    fKnockOndSdNu=0;
    fDecayRange=0;
    fWeakCCLength=0;   
}


Propagator::~Propagator()
{
//Default destructor    
}



Double_t Propagator::oneStep(NeutrinoEvent *nuEvent)
{
   
    //Does all the work, and returns remaining lepton energy
    RyansCons::ParticleType_t particle=nuEvent->getCurrentParticleType();
    Double_t startEnergy=nuEvent->getCurrentParticleEnergy();
    cout << "\toneStep\t" << particle << "\t" << startEnergy << endl;
    if(!RyansCons::isAChargedLepton(particle)) return 0;
    if(startEnergy<fEThresh) return startEnergy;
    Double_t isAtau=RyansCons::isItATau(particle);
    Double_t A=RyansCons::getAForMaterial(fMedium);
    Double_t Z=RyansCons::getZForMaterial(fMedium);
    Double_t density=RyansCons::getDensityForMaterial(fMedium);

    if(!fMeanFree) {
	fMeanFree = new TF1("freey",funcMeanFreePath,1e8,1e12,3);
	fMeanFree->SetParameters(isAtau,fMedium,fNuCut);
	fMeanFree->SetNpx(1000);
    }
    if(!fBremdSdNu) {
	fBremdSdNu = new TF1("myBremdSdNu",funcBremdSdnu,fNuCut,1,4);
	fBremdSdNu->SetParameters(startEnergy,isAtau,A,Z);
	fBremdSdNu->SetNpx(1000);
    }
    if(!fPairdSdNu) {       
	fPairdSdNu = new TF1("myPairdSdNu",funcPairdSdnu,fNuCut,1,4);
	fPairdSdNu->SetParameters(startEnergy,isAtau,A,Z);
	fPairdSdNu->SetNpx(1000);
    }
    if(!fPhotodSdNu) {
	fPhotodSdNu = new TF1("myPhotodSdNu",funcPhotodSdnu,fNuCut,1,4);
	fPhotodSdNu->SetParameters(startEnergy,isAtau,A,Z);
	fPhotodSdNu->SetNpx(1000);
    }
    if(!fKnockOndSdNu) {
	fKnockOndSdNu = new TF1("myKnockOndSdNu",funcKnockOndSdnu,fNuCut,1,4);
	fKnockOndSdNu->SetParameters(startEnergy,isAtau,A,Z);
	fKnockOndSdNu->SetNpx(1000);
    }
    if(!fDecayRange) {
	fDecayRange = new TF1("myDecayRange",funcDecayRangeCmWithRho,0,1e12,2);
	fDecayRange->SetParameters(isAtau,fMedium);
	fDecayRange->SetNpx(1000);
    }
    if(!fWeakCCLength) {
	fWeakCCLength = new TF1("myWeakLength",funcNuCCInteractionLengthInCM,1e8,1e12,2);
	fWeakCCLength->SetParameters(0,density);
	fWeakCCLength->SetNpx(1000);
    }

    Double_t LbarMeanFree=fMeanFree->Eval(startEnergy);
    Double_t LbarDecay=fDecayRange->Eval(startEnergy);
    Double_t LbarWeak=fWeakCCLength->Eval(startEnergy);
    Double_t eta=fRandom->Rndm();
    Double_t eta2=fRandom->Rndm();
    Double_t eta3=fRandom->Rndm();
    Double_t LApproxMeanFree=-1*TMath::Log(eta)*LbarMeanFree;
    Double_t LApproxDecay=-1*TMath::Log(eta2)*LbarDecay;
    Double_t LApproxWeak=-1*TMath::Log(eta3)*LbarWeak;

    Double_t newEnergy=0;
    RyansCons::InteractionType_t whichInt;
//     cout <<"\t\t" << LApproxMeanFree << "\t" << LApproxDecay << "\t"
// 	 << LApproxWeak << endl;
    if(LApproxDecay<LApproxMeanFree && LApproxDecay<LApproxWeak) {
	newEnergy=getNewEnergy(startEnergy,LApproxDecay,isAtau,fMedium,fNuCut,1);
	whichInt=RyansCons::kDecay;
    }
    else if(LApproxWeak <LApproxDecay && LApproxWeak<LApproxMeanFree) {
	newEnergy=getNewEnergy(startEnergy,LApproxWeak,isAtau,fMedium,fNuCut,1);
	whichInt=RyansCons::kCC;
    }		
    else {
	newEnergy=
	    getNewEnergy(startEnergy,LApproxMeanFree,isAtau,fMedium,fNuCut,1);
	whichInt=
	    whichInteraction(fRandom->Rndm(),newEnergy,isAtau,fMedium,fNuCut);
    }

    Double_t thisNu=fNuCut;
    Double_t nuMin=fNuCut;
    Double_t nuMax=1;
    Double_t oldEnergy;	 
    bool allOver=false;
    Double_t afterLength=LApproxMeanFree;

    switch(whichInt) {
	case RyansCons::kBrem: //Bremsstrahlung
	    // At these energies (>1e8 GeV) the dSdNu curve does not change, 
	    // and neither do nu_min 
	    // or nu_max significantly.
	    thisNu=fBremdSdNu->GetRandom();
	    break;
	case RyansCons::kPair: //Pair-production
	    // At these energies (>1e8 GeV) the dSdNu curve does not change, 
	    // and neither do nu_min 
	    // or nu_max significantly.
	    thisNu=fPairdSdNu->GetRandom();
	    break;
	case RyansCons::kPhoto: //Photo-nuclear
	    oldEnergy=fPhotodSdNu->GetParameter(0);
	    if((oldEnergy/newEnergy)>2 || newEnergy>oldEnergy)
		fPhotodSdNu->SetParameter(0,newEnergy);
	    thisNu=fPhotodSdNu->GetRandom();
	    break;
	case RyansCons::kKnockOn: //Knock-on
	    nuMin=fNuCut;
	    nuMax=getKnockOnNuMax(newEnergy,RyansCons::getMass(particle));
	    fKnockOndSdNu->SetRange(nuMin,nuMax);
	    fKnockOndSdNu->SetParameter(0,newEnergy);
	    thisNu=fKnockOndSdNu->GetRandom();
	    break;
	case RyansCons::kDecay: //Decay
	    allOver=true;
	    afterLength=LApproxDecay;
	    thisNu=0.5; // Bullshit warning
	    break;
	    
	case RyansCons::kCC: //Weak CC
	    allOver=true;
	    afterLength=LApproxWeak;
	    thisNu=0.5; //Bullshit warning
	    break;	    
	default:
	    std::cout << "No idea which interaction is number: " 
		      << whichInt << std::endl;
    }
//     cout << "\t\t" << newEnergy << "\t" << newEnergy*(1-thisNu) 
// 	 << "\t" << whichInt << endl;

    nuEvent->AddInteraction(newEnergy,afterLength,whichInt,thisNu*newEnergy);
    return newEnergy*(1.-thisNu);
}

Int_t Propagator::propagateToThreshold(NeutrinoEvent *nuEvent)
{
    Int_t count=0;
    while(this->oneStep(nuEvent)>fEThresh) {
// 	std::cout << "\t" << count << std::endl;
	count++;
    }
    return count;
}
