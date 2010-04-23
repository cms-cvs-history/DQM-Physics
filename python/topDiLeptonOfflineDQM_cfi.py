import FWCore.ParameterSet.Config as cms


topDiLeptonOfflineDQM = cms.EDAnalyzer("TopDiLeptonOfflineDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of the MonitoringEnsemble(s)
  ## [mandatory] : optional PSets may be omitted
  ##
  setup = cms.PSet(
    ## sub-directory to write the monitor histograms to
    ## [mandatory] : should not be changed w/o explicit 
    ## communication to TopCom!
    directory = cms.string("Physics/Top/TopDiLeptonDQM/"),

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
      electronId = cms.InputTag("eidLoose")
    ),
    ## [optional] : when omitted all monitoring plots for jets will
    ## be filled from uncorrected jets
    jetExtras = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo")
    ),
    ## [optional] : when omitted all monitoring plots for triggering
    ## will be empty
    triggerExtras = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9:HLT_Ele15_SW_L1R',
                            'HLT_Mu15:HLT_Ele15_SW_L1R',
                            'HLT_DoubleMu3:HLT_Ele15_SW_L1R',
                            'HLT_Ele15_SW_L1R:HLT_Mu9',
                            'HLT_Ele15_SW_L1R:HLT_DoubleMu3'])    
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
      select = cms.vstring(['HLT_Mu9','HLT_Ele15_SW_L1R','HLT_DoubleMu3'])
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
  ## number of PSets as given below:
  ##
  selection = cms.VPSet(
    cms.PSet(
      ## [mandatory] : 'jets' defines the objects to
      ## select on, 'step0' labels the histograms;
      ## instead of 'step0' you can choose any label
      label  = cms.string("jets/calo:step0"),
      ## [mandatory] : defines the input collection      
      src    = cms.InputTag("ak5CaloJets"),
      ## [mandatory] : can be empty or of any kind
      ## of allowed selection string
      select = cms.string("pt>20 & abs(eta)<2.1 & 0.05<emEnergyFraction & emEnergyFraction<0.95"),
      min    = cms.int32(3),
    ),
  ),
)

topMuonMuonDQM = cms.EDAnalyzer("TopDiLeptonOfflineDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of the MonitoringEnsemble(s)
  ## [mandatory] : optional PSets may be omitted
  ##
  setup = cms.PSet(
    ## sub-directory to write the monitor histograms to
    ## [mandatory] : should not be changed w/o explicit 
    ## communication to TopCom!
    directory = cms.string("Physics/Top/TopMuonMuonDQM/"),

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
      electronId = cms.InputTag("eidLoose")
    ),
    ## [optional] : when omitted all monitoring plots for jets will
    ## be filled from uncorrected jets
    jetExtras = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo")
    ),
    ## [optional] : when omitted all monitoring plots for triggering
    ## will be empty
    triggerExtras = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9:HLT_Ele15_SW_L1R',
                            'HLT_Mu15:HLT_Ele15_SW_L1R',
                            'HLT_DoubleMu3:HLT_Ele15_SW_L1R',
                            'HLT_Ele15_SW_L1R:HLT_Mu9',
                            'HLT_Ele15_SW_L1R:HLT_DoubleMu3'])    
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
      select = cms.vstring(['HLT_Mu9','HLT_Ele15_SW_L1R','HLT_DoubleMu3'])
    ),
    ## [optional] : when omitted no preselection is applied
    beamspot = cms.PSet(
      src    = cms.InputTag("offlineBeamSpot"),
      select = cms.string('abs(z0)<0.20')
    ),
  ),
  
  ## ------------------------------------------------------    
  ## SELECTION
  ##
  ## monitor histrograms are filled after each selection
  ## step, the selection is applied in the order defined
  ## by this vector
  ## [mandatory] : may be empty or contain an arbitrary
  ## number of PSets as given below:
  ##
  selection = cms.VPSet(
    cms.PSet(
      label  = cms.string("muons:step0"),    
      src    = cms.InputTag("muons"),
      select = cms.string("pt>20 & abs(eta)<2.4"),
      min    = cms.int32(1)
    ),
    cms.PSet(
      label  = cms.string("muons:step1"),    
      src    = cms.InputTag("muons"),
      select = cms.string("pt>20 & abs(eta)<2.4 & pt/(pt+isolationR03.sumPt)>0.9 & pt/(pt+isolationR03.emEt+isolationR03.hadEt)>0.9"),
      min    = cms.int32(1)
    ),     
  ),
)

