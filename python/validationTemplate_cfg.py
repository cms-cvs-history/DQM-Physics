import FWCore.ParameterSet.Config as cms
  
process = cms.Process("TOPVAL")

## Message Logger (see: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMessageLogger for more information)
process.MessageLogger = cms.Service("MessageLogger",
	categories = cms.untracked.vstring('MainResults'
#					  ,'Debug'
	)
)

## DQM Services
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Core.DQMStore_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

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
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))

## define output options
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
