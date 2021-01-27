


void findNormal() {

    Double_t x[3]={9000,10000,11000};
    Double_t z[3]={6000,7000,8000};

    TGraph *gr = new TGraph(3,x,z);
    gr->Draw("ap");

}
