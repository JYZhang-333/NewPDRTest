#ifndef SDCrystalScintillator_hh
#define SDCrystalScintillator_hh 1

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "HitCrystalScintillator.hh"

class SDCrystalScintillator : public G4VSensitiveDetector
{
private:
    HitCollectionCrystal *hcCrystal;

public:
    SDCrystalScintillator(const G4String &, const G4String &);
    ~SDCrystalScintillator() override;

    void Initialize(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};

#endif