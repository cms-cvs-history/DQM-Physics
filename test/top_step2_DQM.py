# Auto generated configuration file
# using: 
# Revision: 1.169 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RECO,DQM -n 3 --conditions FrontierConditions_GlobalTag,STARTUP_V4::All --data --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('TOPDQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('step2 nevts:3'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
     '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FEC52E5B-D767-DF11-9A6C-0030486733D8.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FEBD6C22-AD67-DF11-8AB1-0030487C6A66.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE9A8FC7-B267-DF11-A3B1-001D09F24EAC.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE69F9A2-B767-DF11-A1C5-000423D944FC.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE4A23FA-AF67-DF11-9B9C-0030487D05B0.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE3E1C3E-E367-DF11-8275-001D09F29533.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE068AA5-DC67-DF11-8706-001D09F29533.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FE0484BA-C567-DF11-B1C4-001D09F2438A.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FCA9819F-D667-DF11-BB90-001D09F2423B.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FCA92512-C567-DF11-A8FA-000423DD2F34.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FCA83051-BD67-DF11-BE1E-001D09F242EA.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FC5016EE-CE67-DF11-9298-0030487A18F2.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FC260F3C-A867-DF11-99A1-001D09F25438.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FC19F6F4-C967-DF11-8746-001D09F252DA.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FC0507F8-C967-DF11-8755-001D09F2960F.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FAF21623-C767-DF11-B262-001D09F28EC1.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FAC6C98C-BD67-DF11-8B2C-001617E30CC8.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FAB5C69B-D667-DF11-B4EB-000423D996B4.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA9BEAA1-BC67-DF11-98F1-001D09F248F8.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA97486D-BF67-DF11-8308-001D09F24D8A.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA96C90B-DD67-DF11-AACA-001D09F29533.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA8C9FCB-D367-DF11-AC42-000423D94E1C.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA3C313E-E367-DF11-B7EC-0019B9F70607.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA3817F4-DC67-DF11-8286-001D09F232B9.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA2E0ABC-A967-DF11-9F62-001D09F2AF1E.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA0EC4C6-B967-DF11-A04C-0019B9F4A1D7.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/FA086EA2-BC67-DF11-A4C1-001D09F295FB.root',
        '/store/express/Run2010A/ExpressPhysics/FEVT/v2/000/136/100/F8F82ECB-DF67-DF11-9019-001D09F2A465.root',
       )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = cms.untracked.vstring('drop *_*_*_*','keep *_*_*_TOPDQM'), #process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('step2_DQM.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    )
)



## load jet corrections
process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")
process.prefer("ak5CaloL2L3")

## configure message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('TopSingleLeptonDQM'   )
process.MessageLogger.cerr.TopSingleLeptonDQM    = cms.untracked.PSet(limit = cms.untracked.int32(1))
process.MessageLogger.categories.append('TopDiLeptonOfflineDQM')
process.MessageLogger.cerr.TopDiLeptonOfflineDQM = cms.untracked.PSet(limit = cms.untracked.int32(1))






# Additional output definition

# Other statements
#process.GlobalTag.globaltag = 'GR10_P_V5::All'
process.GlobalTag.globaltag = 'GR10_P_V5::All'

# Path and EndPath definitions
process.reconstruction_step = cms.Path(process.reconstruction)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
process.dqmoffline_step = cms.Path(process.dqmPhysics)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.p = cms.Path(#process.content *
                     process.topSingleMuonMediumDQM +
                     process.topSingleElectronMediumDQM +
                     process.topSingleMuonLooseDQM +
                     process.topSingleElectronLooseDQM +
                     process.topDiLeptonOfflineDQM +
                     process.topSingleLeptonDQM
                     )
process.schedule = cms.Schedule(process.p, process.endjob_step, process.out_step)
