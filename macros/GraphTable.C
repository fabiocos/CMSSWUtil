
void GraphTable(Int_t iMark, Int_t iColor) {

  vector<float> x;
  vector<float> y;

  Int_t npt = 0;
  float a,b;

  // read data file
  ifstream in;
  in.open("dat.txt");

  while ( kTRUE ) {

    in >> a >> b;
    x.push_back(a);
    y.push_back(b);

    if ( ! in.good() ) break;

    //    cout << "x = " << x[npt] << " y = " << y[npt] << endl;

    npt++;

  }

  const Int_t ndim = npt;

  Double_t xx[ndim],yy[ndim];

  for (Int_t i=0;i<npt;i++) {
    xx[i] = x[i];
    yy[i] = y[i];
    //    cout << "x = " << xx[i] << " y = " << yy[i] << endl;
  }   
  
  in.close();

  printf(" found %d points\n", npt);
 
  TGraph *gr1 = new TGraph (npt, xx, yy); 

  TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500); 

  c1->cd(); 

  gr1->SetMarkerStyle(iMark);
  gr1->SetMarkerColor(iColor);
  gr1->Draw("AP");

  c1->Print("plot.jpg"); 

}

