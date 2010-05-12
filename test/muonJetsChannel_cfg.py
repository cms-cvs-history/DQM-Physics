from DQM.Physics.validationTemplate_cfg import *
from JetMETCorrections.Configuration.JetCorrectionsRecord_cfi import *


## add jet/MET corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
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
process.metJESCorAK5CaloJet.inputUncorMetLabel  = "corMetGlobalMuons"
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
process.leptonJetsChecker.outputFileName         = 'TopMCValidation_semiMu.root'

process.p = cms.Path(
  corMetGlobalMuons+
  process.metJESCorAK5CaloJet+
  process.leptonJetsChecker
)

## ---
##    replace input file here
## ---
#process.source.fileNames = [
#     ]

## ---
##    replace maximal number of events here
## ---
## process.maxEvents.input = -1

## ---
## replace option arguments here
## ---
## process.options.wantSummary = False
