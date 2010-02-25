#include "DQM/Physics/plugins/TopSingleLeptonDQM.h"

namespace TopSingleLepton {

  // maximal number of leading jets 
  // to be considered in jet loop
  static const unsigned int MAXJETS = 4;

  MonitorEnsemble::MonitorEnsemble(const char* label, const edm::ParameterSet& cfg) : label_(label)
  {
    // sources have to be given; this PSet is not optional
    edm::ParameterSet sources=cfg.getParameter<edm::ParameterSet>("sources");
    jets_ = sources.getParameter<edm::InputTag>("jets" );
    elecs_= sources.getParameter<edm::InputTag>("elecs");
    muons_= sources.getParameter<edm::InputTag>("muons");

    // electronId is optional; in case it's not found the 
    // InputTag will remain empty
    if( cfg.existsAs<edm::ParameterSet>("electronId") ){
      edm::ParameterSet electronId=cfg.getParameter<edm::ParameterSet>("electronId");
      electronId_= electronId.getParameter<edm::InputTag>("electronId");
    }

    // jetCorrector is optional; in case it's not found 
    // the  InputTag will remain empty
    if( cfg.existsAs<edm::ParameterSet>("jetCorrector") ){
      edm::ParameterSet jetCorrector=cfg.getParameter<edm::ParameterSet>("jetCorrector");
      jetCorrector_= jetCorrector.getParameter<std::string>("jetCorrector");
    }

    // setup the verbosity level for booking histograms;
    // per default the verbosity level will be set to 
    // STANDARD. This will also be the chsen level in
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
    store_=edm::Service<DQMStore>().operator->();
    store_->setCurrentFolder("TopSingleLeptonDQM");

    // --- [STANDARD] --- //
    // pt of the leading muon
    hists_["muonPt_"]      = store_->book1D((label_+"MuonPt").c_str(),      "pt(Muon)"         , 150,   0., 150.);   
    // muon multiplicity after potential (user configured) preselection and before std isolation
    hists_["muonMult_"]    = store_->book1D((label_+"MuonMult").c_str(),    "N_{all}(Muon)"    ,  10,   0.,  10.);   
    // muon multiplicity after potential (user configured) preselection and after  std isolation
    hists_["muonMultIso_"] = store_->book1D((label_+"MuonMultIso").c_str(), "N_{iso}(Muon)"    ,  10,   0.,  10.);   
    // pt of the leading electron
    hists_["elecPt_"]      = store_->book1D((label_+"ElecPt").c_str(),      "pt(Elec)"         , 150,   0., 150.);   
    // electron multiplicity after potential (user configured) preselection and before std isolation
    hists_["elecMult_"]    = store_->book1D((label_+"ElecMult").c_str(),    "N_{all}(Elec)"    ,  10,   0.,  10.);   
    // electron multiplicity after potential (user configured) preselection and after  std isolation
    hists_["elecMultIso_"] = store_->book1D((label_+"ElecMultIso").c_str(), "N_{iso}(Elec)"    ,  10,   0.,  10.);   
    // W mass estimate
    hists_["massW_"]       = store_->book1D((label_+"MassW").c_str(),       "M(W)"             , 100,  50., 150.);   
    // Top mass estimate
    hists_["massTop_"]     = store_->book1D((label_+"MassTop").c_str(),     "M(Top)"           , 100, 100., 300.);   
    if( verbosity_==STANDARD) return;

    // --- [VERBOSE] --- //
    // eta of the leading muon
    hists_["muonEta_"]     = store_->book1D((label_+"MuonEta").c_str(),     "#h(Muon)"         ,  30,  -5.,   5.);   
    // std isolation varuiable of the leading muon
    hists_["muonIso_"]     = store_->book1D((label_+"MuonIso").c_str(),     "Iso(Muon)"        ,  30,   0.,   3.);   
    // eta of the leading electron
    hists_["elecEta_"]     = store_->book1D((label_+"ElecEta").c_str(),     "#h(Elec)"         ,  30,  -5.,   5.);   
    // std isolation varuiable of the leading electron
    hists_["elecIso_"]     = store_->book1D((label_+"ElecIso").c_str(),     "Iso(Elec)"        ,  30,   0.,   3.);   
    // pt of the 1. leading jet (corrected to L2+L3)
    hists_["jet1Pt_"]      = store_->book1D((label_+"Jet1Pt").c_str() ,     "pt(Jet1)"         , 100,   0., 200.);   
    // pt of the 2. leading jet (corrected to L2+L3)
    hists_["jet2Pt_"]      = store_->book1D((label_+"Jet2Pt").c_str() ,     "pt(Jet2)"         , 100,   0., 200.);   
    // pt of the 3. leading jet (corrected to L2+L3)
    hists_["jet3Pt_"]      = store_->book1D((label_+"Jet3Pt").c_str() ,     "pt(Jet3)"         , 100,   0., 200.);   
    // pt of the 4. leading jet (corrected to L2+L3)
    hists_["jet4Pt_"]      = store_->book1D((label_+"Jet4Pt").c_str() ,     "pt(Jet4)"         , 100,   0., 200.);   
    if( verbosity_==VERBOSE) return;

    // --- [DEBUG] --- //
    // absolute muon isolation in tracker
    hists_["muonIsoTrk_"]  = store_->book1D((label_+"MuonIsoTrk").c_str(),  "Iso_{trk}(Muon)"  ,  25,   0.,   5.);   
    // absolute muon isolation in ecal
    hists_["muonIsoEcal_"] = store_->book1D((label_+"MuonIsoEcal").c_str(), "Iso_{ecal}(Muon)" ,  25,   0.,   5.);   
    // absolute muon isolation in hcal
    hists_["muonIsoHcal_"] = store_->book1D((label_+"MuonIsoHcal").c_str(), "Iso_{hcal}(Muon)" ,  25,   0.,   5.);   
    // absolute electron isolation in tracker
    hists_["elecIsoTrk_"]  = store_->book1D((label_+"ElecIsoTrk").c_str(),  "Iso_{trk}(Elec)"  ,  25,   0.,   5.);   
    // absolute electron isolation in ecal
    hists_["elecIsoEcal_"] = store_->book1D((label_+"ElecIsoEcal").c_str(), "Iso_{ecal}(Elec)" ,  25,   0.,   5.);   
    // absolute electron isolation in hcal
    hists_["elecIsoHcal_"] = store_->book1D((label_+"ElecIsoHcal").c_str(), "Iso_{hcal}(Elec)" ,  25,   0.,   5.);   
    // pt of the 1. leading jet (uncorrected)
    hists_["jet1PtRaw_"]   = store_->book1D((label_+"Jet1PtRaw").c_str() ,  "pt(Jet1, raw)"    , 100,   0., 200.);   
    // pt of the 2. leading jet (uncorrected)
    hists_["jet2PtRaw_"]   = store_->book1D((label_+"Jet2PtRaw").c_str() ,  "pt(Jet2, raw)"    , 100,   0., 200.);   
    // pt of the 3. leading jet (uncorrected)
    hists_["jet3PtRaw_"]   = store_->book1D((label_+"Jet3PtRaw").c_str() ,  "pt(Jet3, raw)"    , 100,   0., 200.);   
    // pt of the 4. leading jet (uncorrected)
    hists_["jet4PtRaw_"]   = store_->book1D((label_+"Jet4PtRaw").c_str() ,  "pt(Jet4, raw)"    , 100,   0., 200.);   
    return;
  }

