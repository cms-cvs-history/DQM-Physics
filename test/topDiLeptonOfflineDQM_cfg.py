import FWCore.ParameterSet.Config as cms

process = cms.Process("TopDQM")
process.load("DQM.Physics.topDiLeptonOfflineDQM_cfi")

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
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/7672446D-DB2B-DF11-B140-001E0B5FA5A8.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/74D27791-DA2B-DF11-8A16-0017A4770430.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/741BDD8D-DA2B-DF11-AAC0-0017A4770038.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/729B608C-DA2B-DF11-828F-00237DA1985A.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/621FCA6C-DB2B-DF11-9813-001E0B48A1C4.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/606403A4-DC2B-DF11-8246-0019BB32F1EE.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/5ECBE2A8-D92B-DF11-91A2-002264055CE4.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/5AC0F529-E12B-DF11-8908-0017A4770438.root',
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_3XY_V25_preproduction-v1/0002/54A1FE5E-DC2B-DF11-8C14-001B78CE74FE.root'
    )
)

## load jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo")

#process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(#process.content *
                     process.topDiLeptonOfflineDQM +
                     process.dqmSaver
                     )
