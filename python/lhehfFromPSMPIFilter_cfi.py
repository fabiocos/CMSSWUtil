import FWCore.ParameterSet.Config as cms

lhehfFromPSMPIFilter = cms.EDFilter("LHEhfFromPSMPIFilter",
    LHEsrc = cms.InputTag("source"),
    GENsrc = cms.InputTag("genParticles"),
    hfID =cms.int32(5),
    verbose = cms.untracked.bool(True)                                
)                                
