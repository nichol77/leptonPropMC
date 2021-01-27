

void runRFLoop() {
    gSystem->Load("../lib/libPropagation.so");
    gSystem->CompileMacro("RayTree.C","k");
    TFile *fp = new TFile("../results/theRFOutputFile1e19.root");
    TTree *rfTree = (TTree*) fp->Get("rfTree");
    RayTree *t;
    if(rfTree) {
	t = new RayTree(rfTree);
    }
    else {
	cout << "Bollocks" << endl;
	return;
    }
	
   t->Loop();


}
