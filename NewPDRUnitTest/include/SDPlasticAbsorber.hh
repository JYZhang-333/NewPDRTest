#ifndef SDPlasticAbsorber_hh
#define SDPlasticAbsorber_hh

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "HitPlasticAbsorber.hh"

class SDPlasticAbsorber : public G4VSensitiveDetector
{
private:
    HitCollectionAbsorber *hcAbsorber;

public:
    SDPlasticAbsorber(const G4String &, const G4String &);
    virtual ~SDPlasticAbsorber() override;

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};

#endif