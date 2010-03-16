from DQM.Physics.validationTemplate_cfg import *

## add jet/MET corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo")

## define process
process.load("DQM.Physics.leptonJetsChecker_cfi")

# MET correction for Muons
from JetMETCorrections.Type1MET.MuonMETValueMapProducer_cff import *
from JetMETCorrections.Type1MET.MetMuonCorrections_cff import *
from JetMETCorrections.Type1MET.MetType1Corrections_cff import *

process.load("JetMETCorrections.Type1MET.MetType1Corrections_cff")
process.load("JetMETCorrections.Type1MET.MetMuonCorrections_cff")

#process.metJESCorAK5CaloJet  = metJESCorIC5CaloJet.clone()
#process.metJESCorAK5CaloJet.inputUncorJetsLabel = "ak5CaloJets"
#process.metJESCorAK5CaloJet.inputUncorMetLabel  = "corMetGlobalMuons"
#process.metJESCorAK5CaloJet.corrector = "L2L3JetCorrectorAK5Calo"

#process.leptonJetsChecker.labelMETs = cms.InputTag('metJESCorAK5CaloJet')


## configure semi-leptonic channel (muon)
process.leptonJetsChecker.Luminosity             =    20   ## in pb-1
process.leptonJetsChecker.MuonRelIso             =  0.05   ## relative isolation for muon
process.leptonJetsChecker.ApplyMETCut            = False   ##
process.leptonJetsChecker.VetoLooseLepton        =  True   ## 
process.leptonJetsChecker.PtThrMuonLoose         =  10.0   ## pt threshold for second muon veto
process.leptonJetsChecker.EtaThrMuonLoose        =   2.5   ## eta range for second muon veto 
process.leptonJetsChecker.RelIsoThrMuonLoose     =   0.2   ##
process.leptonJetsChecker.PtThrElectronLoose     =  15.0   ##
process.leptonJetsChecker.RelIsoThrElectronLoose =   0.2   ##

process.p = cms.Path(
  #corMetGlobalMuons+
  process.metJESCorAK5CaloJet+
  process.leptonJetsChecker+
  process.dqmEnv+
  process.dqmSaver
)

## configure DQM output
#process.DQM.collectorHost     = 'cmslpc05.fnal.gov'
#process.DQM.collectorPort     = 9190
process.DQMStore.verbose       = 0
process.dqmEnv.subSystemFolder = 'TopMCValidation'
process.dqmSaver.dirName       = '.'
process.dqmSaver.producer      = 'Playback'
process.dqmSaver.convention    = 'Online'
process.dqmSaver.saveByRun     = 1
process.dqmSaver.saveAtJobEnd  = True

## ---
##    replace input file here
## ---
#process.source.fileNames = [
#     '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V3-v1/0024/169B8EB5-B088-DE11-8BAD-001A4B0A28B6.root'
#     ]

## ---
##    replace maximal number of events here
## ---
## process.maxEvents.input = -1

## ---
## replace option arguments here
## ---
## process.options.wantSummary = False
