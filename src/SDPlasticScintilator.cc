#include "G4HCofThisEvent.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "HitPlasticScintillator.hh"
#include "SDPlasticScintillator.hh"

SDPlasticScintillator::SDPlasticScintillator(const G4String& name, const G4String& hitsCollectionName) :
    G4VSensitiveDetector(name) {
    collectionName.insert(hitsCollectionName);
}

SDPlasticScintillator::~SDPlasticScintillator() {}

void SDPlasticScintillator::Initialize(G4HCofThisEvent* histCollectionOfThisEvent) {
    hcPlastic = new HitCollectionPlastic(SensitiveDetectorName, collectionName[0]);

    auto hitCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    histCollectionOfThisEvent->AddHitsCollection(hitCollectionID, hcPlastic);

    // hcPlastic->insert(new HitPlasticScintillator);
}

G4bool SDPlasticScintillator::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    if (!hcPlastic) {
        G4cerr << "Error: PlasticScintillatorCollection not found." << G4endl;
        return false;
    }
    auto preStepPoint = aStep->GetPreStepPoint();
    auto postStepPoint = aStep->GetPostStepPoint();

    auto track = aStep->GetTrack();

    // find the michel electron enter the plastic scintillator
    if (std::abs(track->GetDefinition()->GetPDGEncoding()) != 0 &&
        // track->GetCreatorProcess()->GetProcessName() == "Decay" &&
        aStep->IsFirstStepInVolume()) {
        auto hit = new HitPlasticScintillator;
        hit->AddPDGID(track->GetDefinition()->GetPDGEncoding());

        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        // if (auto creatorProcess = track->GetCreatorProcess()) {
        //     hit->SetProducedProcess(creatorProcess->GetProcessName());
        // } else {
        //     hit->SetProducedProcess("unknown");
        // }
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(0);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(0);

        hcPlastic->insert(hit);
    }

    // find the michel electron annihilated in the plastic scintillator
    if (std::abs(track->GetDefinition()->GetPDGEncoding()) != 0 &&
        track->GetTrackStatus() == fStopAndKill) {
        auto hit = new HitPlasticScintillator;

        hit->AddPDGID(track->GetDefinition()->GetAntiPDGEncoding());
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(1);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(1);

        hcPlastic->insert(hit);
    }

    if (std::abs(track->GetDefinition()->GetPDGEncoding()) != 0 &&
        aStep->IsLastStepInVolume())
    // track->GetCreatorProcess()->GetProcessName() == "Decay")
    // postStepPoint->GetStepStatus() == fGeomBoundary
    // track->GetCreatorProcess()->GetProcessName() == "Decay")

    {
        auto hit = new HitPlasticScintillator;

        hit->AddPDGID(track->GetDefinition()->GetPDGEncoding());
        hit->SetProducedProcess(track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "unknown");
        hit->AddTime(postStepPoint->GetGlobalTime());
        hit->SetAnnihilatedStatus(0);
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetTrackID(track->GetTrackID());
        hit->SetHitID(2);

        hcPlastic->insert(hit);
    }

    return true;
}