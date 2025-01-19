#ifndef HitPlasticAbsorber_hh
#define HitPlasticAbsorber_hh

#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class HitPlasticAbsorber : public G4VHit
{
private:
    G4int pdgID = 0;
    G4double time = 0.;
    G4int annihilatedStatus = 0;
    G4String producedProcess = "unknown";
    G4int eventID = 0;
    G4int trackID = 0;
    G4int hitID = 0;

public:
    HitPlasticAbsorber() = default;
    HitPlasticAbsorber(const HitPlasticAbsorber &) = default;
    ~HitPlasticAbsorber() override = default;

    void AddPDGID(G4int id) { pdgID = id; }
    G4int GetPDGID() const { return pdgID; }

    void AddTime(G4double anTime) { time = anTime; }
    G4double GetTime() const { return time; }

    void SetProducedProcess(G4String aProcess) { producedProcess = aProcess; }
    G4String GetProducedProcess() const { return producedProcess; }

    void SetAnnihilatedStatus(G4int status) { annihilatedStatus = status; }
    G4int GetAnnihilationStatus() const { return annihilatedStatus; }

    void SetEventID(G4int anEventID) { eventID = anEventID; }
    G4int GetEventID() const { return eventID; }

    void SetTrackID(G4int anTrackID) { trackID = anTrackID; }
    G4int GetTrackID() const { return trackID; }

    void SetHitID(G4int anHitID) { hitID = anHitID; }
    G4int GetHitID() const { return hitID; }
};

using HitCollectionAbsorber = G4THitsCollection<HitPlasticAbsorber>;
#endif