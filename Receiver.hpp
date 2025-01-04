#pragma once

#include <vector>
#include <deque>
#include <map>
//#include <b15f/b15f.h>

#include "DataBlock.hpp"
#include "CRC.hpp"
#include "AKNBlock.hpp"

class Receiver{
private:
    std::vector<uint8_t> inputBuffer;
    std::map<u_int16_t, DataBlock> correctlyRecievedBlocks;
    std::deque<AKNBlock> AKNBlocks;

    CRC USED_CRC_INSTANCE;
    B15F& b15 = B15F::getInstance();;

    bool needToRead;

    void readWithSendAKN(AKNBlock);
    void readWithoutSendAKN();
    void contactB15(int, bool);

    bool verifyDataBlock();


public:
    Receiver(CRC);

    bool hasTransmissionStarted();

    void read();
    void processReadBlocks();

};