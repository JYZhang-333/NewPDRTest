#ifndef SDPlasticScintillator_hh
#define SDPlasticScintillator_hh

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "HitPlasticScintillator.hh"

class SDPlasticScintillator : public G4VSensitiveDetector
{
private:
    HitCollectionPlastic *hcPlastic;

public:
    SDPlasticScintillator(const G4String &, const G4String &);
    virtual ~SDPlasticScintillator() override;

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};

#endif