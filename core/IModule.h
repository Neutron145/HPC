#pragma once
#include <cstddef>

//!< Интерфейс для всех лабораторных работ
class IModule {
private:
    //!< Задание параметров вычислений
    virtual void setup() = 0;
    //!< Выполнение вычислений на CPU или GPU
    virtual double runCPU() = 0;
    virtual double runGPU() = 0;

public:
    virtual ~IModule() = default;

    //!< Информация о лабораторной работе
    virtual void whoami() = 0;
    //!< Проведение вычислительного эксперимента и вывод результатов
    virtual void runExperiment() = 0;
};
