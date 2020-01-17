
void HistoTableDiffe() {

   const int mpt = 9999;
   //float x1[mpt];
   vector<float> x1;
   vector<float> x2;

   int npt = 0;
   float a;

   // read data file
   ifstream in;
   in.open("old");

   while ( kTRUE ) {

     //in >> x1[npt];

     in >> a;
     x1.push_back(a);

      if ( ! in.good() ) break;

      //cout << "x = " << x1[npt] << endl;

      npt++;

   }

   in.close();

   printf(" found %d points in old\n", npt);

   npt = 0;

   // read data file
   in.open("new");

   while ( kTRUE ) {

     //in >> x1[npt];

     in >> a;
     x2.push_back(a);

      if ( ! in.good() ) break;

      //cout << "x = " << x2[npt] << endl;

      npt++;

   }

   in.close();

   printf(" found %d points in new\n", npt);

   vector<float> diffe;
   for (int icount = 0; icount < npt; icount++ ) {
     cout << x1[icount] << " " << x2[icount] << endl;
     float delta = x1[icount]-x2[icount];
     diffe.push_back(delta);
     //cout << "i = " << icount << " diffe = " << diffe[icount] << endl;
   }

   TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500);

   vector<float>::const_iterator imin = min_element(diffe.begin(),diffe.end());
   float amin = (*imin)*0.9;
   
   vector<float>::const_iterator imax = max_element(diffe.begin(),diffe.end());
   float amax = (*imax)*1.1;

   cout << "Min = " << *imin << endl;
   cout << "Max = " << *imax << endl;

   TH1F* h1 = new TH1F("h1","array",100,amin,amax);
   //TH1F* h1 = new TH1F("h1","array",100,0.,5.);

   for (Long64_t iLoop=0 ; iLoop<npt ; iLoop++ ) {
     h1->Fill(diffe[iLoop]);
   }

   gStyle->SetOptStat("nemruoi");

   c1->cd();
   h1->Draw("E1");
   c1->Print("plot.jpg");

}
