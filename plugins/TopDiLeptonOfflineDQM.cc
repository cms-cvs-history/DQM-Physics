#include <algorithm>
#include "DQM/Physics/interface/TopDQMHelpers.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DQM/Physics/plugins/TopDiLeptonOfflineDQM.h"

namespace TopDiLeptonOffline {

  MonitorEnsemble::MonitorEnsemble(const char* label, const edm::ParameterSet& cfg) : label_(label)
  {
    // sources have to be given; this PSet is not optional
    edm::ParameterSet sources=cfg.getParameter<edm::ParameterSet>("sources");
    muons_= sources.getParameter<edm::InputTag>("muons");
    elecs_= sources.getParameter<edm::InputTag>("elecs");
    jets_ = sources.getParameter<edm::InputTag>("jets" );
    mets_ = sources.getParameter<std::vector<edm::InputTag> >("mets" );

    // electronExtras are optional; they may be omitted or 
    // empty
    if( cfg.existsAs<edm::ParameterSet>("elecExtras") ){
      edm::ParameterSet elecExtras=cfg.getParameter<edm::ParameterSet>("elecExtras");
      // electronId is optional; in case it's not found the 
      // InputTag will remain empty
      if( cfg.existsAs<edm::InputTag>("electronId") ){
	electronId_= elecExtras.getParameter<edm::InputTag>("electronId");
      }
    }
    // jetExtras are optional; they may be omitted or 
    // empty
    if( cfg.existsAs<edm::ParameterSet>("jetExtras") ){
      edm::ParameterSet jetExtras=cfg.getParameter<edm::ParameterSet>("jetExtras");
      // jetCorrector is optional; in case it's not found 
      // the InputTag will remain empty
      if( jetExtras.existsAs<std::string>("jetCorrector") ){
	jetCorrector_= jetExtras.getParameter<std::string>("jetCorrector");
      }
    }
    // triggerExtras are optional; they may be omitted or 
    // empty
    if( cfg.existsAs<edm::ParameterSet>("triggerExtras") ){
      edm::ParameterSet triggerExtras=cfg.getParameter<edm::ParameterSet>("triggerExtras");
      triggerTable_=triggerExtras.getParameter<edm::InputTag>("src");
      triggerPaths_=triggerExtras.getParameter<std::vector<std::string> >("select");
    }

    // setup the verbosity level for booking histograms;
    // per default the verbosity level will be set to 
    // STANDARD. This will also be the chosen level in
    // the case when the monitoring PSet is not found
    verbosity_=STANDARD;
    if( cfg.existsAs<edm::ParameterSet>("monitoring") ){
      edm::ParameterSet monitoring=cfg.getParameter<edm::ParameterSet>("monitoring");
      if(monitoring.getParameter<std::string>("verbosity") == "DEBUG"   )
	verbosity_= DEBUG;
      if(monitoring.getParameter<std::string>("verbosity") == "VERBOSE" )
	verbosity_= VERBOSE;
      if(monitoring.getParameter<std::string>("verbosity") == "STANDARD")
	verbosity_= STANDARD;
    }
    // and don't forget to do the histogram booking
    book(cfg.getParameter<std::string>("directory"));
  }

