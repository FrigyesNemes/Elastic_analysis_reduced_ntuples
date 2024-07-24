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
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.0_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.100_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.101_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.102_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.103_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.104_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.105_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.106_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.107_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.108_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.109_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.10_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.110_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.111_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.112_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.113_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.114_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.115_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.11_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.12_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.13_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.14_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.15_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.16_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.17_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.18_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.19_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.1_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.20_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.21_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.22_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.23_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.24_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.25_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.26_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.27_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.28_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.29_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.2_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.30_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.31_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.32_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.33_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.34_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.35_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.36_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.37_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.38_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.39_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.3_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.40_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.41_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.42_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.43_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.44_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.45_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.46_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.47_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.48_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.49_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.4_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.50_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.51_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.52_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.53_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.54_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.55_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.56_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.57_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.58_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.59_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.5_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.60_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.61_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.62_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.63_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.64_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.65_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.66_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.67_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.68_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.69_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.6_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.70_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.71_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.72_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.73_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.74_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.75_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.76_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.77_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.78_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.79_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.7_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.80_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.81_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.82_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.83_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.84_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.85_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.86_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.87_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.88_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.89_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.8_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.90_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.91_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.92_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.93_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.94_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.95_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.96_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.97_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.98_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.99_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/ZeroBias/version5/run_324532.9_re_reco_ZeroBias.root",
)
)


process.analyzer = cms.EDAnalyzer("ElasticAnalyzer",
  verbosity = cms.untracked.int32(0),
  diagonal = cms.string("LBRT"),
  tracks = cms.untracked.InputTag('ctppsLocalTrackLiteProducer'),
#  ctppsDiamondRecHits = cms.untracked.InputTag('ctppsDiamondRecHits'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  outputFileName = cms.string("output_run_324532_re_reco_ZeroBias.root"),
  #offsetFileName = cms.string("data/offsets_no_slopes.txt"),
  offsetFileName = cms.string("data/offsets.txt"),
  timestamp_minimum = cms.string("1539414804"),
  timestamp_maximum = cms.string("1539426981"),


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



