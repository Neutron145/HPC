#include <matmul.h>
#include <iostream>

extern void launchMatMulKernel();

MatMul::MatMul() {
    
}

void MatMul::whoami() {
    std::cout << "It is MatMul lab!\n";
    launchMatMulKernel();
}
