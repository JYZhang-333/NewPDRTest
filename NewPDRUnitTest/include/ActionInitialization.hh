#ifndef ActionInitialization_hh
#define ActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;
};

#endif
