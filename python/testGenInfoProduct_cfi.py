import FWCore.ParameterSet.Config as cms

testGenInfoProduct = cms.EDAnalyzer("TestGenInfoProduct",
    genrunCollection = cms.InputTag("generator"),
    genevtCollection = cms.InputTag("generator"),
    runOnly = cms.untracked.bool(False)                                
)                    
