#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "DQM/Physics/src/BPhysicsOniaDQM.h"
#include "DQM/Physics/src/QcdPhotonsDQM.h"
#include "DQM/Physics/src/QcdHighPtDQM.h"
#include "DQM/Physics/src/EwkDQM.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(BPhysicsOniaDQM);
DEFINE_ANOTHER_FWK_MODULE(QcdPhotonsDQM);
DEFINE_ANOTHER_FWK_MODULE(QcdHighPtDQM);
DEFINE_ANOTHER_FWK_MODULE(EwkDQM);
