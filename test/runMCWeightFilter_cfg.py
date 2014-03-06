import FWCore.ParameterSet.Config as cms

process = cms.Process('WFILTER')

# import of standard configurations
process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'
# process.MessageLogger.categories.extend(['WeightRange','EventWeight','WeightSummary'])
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:gen.root')                        
)

# Rivet analysis

process.load('fabiocos.CMSSWUtil.MCWeightFilter_cfi')
process.MCWeightFilter.src = cms.InputTag("generator")
process.MCWeightFilter.WeightMin = cms.double(-1.e9)
process.MCWeightFilter.WeightMax = cms.double(1.e9)
# process.MCWeightFilter.verbose = cms.untracked.bool(True)

process.wfilter_step = cms.Sequence(process.MCWeightFilter)

process.p1 = cms.Path(process.wfilter_step)


