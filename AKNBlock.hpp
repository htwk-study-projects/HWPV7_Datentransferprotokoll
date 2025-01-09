#pragma once
#include <cstdint>
#include <vector>
#include "Block.hpp"

class AKNBlock : public Block{
    private:
        unsigned char controlChar;
        uint16_t blockNummer;

    public:
    AKNBlock(unsigned char, uint16_t);
    std::vector<unsigned char> getFullBlock();

};