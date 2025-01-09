#pragma once

#include <vector>
#include <deque>
#include <map>
#include <set>

#include "Block.hpp"
#include "DataBlock.hpp"
#include "CRC.hpp"

//#include <b15f/b15.h>

class Sender{

protected:

    //für das lesen
    std::vector<uint8_t> inputBuffer;
    std::map<uint16_t, DataBlock> correctlyRecievedBlocks;
    
    //für das senden
    std::map<uint16_t, DataBlock> outputBuffer;  

    CRC USED_CRC_INSTANCE;
    B15F& b15 = B15F::getInstance();

    bool isControlCharacter(char c);
    void addDataBlockToOutputBuffer(std::vector<unsigned char> dataForBlock);
    virtual void writeToB15(int) = 0;
    void sendBlock(Block);

public:

    Sender(CRC crc, bool){};
    uint16_t createDataBlocks();
    void sendDataBlock(uint16_t);
    void sendEndOfTransmitting();
    bool verifyReadBlock();

    virtual void sendAKN() = 0;
    virtual void sendNAKN() = 0;
    virtual bool checkAKN() = 0;

};