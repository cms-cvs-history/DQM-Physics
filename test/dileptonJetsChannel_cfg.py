from DQM.Physics.validationTemplate_cfg import *

## add jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo") 

## define process
process.load("DQM.Physics.leptonJetsChecker_cfi")
process.load("DQM.Physics.dileptonJetsChecker_cfi")

## configure full-leptonic channels
process.leptonJetsChecker.verbose      = False
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

process.metJESCorAK5CaloJet = metJESCorIC5CaloJet.clone()
process.metJESCorAK5CaloJet.inputUncorJetsLabel = "antikt5CaloJets"
process.metJESCorAK5CaloJet.inputUncorMetLabel  = "corMetGlobalMuons"
process.metJESCorAK5CaloJet.corrector = "L2L3JetCorrectorAK5Calo"

process.diLeptonsCheckerDiel.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
process.diLeptonsCheckerDimu.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
process.diLeptonsCheckerElmu.labelMETs = cms.InputTag('metJESCorAK5CaloJet')
	
process.p = cms.Path(
  process.corMetGlobalMuons* 
  process.metJESCorAK5CaloJet*
  process.diLeptonsCheckerDimu*
  process.diLeptonsCheckerDiel*
  process.diLeptonsCheckerElmu
)

## ---
##    replace input file here
## ---
#process.source.fileNames = [
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0007/CE243FB9-A778-DE11-8891-000423D98BC4.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/D63BFAF8-5178-DE11-8439-001D09F24F65.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/B8BA4AF7-5178-DE11-9F72-001D09F23A6B.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/9C8FF416-5278-DE11-B809-0019B9F70607.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/98079214-5278-DE11-8880-001D09F27067.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/62B58021-5278-DE11-9574-0019B9F6C674.root',
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0006/3AEF4E28-5278-DE11-A2D4-000423D6CA72.root'
#     ]

## ---
##    replace maximal number of events here
## ---
process.maxEvents.input = -1

## ---
## replace option arguments here
## ---
## process.options.wantSummary = False
