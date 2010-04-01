#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DQM/Physics/plugins/TopSingleLeptonDQM.h"

namespace TopSingleLepton {

  // maximal number of leading jets 
  // to be used for top mass estimate
  static const unsigned int MAXJETS = 4;
  // nominal mass of the W boson to 
  // be used for the top mass estimate
  static const double WMASS = 80.4;

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
      // jetBDiscriminators are optional; in case they are
      // not found the InputTag will remain empty; they 
      // consist of pairs of edm::JetFlavorAssociation's & 
      // corresponding working points
      includeBTag_=jetExtras.existsAs<edm::ParameterSet>("jetBTaggers");
      if( includeBTag_ ){
	edm::ParameterSet btagEff=jetExtras.getParameter<edm::ParameterSet>("jetBTaggers").getParameter<edm::ParameterSet>("trackCountingEff");
	btagEff_= btagEff.getParameter<edm::InputTag>("label"); btagEffWP_= btagEff.getParameter<double>("workingPoint");
	edm::ParameterSet btagPur=jetExtras.getParameter<edm::ParameterSet>("jetBTaggers").getParameter<edm::ParameterSet>("trackCountingPur");
	btagPur_= btagPur.getParameter<edm::InputTag>("label"); btagPurWP_= btagPur.getParameter<double>("workingPoint");
	edm::ParameterSet btagVtx=jetExtras.getParameter<edm::ParameterSet>("jetBTaggers").getParameter<edm::ParameterSet>("secondaryVertex" );
	btagVtx_= btagVtx.getParameter<edm::InputTag>("label"); btagVtxWP_= btagVtx.getParameter<double>("workingPoint");
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
    std::string current("Physics/Top/TopSingleLeptonDQM/"); current+=label_;
    store_=edm::Service<DQMStore>().operator->();
    store_->setCurrentFolder(current);

    // --- [STANDARD] --- //
    // pt of the leading muon
    hists_["muonPt_"     ] = store_->book1D("MuonPt"     , "pt(Muon)"         , 150,   0., 150.);   
    // muon multiplicity before std isolation
    hists_["muonMult_"   ] = store_->book1D("MuonMult"   , "N_{all}(Muon)"    ,  10,   0.,  10.);   
    // muon multiplicity after  std isolation
    hists_["muonMultIso_"] = store_->book1D("MuonMultIso", "N_{iso}(Muon)"    ,  10,   0.,  10.);   
    // pt of the leading electron
    hists_["elecPt_"     ] = store_->book1D("ElecPt"     , "pt(Elec)"         , 150,   0., 150.);   
    // electron multiplicity before std isolation
    hists_["elecMult_"   ] = store_->book1D("ElecMult"   , "N_{all}(Elec)"    ,  10,   0.,  10.);   
    // electron multiplicity after  std isolation
    hists_["elecMultIso_"] = store_->book1D("ElecMultIso", "N_{iso}(Elec)"    ,  10,   0.,  10.);   
    // multiplicity of jets with pt>20 (corrected to L2+L3)
    hists_["jetMult_"    ] = store_->book1D("JetMult"    , "N_{20}(Jet)"      ,  10,   0.,  10.);   
    // MET (calo)
    hists_["metCalo_"    ] = store_->book1D("METCalo"    , "MET(Calo)"        , 100,   0., 200.);   
    // W mass estimate
    hists_["massW_"      ] = store_->book1D("MassW"      , "M(W)"             , 100,  50., 150.);   
    // Top mass estimate
    hists_["massTop_"    ] = store_->book1D("MassTop"    , "M(Top)"           , 100, 100., 300.);   
    if( verbosity_==STANDARD) return;

    // --- [VERBOSE] --- //
    // eta of the leading muon
    hists_["muonEta_"    ] = store_->book1D("MuonEta"    , "eta(Muon)"        ,  30,  -5.,   5.);   
    // std isolation variable of the leading muon
    hists_["muonIso_"    ] = store_->book1D("MuonIso"    , "Iso(Muon)"        ,  30,   0.,   3.);   
    // eta of the leading electron
    hists_["elecEta_"    ] = store_->book1D("ElecEta"    , "eta(Elec)"        ,  30,  -5.,   5.);   
    // std isolation variable of the leading electron
    hists_["elecIso_"    ] = store_->book1D("ElecIso"    , "Iso(Elec)"        ,  30,   0.,   3.);   
    // multiplicity of btagged jets (for track counting high efficiency) with pt(L2L3)>20
    hists_["jetMultBEff_"] = store_->book1D("JetMultBEff", "N_{20}(BJet,Eff)" ,  10,   0.,  10.);   
    // btag discriminator for track counting high efficiency for jets with pt(L2L3)>20
    hists_["jetBDiscEff_"] = store_->book1D("JetBDiscEff", "Disc_{B,Eff}(Jet)",  50,   0.,  10.);   
    // pt of the 1. leading jet (corrected to L2+L3)
    hists_["jet1Pt_"     ] = store_->book1D("Jet1Pt"     , "pt(Jet1)"         , 100,   0., 200.);   
    // pt of the 2. leading jet (corrected to L2+L3)
    hists_["jet2Pt_"     ] = store_->book1D("Jet2Pt"     , "pt(Jet2)"         , 100,   0., 200.);   
    // pt of the 3. leading jet (corrected to L2+L3)
    hists_["jet3Pt_"     ] = store_->book1D("Jet3Pt"     , "pt(Jet3)"         , 100,   0., 200.);   
    // pt of the 4. leading jet (corrected to L2+L3)
    hists_["jet4Pt_"     ] = store_->book1D("Jet4Pt"     , "pt(Jet4)"         , 100,   0., 200.);   
    // MET (tc)
    hists_["metTC_"      ] = store_->book1D("METTC"      , "MET(TC)"          , 100,   0., 200.);   
    // MET (pflow)
    hists_["metPflow_"   ] = store_->book1D("METPflow"   , "MET(Pflow)"       , 100,   0., 200.);   
    if( verbosity_==VERBOSE) return;

    // --- [DEBUG] --- //
    // absolute muon isolation in tracker for the leading muon
    hists_["muonIsoTrk_" ] = store_->book1D("MuonIsoTrk" , "Iso_{trk}(Muon)"  ,  25,   0.,   5.);   
    // absolute muon isolation in ecal for the leading muon
    hists_["muonIsoEcal_"] = store_->book1D("MuonIsoEcal", "Iso_{ecal}(Muon)" ,  25,   0.,   5.);   
    // absolute muon isolation in hcal for the leading muon
    hists_["muonIsoHcal_"] = store_->book1D("MuonIsoHcal", "Iso_{hcal}(Muon)" ,  25,   0.,   5.);   
    // absolute electron isolation in tracker for the leading electron
    hists_["elecIsoTrk_" ] = store_->book1D("ElecIsoTrk" , "Iso_{trk}(Elec)"  ,  25,   0.,   5.);   
    // absolute electron isolation in ecal for the leading electron
    hists_["elecIsoEcal_"] = store_->book1D("ElecIsoEcal", "Iso_{ecal}(Elec)" ,  25,   0.,   5.);   
    // absolute electron isolation in hcal for the leading electron
    hists_["elecIsoHcal_"] = store_->book1D("ElecIsoHcal", "Iso_{hcal}(Elec)" ,  25,   0.,   5.);   
    // multiplicity of btagged jets (for track counting high purity) with pt(L2L3)>20
    hists_["jetMultBPur_"] = store_->book1D("JetMultBPur", "N_{20}(BJet,Pur)" ,  10,   0.,  10.);   
    // btag discriminator for track counting high purity
    hists_["jetBDiscPur_"] = store_->book1D("JetBDiscPur", "Disc_{B,Pur}(Jet)",  50,   0.,  10.);   
    // multiplicity of btagged jets (for simple secondary vertex) with pt(L2L3)>20
    hists_["jetMultBVtx_"] = store_->book1D("JetMultBVtx", "N_{20}(BJet,Vtx)" ,  10,   0.,  10.);   
    // btag discriminator for simple secondary vertex
    hists_["jetBDiscVtx_"] = store_->book1D("JetBDiscVtx", "Disc_{B,Vtx}(Jet)",  30,   0.,   3.);   
    // pt of the 1. leading jet (uncorrected)
    hists_["jet1PtRaw_"  ] = store_->book1D("Jet1PtRaw"  , "pt(Jet1, raw)"    , 100,   0., 200.);   
    // pt of the 2. leading jet (uncorrected)
    hists_["jet2PtRaw_"  ] = store_->book1D("Jet2PtRaw"  , "pt(Jet2, raw)"    , 100,   0., 200.);   
    // pt of the 3. leading jet (uncorrected)
    hists_["jet3PtRaw_"  ] = store_->book1D("Jet3PtRaw"  , "pt(Jet3, raw)"    , 100,   0., 200.);   
    // pt of the 4. leading jet (uncorrected)
    hists_["jet4PtRaw_"  ] = store_->book1D("Jet4PtRaw"  , "pt(Jet4, raw)"    , 100,   0., 200.);   
    return;
  }

