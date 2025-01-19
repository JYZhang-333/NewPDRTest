#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "HitCrystalScintillator.hh"
#include "HitPlasticAbsorber.hh"
#include "HitPlasticScintillator.hh"
#include "newPDRConstruction.hh"
#include "SDCrystalScintillator.hh"
#include "SDPlasticAbsorber.hh"
#include "SDPlasticScintillator.hh"

void EventAction::BeginOfEventAction(const G4Event*) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    auto crystalHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalScintillatorCollection");
    auto crystalHC = static_cast<HitCollectionCrystal*>(event->GetHCofThisEvent()->GetHC(crystalHCID));

    if (!crystalHC) {
        G4cerr << "Error: CrystalScintillatorCollection not found." << G4endl;
        return;
    }

    auto plasticHCID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScintillatorCollection");
    auto plasticHC = static_cast<HitCollectionPlastic*>(event->GetHCofThisEvent()->GetHC(plasticHCID));

    if (!plasticHC) {
        G4cerr << "Error: PlasticScintillatorCollection not found." << G4endl;
        return;
    }

    auto absorberHCID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticAbsorberCollection");
    auto absorberHC = static_cast<HitCollectionAbsorber*>(event->GetHCofThisEvent()->GetHC(absorberHCID));
    if (!absorberHC) {
        G4cerr << "Error: PlasticAbsorberCollection not found." << G4endl;
        return;
    }

    auto analysisManager = G4AnalysisManager::Instance();

    for (size_t i = 0; i < crystalHC->entries(); i++) {
        auto crystalHit = (*crystalHC)[i];
        if (crystalHit->GetEnergyDeposit() != 0) {
            analysisManager->FillNtupleDColumn(0, 0, crystalHit->GetEnergyDeposit());
            analysisManager->FillNtupleDColumn(0, 1, crystalHit->GetEntryTime());
            analysisManager->FillNtupleIColumn(0, 2, crystalHit->GetPDGID());
            analysisManager->FillNtupleIColumn(0, 3, crystalHit->GetEventID());
            analysisManager->FillNtupleIColumn(0, 4, crystalHit->GetTrackID());

            analysisManager->AddNtupleRow(0);
        }
    }

    for (size_t i = 0; i < plasticHC->entries(); i++) {
        auto plasticHit = (*plasticHC)[i];

        if (plasticHit->GetPDGID() != 0) {
            analysisManager->FillNtupleIColumn(1, 0, plasticHit->GetPDGID());
            analysisManager->FillNtupleDColumn(1, 1, plasticHit->GetTime());
            analysisManager->FillNtupleIColumn(1, 2, plasticHit->GetAnnihilationStatus());
            analysisManager->FillNtupleIColumn(1, 3, plasticHit->GetEventID());
            analysisManager->FillNtupleIColumn(1, 4, plasticHit->GetTrackID());
            analysisManager->FillNtupleIColumn(1, 5, plasticHit->GetHitID());
            analysisManager->FillNtupleSColumn(1, 6, plasticHit->GetProducedProcess());

            analysisManager->AddNtupleRow(1);
        }
    }

    for (size_t i = 0; i < absorberHC->entries(); i++) {
        auto absorberHit = (*absorberHC)[i];

        if (absorberHit->GetPDGID() != 0) {
            analysisManager->FillNtupleIColumn(2, 0, absorberHit->GetPDGID());
            analysisManager->FillNtupleDColumn(2, 1, absorberHit->GetTime());
            analysisManager->FillNtupleIColumn(2, 2, absorberHit->GetAnnihilationStatus());
            analysisManager->FillNtupleIColumn(2, 3, absorberHit->GetEventID());
            analysisManager->FillNtupleIColumn(2, 4, absorberHit->GetTrackID());
            analysisManager->FillNtupleIColumn(2, 5, absorberHit->GetHitID());
            analysisManager->FillNtupleSColumn(2, 6, absorberHit->GetProducedProcess());

            analysisManager->AddNtupleRow(2);
        }
    }
}