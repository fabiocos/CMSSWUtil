std::vector<TString> navigateROOTFile( TString currentfile = "new.root",
                                       TString theDir = "DQMData/Run 1/MTD/Run summary/Vertices" )
{

 gROOT ->Reset();
 auto sfilename = currentfile ;

 delete gROOT->GetListOfFiles()->FindObject(sfilename);

 TFile * sfile = new TFile(sfilename);

 auto baseDir=theDir;

 sfile->cd(baseDir);
 gDirectory->ls();

 TDirectory * d = gDirectory;
 d->GetListOfKeys()->Print();

 std::vector<TString> theHistList;

 TIter i( d->GetListOfKeys() );
 TKey *k;
 std::cout << "Looping... " << std::endl;
 while( (k = (TKey*)i())) {
   TClass * c1 = gROOT->GetClass(k->GetClassName());
   std::cout << k->GetName() << " " << k->GetClassName() << std::endl;
   if ( !c1->InheritsFrom("TH1")) continue;
   std::cout << "Added" << std::endl;
   theHistList.push_back(k->GetName());

 }

 for (unsigned int index = 0; index < theHistList.size() ; index++ ) {
   std::cout << index << " " << theHistList[index] << std::endl;
 }

 return theHistList;
}
