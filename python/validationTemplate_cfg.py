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
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/3291E09D-67B7-DE11-9ED6-003048678C9A.root',
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/626F521F-66B7-DE11-8D68-003048679084.root',
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/7CFBE1E9-6FB7-DE11-9866-0018F3D09688.root',
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0002/08E5EBB7-72B7-DE11-A12F-001A92810A9A.root',
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0002/2C546D09-76B7-DE11-B46C-00304867920C.root',
  '/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0002/304FA853-79B7-DE11-9AE7-001731EF61B4.root'
  )
)
## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))

## define output options
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
