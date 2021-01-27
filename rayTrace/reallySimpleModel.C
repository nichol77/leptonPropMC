#include <iostream>

#include "TCanvas.h"
#include "TF2.h"
#include "TF12.h"
#include "TMarker.h"

const double R_EARTH=6.378140E6;        // radius of Earth in m at bulge
static const double c_SI = 2.99792458e8;

Double_t simple3DTimeOfFlight(Double_t *x, Double_t *par);
Double_t getMinimumPoint(TF2 *func,Double_t *minXptr, Double_t *minYptr, Double_t xMin=0, Double_t xMax=0, Double_t yMin=0, Double_t yMax=0);


void reallySimpleModel() {
//     Double_t nuX=-2000;
//     Double_t nuY=6000000;
//     Double_t ballX=20000;
//     Double_t ballY=6450000;
//     Double_t nIce=1.92;
//     Double_t par[5]={nuX,nuY,ballX,ballY,nIce};
//     TF1 *simple = new TF1("simple",simpleTimeOfFlight,nuX,ballX,5);
//     simple->SetParameters(par);
//     simple->Draw();
    Double_t nuX=1.18094e+06;
    Double_t nuY=-760496;
    Double_t nuZ=6.19992e+06;    
    Double_t ballX=-1.2747e+06;
    Double_t ballY=769556;
    Double_t ballZ=6.2212e+06;
    Double_t nIce=1.76;
    Double_t par[7]={nuX,nuY,nuZ,ballX,ballY,ballZ,nIce};

    TCanvas *can = new TCanvas("can","can");
    TF2 *simple3d = new TF2("simple3d",simple3DTimeOfFlight,-2e6,2e6,-2e6,2e6,7);
    simple3d->SetParameters(par);
    simple3d->Draw("col");

//    simple3d->SetNpx(100);
//    simple3d->SetNpy(100);
    Double_t minimumX,minimumY;
    Double_t minTime=getMinimumPoint(simple3d,&minimumX,&minimumY);
    cout << minimumX << "\t" << minimumY << "\t" << minTime << endl;

    TMarker *marker = new TMarker();
    marker->SetMarkerColor(kBlack);
    marker->SetMarkerSize(2);
    marker->SetMarkerStyle(29);
    marker->DrawMarker(nuX,nuY);    
    marker->SetMarkerStyle(23);
    marker->DrawMarker(ballX,ballY);
    marker->SetMarkerStyle(22);
    marker->SetMarkerColor(kRed);
    marker->DrawMarker(minimumX,minimumY);

    
   //  TCanvas *can2 = new TCanvas("can2","can2");
//     TF2 *simple3dZoom = new TF2("simple3dZoom",simple3DTimeOfFlight,minimumX-100,minimumX+100,minimumY-100,minimumY+100,7);
//     simple3dZoom->SetParameters(par);
// //    simple3dZoom->SetNpx(100);
// //    simple3dZoom->SetNpy(100);
//     simple3dZoom->Draw("col");


//     minTime=getMinimumPoint(simple3dZoom,&minimumX,&minimumY);
//     cout << minimumX << "\t" << minimumY << "\t" << minTime << endl;
    

}



Double_t simpleTimeOfFlight(Double_t *x, Double_t *par) {
    Double_t nuX=par[0];
    Double_t nuY=par[1];
    Double_t ballX=par[2];
    Double_t ballY=par[3];
    Double_t nIce=par[4];

    Double_t surfX=x[0];    
    Double_t surfY=sqrt(R_EARTH*R_EARTH-surfX*surfX);
    
    Double_t iceTime=nIce*sqrt(pow(surfX-nuX,2)+pow(surfY-nuY,2));
    Double_t airTime=sqrt(pow(surfX-ballX,2)+pow(surfY-ballY,2));
    
    return (iceTime+airTime)/c_SI;
}


Double_t simple3DTimeOfFlight(Double_t *x, Double_t *par) {
    Double_t nuX=par[0];
    Double_t nuY=par[1];
    Double_t nuZ=par[2];
    Double_t ballX=par[3];
    Double_t ballY=par[4];
    Double_t ballZ=par[5];
    Double_t nIce=par[6];

    Double_t surfX=x[0];
    Double_t surfY=x[1];
    Double_t surfZ=sqrt(R_EARTH*R_EARTH-surfX*surfX-surfY*surfY);
    
    Double_t iceTime=nIce*sqrt(pow(surfX-nuX,2)+pow(surfY-nuY,2)+pow(surfZ-nuZ,2));
    Double_t airTime=sqrt(pow(surfX-ballX,2)+pow(surfY-ballY,2)+pow(surfZ-ballZ,2));
    
    return (iceTime+airTime)/c_SI;
}



Double_t getMinimumPoint(TF2 *func,Double_t *minXptr, Double_t *minYptr, Double_t xMin, Double_t xMax, Double_t yMin, Double_t yMax) {

    if(xMin>=xMax) {
	xMin=func->GetXmin();
	xMax=func->GetXmax();
    }
    if(yMin>=yMax) {
	yMin=func->GetYmin();
	yMax=func->GetYmax();
    }
   
//    cout << "x:\t" << xMin << "\t" << xMax << endl;
//    cout << "y:\t" << yMin << "\t" << yMax << endl;
	         
   Double_t x,y,z;
   Double_t dx=(xMax-xMin)/func->GetNpx();
   Double_t dy=(yMax-yMin)/func->GetNpy();
   
   Double_t minX=xMin;
   Double_t minY=yMin;
   Double_t minZ=func->Eval(minX+dx,minY+dy);
   for(int ix=0;ix<func->GetNpx();ix++) {
       for(int iy=0;iy<func->GetNpy();iy++) {
	   x=xMin+(ix+0.5)*dx;
	   y=yMin+(iy+0.5)*dy;
	   z=func->Eval(x,y);
	   if(z<minZ) {
	       minX=x;
	       minY=y;
	       minZ=z;
	   }
       }
   }
   Double_t testDelta=sqrt(dx*dx+dy*dy);
   Double_t otherHalf=sqrt(pow(func->GetXmax()-func->GetXmin(),2)+
			   pow(func->GetYmax()-func->GetYmin(),2));
//   cout << testDelta << "\t" << otherHalf << endl;


   if(testDelta<1e-9*otherHalf) {
       *minXptr=minX;
       *minYptr=minY;
       return minZ;
   }
   else return getMinimumPoint(func,minXptr,minYptr,
			       TMath::Max(xMin,minX-dx),
			       TMath::Min(xMax,minX+dx),
			       TMath::Max(yMin,minY-dy),
			       TMath::Min(yMax,minY+dy));
			    


}
