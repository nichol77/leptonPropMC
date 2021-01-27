

void runInteractionTreeLoop() {
    gSystem->Load("lib/libPropagation.so");
    gSystem->CompileMacro("interactionTree.cxx","k");
    TFile *fp = new TFile("results/theInteractionOutputFile1e21.root");
    TTree *theTree = (TTree*) fp->Get("theTree");
    interactionTree *t;
    if(theTree) {
	t = new interactionTree(theTree);
    }
    else {
	cout << "Bollocks" << endl;
	return;
    }
	
    t->Loop(1e9);


}