  void 
  MonitorEnsemble::book(std::string directory)
  {
    //set up the current directory path
    std::string current(directory); current+=label_;
    store_=edm::Service<DQMStore>().operator->();
    store_->setCurrentFolder(current);

    // determine number of bins for trigger monitoring
    unsigned int n=triggerPaths_.size();

    // --- [STANDARD] --- //
    // invariant mass of opposite charge lepton pair
    hists_["invMass_"     ] = store_->book1D("InvMass"     , "M(Lep, Lep)"            ,  80,   0., 320.);
    // invariant mass of opposite charge lepton pair
    hists_["invMassLog_"  ] = store_->book1D("InvMassLog"  , "log(M(Lep, Lep))"       ,  80,   1.,  2.5);
    // invariant mass of wrong charge lepton pair (in log10 for low mass region)
    hists_["invMassWC_"   ] = store_->book1D("InvMassWC"   , "M_{WC}(Lep, Lep)"       ,  80,   0., 320.);
    // invariant mass of wrong charge lepton pair (in log10 for low mass region)
    hists_["invMassWCLog_"] = store_->book1D("InvMassLogWC", "log(M_{WC}(Lep, Lep))"  ,  80,   1.,  2.5);
    // decay channel [1]: muon/muon, [2]:elec/elec, [3]:elec/muon 
    hists_["decayChannel_"] = store_->book1D("decayChannel", "Decay Channel Estimate" ,   3,    0,    3);
    // fired selection trigger
    hists_["triggerSel_"  ] = store_->book1D("TriggerSel"  , "Sel(Lepton Triggers)"   ,   n,   0.,    n);
    // fired monitor triggers
    hists_["triggerMon_"  ] = store_->book1D("TriggerMon"  , "Mon(Lepton Triggers)"   ,   n,   0.,    n);
    // lepton multiplicity before std isolation
    hists_["leptMult_"    ] = store_->book1D("LeptMult"    , "N_{all}(Lepton)"        ,  10,   0.,  10.);   
    // lepton multiplicity after std isolation
    hists_["leptMultIso_" ] = store_->book1D("LeptMultIso" , "N_{iso}(Lepton)"        ,  10,   0.,  10.);
    // pt of the leading lepton
    hists_["lept1Pt_"     ] = store_->book1D("Lept1Pt"     , "pt(Leading Lepton)"     ,  50,   0., 150.);
    // pt of the 2. leading lepton
    hists_["lept2Pt_"     ] = store_->book1D("Lept2Pt"     , "pt(Subleading Lepton)"  ,  50,   0., 150.);
    // multiplicity of jets with pt>30 (corrected to L2+L3)
    hists_["jetMult_"     ] = store_->book1D("JetMult"     , "N_{30}(Jet)"            ,  10,   0.,  10.); 
    // MET (calo)
    hists_["metCalo_"     ] = store_->book1D("METCalo"     , "MET(Calo)"              ,  50,   0., 200.);

    // set bin labels for decayChannel_
    hists_["decayChannel_"]->setBinLabel( 1, "#mu e"  , 1);
    hists_["decayChannel_"]->setBinLabel( 2, "#mu #mu", 1);
    hists_["decayChannel_"]->setBinLabel( 3, "e e"    , 1);

    // set bin labels for trigger monitoring
    for(unsigned int iTrigger=0; iTrigger<triggerPaths_.size(); ++iTrigger){
      hists_["triggerMon_"]->setBinLabel( iTrigger+1, "["+monitorPath(triggerPaths_[iTrigger])+"]", 1);
      hists_["triggerSel_"]->setBinLabel( iTrigger+1, "["+selectionPath(triggerPaths_[iTrigger])+"]|["+selectionPath(triggerPaths_[iTrigger])+"]", 1);
    }
    if( verbosity_==STANDARD) return;

    // --- [VERBOSE] --- //
    // electron multiplicity after std isolation
    hists_["elecMultIso_" ] = store_->book1D("ElecMultIso" , "N_{iso}(Electron)"      ,  10,   0.,  10.);
    // muon multiplicity after std isolation
    hists_["muonMultIso_" ] = store_->book1D("MuonMultIso" , "N_{iso}(Muon)"          ,  10,   0.,  10.);
    // eta of the leading lepton
    hists_["lept1Eta_"    ] = store_->book1D("Lept1Eta"    , "eta(Lepton)"            ,  30,  -5.,   5.); 
    // eta of the subleading lepton
    hists_["lept2Eta_"    ] = store_->book1D("Lept2Eta"    , "eta(Lepton2)"           ,  30,  -5.,   5.);
    // pt of the 2. leading electron
    hists_["elec1Pt_"     ] = store_->book1D("Elec1Pt"     , "pt(Electron1)"          ,  50,   0., 150.);
    // pt of the 2. leading electron
    hists_["elec2Pt_"     ] = store_->book1D("Elec2Pt"     , "pt(Electron2)"          ,  50,   0., 150.);
    // pt of the leading muon
    hists_["muon1Pt_"     ] = store_->book1D("Muon1Pt"     , "pt(Muon1)"              ,  50,   0., 150.);
    // pt of the 2. leading muon
    hists_["muon2Pt_"     ] = store_->book1D("Muon2Pt"     , "pt(Muon2)"              ,  50,   0., 150.);
    // pt of the 1. leading jet (corrected to L2+L3)
    hists_["jet1Pt_"      ] = store_->book1D("Jet1Pt"      , "pt(Jet1)"               ,  50,   0., 200.);   
    // pt of the 2. leading jet (corrected to L2+L3)
    hists_["jet2Pt_"      ] = store_->book1D("Jet2Pt"      , "pt(Jet2)"               ,  50,   0., 200.);
    // MET (PF)
    hists_["metPflow_"    ] = store_->book1D("METPflow"    , "MET(Pflow)"             ,  50,   0., 200.);
    // MET (TC)
    hists_["metTC_"       ] = store_->book1D("METTC"       , "MET(TC)"                ,  50,   0., 200.);
    // calo isolation of the leading muon
    hists_["muon1CalIso_" ] = store_->book1D("Muon1CalIso" , "Iso_Cal(Muon1)"         ,  50,   0.,   1.);
    // calo isolation of the subleading muon
    hists_["muon2CalIso_" ] = store_->book1D("Muon2CalIso" , "Iso_Cal(Muon2)"         ,  50,   0.,   1.);
    // track isolation of the leading muon
    hists_["muon1TrkIso_" ] = store_->book1D("Muon1TrkIso" , "Iso_Trk(Muon1)"         ,  50,   0.,   1.);
    // track isolation of the subleading muon
    hists_["muon2TrkIso_" ] = store_->book1D("Muon2TrkIso" , "Iso_Trk(Muon2)"         ,  50,   0.,   1.);
    // calo isolation of the leading electron
    hists_["elec1CalIso_" ] = store_->book1D("Elec1CalIso" , "Iso_Cal(Elec1)"         ,  50,   0.,   1.);
    // calo isolation of the subleading electron
    hists_["elec2CalIso_" ] = store_->book1D("Elec2CalIso" , "Iso_Cal(Elec2)"         ,  50,   0.,   1.);
    // track isolation of the leading electron
    hists_["elec1TrkIso_" ] = store_->book1D("Elec1TrkIso" , "Iso_Trk(Elec1)"         ,  50,   0.,   1.);
    // track isolation of the subleading electron
    hists_["elec2TrkIso_" ] = store_->book1D("Elec2TrkIso" , "Iso_Trk(Elec2)"         ,  50,   0.,   1.);
    if( verbosity_==VERBOSE) return;

    // --- [DEBUG] --- //
    // eta of the leading jet
    hists_["jet1Eta_"     ] = store_->book1D("Jet1Eta"     , "eta(Jet)"               ,  30,  -5.,   5.); 
    // eta of the 2. leading jet
    hists_["jet2Eta_"     ] = store_->book1D("Jet2Eta"     , "eta(Jet2)"              ,  30,  -5.,   5.);
    // pt of the 1. leading jet (not corrected)
    hists_["jet1PtRaw_"   ] = store_->book1D("Jet1PtRaw"   , "pt(Jet1,raw)"           ,  50,   0., 150.);   
    // pt of the 2. leading jet (not corrected)     
    hists_["jet2PtRaw_"   ] = store_->book1D("Jet2PtRaw"   , "pt(Jet2,raw)"           ,  50,   0., 150.);
    // deltaR between the 2 leading jets
    hists_["dRJet1Jet2_"  ] = store_->book1D("DRJet1Jet2"  , "deltaR(Jet1,Jet2)"      ,  30,   0.,   5.);
    // deltaR between the 2 leading leptons
    hists_["dRLep1Lep2_"  ] = store_->book1D("DRLep1Lep2"  , "deltaR(Lept1,Lept2)"    ,  30,   0.,   5.);
    // deltaR between the lepton and the leading jet
    hists_["dRJet1Lep1_"  ] = store_->book1D("DRJet1Lep1"  , "deltaR(Jet1,Lept1)"     ,  30,   0.,   3.);
    // deltaR between the lepton and MET
    hists_["dRLep1MET_"   ] = store_->book1D("DRLep1MET"   , "deltaR(Lept1,MET)"      ,  30,   0.,   3.);
    // deltaR between leading jet and MET
    hists_["dRJet1MET_"   ] = store_->book1D("DRJet1MET"   , "deltaR(Jet1,MET)"       ,  30,   0.,   3.);
    // deltaPhi of 2 leading jets
    hists_["dPhiJet1Jet2_"] = store_->book1D("DPhiJet1Jet2", "deltaPhi(Jet1,Jet2)"    ,  32,   0.,  3.2);
    // deltaPhi of 2 leading leptons
    hists_["dPhiLep1Lep2_"] = store_->book1D("DPhiLep1Lep2", "deltaPhi(Lept1,Lept2)"  ,  32,   0.,  3.2);
    // deltaPhi of 1. lepton and 1. jet
    hists_["dPhiJet1Lep1_"] = store_->book1D("DPhiJet1Lep1", "deltaPhi(Jet1,Lept1)"   ,  32,   0.,  3.2);
    // deltaPhi of 1. lepton and MET
    hists_["dPhiLep1MET_" ] = store_->book1D("DPhiLep1MET" , "deltaPhi(Lept1,MET)"    ,  32,   0.,  3.2);
    // deltaPhi of 1. jet and MET
    hists_["dPhiJet1MET_" ] = store_->book1D("DPhiJet1MET" , "deltaPhi(Jett1,MET)"    ,  32,   0.,  3.2);
    return;
  }

