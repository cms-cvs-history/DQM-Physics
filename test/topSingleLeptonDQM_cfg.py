import FWCore.ParameterSet.Config as cms

process = cms.Process("TopDQM")
process.load("DQM.Physics.topSingleLeptonDQM_cfi")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/My/Test/DataSet')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.source = cms.Source(
    "PoolSource"
    ,fileNames = cms.untracked.vstring(
     '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0007/CE243FB9-A778-DE11-8891-000423D98BC4.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/D63BFAF8-5178-DE11-8439-001D09F24F65.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/B8BA4AF7-5178-DE11-9F72-001D09F23A6B.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/9C8FF416-5278-DE11-B809-0019B9F70607.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/98079214-5278-DE11-8880-001D09F27067.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/62B58021-5278-DE11-9574-0019B9F6C674.root'
    ,'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/3AEF4E28-5278-DE11-A2D4-000423D6CA72.root'
    )
)

## load jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo")

#process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(#process.content *
                     process.topSingleLeptonDQM +
                     process.dqmSaver
                     )
