#include "DQM/Physics/interface/TopDQMHelpers.h"

Calculate::Calculate(int maxNJets, double wMass, const JetCorrector* corrector): 
  failed_(false), maxNJets_(maxNJets), wMass_(wMass), massWBoson_(-1.), massTopQuark_(-1.), corrector_(corrector)
{
}

double
Calculate::massWBoson(const edm::View<reco::Jet>& jets)
{
  if(!failed_&& massWBoson_<0) operator()(jets); return massWBoson_;
}

double 
Calculate::massTopQuark(const edm::View<reco::Jet>& jets) 
{ 
  if(!failed_&& massTopQuark_<0) operator()(jets); return massTopQuark_; 
}

void
Calculate::operator()(const edm::View<reco::Jet>& jets)
{
  if(maxNJets_<0) maxNJets_=jets.size();
  failed_= jets.size()<(unsigned int) maxNJets_;
  if( failed_){ return; }

  // get from uncorrected jets to corrected jets exploiting
  // the jet corrector; only the first maxNJets_ jets are 
  // considered
  std::vector<reco::Jet> correctedJets;
  for(edm::View<reco::Jet>::const_iterator jet=jets.begin(); jet!=jets.begin()+maxNJets_; ++jet){
    reco::Jet correctedJet=*jet; correctedJet.scaleEnergy(corrector_->correction(*jet));
    correctedJets.push_back(correctedJet);
  }

  // associate those jets with maximum pt of the vectorial 
  // sum to the hadronic decay chain
  double maxPt=-1.;
  std::vector<int> maxPtIndices;
  maxPtIndices.push_back(-1);
  maxPtIndices.push_back(-1);
  maxPtIndices.push_back(-1);
  for(int idx=0; idx<maxNJets_; ++idx){
    for(int jdx=0; jdx<maxNJets_; ++jdx){ 
      if(jdx<=idx) continue;
      for(int kdx=0; kdx<maxNJets_; ++kdx){ 
	if(kdx==idx || kdx==jdx) continue;
	reco::Particle::LorentzVector sum = 
	  correctedJets[idx].p4()+
	  correctedJets[jdx].p4()+
	  correctedJets[kdx].p4();
	if( maxPt<0. || maxPt<sum.pt() ){
	  maxPt=sum.pt();
	  maxPtIndices.clear();
	  maxPtIndices.push_back(idx);
	  maxPtIndices.push_back(jdx);
	  maxPtIndices.push_back(kdx);
	}
      }
    }
  }
  massTopQuark_= (correctedJets[maxPtIndices[0]].p4()+
		  correctedJets[maxPtIndices[1]].p4()+
		  correctedJets[maxPtIndices[2]].p4()).mass();

  // associate those jets that get closest to the W mass
  // with their invariant mass to the W boson
  double wDist =-1.;
  std::vector<int> wMassIndices;
  wMassIndices.push_back(-1);
  wMassIndices.push_back(-1);
  for(unsigned idx=0; idx<maxPtIndices.size(); ++idx){  
    for(unsigned jdx=0; jdx<maxPtIndices.size(); ++jdx){  
      if( jdx==idx || maxPtIndices[idx]>maxPtIndices[jdx] ) 
	continue;
	reco::Particle::LorentzVector sum = 
	  correctedJets[maxPtIndices[idx]].p4()+
	  correctedJets[maxPtIndices[jdx]].p4();
	if( wDist<0. || wDist>fabs(sum.mass()-wMass_) ){
	  wDist=fabs(sum.mass()-wMass_);
	  wMassIndices.clear();
	  wMassIndices.push_back(maxPtIndices[idx]);
	  wMassIndices.push_back(maxPtIndices[jdx]);
	}
    }
  }
  massWBoson_= (correctedJets[wMassIndices[0]].p4()+
		correctedJets[wMassIndices[1]].p4()).mass();
}
