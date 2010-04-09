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
    book();
  }

  void 
  MonitorEnsemble::book()
  {
    //set up the current directory path
    std::string current("Physics/Top/TopDiLeptonOfflineDQM/"); current+=label_;
    store_=edm::Service<DQMStore>().operator->();
    store_->setCurrentFolder(current);

    // --- [STANDARD] --- //
    // pt of the leading lepton
    hists_["leptPt_"      ] = store_->book1D("LeptPt"     , "pt(Lepton)"            , 150,   0., 150.);
    // pt of the 2. leading lepton
    hists_["lept2Pt_"     ] = store_->book1D("Lept2Pt"    , "pt(Lepton2)"           , 150,   0., 150.);
    // lepton multiplicity before std isolation
    hists_["leptMult_"    ] = store_->book1D("LeptMult"   , "N_{all}(Lepton)"       ,  10,   0.,  10.);   
    // lepton multiplicity after std isolation
    hists_["leptMultIso_" ] = store_->book1D("LeptMultIso", "N_{iso}(Lepton)"       ,  10,   0.,  10.);
    // eta of the leading lepton
    hists_["leptEta_"     ] = store_->book1D("LeptEta"    , "eta(Lepton)"           ,  30,  -5.,   5.); 
    // eta of the 2. leading lepton
    hists_["lept2Eta_"    ] = store_->book1D("Lept2Eta"   , "eta(Lepton2)"          ,  30,  -5.,   5.);
     // phi of the leading lepton
    hists_["leptPhi_"     ] = store_->book1D("LeptPhi"    , "phi(Lepton)"           ,  32, -3.2,  3.2); 
    // phi of the 2. leading lepton
    hists_["lept2Phi_"    ] = store_->book1D("Lept2Phi"   , "phi(Lepton2)"          ,  32, -3.2,  3.2);
    // multiplicity of jets with pt>30 (corrected to L2+L3)
    hists_["jetMult_"     ] = store_->book1D("JetMult"    , "N_{30}(Jet)"           ,  10,   0.,  10.); 
    // pt of the 1. leading jet (corrected to L2+L3)
    hists_["jet1Pt_"      ] = store_->book1D("Jet1Pt"     , "pt(Jet1)"              , 100,   0., 200.);   
    // pt of the 2. leading jet (corrected to L2+L3)
    hists_["jet2Pt_"      ] = store_->book1D("Jet2Pt"     , "pt(Jet2)"              , 100,   0., 200.);
    // pt of the 1. leading jet (not corrected)
    hists_["jet1PtRaw_"   ] = store_->book1D("Jet1PtRaw"  , "pt(Jet1,raw)"          , 100,   0., 200.);   
    // pt of the 2. leading jet (not corrected)     
    hists_["jet2PtRaw_"   ] = store_->book1D("Jet2PtRaw"  , "pt(Jet2,raw)"          , 100,   0., 200.);
    // eta of the leading jet
    hists_["jetEta_"      ] = store_->book1D("JetEta"     , "eta(Jet)"              ,  30,  -5.,   5.); 
    // eta of the 2. leading jet
    hists_["jet2Eta_"     ] = store_->book1D("Jet2Eta"    , "eta(Jet2)"             ,  30,  -5.,   5.);
     // phi of the leading jet
    hists_["jetPhi_"      ] = store_->book1D("JetPhi"     , "phi(Jet)"              ,  32, -3.2,  3.2); 
    // phi of the 2. leading jet
    hists_["jet2Phi_"     ] = store_->book1D("Jet2Phi"    , "phi(Jet2)"             ,  32, -3.2,  3.2);
    // MET (calo)
    hists_["metCalo_"     ] = store_->book1D("METCalo"    , "MET(Calo)"             , 100,   0., 200.);
    // MET (PF)
    hists_["metPF_"       ] = store_->book1D("METPF"      , "MET(PF)"               , 100,   0., 200.);
    // MET (TC)
    hists_["metTC_"       ] = store_->book1D("METTC"      , "MET(TC)"               , 100,   0., 200.);
//     // invariant mass of lepton pair
//     hists_["invMLept_"    ] = store_->book1D("InvMLept"   , "invM(ll)"              , 100,   0., 200.);            FIXME
    // fired triggers
//     hists_["triggers_"    ] = store_->book1D("Triggers"   , "fired Triggers"        ,   3,    0,    2);            FIXME
    if( verbosity_==STANDARD) return;

     // --- [VERBOSE] --- //
    // pt of the leading electron
    hists_["elec1Pt_"     ] = store_->book1D("Elec1Pt"    , "pt(Electron1)"         , 150,   0., 150.);
    // pt of the 2. leading electron
    hists_["elec2Pt_"     ] = store_->book1D("Elec2Pt"    , "pt(Electron2)"         , 150,   0., 150.);
    // pt of the leading muon
    hists_["muon1Pt_"     ] = store_->book1D("Muon1Pt"    , "pt(Muon1)"             , 150,   0., 150.);
    // pt of the 2. leading muon
    hists_["muon2Pt_"     ] = store_->book1D("Muon2Pt"    , "pt(Muon2)"             , 150,   0., 150.);
    // electron multiplicity before std isolation
    hists_["elecMult_"    ] = store_->book1D("ElecMult"   , "N_{all}(Electron)"     ,  10,   0.,  10.);   
    // electron multiplicity after std isolation
    hists_["elecMultIso_" ] = store_->book1D("ElecMultIso", "N_{iso}(Electron)"     ,  10,   0.,  10.);
    // muon multiplicity before std isolation
    hists_["muonMult_"    ] = store_->book1D("MuonMult"   , "N_{all}(Muon)"         ,  10,   0.,  10.);   
    // muon multiplicity after std isolation
    hists_["muonMultIso_" ] = store_->book1D("MuonMultIso", "N_{iso}(Muon)"         ,  10,   0.,  10.);
    // eta of the leading electron
    hists_["elec1Eta_"    ] = store_->book1D("Elec1Eta"   , "eta(Electron1)"        ,  30,  -5.,   5.); 
    // eta of the 2. leading electron
    hists_["elec2Eta_"    ] = store_->book1D("Elec2Eta"   , "eta(Electron2)"        ,  30,  -5.,   5.);
    // eta of the leading muon
    hists_["muon1Eta_"    ] = store_->book1D("Muon1Eta"   , "eta(Muon1)"            ,  30,  -5.,   5.); 
    // eta of the 2. leading muon
    hists_["muon2Eta_"    ] = store_->book1D("Muon2Eta"   , "eta(Muon2)"            ,  30,  -5.,   5.);
    // calo isolation variable of the leading muon
    hists_["muon1CalIso_" ] = store_->book1D("Muon1CalIso", "Iso_Cal(Muon1)"        ,  30,   0.,   1.5);
    // calo isolation variable of the 2. leading muon
    hists_["muon2CalIso_" ] = store_->book1D("Muon2CalIso", "Iso_Cal(Muon2)"        ,  30,   0.,   1.5);
    // tracker isolation variable of the leading muon
    hists_["muon1TrkIso_" ] = store_->book1D("Muon1TrkIso", "Iso_Trk(Muon1)"        ,  30,   0.,   1.5);
    // tracker isolation variable of the 2. leading muon
    hists_["muon2TrkIso_" ] = store_->book1D("Muon2TrkIso", "Iso_Trk(Muon2)"        ,  30,   0.,   1.5);
    // calo isolation variable of the leading electron
    hists_["elec1CalIso_" ] = store_->book1D("Elec1CalIso", "Iso_Cal(Elec1)"        ,  30,   0.,   1.5);
    // calo isolation variable of the 2. leading electron
    hists_["elec2CalIso_" ] = store_->book1D("Elec2CalIso", "Iso_Cal(Elec2)"        ,  30,   0.,   1.5);
    // tracker isolation variable of the leading electron
    hists_["elec1TrkIso_" ] = store_->book1D("Elec1TrkIso", "Iso_Trk(Elec1)"        ,  30,   0.,   1.5);
    // tracker isolation variable of the 2. leading electron
    hists_["elec2TrkIso_" ] = store_->book1D("Elec2TrkIso", "Iso_Trk(Elec2)"        ,  30,   0.,   1.5);
    if( verbosity_==VERBOSE) return;

    // --- [DEBUG] --- //
//     // eta of the leading jet, PF
//     hists_["jetEtaPF_"    ] = store_->book1D("JetEtaPF"   , "eta(Jet,PF)"           ,  30,  -5.,   5.);             FIXME, particle flow jet collection not implemented yet
//     // eta of the 2. leading jet, PF
//     hists_["jet2EtaPF_"   ] = store_->book1D("Jet2EtaPF"  , "eta(Jet2,PF)"          ,  30,  -5.,   5.);
//      // phi of the leading jet, PF
//     hists_["jetPhiPF_"    ] = store_->book1D("JetPhiPF"   , "phi(Jet,PF)"           ,  32, -3.2,  3.2); 
//     // phi of the 2. leading jet, PF
//     hists_["jet2PhiPF_"   ] = store_->book1D("Jet2PhiPF"   , "phi(Jet2,PF)"          ,  32, -3.2,  3.2);
//     // multiplicity of jets with pt>30 (corrected to L2+L3, PF)
//     hists_["jetMultPF_"   ] = store_->book1D("JetMultPF"   , "N_{30}(Jet,PF)"        ,  10,   0.,  10.); 
//     // sum of charges of 2 leading leptons
//     hists_["chargeSum_"   ] = store_->book1D("ChargeSum"   , "sum of lept.charges"   ,   5,  -5.,   5.);            FIXME
    // flavour of 2 leading leptons
    hists_["evtFlavour_"  ] = store_->book1D("EvtFlavour"  , "flavour of leptons"   ,   3,    0,    3);
    // deltaR of 2 leading jets
    hists_["dRJet1Jet2_"  ] = store_->book1D("DRJet1Jet2"  , "deltaR(Jet1,Jet2)"    ,  30,   0.,   5.);
    // deltaR of 2 leading leptons
    hists_["dRLep1Lep2_"  ] = store_->book1D("DRLep1Lep2"  , "deltaR(Lept1,Lept2)"  ,  30,   0.,   5.);
    // deltaR of 1. lepton and 1. jet
    hists_["dRJet1Lep1_"  ] = store_->book1D("DRJet1Lep1"  , "deltaR(Jet1,Lept1)"   ,  32,   0.,  3.2);
    // deltaR of 1. lepton and MET
    hists_["dRLep1MET_"   ] = store_->book1D("DRLep1MET"   , "deltaR(Lept1,MET)"    ,  32,   0.,  3.2);
    // deltaR of 1. jet and MET
    hists_["dRJet1MET_"   ] = store_->book1D("DRJet1MET"   , "deltaR(Jet1,MET)"     ,  32,   0.,  3.2);
    // deltaPhi of 2 leading jets
    hists_["dPhiJet1Jet2_"] = store_->book1D("DPhiJet1Jet2", "deltaPhi(Jet1,Jet2)"  ,  32,   0.,  3.2);
    // deltaPhi of 2 leading leptons
    hists_["dPhiLep1Lep2_"] = store_->book1D("DPhiLep1Lep2", "deltaPhi(Lept1,Lept2)",  32,   0.,  3.2);
    // deltaPhi of 1. lepton and 1. jet
    hists_["dPhiJet1Lep1_"] = store_->book1D("DPhiJet1Lep1", "deltaPhi(Jet1,Lept1)" ,  32,   0.,  3.2);
    // deltaPhi of 1. lepton and MET
    hists_["dPhiLep1MET_" ] = store_->book1D("DPhiLep1MET" , "deltaPhi(Lept1,MET)"  ,  32,   0.,  3.2);
    // deltaPhi of 1. jet and MET
    hists_["dPhiJet1MET_" ] = store_->book1D("DPhiJet1MET" , "deltaPhi(Jett1,MET)"  ,  32,   0.,  3.2);
    return;
  }

  void 
  MonitorEnsemble::fill(const edm::Event& event, const edm::EventSetup& setup) const
  {
    // lepton pt to find lepton pair to use
    double ptMuon1=0.,ptMuon2=0.,ptElec1=0.,ptElec2=0.;
    // define variables to hold angular information of leptons and jets
    double phiJet1=0.,etaJet1=0.,phiJet2=0.,etaJet2=0.,phiElec1=0.,etaElec1=0.,phiElec2=0.,etaElec2=0.,phiMuon1=0.,etaMuon1=0.,phiMuon2=0.,etaMuon2=0.,phiMET=0.,etaMET=0.;
    // electron and muon multiplicities to calculate N(Leptons)
    unsigned int nElec=0,nElecIso=0,nMuon=0,nMuonIso=0;

    // load jet corrector if configured such
    const JetCorrector* corrector=0;
    if(!jetCorrector_.empty()){
       corrector = JetCorrector::getJetCorrector(jetCorrector_, setup);
    }
    // fill monitoring plots for jets
    edm::Handle<edm::View<reco::Jet> > jets;
    event.getByLabel(jets_, jets); fill(*jets, event, corrector, phiJet1, etaJet1, phiJet2, etaJet2);
    // fill monitoring plots for electrons
    edm::Handle<edm::View<reco::GsfElectron> > elecs;
    event.getByLabel(elecs_, elecs); fill(*elecs, event, phiElec1, etaElec1, phiElec2, etaElec2, ptElec1, ptElec2, nElec, nElecIso);
    // fill monitoring plots for muons
    edm::Handle<edm::View<reco::Muon> > muons;
    event.getByLabel(muons_, muons); fill(*muons, phiMuon1, etaMuon1, phiMuon2, etaMuon2, ptMuon1, ptMuon2, nMuon, nMuonIso);

    // fill monitoring histograms for met
    for(std::vector<edm::InputTag>::const_iterator met_=mets_.begin(); met_!=mets_.end(); ++met_){
      edm::Handle<edm::View<reco::MET> > met;
      event.getByLabel(*met_, met);
      if(met->begin()!=met->end()){
	unsigned int idx=met_-mets_.begin();
	if(idx==0) {fill("metCalo_" , met->begin()->et()); phiMET = met->begin()->phi(); etaMET = met->begin()->eta();}
	// store angular info only for Calo MET
	if(idx==1) fill("metTC_"   , met->begin()->et());
	if(idx==2) fill("metPflow_", met->begin()->et());
      }
    }
    // fill monitoring plots using multiple collections
    fill(phiJet1,etaJet1,phiJet2,etaJet2,phiElec1,etaElec1,phiElec2,etaElec2,phiMuon1,etaMuon1,phiMuon2,etaMuon2,phiMET,etaMET,ptElec1,ptElec2,ptMuon1,ptMuon2,nElec,nElecIso,nMuon,nMuonIso);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Jet>& jets, const edm::Event& event, const JetCorrector* corrector, double& phiJet1, double& etaJet1, double& phiJet2, double& etaJet2) const 
  {
    // loop jet collection
    unsigned int mult=0, mult30=0;
    for(edm::View<reco::Jet>::const_iterator jet=jets.begin(); jet!=jets.end(); ++jet, ++mult){
      // determine raw jet pt
      double ptRaw  = jet->pt();
      // determine corrected jet pt
      double ptL2L3 = jet->pt();
      if(corrector){
	ptL2L3 *= corrector->correction(*jet);
      }
      if(ptL2L3>30) {++mult30;} // determine jet multiplicity
      // fill pt (raw or L2L3) and store eta and phi for the leading two jets
      if(mult==0) {
	fill("jet1Pt_"   , ptL2L3);
	fill("jet1PtRaw_", ptRaw );
	phiJet1 = jet->phi();
	etaJet1 = jet->eta();
	fill("jetEta_"   , phiJet1);
	fill("jetPhi_"   , etaJet1);
      }
      if(mult==1) {
	fill("jet2Pt_"   , ptL2L3);
	fill("jet2PtRaw_", ptRaw );
      	phiJet2 = jet->phi();
	etaJet2 = jet->eta();
	fill("jet2Eta_"   , phiJet2);
	fill("jet2Phi_"   , etaJet2);
      }
    }
    fill("jetMult_"    , mult30    );
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event, double& phiElec1, double& etaElec1, double& phiElec2, double& etaElec2, double& ptElec1, double& ptElec2, unsigned int& nElec, unsigned int& nElecIso) const
  {
    // check availability of electron id
    edm::Handle<edm::ValueMap<float> > electronId; 
    if(!electronId_.label().empty()) event.getByLabel(electronId_, electronId);

    // loop electron collection
    for(edm::View<reco::GsfElectron>::const_iterator elec=elecs.begin(); elec!=elecs.end(); ++elec, ++nElec){
      if( electronId_.label().empty() ? true : (*electronId)[elecs.refAt(nElec)]>0.99 ){
	// restrict to electrons with good electronId
	double relCaloIso = elec->pt()/(elec->pt()+elec->dr04EcalRecHitSumEt()+elec->dr04HcalTowerSumEt());
	double relTrkIso = elec->pt()/(elec->pt()+elec->dr03TkSumPt());
	if( nElec==0 ){
	  fill("elec1Pt_" , elec->pt() );
	  fill("elec1Eta_", elec->eta());
	  fill("elec1CalIso_" , relCaloIso);
	  fill("elec1TrkIso_" , relTrkIso);
	  phiElec1 = elec->phi();
	  etaElec1 = elec->eta();
	  ptElec1  = elec->pt();
	}
	if( nElec==1 ){
	  fill("elec2Pt_" , elec->pt() );
	  fill("elec2Eta_", elec->eta());
	  fill("elec2CalIso_" , relCaloIso);
	  fill("elec2TrkIso_" , relTrkIso);
	  phiElec2 = elec->phi();
	  etaElec2 = elec->eta();
	  ptElec2  = elec->pt();
	}
	if( relCaloIso>0.8 && relTrkIso>0.9 ){
	  ++nElecIso;
	}
      }
    }
    fill("elecMult_",    nElec   );
    fill("elecMultIso_", nElecIso);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Muon>& muons, double& phiMuon1, double& etaMuon1, double& phiMuon2, double& etaMuon2, double& ptMuon1, double& ptMuon2, unsigned int& nMuon, unsigned int& nMuonIso) const 
  {
    // loop muon collection
    for(edm::View<reco::Muon>::const_iterator muon=muons.begin(); muon!=muons.end(); ++muon, ++nMuon){
      if( muon->combinedMuon().isNull()==0 ){ 
	// restrict to globalMuons
	double relCaloIso = muon->pt()/(muon->pt()+muon->isolationR03().emEt+muon->isolationR03().hadEt);
	double relTrkIso  = muon->pt()/(muon->pt()+muon->isolationR03().sumPt);
	if( nMuon==0 ){
	  fill("muon1Pt_"      , muon->pt() );
	  fill("muon1Eta_"     , muon->eta());
	  fill("muon1CalIso_" , relCaloIso);
	  fill("muon1TrkIso_" , relTrkIso);
	  phiMuon1 = muon->phi();
	  etaMuon1 = muon->eta();
	  ptMuon1  = muon->pt();
	}
	if( nMuon==1 ){
	  fill("muon2Pt_"      , muon->pt() );
	  fill("muon2Eta_"     , muon->eta());
	  fill("muon2CalIso_" , relCaloIso);
	  fill("muon2TrkIso_" , relTrkIso);
	  phiMuon2 = muon->phi();
	  etaMuon2 = muon->eta();
	  ptMuon2  = muon->pt();
	}
	if( relCaloIso>0.9 && relTrkIso>0.9 ){
	  ++nMuonIso;
	}
      }
    }
    fill("muonMult_",    nMuon   );
    fill("muonMultIso_", nMuonIso);
  }
  
  void
  MonitorEnsemble::fill( double& phiJet1, double& etaJet1, double& phiJet2, double& etaJet2, double& phiElec1, double& etaElec1, double& phiElec2, double& etaElec2, double& phiMuon1, double& etaMuon1, double& phiMuon2, double& etaMuon2, double& phiMET, double& etaMET, double& ptElec1, double& ptElec2, double& ptMuon1, double& ptMuon2, unsigned int& nElec, unsigned int& nElecIso, unsigned int& nMuon, unsigned int& nMuonIso ) const
  {
    fill("leptMult_"    , nMuon+nElec       );
    fill("leptMultIso_" , nMuonIso+nElecIso );

    // fill angles of jet and MET
    if( phiJet1 && phiJet2 && etaJet1 && etaJet2 && phiMET && etaMET){
      fill("dPhiJet1Jet2_"   , deltaPhi(phiJet1,phiJet2)               );
      fill("dRJet1Jet2_"     , deltaR(etaJet1,phiJet1,etaJet2,phiJet2) );
      fill("dPhiJet1MET_"     , deltaPhi(phiJet1,phiMET)                );
      fill("dRJet1MET_"       , deltaR(etaJet1,phiJet1,etaMET,phiMET)   );
    }

    // decide which lepton pair to use
    if( ptMuon1 && ptMuon2 && ptMuon2 > ptElec1 ){  // muon-muon event
      fill("dRLep1Lep2_"     , deltaR(etaMuon1,phiMuon1,etaMuon2,phiMuon2) );
      fill("dRJet1Lep1_"     , deltaR(etaJet1,phiJet1,etaMuon1,phiMuon1)   );
      fill("dRLep1MET_"      , deltaR(etaMuon1,phiMuon1,etaMET,phiMET)     );
      fill("dPhiLep1Lep2_"   , deltaPhi(phiMuon1,phiMuon2)                 );
      fill("dPhiJet1Lep1_"   , deltaPhi(phiJet1,phiMuon1)                  );
      fill("dPhiLep1MET_"    , deltaPhi(phiMuon1,phiMET)                   );
      fill("evtFlavour_"     , 0                                           ); // evtFlavour = 0 is muon-muon event
      fill("leptPt_"         , ptMuon1                                     );
      fill("lept2Pt_"        , ptMuon2                                     );
      fill("leptEta_"        , etaMuon1                                    );
      fill("lept2Eta_"       , etaMuon2                                    );
      fill("leptPhi_"        , phiMuon1                                    );
      fill("lept2Phi_"       , phiMuon2                                    );
    }
    else if( ptElec1 && ptElec2 && ptElec2 > ptMuon1 ){  // electron-electron event
      fill("dRLep1Lep2_"     , deltaR(etaElec1,phiElec1,etaElec2,phiElec2) );
      fill("dRJet1Lep1_"     , deltaR(etaJet1,phiJet1,etaElec1,phiElec1)   );
      fill("dRLep1MET_"      , deltaR(etaElec1,phiElec1,etaMET,phiMET)     );
      fill("dPhiLep1Lep2_"   , deltaPhi(phiElec1,phiElec2)                 );
      fill("dPhiJet1Lep1_"   , deltaPhi(phiJet1,phiElec1)                  );
      fill("dPhiLep1MET_"    , deltaPhi(phiElec1,phiMET)                   );
      fill("evtFlavour_"     , 1                                           ); // evtFlavour = 1 is electron-electron event
      fill("leptPt_"         , ptElec1                                     );
      fill("lept2Pt_"        , ptElec2                                     );
      fill("leptEta_"        , etaElec1                                    );
      fill("lept2Eta_"       , etaElec2                                    );
      fill("leptPhi_"        , phiElec1                                    );
      fill("lept2Phi_"       , phiElec2                                    );
    }
    else if( ptElec1 && ptMuon1 ){  // electron-muon event
      fill("evtFlavour_"     , 2                                           ); // evtFlavour = 2 is electron-muon event
      fill("dRLep1Lep2_"     , deltaR(etaElec1,phiElec1,etaMuon1,phiMuon1) );
      fill("dPhiLep1Lep2_"   , deltaPhi(phiElec1,phiMuon1)                 );
      if ( ptElec1 > ptMuon1 ){
	fill("dRJet1Lep1_"     , deltaR(etaJet1,phiJet1,etaElec1,phiElec1)   );
	fill("dRLep1MET_"      , deltaR(etaElec1,phiElec1,etaMET,phiMET)     );
	fill("dPhiJet1Lep1_"   , deltaPhi(phiJet1,phiElec1)                  );
	fill("dPhiLep1MET_"    , deltaPhi(phiElec1,phiMET)                   );
	fill("leptPt_"         , ptElec1                                     );
	fill("leptEta_"        , etaElec1                                    );
	fill("leptPhi_"        , phiElec1                                    );
	fill("lept2Pt_"        , ptMuon1                                     );
	fill("lept2Eta_"       , etaMuon1                                    );
	fill("lept2Phi_"       , phiMuon1                                    );
      }
      if ( ptElec1 < ptMuon1 ){
	fill("dRJet1Lep1_"     , deltaR(etaJet1,phiJet1,etaMuon1,phiMuon1)   );
	fill("dRLep1MET_"      , deltaR(etaMuon1,phiMuon1,etaMET,phiMET)     );
	fill("dPhiJet1Lep1_"   , deltaPhi(phiJet1,phiMuon1)                  );
	fill("dPhiLep1MET_"    , deltaPhi(phiMuon1,phiMET)                   );
	fill("leptPt_"         , ptMuon1                                     );
	fill("leptEta_"        , etaMuon1                                    );
	fill("leptPhi_"        , phiMuon1                                    );
	fill("lept2Pt_"        , ptElec1                                     );
	fill("lept2Eta_"       , etaElec1                                    );
	fill("lept2Phi_"       , phiElec1                                    );
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
