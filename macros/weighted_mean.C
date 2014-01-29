#include <vector>
#include <iostream>

void weighted_mean() {

   const int mpt = 9999;
   //float x1[mpt];
   vector<float> x1;
   vector<float> x2;

   int npt = 0;
   float a;
   float b;

   // read data file
   ifstream in;
   in.open("dat.txt");

   while ( kTRUE ) {

     //in >> x1[npt];

     in >> a >> b;
     x1.push_back(a);
     x2.push_back(b);

      if ( ! in.good() ) break;

      cout << "a = " << x1[npt] << " b = " << x2[npt] << endl;

      npt++;

   }

   in.close();

   printf(" found %d points in dat.txt\n", npt);

   float num = 0.; 
   float den = 0.; 
   for (int icount = 0; icount < npt; icount++ ) {
     cout << x1[icount] << " " << x2[icount] << endl;
     num += x1[icount]/(x2[icount]*x2[icount]);
     den += 1./(x2[icount]*x2[icount]);
   }
   float ave = 0.;
   float erro = 0.;
   if ( den != 0. ) {
     ave = num/den;
     erro = std::sqrt(1./den);
   }

   cout << "Weighted average = " << ave << " +/- " << erro << endl;

}

