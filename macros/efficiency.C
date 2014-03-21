#include "TClass.h"
#include "TH1.h"

void efficiency( TString denfile="60-120.root",
                 TString numfile="orig.root",
                 TString outfile="res.root" ) {

  gROOT ->Reset();
  
  TFile * nume = new TFile(numfile);
  TFile * deno = new TFile(denfile);
  
  nume->cd("/");
  deno->cd("/");

  TString theName = "d02-x01-y01";

  const TH1F *myNume;
  nume->GetObject(theName,myNume);
  myNume->Sumw2();
  myNume->Print("all");

  const TH1F *myDeno;
  deno->GetObject(theName,myDeno);
  myDeno->Sumw2();
  myDeno->Print("all");

  TH1F *Acce = (TH1F*)myNume->Clone("efficiency");
  Acce->Reset();
  Double_t f1(1.),f2(1.);
  Option_t* opt("b");
  Acce->Divide(myNume,myDeno,f1,f2,opt);
  Acce->Print("all");

  TFile * out = new TFile(outfile,"NEW");
  Acce->Write();
  out->Close();

}
