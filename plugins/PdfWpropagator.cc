// PDF weight propagator, needed to transmit PDF weights to Rivet CMSSW interface

// framework & common header files
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include <vector>

class PdfWpropagator : public edm::EDProducer
{
public:
  explicit PdfWpropagator(const edm::ParameterSet&);
  virtual ~PdfWpropagator();
  virtual void beginJob() ;
  virtual void endJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
private:
  
  edm::InputTag genevtCollection_;
  edm::InputTag pdfWCollection_;

  unsigned int iSet_;

  unsigned int nEvt_;
  double Sw_orig_;
  double Sw2_orig_;
  double Sw_rewe_;
  double Sw2_rewe_;

};


using namespace edm;

PdfWpropagator::PdfWpropagator(const edm::ParameterSet& iPSet):  
  genevtCollection_(iPSet.getParameter<edm::InputTag>("genevtCollection")),
  pdfWCollection_(iPSet.getParameter<edm::InputTag>("pdfWCollection")),
  iSet_(iPSet.getParameter<unsigned int>("iSet"))
{    

  produces<GenEventInfoProduct>();

}

PdfWpropagator::~PdfWpropagator() {}

void PdfWpropagator::beginJob() {

  nEvt_ = 0;
  Sw_orig_ = 0.;
  Sw2_orig_ = 0.;
  Sw_rewe_ = 0.;
  Sw2_rewe_ = 0.;

}

void PdfWpropagator::endJob() {

  if ( nEvt_ == 0 ) { edm::LogWarning("NoEvent") << "No selected event for reweighting"; return; }

  double ave_orig = Sw_orig_/(double)nEvt_;

  double ave_rewe = Sw_rewe_/(double)nEvt_;
  double eave_rewe = std::sqrt(Sw2_rewe_)/(double)nEvt_;

  double ratio_ave = Sw_rewe_/Sw_orig_;
  double eratio_ave = std::sqrt(Sw2_rewe_)/Sw_orig_;

  edm::LogInfo("Normalizations") << "# events = " << nEvt_ << " <W_orig> = " << ave_orig << "\n" 
                                 << "<W_rewe> = " << ave_rewe << " +/- " << eave_rewe << "\n"
                                 << "ratio = " << ratio_ave << " +/- " << eratio_ave;

}

void PdfWpropagator::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 

  edm::Handle<GenEventInfoProduct> genevt;
  iEvent.getByLabel(genevtCollection_, genevt );

  edm::Handle<std::vector<double> > weightHandle;
  iEvent.getByLabel(pdfWCollection_, weightHandle );

  if ( weightHandle->size() < iSet_ ) {
    throw cms::Exception("MissingWeights") << "The requested weight cannot be found in the corresponding array";
  }

  // recompute the new event weight overwriting the original one

  std::auto_ptr<GenEventInfoProduct> modEventInfo(new GenEventInfoProduct(*genevt));
  
  nEvt_++;

  double oldW = genevt->weights()[0];
  double pdfW = (*weightHandle)[iSet_];
  double newW = oldW*pdfW;

  Sw_orig_ += oldW;
  Sw2_orig_ += oldW*oldW;

  Sw_rewe_ += newW;
  Sw2_rewe_ += newW*newW;
  
  //  std::cout << "Before " << oldW << " PDF w = " << pdfW << std::endl;
  //  std::cout << "nEvt = " << nEvt_ << " Sw_orig = " << Sw_orig_ << " Sw_rewe = " << Sw_rewe_ << std::endl;

  std::vector<double> tmpW; 
  tmpW.push_back(newW);
  modEventInfo->setWeights(tmpW);

  //  std::cout << "After " << modEventInfo->weights()[0] << std::endl;

  iEvent.put(modEventInfo);

}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PdfWpropagator);
