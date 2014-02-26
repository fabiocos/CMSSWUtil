#include <iostream.h>
#include <vector.h>

void ShapeHistoFromTable(TString fileIn = "input.txt", 
                         TString fileOutRoot = "output.root",
                         TString fileOutPlot = "output.pdf",
                         Int_t iMark = 20 , Int_t iColor = 3) {

  vector<float> x;

  Int_t npt = 0;
  float a;

  // read data file
  ifstream in;
  in.open(fileIn);

  while ( kTRUE ) {

    in >> a;
    x.push_back(a);

    if ( ! in.good() ) break;

    //    cout << "# " << npt << " x = " << x[npt] << endl;

    npt++;

  }

  const Int_t ndim = npt;

  Double_t xx[ndim];

  TH1F myHisto("myHisto","myHisto",ndim,0.,float(ndim));

  for (Int_t i=0;i<npt;i++) {
    xx[i] = x[i];
    //    cout << "# " << i << " x = " << xx[i] << endl;
    myHisto.Fill(i+0.5,xx[i]);
  }   
  
  myHisto.Print("all");

  in.close();

  TFile myFile(fileOutRoot,"RECREATE");
  myHisto.Write();
  myFile.Close();

  printf(" found %d points\n", npt);
 
  TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500); 

  c1->cd(); 

  myHisto.Draw("");

  c1->Print(fileOutPlot); 

}

