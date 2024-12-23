#pragma once

#include <vector>
#include <deque>
#include <map>
#include <b15f/b15f.h>

#include "DataBlock.hpp"
#include "CRC.hpp"
#include "AKNBlock.hpp"

class Reciever{
private:
    std::vector<uint8_t> inputBuffer;
    std::map<u_int16_t, DataBlock> correctlyRecievedBlocks;
    std::deque<AKNBlock> AKNBlocks;

    CRC USED_CRC_INSTANCE;
    B15F& b15;

    bool needToRead;

    void sendAKN(AKNBlock);
    bool verifyDataBlock();

public:
    Reciever(B15F&, CRC);

    void read();
    void processReadBlocks();

};