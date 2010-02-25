import FWCore.ParameterSet.Config as cms


topSingleLeptonDQM = cms.EDAnalyzer("TopSingleLeptonDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of MonitoringEnsemble
  ##
  setup = cms.PSet(
    sources = cms.PSet(
      muons = cms.InputTag("muons"),
      jets  = cms.InputTag("antikt5CaloJets"),
      elecs = cms.InputTag("gsfElectrons")
    ),
    jetCorrector = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo")
    ),
    electronId = cms.PSet(
      electronId = cms.InputTag("eidRobustTight")
    ),
    monitoring = cms.PSet(
      verbosity = cms.string("DEBUG")
    )
  ),
                                  
  ## ------------------------------------------------------
  ## PRESELECTION
  ## 
  preselection = cms.PSet(
    trigger = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9'])
    ),
    beamspot = cms.PSet(
      src    = cms.InputTag("offlineBeamSpot"),
      select = cms.string('abs(z0)<0.20')
    )
  ),
  
  ## ------------------------------------------------------    
  ## SELECTION
  ##
  ## monitor histrograms are filled after each selection
  ## step, the selection is applied in the order defined
  ## by this vector
  ##
  selection = cms.VPSet(
    cms.PSet(
      label  = cms.string("jets:step0"),              # mandatory but can be any name
      src    = cms.InputTag("antikt5CaloJets"),       # mandatory
      select = cms.string("pt>20 & abs(eta)<2.1"),    # mandatory but can be empty
      min    = cms.int32(4),                          # optional
     #max    = cms.int32(4),                          # optional
    ),
    cms.PSet(
      label  = cms.string("elecs:step1"),    
      src    = cms.InputTag("gsfElectrons"),          # mandatory
      select = cms.string("pt>20 & abs(eta)<2.1"),    # mandatory but can be empty
      max    = cms.int32(1),                          # optional
    ),    
    cms.PSet(
      label  = cms.string("muons:step2"),    
      src    = cms.InputTag("muons"),                 # mandatory
      select = cms.string("pt>20 & abs(eta)<2.1"),    # mandatory but can be empty
      min    = cms.int32(1),                          # optional
     #max    = cms.int32(1),                          # optional
    ),
  ),
)
