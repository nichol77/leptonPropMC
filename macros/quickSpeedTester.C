#include "interactions.h"
#include "TStopwatch.h"

#include <iostream>
using namespace std;


void quickSpeedTester() {
    Double_t energy=1e9;
    Double_t length=64400;
    Double_t isAtau=1;
    Double_t material=2;
    Double_t nu_cut=0.001;

    TStopwatch watchy;
    watchy.Start(1);
    for(int i=0;i<20;i++) {
	getNewEnergy(energy,length,isAtau,material,nu_cut);	
    }
    watchy.Stop();
    cout << watchy.RealTime() << "\t" << watchy.CpuTime() << endl;

}
