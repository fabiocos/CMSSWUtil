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
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
private:
  
  edm::InputTag genevtCollection_;
  edm::InputTag pdfWCollection_;

  unsigned int iSet_;

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

  std::cout << "Before " << genevt->weights()[0] << " PDF w = " << (*weightHandle)[iSet_] << std::endl;

  std::vector<double> tmpW(genevt->weights()); 
  tmpW[0] *= (*weightHandle)[iSet_];
  modEventInfo->setWeights(tmpW);

  std::cout << "After " << modEventInfo->weights()[0] << std::endl;

  iEvent.put(modEventInfo);

}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PdfWpropagator);
