#ifndef TOPSINGLELEPTONDQM
#define TOPSINGLELEPTONDQM

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Event.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

/**
   \class   MonitorEnsemble TopDQMHelpers.h "DQM/Physics/interface/TopDQMHelpers.h"

   \brief   Helper class to define histograms for monitoring of muon/electron/jet/met quantities.

   Helper class to contain histograms for the monitoring of muon/electron/jet/met quantities. 
   This class can be instantiated several times after several event selection steps. It can 
   be used to fill histograms in three different granularity levels according to STANDARD 
   (<10 histograms), VERBOSE(<20 histograms), DEBUG(<30 histgorams). Note that for the sake 
   of simplicity and to force the analyst to keep the number of histograms to be monitored 
   small the MonitorEnsemble class contains the histograms for all objects at once. It should 
   not contain more than 10 histograms though in the STANDARD configuration, as these 
   histograms will be monitored at each SelectionStep. Monitoring of histograms after selec-
   tion steps within the same object collection needs to be done within the MonitorEnsemble.
   It will not be covered by the SelectionStep class.
*/

namespace TopSingleLepton {

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
    bool booked(const char* histName) const { return hists_.find(histName)!=hists_.end(); };
    /// fill histogram if it had been booked before
    void fill(const char* histName, double value) const { if(booked(histName)) hists_.find(histName)->second->Fill(value); };
    /// fill electron histograms
    void fill(const edm::View<reco::GsfElectron>& elecs, const edm::Event& event) const;
    /// fill jet histograms
    void fill(const edm::View<reco::Jet>& jets, const edm::EventSetup& setup) const;
    /// fill muon histograms
    void fill(const edm::View<reco::Muon>& muons) const;

  private:
    /// verbosity level for booking
    Level verbosity_;
    /// instance label 
    std::string label_;
    /// input sources for monitoring
    edm::InputTag elecs_, muons_, jets_;
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
   \class   TopSingleLeptonDQM TopSingleLeptonDQM.h "DQM/Physics/plugins/TopSingleLeptonDQM.h"

   \brief   Add a one line description here

   Plugin to apply a monitored selection of top like events with some minimal flexibility 
   in the number and definition of the selection steps. To achieve this flexibility the it 
   employes the SelectionStep class. The MonitorEnsemble class is used to provide a well 
   defined set of histograms to be monitored after each selection step. The SelectionStep 
   class provides a flexible and intuitive selection via the StringCutParser. SelectionStep 
   and MonitorEnsemble classes are interleaved. The monitoring starts after a preselection 
   step (which is not monitored in the context of this module) with an instance of the 
   MonitorEnsemble class.
*/

/// define MonitorEnsembple to be used
using TopSingleLepton::MonitorEnsemble;

class TopSingleLeptonDQM : public edm::EDAnalyzer  {
 public: 
  /// default constructor
  TopSingleLeptonDQM(const edm::ParameterSet& cfg);
  /// defasult destructor
    ~TopSingleLeptonDQM(){};
  
  /// do this during the event loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
    
 private:
  /// check for trigger accept
  //bool accept(const edm::TriggerResults& triggerTable) const;
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

  /// definition of selection order
  std::vector<std::string> selectionOrder_;
  /// first arg: selection step name, second arg: parameter set
  std::map<std::string, std::pair<edm::ParameterSet, MonitorEnsemble*> > selection_;
};

#endif
