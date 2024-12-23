#pragma once

#include <vector>
#include <map>
#include <b15f/b15f.h>

#include "DataBlock.hpp"
#include "CRC.hpp"

class Reciever{
private:
    std::vector<uint8_t> inputBuffer;
    std::map<u_int16_t, DataBlock> correctlyRecievedBlocks;

    CRC USED_CRC_INSTANCE;
    B15F& b15;

public:
    Reciever(B15F&, CRC);

};