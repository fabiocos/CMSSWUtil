
void MultiGraphTable() {

  vector<float> x;
  vector<float> y;

  Int_t npt = 0;
  float a,b;

  // read data file
  ifstream in;
  in.open("vsize.txt");

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

  // read data file
  in.open("rss.txt");

  npt = 0;
  x.clear();
  y.clear();

  while ( kTRUE ) {

    in >> a >> b;
    x.push_back(a);
    y.push_back(b);

    if ( ! in.good() ) break;

    //    cout << "x = " << x[npt] << " y = " << y[npt] << endl;

    npt++;

  }

  const Int_t ndim = npt;

  Double_t x2[ndim],y2[ndim];

  for (Int_t i=0;i<npt;i++) {
    x2[i] = x[i];
    y2[i] = y[i];
    //    cout << "x = " << x2[i] << " y = " << y2[i] << endl;
  }   
  
  in.close();

  printf(" found %d points\n", npt);
 
  TGraph *gr2 = new TGraph (npt, x2, y2); 

  TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500); 

  c1->cd(); 

  gr1->SetTitle("Memory vs # event - RECO+DQM");

  gr1->SetMarkerStyle(21);
  gr1->SetMarkerColor(4);
  gr1->Draw("AP");
  gr2->SetMarkerStyle(2);
  gr2->SetMarkerColor(2);
  gr2->Draw("P");

  leg = new TLegend(0.6,0.15,0.85,0.4);
  leg->AddEntry(gr1,"VSIZE","p");
  leg->AddEntry(gr2,"RSS","p");
  leg->Draw();
  leg->SetHeader("Memory type");
  leg->Draw();

  c1->Print("plot.jpg"); 

}

