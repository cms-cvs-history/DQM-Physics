import FWCore.ParameterSet.Config as cms


topSingleLeptonDQM = cms.EDAnalyzer("TopSingleLeptonDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of the MonitoringEnsemble(s)
  ## [mandatory] : optional PSets may be omitted
  ##
  setup = cms.PSet(
    ## [mandatory]
    sources = cms.PSet(
      muons = cms.InputTag("muons"),
      elecs = cms.InputTag("gsfElectrons"),
      jets  = cms.InputTag("ak5CaloJets"),
      mets  = cms.VInputTag("met", "tcMet", "pfMet")
    ),
    ## [optional] : when omitted the verbosity level is set to STANDARD
    monitoring = cms.PSet(
      verbosity = cms.string("DEBUG")
    ),
    ## [optional] : when omitted all monitoring plots for the electron
    ## will be filled w/o preselection on the electronId variable
    elecExtras = cms.PSet(
      electronId = cms.InputTag("eidRobustTight")
    ),
    ## [optional] : when omitted all monitoring plots for jets will
    ## be filled from uncorrected jets
    jetExtras = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo"),
      jetBTaggers  = cms.PSet( trackCountingEff = cms.PSet(label = cms.InputTag("trackCountingHighEffBJetTags" ), workingPoint = cms.double(1.25)),
                               trackCountingPur = cms.PSet(label = cms.InputTag("trackCountingHighPurBJetTags" ), workingPoint = cms.double(3.00)),
                               secondaryVertex  = cms.PSet(label = cms.InputTag("simpleSecondaryVertexBJetTags"), workingPoint = cms.double(2.05))
                               )
    )
  ),
                                  
  ## ------------------------------------------------------
  ## PRESELECTION
  ##
  ## setup of the event preselection, which will not
  ## be monitored
  ## [mandatory] : but may be empty
  ##
  preselection = cms.PSet(
    ## [optional] : when omitted no preselection is applied
    trigger = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9'])
    ),
    ## [optional] : when omitted no preselection is applied
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
  ## [mandatory] : may be empty or contain an arbitrary
  ## number of PSets as given below
  ##
  selection = cms.VPSet(
    cms.PSet(
      label  = cms.string("jets:step0"),                   # mandatory: 'jets' defined the objects to select on, but can be any name after the ':'
      src    = cms.InputTag("ak5CaloJets"),                # mandatory
      select = cms.string("pt>30 & abs(eta)<2.1"),         # mandatory but can be empty
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo"),# optional
      min    = cms.int32(4),                               # optional
    ),
    cms.PSet(
      label  = cms.string("elecs:step1"),    
      src    = cms.InputTag("gsfElectrons"),
      select = cms.string("pt>20 & abs(eta)<2.1 & (dr04TkSumPt+dr04EcalRecHitSumEt+dr04HcalTowerSumEt)/pt<0.2"),
      electronId  = cms.InputTag("eidRobustTight"),
      max    = cms.int32(0),
    ),
    cms.PSet(
      label  = cms.string("muons:step2"),    
      src    = cms.InputTag("muons"),
      select = cms.string("pt>20 & abs(eta)<2.1 & (isolationR03.sumPt+isolationR03.emEt+isolationR03.hadEt)/pt<0.1"),
      min    = cms.int32(1),
      max    = cms.int32(1)
    ),    
  ),
)
