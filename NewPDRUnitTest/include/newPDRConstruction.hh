#ifndef newPDRConstruction_hh
#define newPDRConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "tls.hh"

#include <utility>
#include <vector>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

class DetectorMessenger;

class newPDRConstruction : public G4VUserDetectorConstruction
{
public:
    newPDRConstruction();
    ~newPDRConstruction() override;

    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;
};

#endif