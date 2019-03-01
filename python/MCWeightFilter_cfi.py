import FWCore.ParameterSet.Config as cms
#Test comment
MCWeightFilter = cms.EDFilter("MCWeightFilter",
    src = cms.InputTag("generator"),
    WeightMin = cms.double(-1e9),                          
    WeightMax = cms.double(1e9),                          
    verbose = cms.untracked.bool(False)                                
)                                
