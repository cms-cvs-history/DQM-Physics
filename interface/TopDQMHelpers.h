#ifndef TOPDQMHELPERS
#define TOPDQMHELPERS

#include <string>
#include <vector>

#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

/**
   \fn      accept TopDQMHelpers.h "DQM/Physics/interface/TopDQMHelpers.h"
   
   \brief   Helper function to determine trigger accepts.
   
   Helper function to determine trigger accepts for given TriggerResults and 
   given set of TriggerPaths.
*/

inline bool 
accept(const edm::TriggerResults& triggerTable, const std::vector<std::string>& triggerPaths)
{
  bool accept=false;
  edm::TriggerNames triggerNames;
  triggerNames.init(triggerTable);
  for(unsigned int i=0; i<triggerNames.triggerNames().size(); ++i){
    for(unsigned int j=0; j<triggerPaths.size(); ++j){
      if(triggerNames.triggerNames()[i] == triggerPaths[j]) {
	if(triggerTable.accept(i)){
	  accept=true;
	}
	if(accept==true) break;
      }
    }
  }
  return accept;
}

#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

/**
   \class   Calculate TopDQMHelpers.h "DQM/Physics/interface/TopDQMHelpers.h"
   
   \brief   Helper class for the calculation of a top and a W boson mass estime.
   
   Helper class for the calculation of a top and a W boson mass estimate. The
   core implementation originates from the plugin TtSemiLepHypMaxSumPtWMass 
   in TopQuarkAnalysis/TopJetCombination package. It may be extended to include
   b tag information.
*/

class Calculate {
 public:
  /// default constructor
  Calculate(int maxNJets, double wMass);
  /// default destructor
  ~Calculate(){};
     
  /// calculate W boson mass estimate
  double massWBoson(const edm::View<reco::Jet>& jets, const edm::EventSetup& setup);
  /// calculate W boson mass estimate
  double massTopQuark(const edm::View<reco::Jet>& jets, const edm::EventSetup& setup); 
  
 private:
  /// do the calculation; this is called only once per event by the first 
  /// function call to return a mass estimate. The once calculated values 
  /// are cached afterwards
  void operator()(const edm::View<reco::Jet>& jets, const edm::EventSetup& setup);

 private:
  /// indicate failed associations
  bool failed_;
  /// max. number of jets to be considered 
  int maxNJets_;
  /// paramater of the w boson mass
  double wMass_;
  /// cache of w boson mass estimate
  double massWBoson_;
  /// cache of top quark mass estimate
  double massTopQuark_;
};


#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

/**
   \class   SelectionStep TopDQMHelpers.h "DQM/Physics/interface/TopDQMHelpers.h"
   
   \brief   Templated helper class to allow a selection on a certain object collection.
   
   Templated helper class to allow a selection on a certain object collection, which may 
   be monitored by a separate class afterwards. The class wraps and slightly extends the 
   features of the StringCutParser to allow also to apply event based selections, according 
   to a minimal or maximal number of elements in the collection after the object selection 
   has been applied. It takes an edm::ParameterSet in the constructor, which should contain
   the following elements:
   
    - src          : the input collection (mandatory).
    - select       : the selection string (mandatory).
    - min          : whether there is a min value on which to reject the whole event after 
                     the selection (optional).
    - max          : whether there is a max value on which to reject the whole event after 
                     the selection (optional).
    - electronId   : input tag of an electronId association map (optional). 
    - jetCorrector : label of jet corrector (optional).
   The parameters _src_ and _select_ are mandatory. The parameters _min_ and _max_ are 
   optional. The parameters _electronId_ and _jetCorrector_ are optional. They are added 
   to keep the possibility to apply selections on id'ed electrons or on corrected jets. 
   They may be omitted in the PSet for simplification reasons if not needed at any time. 
   They are not effiective for other object collections but electrons or jets. 

   The class has one template value, which is the object collection to apply the selection 
   on. This has to be parsed to the StringCutParser class. The function select is overrided 
   for jets to circumvent problems with the template specialisation. Note that for MET not 
   type1 or muon corrections are supported on reco candidates.
*/