  void 
  MonitorEnsemble::fill(const edm::Event& event, const edm::EventSetup& setup) const
  {
    // buffer for MET
    LorentzVector metBuffer;
    // buffer for information on lepton isolation
    std::vector<bool> leptonIsolation; 
    // buffer for the two leading jets/electrons/muons
    std::vector<LorentzVector> leadingJets, leadingElecs, leadingMuons;

    // load jet corrector if configured such
    const JetCorrector* corrector=0;
    if(!jetCorrector_.empty()){
       corrector = JetCorrector::getJetCorrector(jetCorrector_, setup);
    }
    // fill monitoring plots for jets
    edm::Handle<edm::View<reco::Jet> > jets;
    event.getByLabel(jets_, jets); fill(*jets, event, corrector, leadingJets);
    // fill monitoring plots for muons
    edm::Handle<edm::View<reco::Muon> > muons;
    event.getByLabel(muons_, muons); fill(*muons, leadingMuons, leptonIsolation);
    // fill monitoring plots for electrons
    edm::Handle<edm::View<reco::GsfElectron> > elecs;
    event.getByLabel(elecs_, elecs); fill(*elecs, event, leadingElecs, leptonIsolation);

    // fill monitoring histograms for met
    for(std::vector<edm::InputTag>::const_iterator met_=mets_.begin(); met_!=mets_.end(); ++met_){
      edm::Handle<edm::View<reco::MET> > met;
      event.getByLabel(*met_, met);
      if(met->begin()!=met->end()){
	unsigned int idx=met_-mets_.begin();
	// store p4 info in addition for CaloMET
	if(idx==0){ fill("metCalo_" , met->begin()->et());
	  metBuffer = met->begin()->p4(); 
	}
	if(idx==1){ fill("metTC_"   , met->begin()->et());}
	if(idx==2){ fill("metPflow_", met->begin()->et());}
      }
    }

    // lepton multiplicity is the size of leptonIsolation
    fill("leptMult_"    , leptonIsolation.size() );
    // isolated lepton multipicity is the number of counted 
    // *isolated* (i.e. true) occurences in leptonIsolation
    fill("leptMultIso_" , std::count(leptonIsolation.begin(), leptonIsolation.end(), true) );
    // angles between jets and MET
    if( leadingJets.size()>1 ){
      fill("dPhiJet1Jet2_" , reco::deltaPhi(leadingJets[0].phi(), leadingJets[1].phi()));
      fill("dPhiJet1MET_"  , reco::deltaPhi(leadingJets[0].phi(), metBuffer.phi()));
      fill("dRJet1Jet2_"   , reco::deltaR  (leadingJets[0], leadingJets[1]));
      fill("dRJet1MET_"    , reco::deltaR  (leadingJets[0], metBuffer));
    }
    // lepton monitoring for ELECMUON type decays    
    if(decayChannel(leadingMuons, leadingElecs)==ELECMUON){
      // fill plots for leptons depending
      // on which is the leading lepton 
      if( leadingElecs[0].pt()>leadingMuons[0].pt() )
	fill(leadingElecs[0], leadingMuons[0], leadingJets[0], metBuffer);
      else{
	fill(leadingMuons[0], leadingElecs[0], leadingJets[0], metBuffer);
      }
      if(!triggerTable_.label().empty()){
	// fill plots for trigger monitoring
	edm::Handle<edm::TriggerResults> triggerTable;
	event.getByLabel(triggerTable_, triggerTable);
	fill(*triggerTable, leadingElecs[0], leadingMuons[0]);
      }
      fill("decayChannel_", 0.5); 
    }
    // lepton monitoring for DIMUON type decays   
    if(decayChannel(leadingMuons, leadingElecs)==DIMUON){
      fill("decayChannel_", 1.5); 
      fill(leadingMuons, leadingJets[0], metBuffer);
    }
    // lepton monitoring for DIELEC type decays   
    if(decayChannel(leadingMuons, leadingElecs)==DIELEC){
      fill("decayChannel_", 2.5); 
      fill(leadingElecs, leadingJets[0], metBuffer);
    }
  }

