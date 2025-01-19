#include "SDPlasticAbsorber.hh"

#include "G4HCofThisEvent.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "HitPlasticAbsorber.hh"

SDPlasticAbsorber::SDPlasticAbsorber(const G4String& name, const G4String& hitsCollectionName) :
    G4VSensitiveDetector(name) {
    collectionName.insert(hitsCollectionName);
}

SDPlasticAbsorber::~SDPlasticAbsorber() {}

void SDPlasticAbsorber::Initialize(G4HCofThisEvent* hitCollectionOfThisEvent) {
    hcAbsorber = new HitCollectionAbsorber(SensitiveDetectorName, collectionName[0]);

    auto hitCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hitCollectionOfThisEvent->AddHitsCollection(hitCollectionID, hcAbsorber);
}

G4bool SDPlasticAbsorber::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    if (!hcAbsorber) {
        G4cerr << "Error: PlasticAbsorberCollection not found." << G4endl;
        return false;
    }

    auto preStepPoint = aStep->GetPreStepPoint();
    auto postStepPoint = aStep->GetPostStepPoint();

    auto track = aStep->GetTrack();

    // figure out the entering particle
    // 找到所有进入体积的粒子
    if ( // track->GetDefinition()->GetPDGEncoding() != 0 and
        track->GetDefinition()->GetPDGEncoding() != 0 &&
        // track->GetCreatorProcess()->GetProcessName() == "Decay" and
        aStep->IsFirstStepInVolume()) {
        // std::cout << track->GetDefinition()->GetPDGEncoding() << std::endl;
        auto hit = new HitPlasticAbsorber;
        hit->AddPDGID(track->GetDefinition()->GetPDGEncoding());
        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(0);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(0);

        hcAbsorber->insert(hit);
    }

    // find the michel electron annihilated in the plastic scintillator
    // 找到所有在体积内停止的粒子
    if ( // track->GetDefinition()->GetPDGEncoding() != 0 and
        track->GetDefinition()->GetPDGEncoding() != 0 &&
        // track->GetCreatorProcess()->GetProcessName() == "Decay" &&
        track->GetTrackStatus() == fStopAndKill) {
        auto hit = new HitPlasticAbsorber;

        // hit->AddPDGID(track->GetDefinition()->GetAntiPDGEncoding());
        hit->AddPDGID(track->GetDefinition()->GetPDGEncoding());
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(1);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(1);

        hcAbsorber->insert(hit);
    }

    if (track->GetDefinition()->GetPDGEncoding() != 0 and
        aStep->IsLastStepInVolume())
    // track->GetCreatorProcess()->GetProcessName() == "Decay")
    // track->GetCreatorProcess()->GetProcessName() == "Decay"

    // postStepPoint->GetStepStatus() == fGeomBoundary

    {
        auto hit = new HitPlasticAbsorber;

        hit->AddPDGID(track->GetDefinition()->GetPDGEncoding());
        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(0);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(2);

        hcAbsorber->insert(hit);
    }

    return true;
}