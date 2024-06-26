import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process("CTPPSReRecoWithAlignment", eras.ctpps_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# define global tag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_HLT_v7', '')

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
		"/store/data/Run2018D/Totem1/AOD/PromptReco-v2/000/324/581/00000/86C24AE4-F98A-4549-8990-85C28740DAC2.root"
  ),

  lumisToProcess = cms.untracked.VLuminosityBlockRange("324581:1-324581:max"),

  dropDescendantsOfDroppedBranches=cms.untracked.bool(False),
  inputCommands = cms.untracked.vstring(
    'drop *',
    'keep TotemRPRecHitedmDetSetVector_*_*_*',
    'keep CTPPSPixelRecHitedmDetSetVector_*_*_*',
    'keep CTPPSDiamondRecHitedmDetSetVector_*_*_*',
    'keep CTPPSDiamondLocalTrackedmDetSetVector_*_*_*',
    'keep edmTriggerResults_*_*_*',
    'keep GlobalAlgBlkBXVector_*_*_*',
  )
)

#process.maxEvents = cms.untracked.PSet(
#  input = cms.untracked.int32(1000)
#)

# RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_DD_cff")

# use the correct geometry
del(process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("data/geometry/2018_900GeV_beta11/RP_Dist_Beam_Cent.xml")

# add alignment corrections
process.ctppsIncludeAlignmentsFromXML.RealFiles += cms.vstring("data/alignment/2018_900GeV_beta11/s/version1/45.xml", "data/alignment/2018_900GeV_beta11/s/version1/56.xml")

# reconstruction sequences
process.path_reco = cms.Path(
  process.totemRPUVPatternFinder
  * process.totemRPLocalTrackFitter

  * process.ctppsPixelLocalTracks

  * process.ctppsLocalTrackLiteProducer
)

# output configuration
process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string("/pool/re_reco_Totem1/324581/0/output.root"),
  outputCommands = cms.untracked.vstring(
    "drop *",

    'keep TotemRPRecHitedmDetSetVector_*_*_*',
    'keep TotemRPUVPatternedmDetSetVector_*_*_*',
    #'keep TotemRPLocalTrackedmDetSetVector_*_*_*',

    'keep CTPPSDiamondRecHitedmDetSetVector_*_*_*',

    'keep CTPPSPixelRecHitedmDetSetVector_*_*_*',

    'keep CTPPSLocalTrackLites_*_*_*'
  )
)

process.outpath = cms.EndPath(process.output)
