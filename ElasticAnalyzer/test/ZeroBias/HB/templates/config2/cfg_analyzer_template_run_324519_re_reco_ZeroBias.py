import FWCore.ParameterSet.Config as cms

process = cms.Process("ElasticAnalysis")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#process.load("CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi")
#process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring('alignment_files/results_cumulative_factored_Jan.xml')

from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag.globaltag = "101X_dataRun2_Express_v8"
process.GlobalTag.globaltag = "101X_dataRun2_Prompt_v11"

process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
#  input = cms.untracked.int32(200000)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.0_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.10_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.11_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.12_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.13_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.14_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.15_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.16_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.17_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.18_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.19_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.1_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.20_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.21_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.22_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.23_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.24_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.25_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.26_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.27_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.28_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.29_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.2_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.30_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.31_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.32_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.33_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.34_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.35_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.36_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.37_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.38_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.39_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.3_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.40_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.41_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.42_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.43_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.44_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.45_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.46_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.47_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.48_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.49_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.4_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.5_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.6_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.7_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.8_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324519.9_re_reco_ZeroBias.root",
)
)


process.analyzer = cms.EDAnalyzer("ElasticAnalyzer",
  verbosity = cms.untracked.int32(0),
  diagonal = cms.string("LBRT"),
  tracks = cms.untracked.InputTag('ctppsLocalTrackLiteProducer'),
#  ctppsDiamondRecHits = cms.untracked.InputTag('ctppsDiamondRecHits'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  outputFileName = cms.string("output_run_324519_re_reco_ZeroBias.root"),
  #offsetFileName = cms.string("data/offsets_no_slopes.txt"),
  offsetFileName = cms.string("data/offsets.txt"),
  timestamp_minimum = cms.string("1539374116"),
  timestamp_maximum = cms.string("1539378067"),


   position_distribution = cms.PSet(
      type = cms.string("box"),
      x_mean = cms.double(5.0),       #in mm
      x_width = cms.double(10.0),
      x_min = cms.double(0.0),
      x_max = cms.double(0.0),

      y_mean = cms.double(0.0),
      y_width = cms.double(20.0),
      y_min = cms.double(0.0),
      y_max = cms.double(0.0)
    ),
)

process.p = cms.Path(process.analyzer)