topElectronMuonDQM = cms.EDAnalyzer("TopDiLeptonOfflineDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of the MonitoringEnsemble(s)
  ## [mandatory] : optional PSets may be omitted
  ##
  setup = cms.PSet(
    ## sub-directory to write the monitor histograms to
    ## [mandatory] : should not be changed w/o explicit 
    ## communication to TopCom!
    directory = cms.string("Physics/Top/TopElectronMuonDQM/"),

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
      electronId = cms.InputTag("eidLoose")
    ),
    ## [optional] : when omitted all monitoring plots for jets will
    ## be filled from uncorrected jets
    jetExtras = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo")
    ),
    ## [optional] : when omitted all monitoring plots for triggering
    ## will be empty
    triggerExtras = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9:HLT_Ele15_SW_L1R',
                            'HLT_Mu15:HLT_Ele15_SW_L1R',
                            'HLT_DoubleMu3:HLT_Ele15_SW_L1R',
                            'HLT_Ele15_SW_L1R:HLT_Mu9',
                            'HLT_Ele15_SW_L1R:HLT_DoubleMu3'])    
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
      select = cms.vstring(['HLT_Mu9','HLT_Ele15_SW_L1R','HLT_DoubleMu3'])
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
  ## number of PSets as given below:
  ##
  selection = cms.VPSet(
    cms.PSet(
      label  = cms.string("elecs:step0"),
      src    = cms.InputTag("gsfElectrons"),
      select = cms.string("pt>15 & abs(eta)<2.4"),
      electronId  = cms.InputTag("eidLoose"),
      min    = cms.int32(1)
    ),
    cms.PSet(
      label  = cms.string("muons:step1"),    
      src    = cms.InputTag("muons"),
      select = cms.string("pt>20 & abs(eta)<2.4"),
      min    = cms.int32(1)
    ),     
    cms.PSet(
      label  = cms.string("elecs:step2"),    
      src    = cms.InputTag("gsfElectrons"),
      select = cms.string("pt>20 & abs(eta)<2.4 & pt/(pt+dr03TkSumPt)>0.9 & pt/(pt+dr04EcalRecHitSumEt+dr04HcalTowerSumEt)>0.8"),
      electronId  = cms.InputTag("eidLoose"),
      min    = cms.int32(1)
    ),
    cms.PSet(
      label  = cms.string("muons:step3"),    
      src    = cms.InputTag("muons"),
      select = cms.string("pt>20 & abs(eta)<2.4 & pt/(pt+isolationR03.sumPt)>0.9 & pt/(pt+isolationR03.emEt+isolationR03.hadEt)>0.9"),
      min    = cms.int32(1)
    ),        
  ),
)

topElectronElectronDQM = cms.EDAnalyzer("TopDiLeptonOfflineDQM",
  ## ------------------------------------------------------
  ## SETUP
  ##
  ## configuration of the MonitoringEnsemble(s)
  ## [mandatory] : optional PSets may be omitted
  ##
  setup = cms.PSet(
    ## sub-directory to write the monitor histograms to
    ## [mandatory] : should not be changed w/o explicit 
    ## communication to TopCom!
    directory = cms.string("Physics/Top/TopElectronElectronDQM/"),
    
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
      electronId = cms.InputTag("eidLoose")
    ),
    ## [optional] : when omitted all monitoring plots for jets will
    ## be filled from uncorrected jets
    jetExtras = cms.PSet(
      jetCorrector = cms.string("L2L3JetCorrectorAK5Calo")
    ),
    ## [optional] : when omitted all monitoring plots for triggering
    ## will be empty
    triggerExtras = cms.PSet(
      src    = cms.InputTag("TriggerResults","","HLT"),
      select = cms.vstring(['HLT_Mu9:HLT_Ele15_SW_L1R',
                            'HLT_Mu15:HLT_Ele15_SW_L1R',
                            'HLT_DoubleMu3:HLT_Ele15_SW_L1R',
                            'HLT_Ele15_SW_L1R:HLT_Mu9',
                            'HLT_Ele15_SW_L1R:HLT_DoubleMu3'])    
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
      select = cms.vstring(['HLT_Mu9','HLT_Ele15_SW_L1R','HLT_DoubleMu3'])
    ),
    ## [optional] : when omitted no preselection is applied
    beamspot = cms.PSet(
      src    = cms.InputTag("offlineBeamSpot"),
      select = cms.string('abs(z0)<0.20')
    ),
  ),
  
  ## ------------------------------------------------------    
  ## SELECTION
  ##
  ## monitor histrograms are filled after each selection
  ## step, the selection is applied in the order defined
  ## by this vector
  ## [mandatory] : may be empty or contain an arbitrary
  ## number of PSets as given below:
  ##
  selection = cms.VPSet(
    cms.PSet(
      label  = cms.string("elecs:step0"),    
      src    = cms.InputTag("gsfElectrons"),
      select = cms.string("pt>20 & abs(eta)<2.4"),
      electronId  = cms.InputTag("eidLoose"),
      min    = cms.int32(1)
    ),
    cms.PSet(
      label  = cms.string("elecs:step1"),    
      src    = cms.InputTag("gsfElectrons"),
      select = cms.string("pt>20 & abs(eta)<2.4 & pt/(pt+dr03TkSumPt)>0.9 & pt/(pt+dr04EcalRecHitSumEt+dr04HcalTowerSumEt)>0.8"),
      electronId  = cms.InputTag("eidLoose"),
      min    = cms.int32(1)
    ),
  ),
)
