)
)


process.analyzer = cms.EDAnalyzer("ElasticAnalyzer",
  verbosity = cms.untracked.int32(0),
  diagonal = cms.string("LBRT"),
  tracks = cms.untracked.InputTag('ctppsLocalTrackLiteProducer'),
#  ctppsDiamondRecHits = cms.untracked.InputTag('ctppsDiamondRecHits'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  outputFileName = cms.string("output_run_XXXX_re_reco_ZeroBias.root"),
  #offsetFileName = cms.string("data/offsets_no_slopes.txt"),
  offsetFileName = cms.string("data/offsets.txt"),

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



