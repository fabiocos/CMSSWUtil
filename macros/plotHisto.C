void plotHisto( TString inputfile = "old.root",
                TString plotfile = "old.jpg",
                TString theDir = "DQMData/Run 1/MTD/Run summary/Vertices", 
                TString theHisto = "TimeRes",
                bool printStat = true, TString drawOption = "", bool logX = false, bool logY = false, bool logZ = false, 
                Double_t minX=0., Double_t maxX=0.)
{

 gROOT ->Reset();
 auto sfilename = inputfile;

 TFile * sfile = new TFile(sfilename);

 auto baseDir=theDir;

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

 showShape->SaveAs(plotfile);

 return;

}
