#include "SDCrystalScintillator.hh"

#include "G4HCofThisEvent.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "HitCrystalScintillator.hh"

SDCrystalScintillator::SDCrystalScintillator(const G4String& sdName, const G4String& hitsCollectionName) :
    G4VSensitiveDetector(sdName) {
    collectionName.insert(hitsCollectionName);
}

SDCrystalScintillator::~SDCrystalScintillator() {}

void SDCrystalScintillator::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) {
    hcCrystal = new HitCollectionCrystal(SensitiveDetectorName, collectionName[0]);

    auto hitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hitsCollectionOfThisEvent->AddHitsCollection(hitsCollectionID, hcCrystal);

    hcCrystal->insert(new HitCrystalScintillator);
}

G4bool SDCrystalScintillator::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    if (!hcCrystal) {
        G4cerr << "Error: CrystalScintillatorCollection not found." << G4endl;
        return false;
    }

    if (aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::Definition()) {
        (*hcCrystal)[0]->AddEnergyDeposit(aStep->GetTotalEnergyDeposit());
        (*hcCrystal)[0]->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    }

    // auto preStepPoint = aStep->GetPreStepPoint();
    // auto postStepPoint = aStep->GetPostStepPoint();
    // auto stepStatus = preStepPoint->GetStepStatus();
    // auto track = aStep->GetTrack();

    // // we need to check all the input particles

    // if (stepStatus == fGeomBoundary &&
    //     aStep->IsFirstStepInVolume())
    // {
    //     (*hcCrystal)[0]->AddPDGID(track->GetDefinition()->GetPDGEncoding());
    //     (*hcCrystal)[0]->AddEntryTime(preStepPoint->GetGlobalTime());
    //     (*hcCrystal)[0]->SetTrackID(track->GetTrackID());
    // }

    return true;
}