  void 
  MonitorEnsemble::fill(const edm::TriggerResults& triggerTable, const LorentzVector& leptonA, const LorentzVector& leptonB) const
  {
    for(unsigned int iTrigger=0; iTrigger<triggerPaths_.size(); ++iTrigger){
      if( accept(triggerTable, monitorPath(triggerPaths_[iTrigger])) ){
	if( (leptonA+leptonB).mass()>60 ){
	  fill("triggerMon_", iTrigger+0.5 );
	  if(accept(triggerTable, selectionPath(triggerPaths_[iTrigger]))){
	    fill("triggerSel_", iTrigger+0.5 );
	  }
	}
      }
    }
  }

  void 
  MonitorEnsemble::fill(const std::vector<LorentzVector>& leptons, const LorentzVector& leadingJet, const LorentzVector& met) const
  {
    fill("dPhiLep1Lep2_"   , reco::deltaPhi(leptons[0].phi(),leptons[1].phi()));
    fill("dPhiJet1Lep1_"   , reco::deltaPhi(leptons[0].phi(),leadingJet.phi()));
    fill("dPhiLep1MET_"    , reco::deltaPhi(leptons[0].phi(),met.phi()));
    fill("dRLep1Lep2_"     , reco::deltaR(leptons[0],leptons[1]));
    fill("dRJet1Lep1_"     , reco::deltaR(leptons[0],leadingJet));
    fill("dRLep1MET_"      , reco::deltaR(leptons[0],met));
    fill("lept1Pt_"        , leptons[0].pt ());
    fill("lept1Eta_"       , leptons[0].eta());
    fill("lept2Pt_"        , leptons[1].pt ());
    fill("lept2Eta_"       , leptons[1].eta());
  }

