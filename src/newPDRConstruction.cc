#include "newPDRConstruction.hh"

#include "G4Box.hh"                 // box shape
#include "G4LogicalVolume.hh"       // logical volume
#include "G4NistManager.hh"         // NIST material manager
#include "G4PVPlacement.hh"         // placement of logical volume
#include "G4SDManager.hh"           // manager for sensitive detector
#include "G4SystemOfUnits.hh"       // unit definitions
#include "SDCrystalScintillator.hh" // SD for the crystal scintillator
#include "SDPlasticAbsorber.hh"     // SD for the plastic absorber
#include "SDPlasticScintillator.hh" // SD for the plastic scintillator

newPDRConstruction::newPDRConstruction() {}
newPDRConstruction::~newPDRConstruction() {}

G4VPhysicalVolume* newPDRConstruction::Construct() {
    // define the materials that define the detector will used
    const auto nistManager = G4NistManager::Instance();

    const auto galactic = new G4Material("galactic", 1, 1.008 * g / mole, 1.e-25 * g / cm3, kStateGas, 2.73 * kelvin, 3.e-18 * pascal);
    const auto air = nistManager->FindOrBuildMaterial("G4_AIR");
    const auto alumium = nistManager->FindOrBuildMaterial("G4_Al");
    const auto silicon = nistManager->FindOrBuildMaterial("G4_Si");
    const auto pvc = nistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
    const auto Cu = nistManager->FindOrBuildMaterial("G4_Cu");
    const auto Al = nistManager->FindOrBuildMaterial("G4_Al");

    // define the elements that will be used in the detector construction
    const auto carbonElement = nistManager->FindOrBuildElement("C");
    const auto hydrogenElement = nistManager->FindOrBuildElement("H");
    const auto oxygenElement = nistManager->FindOrBuildElement("O");

    const auto siliconElement = nistManager->FindOrBuildElement("Si");
    const auto cesiumElement = nistManager->FindOrBuildElement("Cs");
    const auto iododeElement = nistManager->FindOrBuildElement("I");
    const auto thaliumElement = nistManager->FindOrBuildElement("Tl");

    // define the silicon oil for the SiPM and detector coupling
    const auto siliconOil = new G4Material("siliconOil", 0.97 * g / cm3, 4, kStateSolid);
    siliconOil->AddElement(carbonElement, 2);
    siliconOil->AddElement(hydrogenElement, 6);
    siliconOil->AddElement(oxygenElement, 1);
    siliconOil->AddElement(siliconElement, 1);

    // define the crystal for the additional detector to detect the 511 gamma
    const auto csI = new G4Material("csI", 4.51 * g / cm3, 3, kStateSolid);
    csI->AddElement(cesiumElement, 0.507556);
    csI->AddElement(iododeElement, 0.484639);
    csI->AddElement(thaliumElement, 0.007805);

    ///////////////////////////////////////////////////////////////////////////////////////////
    // construct the volumnes for the detector
    ///////////////////////////////////////////////////////////////////////////////////////////

    const auto worldSize = 2 * m;
    const auto solidWorld = new G4Box("world", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    const auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    const auto physicsWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0, true);

    // put the detector into the world
    const auto scintillatorWidth = 120 * mm;
    const auto scintillatorHeight = 175 * mm;
    const auto scintillatorThickness = 50 * mm;

    const auto airGapThickness = 1 * mm;
    const auto absorberThickness = 35 * mm;

    const auto solidPlasticScin = new G4Box("scintillator", 0.5 * scintillatorThickness, 0.5 * scintillatorWidth, 0.5 * scintillatorHeight);
    const auto logicPlasticScin = new G4LogicalVolume(solidPlasticScin, pvc, "plastic_scintillator");
    const auto physicPlasticScin = new G4PVPlacement(nullptr,
                                                     G4ThreeVector(150 * mm + 0.5 * scintillatorThickness, 0, 150 * mm + 0.5 * scintillatorHeight),
                                                     logicPlasticScin,
                                                     "plastic_scintillator",
                                                     logicWorld,
                                                     false,
                                                     0,
                                                     true);

    // put the absorber between the plastic scintillator and the crystal scintillator
    const auto solidAbsorber = new G4Box("Absorber", 0.5 * absorberThickness, 0.5 * scintillatorWidth, 0.5 * scintillatorHeight);
    // const auto logicAbsorber = new G4LogicalVolume(solidAbsorber, pvc, "plastic_absorber");
    const auto logicAbsorber = new G4LogicalVolume(solidAbsorber, Al, "plastic_absorber");
    const auto physicAbsorber = new G4PVPlacement(nullptr,
                                                  G4ThreeVector(150 * mm + scintillatorThickness + airGapThickness + 0.5 * absorberThickness, 0, 150 + 0.5 * scintillatorHeight),
                                                  logicAbsorber,
                                                  "plastic_absorber",
                                                  logicWorld,
                                                  false,
                                                  0,
                                                  true);

    const auto csIWidth = 120 * mm;
    const auto csIThickness = 15 * mm;
    const auto csIHeigth = 175 * mm;

    const auto solidCsI = new G4Box("csI", 0.5 * csIThickness, 0.5 * csIWidth, 0.5 * csIHeigth);
    const auto logicCsI = new G4LogicalVolume(solidCsI, csI, "Crystal_Scintillator");
    const auto physicCsI = new G4PVPlacement(nullptr,
                                             G4ThreeVector(150 * mm + scintillatorThickness + airGapThickness + absorberThickness + airGapThickness + 0.5 * csIThickness, 0, 150 * mm + 0.5 * scintillatorHeight),
                                             logicCsI,
                                             "Crystal_Scintillator",
                                             logicWorld,
                                             false,
                                             0,
                                             true);

    return physicsWorld;
}

void newPDRConstruction::ConstructSDandField() {

    auto SDPlasticScin = new SDPlasticScintillator("SDPlasticScintillator", "PlasticScintillatorCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(SDPlasticScin);
    SetSensitiveDetector("plastic_scintillator", SDPlasticScin, true);

    auto SDCsI = new SDCrystalScintillator("SDCrystalScintillator", "CrystalScintillatorCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(SDCsI);
    SetSensitiveDetector("Crystal_Scintillator", SDCsI, true);

    auto SDPlasticAbs = new SDPlasticAbsorber("SDPlasticAbsorber", "PlasticAbsorberCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(SDPlasticAbs);
    SetSensitiveDetector("plastic_absorber", SDPlasticAbs, true);
}