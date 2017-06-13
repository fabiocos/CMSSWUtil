#ifndef LHEHFFROMPSMPIFILTER_h
#define LHEHFFROMPSMPIFILTER_h
// -*- C++ -*-
//
// Package:    LHEhfFromPSMPIFilter
// Class:      LHEhfFromPSMPIFilter
// 
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//
// class decleration
//

class LHEhfFromPSMPIFilter : public edm::EDFilter {
   public:
      explicit LHEhfFromPSMPIFilter(const edm::ParameterSet&);
      ~LHEhfFromPSMPIFilter();


      virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------
      
      edm::InputTag lhesrc_;
      edm::InputTag gensrc_;
      int hfID_;
      bool verbose_;
};
#endif
