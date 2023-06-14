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

from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag.globaltag = "101X_dataRun2_Express_v8"
process.GlobalTag.globaltag = "101X_dataRun2_Prompt_v11"

process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#"root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/run_324536.1_re_reco_Totem1.root",
#"root://eostotem//eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/run_324575.0_re_reco_Totem1.root",
#"root://eostotem.cern.ch///eos/totem/data/offline/2018/450GeV/beta11/ZeroBias/version5/run_324579.6_re_reco_ZeroBias.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/run_324579.0_re_reco_Totem1.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/run_324579.1_re_reco_Totem1.root",
"root://eostotem//eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/run_324579.2_re_reco_Totem1.root",
#"root://eostotem//eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/run_324578.0_re_reco_Totem1.root",
#"root://eostotem.cern.ch///eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/run_324458.1_re_reco_Totem1.root",
#"root://eostotem.cern.ch///eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/run_324530.2_re_reco_Totem1.root",
)
)


process.analyzer = cms.EDAnalyzer("ElasticAnalyzer",
  verbosity = cms.untracked.int32(0),
  diagonal = cms.string("LBRT"),
  tracks = cms.untracked.InputTag('ctppsLocalTrackLiteProducer'),
  outputFileName = cms.string("output.root")
)

process.p = cms.Path(process.analyzer)



