import FWCore.ParameterSet.Config as cms

process = cms.Process("TopDQM")

## Message Logger (see: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMessageLogger for more information)
process.MessageLogger = cms.Service("MessageLogger",
	categories = cms.untracked.vstring('MainResults'
#					  ,'Debug'
	)
)

## source Input File
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  ## the following files are in for testing
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0011/FE3B3C36-B413-DF11-9F84-002618FDA216.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/EC6B5FD5-6013-DF11-9795-001BFCDBD100.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/AE53FEB0-5F13-DF11-956B-001A928116C0.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/3621BC47-6013-DF11-9376-001731AF6A7D.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/280BA505-6513-DF11-867B-001A92971BB2.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/222EAB18-6B13-DF11-97D3-001A92971AEC.root',
        '/store/relval/CMSSW_3_4_2/RelValProdTTbar/GEN-SIM-RECO/MC_3XY_V15-v1/0010/166D53B0-6613-DF11-A4DC-0030486791C6.root'

  )
)
## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200))

## define output options
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/My/Test/DataSet')

## add jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo") 

## define process
## process.load("DQM.Physics.leptonJetsChecker_cfi")
process.load("DQM.Physics.dileptonJetsChecker_cfi")

## configure full-leptonic channels
## process.leptonJetsChecker.verbose      = False
#process.diLeptonsCheckerDiel.eidTight = 'eidRobustTight'
#process.diLeptonsCheckerDiel.eidTight = 'eidTight'
#process.diLeptonsCheckerDiel.eidTight = 'eidLoose'  ## this parameter does notv even exist!?!
#process.diLeptonsCheckerDiel.eidTight = 'eidRobustLoose'

# MET correction for Muons
from JetMETCorrections.Type1MET.MuonMETValueMapProducer_cff import *
from JetMETCorrections.Type1MET.MetMuonCorrections_cff import *
from JetMETCorrections.Type1MET.MetType1Corrections_cff import *

process.load("JetMETCorrections.Type1MET.MetType1Corrections_cff")
process.load("JetMETCorrections.Type1MET.MetMuonCorrections_cff")


process.diLeptonsCheckerDiel.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
process.diLeptonsCheckerDimu.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
process.diLeptonsCheckerElmu.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
	
process.p = cms.Path(
  process.metJESCorAK5CaloJet*
  process.diLeptonsCheckerDimu*
  process.diLeptonsCheckerDiel*
  process.diLeptonsCheckerElmu*
  process.dqmSaver
)

