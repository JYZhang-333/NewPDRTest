#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Timer.hh"
#include "HitCrystalScintillator.hh"
#include "HitPlasticAbsorber.hh"
#include "HitPlasticScintillator.hh"
#include "newPDRConstruction.hh"
#include "SDCrystalScintillator.hh"
#include "SDPlasticAbsorber.hh"
#include "SDPlasticScintillator.hh"

RunAction::RunAction() :
    G4UserRunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
    auto analysisManager = G4AnalysisManager::Instance();

#ifdef G4MULTITHREADED
    analysisManager->SetNtupleMerging(true);
#endif

    analysisManager->OpenFile("test2.root");

    analysisManager->CreateNtuple("CrystalScintillator", "CrystalScintillator");
    analysisManager->CreateNtupleDColumn("EnergyDeposit");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleIColumn("pdgID");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleIColumn("HitID");
    analysisManager->CreateNtupleIColumn("ProducedProcess");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("PlasticScintillator", "PlasticScintillator");
    analysisManager->CreateNtupleIColumn("pdgID");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleIColumn("annihilatedStatus");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleIColumn("HitID");
    analysisManager->CreateNtupleSColumn("ProducedProcess");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("Absorber", "Absorber");
    analysisManager->CreateNtupleIColumn("pdgID");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleIColumn("annihilatedStatus");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleIColumn("HitID");
    analysisManager->CreateNtupleSColumn("ProducedProcess");
    analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    analysisManager->Clear();
}