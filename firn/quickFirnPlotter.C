Double_t firnDensityFromTop(Double_t x);
Double_t simpleRefractiveIndex(Double_t *x, Double_t *par);
Double_t getFirnRefractiveIndex(Double_t depth);
Double_t funcd2zdu2(Double_t *z, Double_t *par);



void quickFirnPlotter() {


    Double_t z0=-1000;
    Double_t u0=0;
    Double_t uEnd=10000;
    Double_t zEnd=20000;

    TF1 *simpy = new TF1("simpy",simpleTime,u0,uEnd,4);
    simpy->SetParameters(u0,z0,uEnd,zEnd);
    simpy->SetNpx(10000);
    Double_t ui=simpy->GetMinimumX();
//    simpy->Draw("al");
    cout << ui << endl;

    TF1 *liney = new TF1("liney",simpleLine,u0,uEnd,5);
    liney->SetParameters(u0,z0,uEnd,zEnd,ui);
    liney->SetNpx(10000);
    liney->Draw("al");


    TF1 firny("firny",simpleRefractiveIndex,-2000,1000000,0);

    Double_t sinTheta0=(ui-u0)/TMath::Sqrt(z0*z0+(ui-u0)*(ui-u0));
//    Double_t theta0=TMath::ATan((u0-ui)/z0); //Radians    
    Int_t numSteps=10000;

    Double_t *theUs = new Double_t[numSteps];
    Double_t *theZs = new Double_t[numSteps];
    
    Double_t stepSize=(uEnd-u0)/Double_t(numSteps);
    Double_t theta0rad=TMath::ASin(sinTheta0);
    Double_t n0=firny.Eval(z0);
    Double_t k=sinTheta0*n0;
    cout << theta0rad*TMath::RadToDeg() << "\t" << n0 << "\t" << k 
	 << "\t" << TMath::RadToDeg()*TMath::ASin(k) << endl;
    cout << "Critical: " << TMath::RadToDeg()*TMath::ASin(1/1.79) << endl;
    
    cout << "End height: " << (uEnd-ui)/TMath::Tan(TMath::ASin(k)) << endl;


    theUs[0]=u0;
    theZs[0]=z0;
    for(int step=1;step<numSteps;step++) {
	Double_t n=firny.Eval(theZs[step-1]);
	Double_t sinT=k/n;
	Double_t sin2T=sinT*sinT;
	Double_t du=stepSize;
	Double_t dz=TMath::Sqrt((du*du-sin2T*du*du)/sin2T);
	theUs[step]=theUs[step-1]+du;
	theZs[step]=theZs[step-1]+dz;

//	cout << n << "\t" << theUs[step] << "\t" << theZs[step] << endl;

    }
    

    TGraph *grNew = new TGraph(numSteps,theUs,theZs);
    grNew->Draw("l");

    cout << liney->Eval(10000) << "\t" << theUs[numSteps-1] << "\t" << theZs[numSteps-1] << endl;

//    TF1 *firny = new TF1("firny",firnDensityFromTop,0,200,0);
//    firny->Draw();

//    TF1 *firny = new TF1("firny",simpleRefractiveIndex,0,200,0);
//    firny->Draw();

//     TF1 *strange = new TF1("strange",funcdudz,-1000,0,1);
//     for(double k=0.1;k<1;k+=0.1) {
// 	strange->SetParameter(0,k);
// 	if(k>0.1) {
// 	    strange->DrawClone("same");
// 	}
// 	else strange->DrawClone();
//     }



}

Double_t simpleLine(Double_t *x, Double_t *par) {
    Double_t u0=par[0];
    Double_t z0=par[1];
    Double_t u1=par[2];
    Double_t z1=par[3];
    Double_t ui=par[4];
    Double_t u=x[0];
    if(u<ui) return z0-z0*(u-u0)/(ui-u0);
    return z1*(u-ui)/(u1-ui);
}

Double_t simpleTime(Double_t *x, Double_t *par) {
    Double_t u0=par[0];
    Double_t z0=par[1];
    Double_t u1=par[2];
    Double_t z1=par[3];

    Double_t n1=getFirnRefractiveIndex(-1*z0);
    Double_t n2=1;
    Double_t ui=x[0];

    return n1*TMath::Sqrt((ui-u0)*(ui-u0)+z0*z0)+
	n2*TMath::Sqrt((u1-ui)*(u1-ui)+z1*z1);

}



// Double_t funcd2zdu2(Double_t *z, Double_t *par) {
    
//     Double_t n=getFirnRefractiveIndex(-1*z[0]); // Test;
//     Double_t a=918;
//     Double_t b=539;
//     Double_t c=32.5;
    
//     Double_t top=(-1*b/c)*TMath::Exp(z[0]/c);
//     Double_t bottom=a-(b*TMath::Exp(z[0]/c));

//     return top/bottom;
// }

// Double_t funcdudz(Double_t *z, Double_t *par) {
    
//     Double_t n=getFirnRefractiveIndex(-1*z[0]); // Test;
//     Double_t k=par[0];
// //    Double_t a=918;
// //    Double_t b=539;
// //    Double_t c=32.5;
    
// //    Double_t top=(-1*b/c)*TMath::Exp(z[0]/c);
// //    Double_t bottom=a-(b*TMath::Exp(z[0]/c));

//     return 1. / TMath::Sqrt((n*n/(k*k))-1);
// }


Double_t simpleRefractiveIndex(Double_t *x, Double_t *par) {

    if(x[0]<0) return getFirnRefractiveIndex(-1*x[0]);   
    //if(x[0]<0) return 1.79;   
    else return 1;
}

Double_t getFirnRefractiveIndex(Double_t depth) {
    Double_t density=firnDensityFromTop(depth);
    static Int_t gotLine=0;
    static Double_t gradient=0;
    static Double_t intercept=0;
    if(!gotLine) {
	Double_t surfaceDepth=0;
	Double_t bottomDepth=200; //Doesn't really matter
	Double_t surfaceDensity=firnDensityFromTop(surfaceDepth);
	Double_t bottomDensity=firnDensityFromTop(bottomDepth);
	Double_t surfaceIndex=1.35;
	Double_t bottomIndex=1.79;
	gradient=(bottomIndex-surfaceIndex)/(bottomDensity-surfaceDensity);
	intercept=surfaceIndex-gradient*surfaceDensity;
	cout << gradient << "\t" << intercept << endl;
	gotLine=1;
    }

    return intercept+(gradient*density);

}

Double_t firnDensityFromTop(Double_t x) {
    Double_t depth=x;
//     Double_t Y0=365.96;
//     Double_t a=14.72;
//     Double_t b=-.344;
//     Double_t c=.00422;
//     Double_t d=-.000018;
    
//     if(depth<90) 
// 	return Y0+a*depth+b*pow(depth,2)+c*pow(depth,3)+d*pow(depth,4);

//     if(depth<120) {
// 	Double_t gradient=(920.-800.)/(120.-90.);
// 	Double_t intercept=800.-(gradient*90.);
// 	return (gradient*depth)+intercept;
//     }
//     return 920;

    return (918. - 539.*TMath::Exp(-1*depth/32.5));
}
