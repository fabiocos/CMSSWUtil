/*

  Function to evaluate the PDF uncertainty envelope for a given histogram theHisto in the root directory theDir.
  The default prediction is in NorigFile; 
  the central predction for the envelop is in NcenFile (which may or may not coincide with the default);
  the variations have common names NvarFileRoot+"_"+nset , where nset is the number of variations.
  Histograms can be scaled according to user defined scaling factors scaleOrig and scaleCen (used also for variations).

  The flag errFlag specify which kind of PDF envelop computation must be done:
  1 -> symmetric Hessian eigenvectors
  2 -> asymmetric Hessien eigenvectors
  3 -> MC variations (NNPDF)

  A modified copy of the histogram in the default adding the UP and DOWN envelope computed wrt the central prediction
  is put in the NoutFileUP and NoutFileDOWN files respectively.

 */

#include <sstream>
#include <iostream.h>
#include <algorithm>

#include "TClass.h"
#include "TDirectory.h"
#include "TFile.h"

void HistoPDFUncertainty( TString theDir = "/",
                          TString theHisto = "d01-x01-y01", 
                          Int_t nset, Int_t errFlag, 
                          Double_t scaleOrig = 1., Double_t scaleCen = 1.,
                          TString NorigFile = "default.root",
                          TString NcenFile = "CT10_0.root",
                          TString NvarFileRoot = "CT10",
                          TString NoutFileUP = "d01-x01-y01_CT10_up.root",
                          TString NoutFileDOWN = "d01-x01-y01_CT10_down.root")
{

  if ( errFlag <1 || errFlag >3 ) {       
    std::cout << "Unknown type of PDF envelope required " << errFlag << " Aborting..." << std::endl; return 2;
  } 
  else  std::cout << "Using PDF envelope type " << errFlag << std::endl; 

  gROOT ->Reset();

  char* baseDir = theDir;
  TString theName = theDir+"/"+theHisto;

  // Original histogram to be modified

  TFile * origFile = new TFile((char*)NorigFile);
  origFile->cd(baseDir);

  TH1 *myOrigHisto;
  origFile->GetObject(theName,myOrigHisto);

  std::vector<Double_t> origC, origE;
  getHistoInfo( myOrigHisto, origC, origE, scaleOrig ); 

  Int_t nBin = origC.size();
  //  std::cout << "nBin = " << nBin << std::endl;

  /* myOrigHisto->Print("all");   */
  /* for ( unsigned int isize=0; isize < origC.size(); isize++ ) { */
  /*   std::cout << isize << " " << origC[isize] << " " << origE[isize] << std::endl; */
  /* } */

  // CENTRAL value of PDF reweighting 

  TFile * cenFile = new TFile((char*)NcenFile);
  cenFile->cd(baseDir);

  TH1 *myCenHisto;
  cenFile->GetObject(theName,myCenHisto);

  std::vector<Double_t> cenC, cenE;
  getHistoInfo( myCenHisto, cenC, cenE, scaleCen ); 

  /* myCenHisto->Print("all"); */
  /* for ( unsigned int isize=0; isize < cenC.size(); isize++ ) { */
  /*   std::cout << isize << " " << cenC[isize] << " " << cenE[isize] << std::endl; */
  /* } */

  if ( cenC.size() != origC.size() ) { std::cout << "Inconsistent histogram size!!!!" << std::endl; return 1; } 

  cenFile->Close();

  // Load matrix of bin/error values for each variant

  std::vector< std::vector<Double_t> > varC;
  std::vector< std::vector<Double_t> > varE;

  const Int_t num(nset);
  TString NvarFile[num]; 

  for ( int iFile = 0; iFile < num; iFile++ ) {

    NvarFile[iFile] = NvarFileRoot+"_"+ItoA(iFile+1)+".root";
    //    std::cout << iFile << " " << NvarFile[iFile] << std::endl;

    TFile * varFile = new TFile((char *)NvarFile[iFile]);
    varFile->cd(baseDir);
    
    TH1 *myVarHisto;
    varFile->GetObject(theName,myVarHisto);
    std::vector<Double_t> tmpC, tmpE;
    getHistoInfo( myVarHisto, tmpC, tmpE, scaleCen );
    
    varC.push_back(tmpC);
    varE.push_back(tmpE);

    /* myVarHisto->Print("all"); */
    /* for ( unsigned int isize=0; isize < cenC.size(); isize++ ) { */
    /*   std::cout << isize << " " << (varC[iFile])[isize] << " " << (varE[iFile])[isize] << std::endl; */
    /* } */

    varFile->Close();

  }

  // Compute for each bin the UP and DOWN fluctuation according to the specified mode

  TH1 * histUP = (TH1 *)myOrigHisto->Clone();
  histUP->Reset("M");

  TH1 * histDOWN = (TH1 *)myOrigHisto->Clone();
  histDOWN->Reset("M");

  std::vector<Double_t> deltaXP;
  std::vector<Double_t> deltaXM;

  for ( Int_t iBin = 0; iBin < nBin; iBin++ ) {

    if ( errFlag == 1 || errFlag == 2 ) {

      Double_t X0(cenC[iBin]);
      std::vector<Double_t> XP;
      std::vector<Double_t> XM;

      for ( Int_t iFile = 0; iFile < num; iFile=iFile+2 ) {
        XP.push_back((varC[iFile+1])[iBin]);
        XM.push_back((varC[iFile])[iBin]);
      }

      if ( errFlag == 1 ) {
        Double_t delta = 0.;
        SymHessian(XP, XM, delta);
        deltaXP.push_back(delta);
        histUP->SetBinContent(iBin,origC[iBin]+deltaXP[iBin]);
        histDOWN->SetBinContent(iBin,origC[iBin]-deltaXP[iBin]);
      }

      if ( errFlag == 2 ) {
        Double_t deltaP(0.);
        Double_t deltaM(0.);
        AsymHessian(X0, XP, XM, deltaP, deltaM);
        deltaXP.push_back(deltaP);
        deltaXM.push_back(deltaM);
        histUP->SetBinContent(iBin,origC[iBin]+deltaXP[iBin]);
        histDOWN->SetBinContent(iBin,origC[iBin]-deltaXM[iBin]);
      }

    }

    else if ( errFlag == 3 ) {

      std::vector<Double_t> DX;

      for ( Int_t iFile = 0; iFile < num; iFile++ ) {
        DX.push_back((varC[iFile])[iBin]);
      }

      Double_t delta(0.);
      MC(DX, delta);
      deltaXP.push_back(delta);

      histUP->SetBinContent(iBin,origC[iBin]+deltaXP[iBin]);
      histDOWN->SetBinContent(iBin,origC[iBin]-deltaXP[iBin]);

    }

  }

  /* gDirectory->pwd(); */
  /* gDirectory->ls(); */
  /* histUP->Print("all"); */
  /* histDOWN->Print("all"); */


  // Open output files

  TFile * outFileUP = new TFile((char *)NoutFileUP,"NEW");
  outFileUP->cd(baseDir);
  histUP->Write();
  outFileUP->Close();

  TFile * outFileDOWN = new TFile((char *)NoutFileDOWN,"NEW");
  outFileDOWN->cd(baseDir);
  histDOWN->Write();
  outFileDOWN->Close();
  
  return;
  
}