  void MonitorEnsemble::
  fill(const LorentzVector& leadingLepton, const LorentzVector& subleadingLepton, const LorentzVector& leadingJet, const LorentzVector& met) const
  {
    fill("dPhiLep1Lep2_"   , reco::deltaPhi(leadingLepton.phi(), subleadingLepton.phi()));
    fill("dPhiJet1Lep1_"   , reco::deltaPhi(leadingLepton.phi(), leadingJet.phi()));
    fill("dPhiLep1MET_"    , reco::deltaPhi(leadingLepton.phi(), met.phi()));
    fill("dRLep1Lep2_"     , reco::deltaR(leadingLepton, subleadingLepton));
    fill("dRJet1Lep1_"     , reco::deltaR(leadingLepton, leadingJet));
    fill("dRLep1MET_"      , reco::deltaR(leadingLepton, met));
    fill("lept1Pt_"        , leadingLepton.pt ());
    fill("lept1Eta_"       , leadingLepton.eta());
    fill("lept2Pt_"        , subleadingLepton.pt ());
    fill("lept2Eta_"       , subleadingLepton.eta());
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Jet>& jets, const edm::Event& event, const JetCorrector* corrector, std::vector<LorentzVector>& leadingJetBuffer) const 
  {
    // loop jet collection
    unsigned int mult=0, mult30=0;
    for(edm::View<reco::Jet>::const_iterator jet=jets.begin(); jet!=jets.end(); ++jet, ++mult){
      // determie jet correction scale
      reco::Jet correctedJet=*jet; correctedJet.scaleEnergy(corrector ?  corrector->correction(*jet) : 1.);
      if(correctedJet.pt()>30) {++mult30;} // determine jet multiplicity
      // fill monitor plots for leading and sub-leading jet and buffer for later use
      if(mult==0) {
	// add leading jet to buffer
	leadingJetBuffer.push_back(correctedJet.p4());
	fill("jet1Pt_"    , correctedJet.pt());
	fill("jet1PtRaw_" , jet->pt() );
	fill("jetEta_"    , jet->eta());
      }
      if(mult==1) {
	// add second leading jet to buffer
	leadingJetBuffer.push_back(correctedJet.p4());
	fill("jet2Pt_"    , correctedJet.pt());
	fill("jet2PtRaw_" , jet->pt() );
	fill("jet2Eta_"   , jet->eta());
      }
    }
    fill("jetMult_", mult30);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event, std::vector<LorentzVector>& leadingElecBuffer, std::vector<bool>& leptonIsolationBuffer) const
  {
    // check availability of electron id
    edm::Handle<edm::ValueMap<float> > electronId; 
    if(!electronId_.label().empty()) event.getByLabel(electronId_, electronId);
    // buffer for isolated electrons for 
    // invariant mass plot
    std::vector<reco::GsfElectron> isolatedElectrons;

    // loop electron collection
    unsigned int mult=0, multIso=0;
    for(edm::View<reco::GsfElectron>::const_iterator elec=elecs.begin(); elec!=elecs.end(); ++elec, ++mult){
      if( electronId_.label().empty() ? true : (*electronId)[elecs.refAt(mult)]>0.99 ){
	// restrict to electrons with good electronId
	double isolationCalo  = elec->pt()/(elec->pt()+elec->dr04EcalRecHitSumEt()+elec->dr04HcalTowerSumEt());
	double isolationTrack = elec->pt()/(elec->pt()+elec->dr03TkSumPt());
	// fill monitor plots for leading and sub-leading 
	// electron and buffer for later use
	if( mult==0 ){
	  leadingElecBuffer.push_back(elec->p4() );
	  fill("elec1Pt_"    , elec->pt () );
	  fill("elec1CalIso_", isolationCalo );
	  fill("elec1TrkIso_", isolationTrack);
	}
	if( mult==1 ){
	  leadingElecBuffer.push_back(elec->p4() );
	  fill("elec2Pt_"    , elec->pt () );
	  fill("elec2CalIso_", isolationCalo );
	  fill("elec2TrkIso_", isolationTrack);
	}
	if( isolationCalo>0.8 && isolationTrack>0.9 ){
	  leptonIsolationBuffer.push_back(true);
	  isolatedElectrons.push_back(*elec);
	  ++multIso;
	}
	else{	
	  leptonIsolationBuffer.push_back(false);

	}
      }
    }
    fill("elecMultIso_", multIso);
    // fill invariant mass plots
    if(isolatedElectrons.size()>1){
      if(isolatedElectrons[0].charge()*isolatedElectrons[1].charge()<0){
	fill("invMass_", (isolatedElectrons[0].p4()+isolatedElectrons[1].p4()).mass() );
	fill("invMassLog_", log10( (isolatedElectrons[0].p4()+isolatedElectrons[1].p4()).mass() ));
	
      }
      else{
	fill("invMassWC_", (isolatedElectrons[0].p4()+isolatedElectrons[1].p4()).mass() );
	fill("invMassWCLog_", log10( (isolatedElectrons[0].p4()+isolatedElectrons[1].p4()).mass() ));
      }
    }
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Muon>& muons, std::vector<LorentzVector>& leadingMuonBuffer, std::vector<bool>& leptonIsolationBuffer) const 
  {
    // buffer for isolated electrons for 
    // invariant mass plot
    std::vector<reco::Muon> isolatedMuons;

    // loop muon collection
    unsigned int mult=0, multIso=0;
    for(edm::View<reco::Muon>::const_iterator muon=muons.begin(); muon!=muons.end(); ++muon, ++mult){
      if( muon->combinedMuon().isNull()==0 ){ 
	// restrict to globalMuons
	double isolationCalo  = muon->pt()/(muon->pt()+muon->isolationR03().emEt+muon->isolationR03().hadEt);
	double isolationTrack = muon->pt()/(muon->pt()+muon->isolationR03().sumPt);
	if( mult==0 ){
	  leadingMuonBuffer.push_back(muon->p4() );
	  fill("muon1Pt_"     , muon->pt () );
	  fill("muon1CalIso_" , isolationCalo );
	  fill("muon1TrkIso_" , isolationTrack);
	}
	if( mult==1 ){
	  leadingMuonBuffer.push_back(muon->p4() );
	  fill("muon2Pt_"     , muon->pt () );
	  fill("muon2CalIso_" , isolationCalo );
	  fill("muon2TrkIso_" , isolationTrack);
	}
	if( isolationCalo>0.9 && isolationTrack>0.9 ){
	  leptonIsolationBuffer.push_back(true);
	  isolatedMuons.push_back(*muon);
	  ++multIso;
	}
	else{
	  leptonIsolationBuffer.push_back(false);
	}
      }
    }
    fill("muonMultIso_", multIso);
    // fill invariant mass plots
    if(isolatedMuons.size()>1){
      if(isolatedMuons[0].charge()*isolatedMuons[1].charge()<0){
	fill("invMass_", (isolatedMuons[0].p4()+isolatedMuons[1].p4()).mass() );
	fill("invMassLog_", log10( (isolatedMuons[0].p4()+isolatedMuons[1].p4()).mass() ));
      }
      else{
	fill("invMassWC_", (isolatedMuons[0].p4()+isolatedMuons[1].p4()).mass() );
	fill("invMassWCLog_", log10( (isolatedMuons[0].p4()+isolatedMuons[1].p4()).mass() ));
      }
    }
  }
  
}

