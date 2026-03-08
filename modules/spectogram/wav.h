#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct Audio {
    std::vector<float> samples;
    uint16_t channels;
    uint32_t sample_rate;
};

Audio loadWav(const std::string& filename);

