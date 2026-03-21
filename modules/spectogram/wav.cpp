#include <wav.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

#pragma pack(push, 1)
struct WavHeader {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];

    char subchunk1[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};
#pragma pop()

std::ostream& operator<<(std::ostream& os, const WavHeader& header) {
    os << "Format: " << header.audio_format
        << "\nChannels: " << header.num_channels
        << "\nSample rate: " << header.sample_rate << '\n';
    return os;
}

Audio loadWav(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("File not opened");
    }

    WavHeader header;
    if(!file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader))) {
        throw std::runtime_error("Header not readed");
    }

    if (std::strcmp(header.chunk_id, "RIFF") == 0 || std::strcmp(header.format, "WAVE") == 0) {
        throw std::runtime_error("Invalid header");
    }

    if (header.audio_format != 1) {
        throw std::runtime_error("WAV file is not PCM. audio_format: " + std::to_string(header.audio_format));
    }

    if (header.bits_per_sample != 16) {
        throw std::runtime_error("Bits per sample is not 16 bit. bits_per_sample: " + std::to_string(header.bits_per_sample));
    }

    char subchunk_id[5] = {0};
    uint32_t subchunk2_size = 0;
    bool data_subchunk_if_found = false;

    while(file.read(subchunk_id, 4)) {
        file.read(reinterpret_cast<char*>(&subchunk2_size), 4);
        if(std::strcmp(subchunk_id, "data") == 0) {
            data_subchunk_if_found = 1;
            break;
        }

        file.seekg(subchunk2_size, std::ios::cur);

    }
    
    if (!data_subchunk_if_found) {
        throw std::runtime_error("Data not found");
    }

    std::cout << header;

    int count_samples = subchunk2_size / (header.bits_per_sample / 8);
    std::vector<int16_t> raw_samples(count_samples);
    file.read(reinterpret_cast<char*>(raw_samples.data()), count_samples);

    Audio audio;
    audio.sample_rate = header.sample_rate;
    audio.channels = header.num_channels;
    audio.samples.reserve(count_samples);

    for(int i = 0; i < raw_samples.size(); i++) {
        audio.samples.push_back(static_cast<float>(raw_samples[i]) / 32768.0f);
    }

    return audio;
}
