#ifndef TOPDILEPTONOFFLINEDQM
#define TOPDILEPTONOFFLINEDQM

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Event.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Math/interface/deltaR.h"


/**
   \class   MonitorEnsemble TopDQMHelpers.h "DQM/Physics/interface/TopDQMHelpers.h"

   \brief   Helper class to define histograms for monitoring of muon/electron/jet/met quantities.

   Helper class to contain histograms for the monitoring of muon/electron/jet/met quantities. 
   This class can be instantiated several times after several event selection steps. It can 
   be used to fill histograms in three different granularity levels according to STANDARD 
   (<10 histograms), VERBOSE(<20 histograms), DEBUG(<30 histgorams). Note that for the sake 
   of simplicity and to force the analyst to keep the number of histograms to be monitored 
   small the MonitorEnsemble class contains the histograms for all objects at once. It should 
   not contain much more than 10 histograms though in the STANDARD configuration, as these 
   histograms will be monitored at each SelectionStep. Monitoring of histograms after selec-
   tion steps within the same object collection needs to be implemented within the Monitor-
   Ensemble. It will not be covered by the SelectionStep class.
*/

namespace TopDiLeptonOffline {

  class MonitorEnsemble {
  public:
    /// different verbosity levels
    enum Level{ STANDARD, VERBOSE, DEBUG };
    
  public:
    /// default contructor
    MonitorEnsemble(const char* label, const edm::ParameterSet& cfg);
    /// default destructor
    ~MonitorEnsemble(){};
    
    /// book histograms
    void book();
    /// fill monitor histograms with electronId and jetCorrections
    void fill(const edm::Event& event, const edm::EventSetup& setup) const;

  private:
    /// check if histogram was booked
    bool booked(const std::string histName) const { return hists_.find(histName.c_str())!=hists_.end(); };
    /// fill histogram if it had been booked before
    void fill(const std::string histName, double value) const { if(booked(histName.c_str())) hists_.find(histName.c_str())->second->Fill(value); };
    /// fill jet histograms
    void fill(const edm::View<reco::Jet>& jets, const edm::Event& event, const JetCorrector* corrector, double& phiJet1, double& etaJet1, double& phiJet2, double& etaJet2) const;
    /// fill electron histograms
    void fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event, double& phiElec1, double& etaElec1, double& phiElec2, double& etaElec2, double& ptElec1, double& ptElec2) const;
    /// fill muon histograms
    void fill(const edm::View<reco::Muon>& muons, double& phiMuon1, double& etaMuon1, double& phiMuon2, double& etaMuon2, double& ptMuon1, double& ptMuon2) const;
    /// fill histograms including differing objects
    void fill(double& phiJet1, double& etaJet1, double& phiJet2, double& etaJet2, double& phiElec1, double& etaElec1, double& phiElec2, double& etaElec2, double& phiMuon1, double& etaMuon1, double& phiMuon2, double& etaMuon2, double& phiMET, double& etaMET, double& ptElec1, double& ptElec2, double& ptMuon1, double& ptMuon2) const;
  private:
    /// verbosity level for booking
    Level verbosity_;
    /// instance label 
    std::string label_;
    /// input sources for monitoring
    edm::InputTag elecs_, muons_, jets_; 
    /// considers a vector of METs
    std::vector<edm::InputTag> mets_;
    /// electronId
    edm::InputTag electronId_;
    /// jetCorrector
    std::string jetCorrector_;

    /// storage manager
    DQMStore* store_;
    /// histogram container  
    std::map<std::string,MonitorElement*> hists_;
  };

}

#include <utility>

#include "DQM/Physics/interface/TopDQMHelpers.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
  
/**
   \class   TopDiLeptonOfflineDQM TopDiLeptonOfflineDQM.h "DQM/Physics/plugins/TopDiLeptonOfflineDQM.h"

   \brief   Module to apply a monitored selection of top like events in the di-leptonic channel

   Plugin to apply a monitored selection of top like events with some minimal flexibility 
   in the number and definition of the selection steps. To achieve this flexibility it 
   employes the SelectionStep class. The MonitorEnsemble class is used to provide a well 
   defined set of histograms to be monitored after each selection step. The SelectionStep 
   class provides a flexible and intuitive selection via the StringCutParser. SelectionStep 
   and MonitorEnsemble classes are interleaved. The monitoring starts after a preselection 
   step (which is not monitored in the context of this module) with an instance of the 
   MonitorEnsemble class. The following objects are supported for selection:

    - jets  : of type reco::Jet
    - elecs : of type reco::GsfElectron
    - muons : of type reco::Muon
    - met   : of type reco::MET

   These types have to be present as prefix of the selection step paramter _label_ separated 
   from the rest of the label by a ':' (e.g. in the form "jets:step0"). The class expects 
   selection labels of this type. They will be disentangled by the private helper functions 
   _objectType_ and _seletionStep_ as declared below.
*/

/// define MonitorEnsembple to be used
using TopDiLeptonOffline::MonitorEnsemble;

class TopDiLeptonOfflineDQM : public edm::EDAnalyzer  {
 public: 
  /// default constructor
  TopDiLeptonOfflineDQM(const edm::ParameterSet& cfg);
  /// default destructor
  ~TopDiLeptonOfflineDQM(){};
  
  /// do this during the event loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
    
 private:
  /// deduce object type from ParameterSet label, the label
  /// is expected to be of type 'objectType:selectionStep'
  std::string objectType(const std::string& label) { return label.substr(0, label.find(':')); };  
  /// deduce selection step from ParameterSet label, the 
  /// label is expected to be of type 'objectType:selectionStep' 
  std::string selectionStep(const std::string& label) { return label.substr(label.find(':')+1); };  

 private:
  /// trigger table
  edm::InputTag triggerTable_;
  /// trigger paths
  std::vector<std::string> triggerPaths_;
  /// beamspot 
  edm::InputTag beamspot_;
  /// string cut selector
  StringCutObjectSelector<reco::BeamSpot>* beamspotSelect_;

  /// needed to guarantee the selection order as defined by the order of
  /// ParameterSets in the _selection_ vector as defined in the config
  std::vector<std::string> selectionOrder_;
  /// this is the heart component of the plugin; std::string keeps a label 
  /// the selection step for later identification, edm::ParameterSet keeps
  /// the configuration of the selection for the SelectionStep class, 
  /// MonitoringEnsemble keeps an instance of the MonitorEnsemble class to 
  /// be filled _after_ each selection step
  std::map<std::string, std::pair<edm::ParameterSet, MonitorEnsemble*> > selection_;
};

#endif