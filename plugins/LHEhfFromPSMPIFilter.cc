
#include "LHEhfFromPSMPIFilter.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <iostream>

using namespace edm;
using namespace std;


LHEhfFromPSMPIFilter::LHEhfFromPSMPIFilter(const edm::ParameterSet& iConfig) :
  lhesrc_(iConfig.getParameter<InputTag>( "LHEsrc" ) ),
  gensrc_(iConfig.getParameter<InputTag>( "GENsrc" ) ),
  hfID_(iConfig.getParameter<int>( "hfID" ) ),
  verbose_(iConfig.getUntrackedParameter<bool>( "verbose","False" ) )
{
   //now do what ever initialization is needed

}


LHEhfFromPSMPIFilter::~LHEhfFromPSMPIFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
bool LHEhfFromPSMPIFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   bool accepted = false;

   Handle<LHEEventProduct> lheevt;
   iEvent.getByLabel( lhesrc_, lheevt );

   Handle<vector<reco::GenParticle> > genPart;
   iEvent.getByLabel( gensrc_, genPart );

   const lhef::HEPEUP hepeup_ = lheevt->hepeup();

   const int nup_ = hepeup_.NUP; 
   const std::vector<int> idup_ = hepeup_.IDUP;
   const std::vector<lhef::HEPEUP::FiveVector> pup_ = hepeup_.PUP;

   unsigned int cHF = 0;

   for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {

     // search for HF only in final state

     //     if ( icount > 2 && std::abs(idup_[icount]) == hfID_ ) { cHF++; }
     
     if ( std::abs(idup_[icount]) == hfID_ ) { cHF++; }

   }

   int pid = 0;

   if ( cHF == 0 ) { 

     for (vector<reco::GenParticle>::const_iterator itgen=genPart->begin(); itgen!=genPart->end(); itgen++) {
       pid = itgen->pdgId();
       if ((abs(pid)/100)%10 == hfID_ || (abs(pid)/1000)%10 == hfID_) { accepted = true; continue; }
     }
     
   }

   if ( accepted && verbose_ ) {
     std::cout << "HF production from PS or MPI:" << std::endl;
     for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {
       std::cout << "# " << icount << " " << idup_[icount] << " " << (pup_[icount])[0] 
                 << " " << (pup_[icount])[1] << " " << (pup_[icount])[2] << " " 
                 << (pup_[icount])[3] << " " << (pup_[icount])[4] << std::endl;
     }
     std::cout << "HF hadron ID = " << pid << std::endl;
   }

   if (accepted){
     return true; } else {return false;}
   
}