  void 
  MonitorEnsemble::fill(const edm::Event& event, const edm::EventSetup& setup) const
  {
    edm::Handle<edm::View<reco::Muon> > muons;
    event.getByLabel(muons_, muons); fill(*muons);
    edm::Handle<edm::View<reco::GsfElectron> > elecs;
    event.getByLabel(elecs_, elecs); fill(*elecs, event);
    edm::Handle<edm::View<reco::Jet> > jets;
    event.getByLabel(jets_, jets); fill(*jets, setup);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::Jet>& jets, const edm::EventSetup& setup) const 
  {
    const JetCorrector* corrector = 0;

    if(!jetCorrector_.empty()){
      corrector = JetCorrector::getJetCorrector(jetCorrector_, setup);
    }
    unsigned int idx=0;
    for(edm::View<reco::Jet>::const_iterator jet=jets.begin(); jet!=jets.end(); ++jet, ++idx){
      if( idx<MAXJETS ){
	// determine raw jet pt
	double ptRaw = jet->pt();
	// determine corrected jet pt
	double ptCorrected = jet->pt();
	if(corrector){
	  ptCorrected *= corrector->correction(*jet);
	}
	switch (idx){
	case 0 : // 1. leading jet
	  fill("jet1Pt"   , ptCorrected); 
	  fill("jet1PtRaw", ptRaw      ); 
	  break; 
	case 1 : // 2. leading jet
	  fill("jet2Pt"   , ptCorrected); 
	  fill("jet2PtRaw", ptRaw      ); 
	  break;
	case 2 : // 3. leading jet
	  fill("jet3Pt"   , ptCorrected); 
	  fill("jet3PtRaw", ptRaw      ); 
	  break;
	case 3 : // 4. leading jet
	  fill("jet4Pt"   , ptCorrected); 
	  fill("jet4PtRaw", ptRaw      ); 
 	  break;
 	}
      }
      break;
    }
  }
  
