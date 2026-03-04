#pragma once
#include <IModule.h>

class MatMul : public IModule {
    public:
        explicit MatMul();

        void whoami() override;
};
