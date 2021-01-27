

void quickTempTextReader()
{
    ifstream Input("tempText2.txt");
    if(!Input) {
	cerr << "Couldn't bloody open the file." << endl;
	exit(0);
    }


    Double_t energy[100];
    Double_t sigCC[100];
    Double_t sigNC[100];
    Double_t sigTot[100];

    Int_t numPoints=0;
    while(Input >> energy[numPoints] >> sigCC[numPoints] 
	  >> sigNC[numPoints] >> sigTot[numPoints]) {
	numPoints++;
    }

    cout << "Double_t fNubarEnergy[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << energy[i] << ",";    
    cout << energy[numPoints-1] << "};" << endl;
    cout << "Double_t fNubarSigmaCC[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << sigCC[i] << ",";    
    cout << sigCC[numPoints-1] << "};" << endl;
    cout << "Double_t fNubarSigmaNC[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << sigNC[i] << ",";    
    cout << sigNC[numPoints-1] << "};" << endl;
    cout << "Double_t fNubarSigmaTot[" << numPoints << "]={";
    for(int i=0;i<numPoints-1;i++)  
	cout << sigTot[i] << ",";    
    cout << sigTot[numPoints-1] << "};" << endl;

}
