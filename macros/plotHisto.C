#include "TClass.h"

void plotHisto( TString inputfile = "DQM_V0001_R000000001__ConverterTester__Test__RECO.root",
                TString plotfile = "new.jpg",
                TString theDir = "DQMData/Run\ 1/HcalRecHitsV/Run\ summary/HcalRecHitTask", 
                TString theHisto = "HcalRecHitTask_energy_of_rechits_HF",
                bool printStat = true, TString drawOption = "", bool logX = false, bool logY = false, bool logZ = false, 
                Double_t minX=0., Double_t maxX=0.)
{

 gROOT ->Reset();
 char*  sfilename = inputfile;

 TFile * sfile = new TFile(sfilename);

 char* baseDir=theDir;

 sfile->cd(baseDir);
 gDirectory->ls();

 const int csize = 500;

 TCanvas * showShape = new TCanvas("histoPlot","histoPlot",2*csize,csize);

 TString theName = theDir+"/"+theHisto;
 TH1 *myHisto;
 sfile->GetObject(theName,myHisto);

 if (printStat) {
   myHisto->SetStats(kTRUE);
   gStyle->SetOptStat(1111);
 }
 if (logX) gPad->SetLogx();
 if (logY) gPad->SetLogy();
 if (logZ) gPad->SetLogz();
 if ( minX != maxX && myHisto ) {
   TAxis *xaxis = myHisto->GetXaxis();
   xaxis->SetRangeUser(minX,maxX);
 }

 if ( myHisto) { 
   myHisto->Draw(drawOption);
 } else {
   std::cout << "Null pointer!" << std::endl;
 } 

 histoPlot->SaveAs(plotfile);

 return;

}
