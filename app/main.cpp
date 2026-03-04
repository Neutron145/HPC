#include <iostream>
#include <IModule.h>
#include <matmul.h>
#include <memory>

using namespace std;

int main() {
    std::unique_ptr<IModule> currentLab = std::make_unique<MatMul>();
    currentLab->whoami();
    return 0;
}
