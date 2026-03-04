#pragma once

class IModule {
public:
    virtual ~IModule() = default;

    virtual void whoami() = 0;

};
