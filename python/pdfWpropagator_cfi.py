import FWCore.ParameterSet.Config as cms

pdfWpropagator = cms.EDProducer("PdfWpropagator",
    genevtCollection = cms.InputTag("generator"),
    pdfWCollection = cms.InputTag("pdfWeights:CT10"),
    iSet = cms.uint32(0)                                
)                    
