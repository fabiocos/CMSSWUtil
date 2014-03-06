// Trivial filter to select a given weight range for MC events according to 
// first weight stored in GenEventInfoProduct

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


class MCWeightFilter : public edm::EDFilter {
public:
  explicit MCWeightFilter(const edm::ParameterSet&);
  ~MCWeightFilter();
  
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

private:
  // ----------member data ---------------------------
  
  edm::InputTag src_;
  double wmin_;
  double wmax_;
  bool verbose_;

  int numEventsTotal_, numEventsPassed_;
  double SumW_,SumW2_;

};

MCWeightFilter::MCWeightFilter(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>( "src" ) ),
  wmin_(iConfig.getParameter<double>( "WeightMin" ) ),
  wmax_(iConfig.getParameter<double>( "WeightMax" ) ),
  verbose_(iConfig.getUntrackedParameter<bool>( "verbose","False" ) ),
  numEventsTotal_(0),numEventsPassed_(0),SumW_(0.),SumW2_(0.)
{

  if ( wmin_ > wmax_ ) {
    throw cms::Exception("WrongInput") << "Minimum weight " << wmin_ << " greater than maximum " << wmax_;
  }
  else {
    if ( verbose_ ) edm::LogInfo("WeightRange") << "WeightMin = " << wmin_ << " WeightMax = " << wmax_;
  }

}


MCWeightFilter::~MCWeightFilter()
{
}

bool MCWeightFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   bool accepted = false;

   edm::Handle<GenEventInfoProduct> info;
   iEvent.getByLabel( src_, info );

   double thisWeight = info->weights()[0];

   if ( verbose_ ) edm::LogInfo("EventWeight") << iEvent.id() << " Weight = " << thisWeight;
   if ( thisWeight >= wmin_ && thisWeight <= wmax_ ) { accepted = true; }
   else { edm::LogWarning("OutOfRangeWeight") << iEvent.id() << " Weight = " << thisWeight; }

   numEventsTotal_++;
   if (accepted){ 
     numEventsPassed_++;
     SumW_ += thisWeight;
     SumW2_ += thisWeight*thisWeight;
     return true; 
   } else {return false;}

}

void MCWeightFilter::endJob()
{

  double eff = ( numEventsTotal_ > 0 ? (double)numEventsPassed_/(double)numEventsTotal_ : 1. ) ;
  double err = ( numEventsTotal_ > 0 ? std::sqrt((double)numEventsPassed_*(1.-(double)numEventsPassed_/(double)numEventsTotal_))/(double)numEventsTotal_ : 1. ); 
  double wave = ( numEventsPassed_ > 0 ? SumW_/(double)numEventsPassed_ : 0. );
  double wave2 = ( numEventsPassed_ > 0 ? std::sqrt(SumW2_)/(double)numEventsPassed_ : 0. );
  double wequiv = ( SumW2_ > 0. ? SumW_*SumW_/SumW2_ : 0. );
  if ( verbose_ ) edm::LogInfo("WeightSummary") 
                    << "Weight Filter Summary: \n" 
                    << "--------------------------------------------------------------\n"
                    << "Total events               = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(0) << numEventsTotal_ << "\n" 
                    << "Passed events              = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(0) << numEventsPassed_ << "\n"
                    << "Filter efficiency          = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(6) << eff << " +/- " << std::setfill(' ') << std::setw(14) << std::setprecision(6) << err << "\n"
                    << "Passed average weight      = " << std::setfill(' ') << std::setw(14) << std::scientific << std::setprecision(3) << wave << " +/- " << std::setfill(' ') << std::setw(14) << std::setprecision(3) << wave2 << "\n"
                    << "Passed equivalent # events = " << std::setfill(' ') << std::setw(16) << std::fixed << std::setprecision(1) << wequiv;

}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MCWeightFilter);

