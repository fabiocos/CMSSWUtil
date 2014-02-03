#!/bin/bash

set -x

rm -f extractVJETSHistos.C aida.root CMS_SMP_12_017.aida *.dat

cat > extractVJETSHistos.C << EOF
#include "TClass.h"
#include "TH1.h"

void extractVJETSHistos( TString inputfile = "input.root",
                         TString outputfile = "output.root" ,
                         TString outmode = "NEW",
                         TString prefix = "ele" )
{


 gROOT ->Reset();
 char*  sfilename = inputfile;

 TFile * sfile = new TFile(sfilename);

 char* baseDir="/";

 std::vector<TString> theHistoN;
 std::vector<TString> theHistoTitle;

 TString thePrefix;
 if ( prefix == "ele" ) thePrefix = "Electron channel, ";  
 if ( prefix == "muo" ) thePrefix = "Muon channel, ";  
 if ( prefix == "tot" ) thePrefix = "Combined channels, ";  

 theHistoN.push_back("HReco_leading");
 theHistoTitle.push_back("HT, at least 1 jet");
 theHistoN.push_back("HReco_subleading");
 theHistoTitle.push_back("HT, at least 2 jets");
 theHistoN.push_back("HReco_subsubleading");
 theHistoTitle.push_back("HT, at least 3 jets");
 theHistoN.push_back("HReco_subsubsubleading");
 theHistoTitle.push_back("HT, at least 4 jets");
 theHistoN.push_back("JetMultiplicityUnfolded");
 theHistoTitle.push_back("Jet multiplicity");
 theHistoN.push_back("jReco_leading");
 theHistoTitle.push_back("Leading jet p_t, at least 1 jet");
 theHistoN.push_back("jReco_leadingeta");
 theHistoTitle.push_back("Leading jet eta, at least 1 jet");
 theHistoN.push_back("jReco_subleading");
 theHistoTitle.push_back("2nd leading jet p_t, at least 2 jets");
 theHistoN.push_back("jReco_subleadingeta");
 theHistoTitle.push_back("2nd leading jet eta, at least 2 jets");
 theHistoN.push_back("jReco_subsubleading");
 theHistoTitle.push_back("3rd leadingjet p_t, at least 3 jets");
 theHistoN.push_back("jReco_subsubleadingeta");
 theHistoTitle.push_back("3rd leading jet eta, at least 3 jets");
 theHistoN.push_back("jReco_subsubsubleading");
 theHistoTitle.push_back("4th leading jet p_t, at least 4 jets");
 theHistoN.push_back("jReco_subsubsubleadingeta");
 theHistoTitle.push_back("4th leading jet eta, at least 4 jets");

 TFile * out = new TFile(outputfile,outmode);
 
 for ( unsigned int i=0; i<theHistoN.size(); i++ ) {  
   
   sfile->cd(baseDir);
//   gDirectory->ls();
   
   TString theName = theHistoN[i];
   std::cout << theName << std::endl;
   TH1 *myHisto;
   sfile->GetObject(theName,myHisto);

   if ( myHisto ) {
     TH1 * newHisto(myHisto);
     newHisto->SetName((char *)(prefix+"_"+theHistoN[i]));
     newHisto->SetTitle((char *)(thePrefix+ theHistoTitle[i]));
     out->cd(baseDir);
     gDirectory->ls();
     newHisto->Write();
   }

 }

 out->Close();
 
 
}
EOF

#root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApproval.root","ele.root")'
#root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApprovalMu.root","muo.root")'
#root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApprovalCombined.root","comb.root")'

root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApproval.root","aida.root","NEW","ele")'
root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApprovalMu.root","aida.root","UPDATE","muo")'
root -l -q -b 'extractVJETSHistos.C("UnfoldingOfficialV57_3PostApprovalCombined.root","aida.root","UPDATE","tot")'

root2flat -a CMS_SMP_12_017 -e aida.root
flat2aida -o - *.dat | sed 's/path="/path="\/REF\//' | sed 's/value="CMS/value="\/REF\/CMS/' > CMS_SMP_12_017.aida 