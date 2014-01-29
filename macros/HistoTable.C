#include <vector>
#include <algorithm>

void plot() {

   const int mpt = 9999;
   //float x1[mpt];
   vector<float> x1;

   int npt = 0;
   float a;

   // read data file
   ifstream in;
   in.open("dat.txt");

   while ( kTRUE ) {

     //in >> x1[npt];

     in >> a;
     x1.push_back(a);

      if ( ! in.good() ) break;

      cout << "x = " << x1[npt] << endl;

      npt++;

   }

   in.close();

   printf(" found %d points\n", npt);

   TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500);

   //vector<float>::const_iterator imin = min_element(x1.begin(),x1.end());
   //float amin = (*imin)*0.9;
   
   //vector<float>::const_iterator imax = max_element(x1.begin(),x1.end());
   //float amax = (*imax)*1.1;

   float amin = x1[0];
   float amax = x1[0];
   
   for ( unsigned int index = 0; index < x1.size() ; index++ ) { 
     //std::cout << "B " << amin << " " << amax << std::endl; 
     amin = std::min(amin,x1[index]);
     amax = std::max(amax,x1[index]);
     //std::cout << "A " << amin << " " << amax << std::endl;
   }

   amin = amin*0.9;
   amax = amax*1.1;

   cout << "Min = " << amin << endl;
   cout << "Max = " << amax << endl;

   TH1F* h1 = new TH1F("h1","array",100,amin,amax);
   //TH1F* h1 = new TH1F("h1","array",100,0.,5.);

   for (Long64_t iLoop=0 ; iLoop<npt ; iLoop++ ) {
     h1->Fill(x1[iLoop]);
   }

   gStyle->SetOptStat("nemruoi");

   std::cout << "Mean = " << h1->GetMean() << " +/- " << h1->GetMeanError() << std::endl;
   std::cout << "RMS  = " << h1->GetRMS() << " +/- " << h1->GetRMSError() << std::endl;

   c1->cd();
   h1->Draw("E1");
   c1->Print("plot.pdf");

}

