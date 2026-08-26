#include "Modules/ModuleManager.h"

class FAdMobPluginModule : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FAdMobPluginModule, AdMobPlugin)