TopDiLeptonOfflineDQM::TopDiLeptonOfflineDQM(const edm::ParameterSet& cfg): triggerTable_(""), beamspot_("") 
{
  // configure preselection
  edm::ParameterSet presel=cfg.getParameter<edm::ParameterSet>("preselection");
  if( presel.existsAs<edm::ParameterSet>("trigger") ){
    edm::ParameterSet trigger=presel.getParameter<edm::ParameterSet>("trigger");
    triggerTable_=trigger.getParameter<edm::InputTag>("src");
    triggerPaths_=trigger.getParameter<std::vector<std::string> >("select");
  } 
  if( presel.existsAs<edm::ParameterSet>("beamspot" ) ){
    edm::ParameterSet beamspot=presel.getParameter<edm::ParameterSet>("beamspot");
    beamspot_= beamspot.getParameter<edm::InputTag>("src");
    beamspotSelect_= new StringCutObjectSelector<reco::BeamSpot>(beamspot.getParameter<std::string>("select"));
  }
  if( presel.existsAs<edm::ParameterSet>("muons" ) ){
    preselMuon_=presel.getParameter<edm::ParameterSet>("muons");
  }
  if( presel.existsAs<edm::ParameterSet>("elecs" ) ){
    preselElec_=presel.getParameter<edm::ParameterSet>("elecs");
  }

  // conifgure the selection
  std::vector<edm::ParameterSet> sel=cfg.getParameter<std::vector<edm::ParameterSet> >("selection");
  for(unsigned int i=0; i<sel.size(); ++i){
    selectionOrder_.push_back(sel.at(i).getParameter<std::string>("label"));
    selection_[selectionStep(selectionOrder_.back())] = std::make_pair(sel.at(i), new MonitorEnsemble(selectionStep(selectionOrder_.back()).c_str(), cfg.getParameter<edm::ParameterSet>("setup")));
  }
}