  void 
  MonitorEnsemble::fill(const edm::Event& event, const edm::EventSetup& setup) const
  {
    // load jet corrector if configured such
    const JetCorrector* corrector=0;
    if(!jetCorrector_.empty()){
      corrector = JetCorrector::getJetCorrector(jetCorrector_, setup);
    }
    // fill monitoring plots for jets
    edm::Handle<edm::View<reco::Jet> > jets;
    event.getByLabel(jets_, jets); fill(*jets, event, corrector);
    // fill monitoring plots for electrons
    edm::Handle<edm::View<reco::GsfElectron> > elecs;
    event.getByLabel(elecs_, elecs); fill(*elecs, event);
    // fill monitoring plots for muons
    edm::Handle<edm::View<reco::Muon> > muons;
    event.getByLabel(muons_, muons); fill(*muons);

    // fill monitoring histograms for met
    for(std::vector<edm::InputTag>::const_iterator met_=mets_.begin(); met_!=mets_.end(); ++met_){
      edm::Handle<edm::View<reco::MET> > met;
      event.getByLabel(*met_, met);
      if(met->begin()!=met->end()){
	unsigned int idx=met_-mets_.begin();
	if(idx==0) fill("metCalo_" , met->begin()->et());
	if(idx==1) fill("metTC_"   , met->begin()->et());
	if(idx==2) fill("metPflow_", met->begin()->et());
      }
    }
    // fill W boson and top mass estimates
    Calculate eventKinematics(MAXJETS, WMASS, corrector);
    fill("massW_"   , eventKinematics.massWBoson  (*jets));
    fill("massTop_" , eventKinematics.massTopQuark(*jets));
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Jet>& jets, const edm::Event& event, const JetCorrector* corrector) const 
  {
    // check availability of the btaggers
    edm::Handle<reco::JetTagCollection> btagEff, btagPur, btagVtx;
    if( includeBTag_ ){ 
      event.getByLabel(btagEff_, btagEff);
      event.getByLabel(btagPur_, btagPur);
      event.getByLabel(btagVtx_, btagVtx); 
    }
    // loop jet collection
    unsigned int mult=0, mult20=0, mult20BEff=0, mult20BPur=0, mult20BVtx=0;
    for(edm::View<reco::Jet>::const_iterator jet=jets.begin(); jet!=jets.end(); ++jet, ++mult){
      // determine raw jet pt
      double ptRaw  = jet->pt();
      // determine corrected jet pt
      double ptL2L3 = jet->pt();
      if(corrector){
	ptL2L3 *= corrector->correction(*jet);
      }
      if(ptL2L3>20){ ++mult20; // determine jet multiplicity
	if( includeBTag_ ){
	  // fill b-discriminators
	  edm::RefToBase<reco::Jet> jetRef = jets.refAt(mult);	
	  fill("jetBDiscEff_", (*btagEff)[jetRef]);
	  fill("jetBDiscPur_", (*btagPur)[jetRef]);
	  fill("jetBDiscVtx_", (*btagVtx)[jetRef]);
	  if( (*btagEff)[jetRef]>btagEffWP_ ) ++mult20BEff; 
	  if( (*btagPur)[jetRef]>btagPurWP_ ) ++mult20BPur; 
	  if( (*btagVtx)[jetRef]>btagVtxWP_ ) ++mult20BVtx; 
	}
      }
      // fill pt (raw or L2L3) for the leading four jets  
      if(mult==0) {fill("jet1Pt_"   , ptL2L3); fill("jet1PtRaw_", ptRaw );}
      if(mult==1) {fill("jet2Pt_"   , ptL2L3); fill("jet2PtRaw_", ptRaw );}
      if(mult==2) {fill("jet3Pt_"   , ptL2L3); fill("jet3PtRaw_", ptRaw );}
      if(mult==3) {fill("jet4Pt_"   , ptL2L3); fill("jet4PtRaw_", ptRaw );}
    }
    fill("jetMult_"    , mult20    );
    fill("jetMultBEff_", mult20BEff);
    fill("jetMultBPur_", mult20BPur);
    fill("jetMultBVtx_", mult20BVtx);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event) const
  {
    // check availability of electron id
    edm::Handle<edm::ValueMap<float> > electronId; 
    if(!electronId_.label().empty()) event.getByLabel(electronId_, electronId);

    // loop electron collection
    unsigned int mult=0, multIso=0;
    for(edm::View<reco::GsfElectron>::const_iterator elec=elecs.begin(); elec!=elecs.end(); ++elec, ++mult){
      if( electronId_.label().empty() ? true : (*electronId)[elecs.refAt(mult)]>0.99 ){
	// restrict to electrons with good electronId
	double isolation = (elec->dr03TkSumPt()+elec->dr04EcalRecHitSumEt()+elec->dr04HcalTowerSumEt())/elec->pt();
	if( mult==0 ){
	  // restrict to the leading electron
	  fill("elecPt_" , elec->pt() );
	  fill("elecEta_", elec->eta());
	  fill("elecIso_"     , isolation);
	  fill("elecIsoTrk_"  , elec->dr03TkSumPt());
	  fill("elecIsoEcal_" , elec->dr04EcalRecHitSumEt());
	  fill("elecIsoHcal_" , elec->dr04HcalTowerSumEt() );
	}
	if( isolation ){
	  ++multIso;
	}
      }
    }
    fill("elecMult_",    mult   );
    fill("elecMultIso_", multIso);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Muon>& muons) const 
  {
    // loop muon collection
    unsigned int mult=0, multIso=0;
    for(edm::View<reco::Muon>::const_iterator muon=muons.begin(); muon!=muons.end(); ++muon, ++mult){
      if( muon->combinedMuon().isNull()==0 ){ 
	// restrict to globalMuons
	double isolation = (muon->isolationR03().sumPt+muon->isolationR03().emEt+muon->isolationR03().hadEt)/muon->pt();
	if( mult==0 ){
	  // restrict to leading muon
	  fill("muonPt_"      , muon->pt() );
	  fill("muonEta_"     , muon->eta());
	  fill("muonIso_"     , isolation  );
	  fill("muonIsoTrk_"  , muon->isolationR03().sumPt);
	  fill("muonIsoEcal_" , muon->isolationR03().emEt );
	  fill("muonIsoHcal_" , muon->isolationR03().hadEt);
	}
	if( isolation ){
	  ++multIso;
	}
      }
    }
    fill("muonMult_",    mult   );
    fill("muonMultIso_", multIso);
  }
  
}


TopSingleLeptonDQM::TopSingleLeptonDQM(const edm::ParameterSet& cfg): triggerTable_(""), beamspot_("") 
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
TopSingleLeptonDQM::analyze(const edm::Event& event, const edm::EventSetup& setup)
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
DEFINE_FWK_MODULE(TopSingleLeptonDQM);
