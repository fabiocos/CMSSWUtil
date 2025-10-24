#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TInterpreter.h>
#include <TROOT.h>

#include <cmath>

double fitf(double *x, double *par)
{
   double arg1 = 0;
   if (par[2] != 0) arg1 = (x[0] - par[1])/par[2];
   double arg2 = 0;
   if (par[5] != 0) arg2 = (x[0] - par[4])/par[5];

   double fitval = par[0]*std::exp(-0.5*arg1*arg1)+par[3]*std::exp(-0.5*arg2*arg2);
   return fitval;
}
void fitTemplate(TString inputfile = "old.root",
                 TString theDir = "DQMData/Run 1/MTD/Run summary/Vertices",
                 TString theHisto = "TimeRes")
{
   TH1 *myHisto;

   auto sfilename = inputfile;
   TFile * sfile = new TFile(sfilename);
   auto baseDir=theDir;
   sfile->cd(baseDir);
   TString theName = theDir+"/"+theHisto;
   sfile->GetObject(theName,myHisto);

   TCanvas *c1 = new TCanvas("c1","the fit canvas",500,400);

   // Creates a Root function based on function fitf above
   TF1 *func = new TF1("fitf",fitf,-5.,5.,6);

   // Sets initial values and parameter names
   func->SetParameters(100,0.,0.1,100,0.,0.1);
   func->SetParNames("P0","P1","P2","P3","P4","P5");
   func->SetRange(-0.1,0.1);

   gStyle->SetOptFit(111111);

   // Fit histogram in range defined by function
   myHisto->Fit(func,"r");

   // Gets integral of function between fit limits
   printf("Integral of function = %g\n",func->Integral(-2,2));
}
