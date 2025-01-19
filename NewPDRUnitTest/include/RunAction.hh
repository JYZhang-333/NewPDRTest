#ifndef RunAction_hh
#define RunAction_hh 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class G4Timer;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run *run) override;
    void EndOfRunAction(const G4Run *run) override;
};

#endif