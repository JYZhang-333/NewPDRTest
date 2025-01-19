#include "ActionInitialization.hh"
#include "FTFP_BERT.hh"
#include "G4DecayPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4Timer.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "newPDRConstruction.hh"
#include "Randomize.hh"

int main(int argc, char** argv) {
    const auto timer = new G4Timer;
    timer->Start();

    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    G4Random::setTheEngine(new CLHEP::MTwistEngine);
    G4Random::setTheSeed(42);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

#ifdef G4MULTITHREADED
    G4int nThreads = 0;
    nThreads = G4Threading::G4GetNumberOfCores();
    runManager->SetNumberOfThreads(nThreads);
#endif

    runManager->SetUserInitialization(new newPDRConstruction());

    G4VModularPhysicsList* physicsList = new FTFP_BERT((0));
    physicsList->RegisterPhysics(new G4OpticalPhysics(0));
    auto opticalParams = G4OpticalParameters::Instance();
    opticalParams->SetBoundaryInvokeSD(true);

    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4(0));
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new ActionInitialization());

    G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    timer->Stop();
    // G4cout << "Time runs: " << *timer << G4endl;

    delete visManager;
    delete runManager;
    delete timer;
}