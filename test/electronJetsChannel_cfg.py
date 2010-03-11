from DQM.Physics.validationTemplate_cfg import *

## add jet corrections
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

process.metJESCorAK5CaloJet  = metJESCorIC5CaloJet.clone()
process.metJESCorAK5CaloJet.inputUncorJetsLabel = "antikt5CaloJets"
process.metJESCorAK5CaloJet.inputUncorMetLabel  = "corMetGlobalMuons"
process.metJESCorAK5CaloJet.corrector = "L2L3JetCorrectorAK5Calo"

process.leptonJetsChecker.labelMETs = cms.InputTag('metJESCorAK5CaloJet')

## configure semi-leptonic channel (electron)
process.leptonJetsChecker.PerformOctoberXDeltaRStep   =               True
process.leptonJetsChecker.Luminosity                  =                 20
process.leptonJetsChecker.MuonRelIso                  =               0.05
process.leptonJetsChecker.leptonType                  =         'electron'
process.leptonJetsChecker.useElectronID               =               True
process.leptonJetsChecker.electronIDLabel             =   'eidRobustTight'
process.leptonJetsChecker.JetDeltaRLeptonJetThreshold =                0.3
process.leptonJetsChecker.applyLeptonJetDeltaRCut     =               True
process.leptonJetsChecker.ApplyMETCut                 =              False
process.leptonJetsChecker.useTrigger                  =               True
process.leptonJetsChecker.triggerPath                 = 'HLT_Ele15_LW_L1R' 
process.leptonJetsChecker.PtThrElectrons              =               30.0
process.leptonJetsChecker.vetoEBEETransitionRegion    =               True
process.leptonJetsChecker.VetoLooseLepton             =              False
process.leptonJetsChecker.verbose                     =              False
process.leptonJetsChecker.btaggingMatchDr             =                0.5
process.leptonJetsChecker.includeBtagInfo             =               True
process.leptonJetsChecker.btaggingAlgoLabels          = ["trackCountingHighPurBJetTags",
                                                      "trackCountingHighEffBJetTags",
                                                      "simpleSecondaryVertexBJetTags",
                                                      "softMuonBJetTags"]

## Make sure that the indices are the same in the cuts and the algos
## (the cut goes with the algo!). There is a check on the number of
## entries being the same for both vectors. To use PAIRS of tags (algo
## +cuts in a vector of pairs) would be nicer... but this works for
## now. 
process.leptonJetsChecker.btaggingSelectors = cms.PSet(
    taggers=cms.vstring("simpleSecondaryVertexBJetTags",
                        "simpleSecondaryVertexBJetTags",
                        "simpleSecondaryVertexBJetTags",
                        "softMuonBJetTags"),
    cuts   =cms.vdouble(1.25,
                        2.05,
                        3.00,
			0.00)
)
	
process.p = cms.Path(
  corMetGlobalMuons+
  process.metJESCorAK5CaloJet+
  process.leptonJetsChecker
)

## ---
##    replace input file here
## ---
## process.source.fileNames = []

## ---
##    replace maximal number of events here
## ---
## process.maxEvents.input = -1

## ---
## replace option arguments here
## ---
## process.options.wantSummary = False
