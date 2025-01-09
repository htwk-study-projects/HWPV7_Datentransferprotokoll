#pragma once
#include <deque>
#include <map>
#include <set>
#include <b15f/b15f.h>

#include "DataBlock.hpp"
#include "CRC.hpp"


class Sender{
private:
    std::map<u_int16_t, DataBlock> outputBuffer;
    std::deque<uint16_t> blockNumbersToSend;
    std::set<uint16_t> failedBlockNumbers;  

    std::vector<uint8_t> inputBuffer;

    CRC USED_CRC_INSTANCE;
    B15F& b15 = B15F::getInstance();;

    bool isControlCharacter(char c);
    void addBlockToOutputBuffer(std::vector<unsigned char> dataForBlock);
    void writeToB15(int);
    void sendDataBlock(DataBlock);

public:
    Sender(CRC);
    void createDataBlocks();
    void send();
    void checkAKNFromReceiver();
    bool addBlocksForAdditionalSending();
    void sendEndOfTransmitting();
};