#pragma once
#include <cstdint>
#include <vector>

class AKNBlock{
    private:
        unsigned char controlChar;
        uint16_t blockNummer;
    public:

    AKNBlock(unsigned char, uint16_t);
    std::vector<unsigned char> getFullAKNBlock();

};