void getHistoInfo( TH1* myHisto, std::vector<Double_t> & cont, std::vector<Double_t> & erro, Double_t scalefac=1. ) {

  Int_t nBin(myHisto->GetNbinsX());
  for ( Int_t it=0; it <=nBin+1; it++ ) {
    cont.push_back(myHisto->GetBinContent(it)*scalefac);
    erro.push_back(myHisto->GetBinError(it)*scalefac);
  }


}

TString ItoA(Int_t & i) {

  std::stringstream strs;
  strs << i;
  std::string temp_str = strs.str();
  //  char* char_type = (char*) temp_str.c_str();
  TString theString(temp_str);

  return theString;

}

void AsymHessian( Double_t & X0, std::vector<Double_t> & XP, std::vector<Double_t> XM, Double_t & deltaXP, Double_t & deltaXM ) {

  Int_t nVar = XP.size();

  deltaXP = 0.;
  deltaXM = 0.;
  
  for ( Int_t it=0; it < nVar; it++ ) {
    Double_t addXP = std::max( std::max(XP[it]-X0,XM[it]-X0) ,0.);
    Double_t addXM = std::max( std::max(X0-XP[it],X0-XM[it]) ,0.);
    deltaXP += addXP*addXP;
    deltaXM += addXM*addXM;
  }

  deltaXP = std::sqrt(deltaXP);
  deltaXM = std::sqrt(deltaXM);

  return;

}

void SymHessian( std::vector<Double_t> & XP, std::vector<Double_t> XM, Double_t & deltaX ) {

  Int_t nVar = XP.size();

  deltaX = 0.;
  
  for ( Int_t it=0; it < nVar; it++ ) {
    deltaX += ( XP[it] - XM[it] ) * ( XP[it] - XM[it] ) ;
  }

  deltaX = std::sqrt(deltaX);

  return;

}

void MC( std::vector<Double_t> & DX, Double_t & deltaX ) {

  Int_t nVar = DX.size();

  deltaX = 0.;

  Double_t Sum = 0;
  Double_t Sum2 = 0;
  
  for ( Int_t it=0; it < nVar; it++ ) {
    Sum += DX[it];
    Sum2 += DX[it]*DX[it];
  }

  deltaX = std::sqrt( std::max( (Sum2 - Sum*Sum/(Double_t)nVar)/(Double_t)(nVar-1) , 0. ) );

  return;

}