void 
TopDiLeptonOfflineDQM::analyze(const edm::Event& event, const edm::EventSetup& setup)
{ 
  if(!triggerTable_.label().empty()){
    edm::Handle<edm::TriggerResults> triggerTable;
    event.getByLabel(triggerTable_, triggerTable);
    if(!accept(*triggerTable, triggerPaths_)) return;
  }
  if(!beamspot_.label().empty()){
    edm::Handle<reco::BeamSpot> beamspot;
    event.getByLabel(beamspot_, beamspot);
    if(!(*beamspotSelect_)(*beamspot)) return;
  }
  if(!preselMuon_.empty()){
    SelectionStep<reco::Muon> step(preselMuon_);
    if(!step.select(event)) return;
  }
  if(!preselElec_.empty()){
    SelectionStep<reco::GsfElectron> step(preselElec_);
    if(!step.select(event)) return;
  }

  // apply selection steps
  for(std::vector<std::string>::const_iterator selIt=selectionOrder_.begin(); selIt!=selectionOrder_.end(); ++selIt){
    std::string key = selectionStep(*selIt), type = objectType(*selIt);
    if(selection_.find(key)!=selection_.end()){
      if(type=="muons"){
	SelectionStep<reco::Muon> step(selection_[key].first);
	if(step.select(event)){
	  selection_[key].second->fill(event, setup);
	} else break;
      }
      if(type=="elecs"){
	SelectionStep<reco::GsfElectron> step(selection_[key].first);
	if(step.select(event)){ 
	  selection_[key].second->fill(event, setup);
	} else break;
      }
      if(type=="jets" ){
	SelectionStep<reco::Jet> step(selection_[key].first);
	if(step.select(event, setup)){
	  selection_[key].second->fill(event, setup);
	} else break;
      }
      if(type=="jets/pf" ){
	std::cout << "type: pf-jets" << std::endl;
	SelectionStep<reco::PFJet> step(selection_[key].first);
	if(step.select(event, setup)){
	  selection_[key].second->fill(event, setup);
	} else break;
      }
      if(type=="jets/calo" ){
	SelectionStep<reco::CaloJet> step(selection_[key].first);
	if(step.select(event, setup)){
	  selection_[key].second->fill(event, setup);
	} else break;
      }
      if(type=="met" ){
	SelectionStep<reco::MET> step(selection_[key].first);
	if(step.select(event)){
	  selection_[key].second->fill(event, setup);
	} else break;
      }
    }
  }
}

/// define plugin as module	
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TopDiLeptonOfflineDQM);