  void
  MonitorEnsemble::fill(const edm::View<reco::Muon>& muons) const 
  {
    unsigned int idx=0, mult=0, multIso=0;
    for(edm::View<reco::Muon>::const_iterator muon=muons.begin(); muon!=muons.end(); ++muon, ++idx){
      if( muon->combinedMuon().isNull()==0 ){
	// restrict to globalMuons
	double isolation = (muon->isolationR03().sumPt+muon->isolationR03().emEt+muon->isolationR03().hadEt)/muon->pt();
	if( idx==0 ){
	  // restrict to leading muon
	  fill("muonPt_" , muon->pt() );
	  fill("muonEta_", muon->eta());
	}
	++mult;
	fill("muonIso_"     , isolation);
	fill("muonIsoTrk_"  , muon->isolationR03().sumPt);
	fill("muonIsoEcal_" , muon->isolationR03().emEt );
	fill("muonIsoHcal_" , muon->isolationR03().hadEt);
	if( isolation ){
	  ++multIso;
	}
      }
    }
    fill("muonMult_",    mult   );
    fill("muonMultIso_", multIso);
  }

  void
  MonitorEnsemble::fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event) const
  {
    edm::Handle<edm::ValueMap<float> > electronId; 
    if(!electronId_.label().empty()) event.getByLabel(electronId_, electronId);

    unsigned int idx=0, mult=0, multIso=0;
    for(edm::View<reco::GsfElectron>::const_iterator elec=elecs.begin(); elec!=elecs.end(); ++elec, ++idx){
      if( electronId_.label().empty() ? true : (*electronId)[elecs.refAt(idx)]>0.99 ){
	// restrict to electrons with good electronId
	double isolation = (elec->dr03TkSumPt()+elec->dr04EcalRecHitSumEt()+elec->dr04HcalTowerSumEt())/elec->pt();
	if( idx==0 ){
	  // restrict to the leading electron
	  fill("elecPt_" , elec->pt() );
	  fill("elecEta_", elec->eta());
	}
	++mult;
	fill("elecIso_"     , isolation);
	fill("elecIsoTrk_"  , elec->dr03TkSumPt());
	fill("elecIsoEcal_" , elec->dr04EcalRecHitSumEt());
	fill("elecIsoHcal_" , elec->dr04HcalTowerSumEt() );
	if( isolation ){
	  ++multIso;
	}
      }
    }
    fill("elecMult_",    mult   );
    fill("elecMultIso_", multIso);
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