template <typename Object> 
class SelectionStep {
public:
  /// default constructor
  SelectionStep(const edm::ParameterSet& cfg);
  /// default destructor
  ~SelectionStep(){};

  /// apply selection
  bool select(const edm::Event& event);
  /// apply selection (w/o using the template class Object), 
  /// override for jets
  bool select(const edm::Event& event, const edm::EventSetup& setup); 
    
private:
  /// input collection
  edm::InputTag src_;
  /// min/max for object multiplicity
  int min_, max_; 
  /// electronId as extra selection type
  edm::InputTag electronId_;
  /// jet corrector as extra selection type
  std::string jetCorrector_;
  /// string cut selector
  StringCutObjectSelector<Object> select_;
};

/// default constructor
template <typename Object> 
SelectionStep<Object>::SelectionStep(const edm::ParameterSet& cfg) :
  src_( cfg.getParameter<edm::InputTag>( "src"   )),
  select_( cfg.getParameter<std::string>("select"))
{
  // construct min/max if the corresponding params
  // exist otherwise they are initialized with -1
  cfg.exists("min") ? min_= cfg.getParameter<int>("min") : min_= -1;
  cfg.exists("max") ? max_= cfg.getParameter<int>("max") : max_= -1;
  // read electron extras if they exist
  if(cfg.exists("electronId"  )){ electronId_= cfg.getParameter<edm::InputTag>("electronId"  ); }
  // read jet corrector label if it exists
  if(cfg.exists("jetCorrector")){ jetCorrector_= cfg.getParameter<std::string>("jetCorrector"); }
}

/// apply selection
template <typename Object> 
bool SelectionStep<Object>::select(const edm::Event& event)
{
  // fetch input collection
  edm::Handle<edm::View<Object> > src; 
  event.getByLabel(src_, src);

  // load electronId value map if configured such
  edm::Handle<edm::ValueMap<float> > electronId;
  if(!electronId_.label().empty()) event.getByLabel(electronId_, electronId);

  // determine multiplicity of selected objects
  int n=0;
  for(typename edm::View<Object>::const_iterator obj=src->begin(); obj!=src->end(); ++obj){
    // special treatment for electrons
    if(dynamic_cast<const reco::GsfElectron*>(&*obj)){
      unsigned int idx = obj-src->begin();
      if( electronId_.label().empty() ? true : (*electronId)[src->refAt(idx)]>0.99 ){   
	if(select_(*obj))++n;
      }
    }
    // normal treatment
    else{
      if(select_(*obj))++n;
    }
  }
  bool accept=(min_>=0 ? n>=min_:true) && (max_>=0 ? n<=max_:true);
  return (min_<0 && max_<0) ? (n>0):accept;
}

/// apply selection (w/o using the template class Object), override for jets
template <typename Object> 
bool SelectionStep<Object>::select(const edm::Event& event, const edm::EventSetup& setup)
{
  // fetch input collection
  edm::Handle<edm::View<reco::Jet> > src; 
  event.getByLabel(src_, src);
  
  // load jet corrector if configured such
  const JetCorrector* corrector=0;
  if(!jetCorrector_.empty()){
    corrector = JetCorrector::getJetCorrector(jetCorrector_, setup);
  }

  // determine multiplicity of selected objects
  int n=0;
  for(typename edm::View<reco::Jet>::const_iterator obj=src->begin(); obj!=src->end(); ++obj){
    // special treatment for jets; here the problems with template specialisation 
    // arise when dynamically casting a const Object to a reco::Jet which has to 
    // be mutable by definition to allow to change four vector according to the 
    // jet energy corrections derived from the jet correction service
    if(corrector){
      reco::Jet jet=*obj; jet.setP4((*obj).p4()*corrector->correction(*obj));
      if(select_(jet))++n;
    }
  }
  bool accept=(min_>=0 ? n>=min_:true) && (max_>=0 ? n<=max_:true);
  return (min_<0 && max_<0) ? (n>0):accept;
}

#endif
