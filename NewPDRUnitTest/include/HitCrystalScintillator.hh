#ifndef HitCrystalScintillator_hh
#define HitCrystalScintillator_hh

#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class HitCrystalScintillator : public G4VHit
{
private:
    G4double totalEnergyDeposit = 0.;
    G4int pdgID = 0;
    G4double time = 0.;
    G4int finalStatus = 0;
    G4int eventID = 0;
    G4int trackID = 0;

public:
    HitCrystalScintillator() = default;
    ~HitCrystalScintillator() override = default;

    void AddEnergyDeposit(G4double EnergyDeposit) { totalEnergyDeposit += EnergyDeposit; }
    G4double GetEnergyDeposit() const { return totalEnergyDeposit; }

    void AddPDGID(G4int anID) { pdgID = anID; }
    G4int GetPDGID() const { return pdgID; }

    void AddEntryTime(G4double aTime) { time = aTime; }
    G4double GetEntryTime() const { return time; }

    void SetEventID(G4int anEventID) { eventID = anEventID; }
    G4int GetEventID() const { return eventID; }

    void SetTrackID(G4int anTrackID) { trackID = anTrackID; }
    G4int GetTrackID() const { return trackID; }
};

using HitCollectionCrystal = G4THitsCollection<HitCrystalScintillator>;

#endif