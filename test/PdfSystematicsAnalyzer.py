import FWCore.ParameterSet.Config as cms

# Process name
process = cms.Process("PDFANA")

# Max events
process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1)
    input = cms.untracked.int32(10)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
    oncePerEventMode=cms.untracked.bool(False))

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input files (on disk)
process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring("file:sherpa_DYToLL_M_50_meps_7TeV_sherpa_MASTER_cff_py_GEN_1_1_Bee.root")
)

# Produce PDF weights (maximum is 3)
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
      # Fix POWHEG if buggy (this PDF set will also appear on output, 
      # so only two more PDF sets can be added in PdfSetNames if not "")
      # FixPOWHEG = cms.untracked.string("CT10.LHgrid"),
      # GenTag = cms.untracked.InputTag("prunedGenParticles"),
      useFirstAsDefault = cms.untracked.bool(True),
      PdfInfoTag = cms.untracked.InputTag("generator"),
      PdfSetNames = cms.untracked.vstring(
             "CT10.LHgrid"
      )
)

process.load("fabiocos.CMSSWUtil.pdfWpropagator_cfi")
process.load("fabiocos.CMSSWUtil.testGenInfoProduct_cfi")
process.testGenInfoProduct.genevtCollection = cms.InputTag("pdfWpropagator")
process.pdfWpropagator.iSet = cms.uint32(1)

# Output definition
process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('file:pdfana.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pdfana')
    )
)

process.load("GeneratorInterface.RivetInterface.rivetAnalyzer_cfi")
process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_EWK_12_XXX')
process.rivetAnalyzer.UseExternalWeight = cms.bool(True)
process.rivetAnalyzer.GenEventInfoCollection = cms.InputTag("pdfWpropagator")
process.rivetAnalyzer.OutputFile = cms.string("rivet_1.aida")

# Main path
#process.pdfana = cms.Path(process.pdfWeights*process.pdfWpropagator*process.testGenInfoProduct*process.rivetAnalyzer)
process.pdfana = cms.Path(process.pdfWeights*process.pdfWpropagator*process.rivetAnalyzer)

#process.end = cms.EndPath(process.RAWSIMoutput)
