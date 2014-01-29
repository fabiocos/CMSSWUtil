#include <iostream.h>

#include "TClass.h"
#include "TPaveStats.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TFile.h"
#include "TTree.h"
#include "TText.h"
#include "TLegend.h"

void compareMulti( Int_t fileMax = 4,
                   TString file1 = "one.root", TString title1 = "run 202299 all",
                   TString file2 = "two.root", TString title2 = "run 198603 all",
                   TString file3 = "three.root", TString title3 = "MC std Summer12 PU",
                   TString file4 = "four.root", TString title4 = "MC inTime PU=45",
                   TString plotfile = "new.pdf",
                   TString theDir = "ecalMinBiasAnalysis",
                   TString theHisto = "vtx", 
                   bool printStat = true, TString drawOption = "E1", bool logX = false, bool logY = false, bool logZ = false, 
                   Double_t minX=0., Double_t maxX=0., Double_t minY=0., Double_t maxY=0., 
                   bool Normalize = false, bool Scale = false, Double_t scale1 = 1., Double_t scale2 = 1., Double_t scale3 = 1., Double_t scale4 = 1.,
                   TString legendP = "BR" ) 
{

  if ( Normalize && Scale ) { std::cout << "Incompatible Normalize and Scale options!" << std::endl; return; } 
  
  gROOT ->Reset();
  char*  filename1 = file1 ;
  char*  filename2 = file2 ;
  char*  filename3 = file3 ;
  char*  filename4 = file4 ;

  
  TFile * rfile1 = new TFile(filename1);
  TFile * rfile2 = new TFile(filename2);
  if ( fileMax > 2 ) { TFile * rfile3 = new TFile(filename3); } 
  if ( fileMax > 3 ) { TFile * rfile4 = new TFile(filename4); }
  
  char* baseDir=theDir;
  
  rfile1->cd(baseDir);
  rfile2->cd(baseDir);
  if ( fileMax > 2 ) { rfile3->cd(baseDir); } 
  if ( fileMax > 3 ) { rfile4->cd(baseDir); }

  int color1 = 1;
  int color2 = 2;
  int color3 = 4;
  int color4 = 28;
  
  int marker1 = 21;
  int marker2 = 20;
  int marker3 = 22;
  int marker4 = 28;
  
  Double_t markerSize = 0.75;

  const int csize = 500;
  
  TCanvas * myCanvas = new TCanvas("histoPlot","histoPlot",2*csize,csize);
  
  TString theName = theDir+"/"+theHisto;
  TH1 *myHisto1;
  rfile1->GetObject(theName,myHisto1);
  TH1 *myHisto2;
  rfile2->GetObject(theName,myHisto2);
  TH1 *myHisto3;
  if ( fileMax > 2 ) {
    rfile3->GetObject(theName,myHisto3);
  }
  TH1 *myHisto4;
  if ( fileMax > 3 ) {
    rfile4->GetObject(theName,myHisto4);
  }
  
  if (printStat) {
    myHisto1->SetStats(kTRUE);
    gStyle->SetOptStat(1001111); }
  else {
    myHisto1->SetStats(kFALSE);
    myHisto2->SetStats(kFALSE);
    if ( myHisto3 ) { myHisto3->SetStats(kFALSE);} 
    if ( myHisto4 ) { myHisto4->SetStats(kFALSE);}
  }
  if (logX) gPad->SetLogx();
  if (logY) gPad->SetLogy();
  if (logZ) gPad->SetLogz();
  if ( minX != maxX && myHisto1 ) {
    TAxis *xaxis = myHisto1->GetXaxis();
    xaxis->SetRangeUser(minX,maxX);
  }
  if ( minY != maxY && myHisto1 ) {
    TAxis *yaxis = myHisto1->GetYaxis();
    yaxis->SetRangeUser(minY,maxY);
  }
  
  myHisto1->SetLineColor(color1);
  myHisto1->SetMarkerStyle(marker1);
  myHisto1->SetMarkerSize(markerSize);
  myHisto1->SetMarkerColor(color1);
  
  myHisto2->SetLineColor(color2); 
  myHisto2->SetMarkerStyle(marker2); 
  myHisto2->SetMarkerSize(markerSize); 
  myHisto2->SetMarkerColor(color2);  

  if ( fileMax > 2 ) {
    myHisto3->SetLineColor(color3); 
    myHisto3->SetMarkerStyle(marker3); 
    myHisto3->SetMarkerSize(markerSize); 
    myHisto3->SetMarkerColor(color3);  
  }

  if ( fileMax > 3 ) {
    myHisto4->SetLineColor(color4); 
    myHisto4->SetMarkerStyle(marker4); 
    myHisto4->SetMarkerSize(markerSize); 
    myHisto4->SetMarkerColor(color4);  
  }
  
  Double_t scala;

  if ( myHisto1) { 
    if ( Normalize ) { myHisto1->Sumw2(); (scale1 < 0.) ? (scala = std::fabs(scale1)) : (scala = myHisto1->Integral(-1,-2,"")/scale1) ; 
      TH1 * tcopy1 = myHisto1->DrawNormalized(drawOption, scala); }
    else if ( Scale) { myHisto1->Sumw2(); myHisto1->Scale(std::fabs(scale1),""); 
      if ( minX != maxX && myHisto1 ) {
        TAxis *xaxis = myHisto1->GetXaxis();
        xaxis->SetRangeUser(minX,maxX);
      }
      if ( minY != maxY && myHisto1 ) {
        TAxis *yaxis = myHisto1->GetYaxis();
        yaxis->SetRangeUser(minY,maxY);
      }
      myHisto1->Draw(drawOption); 
    } 
    else { myHisto1->Draw(drawOption); } 
    gPad->Update();
    if ( printStat ) {
      if ( !Normalize ) {
        TPaveStats *s = (TPaveStats*)myHisto1->FindObject("stats"); }
      else { TPaveStats *s = (TPaveStats*)tcopy1->FindObject("stats"); }
      Double_t y1 = 1.;
      Double_t y2 = 0.85;
      s->SetY1NDC(y1);
      s->SetY2NDC(y2);
      s->SetTextColor(color1);
    }
  } else {
    std::cout << "Null pointer!" << std::endl;
  } 

  TString ndrawOption = drawOption + "sames";
  
  if ( myHisto2 ) { 
    if ( Normalize ) { myHisto2->Sumw2(); (scale2 < 0.) ? (scala = std::fabs(scale2)) : (scala = myHisto2->Integral(-1,-2,"")/scale2) ; 
      TH1 * tcopy2 = myHisto2->DrawNormalized(ndrawOption, scala); }
    else if ( Scale) { myHisto2->Sumw2(); myHisto2->Scale(std::fabs(scale2),""); myHisto2->Draw(ndrawOption); } 
    else { myHisto2->Draw(ndrawOption); } 
    gPad->Update();
    if ( printStat ) {
      if ( !Normalize ) {
        TPaveStats *s = (TPaveStats*)myHisto2->FindObject("stats"); }
      else { TPaveStats *s = (TPaveStats*)tcopy2->FindObject("stats"); }
      Double_t y1 = 0.7;
      Double_t y2 = 0.85;
      s->SetY1NDC(y1);
      s->SetY2NDC(y2); 
      s->SetTextColor(color2);
    }
  } else {
    std::cout << "Null pointer!" << std::endl;
  }
  if ( fileMax > 2 ) {
    if ( myHisto3 ) { 
      if ( Normalize ) { myHisto3->Sumw2(); (scale3 < 0.) ? (scala = std::fabs(scale3)) : (scala = myHisto3->Integral(-1,-2,"")/scale3) ; 
        TH1 * tcopy3 = myHisto3->DrawNormalized(ndrawOption, scala); }
      else if ( Scale) { myHisto3->Sumw2(); myHisto3->Scale(std::fabs(scale3),""); myHisto3->Draw(ndrawOption); } 
      else { myHisto3->Draw(ndrawOption); } 
      gPad->Update();
      if ( printStat ) {
        if ( !Normalize ) {
          TPaveStats *s = (TPaveStats*)myHisto3->FindObject("stats"); }
        else { TPaveStats *s = (TPaveStats*)tcopy3->FindObject("stats"); }
        Double_t y1 = 0.55;
        Double_t y2 = 0.7;
        s->SetY1NDC(y1);
        s->SetY2NDC(y2);
        s->SetTextColor(color3);
      }
    } else {
      std::cout << "Null pointer!" << std::endl;
    }
  }
  if ( fileMax > 3 ) {
    if ( myHisto4 ) { 
      if ( Normalize ) { myHisto4->Sumw2(); (scale4 < 0.) ? (scala = std::fabs(scale4)) : (scala = myHisto4->Integral(-1,-2,"")/scale4) ; 
        TH1 * tcopy4 = myHisto4->DrawNormalized(ndrawOption, scala); }
      else if ( Scale) { myHisto4->Sumw2(); myHisto4->Scale(std::fabs(scale4),""); myHisto4->Draw(ndrawOption); } 
      else { myHisto4->Draw(ndrawOption); } 
      gPad->Update();
      if ( printStat ) {
        if ( !Normalize ) {
          TPaveStats *s = (TPaveStats*)myHisto4->FindObject("stats"); }
        else { TPaveStats *s = (TPaveStats*)tcopy4->FindObject("stats"); } 
        Double_t y1 = 0.4;
        Double_t y2 = 0.55;
        s->SetY1NDC(y1);
        s->SetY2NDC(y2);
        s->SetTextColor(color4);
      }
    } else {
      std::cout << "Null pointer!" << std::endl;
    }
  }

  //  TPaveText *pt = new TPaveText(.1,.3,.4,.1,"NDC");
/*   TText* t1=pt->AddText("Noise only"); */
/*   TText* t2=pt->AddText("Low p particles"); */
/*   TText* t3=pt->AddText("5.5 < eta < 6.2"); */
/*   t1->SetTextColor(color1); */
/*   t2->SetTextColor(color2); */
/*   t3->SetTextColor(color3); */
/*   pt->Draw(); */

  Double_t x1 = 0.;
  Double_t y1 = 0.;
  Double_t x2 = 0.;
  Double_t y2 = 0.;  
  if ( legendP == "BR" ) {
    x1 = .65; y1 = .15; x2 = .85; y2 = .35;
  }
  else if ( legendP == "BL" ) {
    x1 = .15; y1 = .15; x2 = .35; y2 = .35;
  }
  else if ( legendP == "TL" ) {
    x1 = .15; y1 = .7; x2 = .35; y2 = .85;
  }
  else if ( legendP == "TR" ) {
    x1 = .65; y1 = .7; x2 = .85; y2 = .85;
  }
  else { std::cout << "Invalid legend location, no printout!" << std::endl; }

  TLegend *leg = new TLegend(x1,y1,x2,y2);
  leg->SetTextFont(62);
  leg->SetTextSize(0.03);
  leg->AddEntry(myHisto1,title1,"P");
  leg->AddEntry(myHisto2,title2,"P");
  if ( fileMax > 2 ) { leg->AddEntry(myHisto3,title3,"P"); }
  if ( fileMax > 3 ) { leg->AddEntry(myHisto4,title4,"P"); }
  leg->Draw();
  
  histoPlot->SaveAs(plotfile);

  return;
  
}

