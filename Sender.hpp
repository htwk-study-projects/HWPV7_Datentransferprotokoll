#pragma once

#include <vector>
#include <deque>
#include <map>
#include <set>

#include "DataBlock.hpp"
#include "CRC.hpp"

//#include <b15f/b15f.h>

class Sender{

protected:

    //für das lesen
    std::vector<uint8_t> inputBuffer;
    std::map<uint16_t, std::vector<unsigned char>> correctlyRecievedData;
    
    //für das senden
    std::map<uint16_t, DataBlock> outputBuffer;  

    CRC USED_CRC_INSTANCE;
    B15F& b15 = B15F::getInstance();

    bool isControlCharacter(char c);
    void addDataBlockToOutputBuffer(std::vector<unsigned char> dataForBlock);
    virtual void writeToB15(int) = 0;
    void sendBlock(DataBlock);
    void saveCorrectData(std::vector<unsigned char>);

public:

    Sender(CRC crc, bool);
    uint16_t createDataBlocks();
    void sendDataBlock(uint16_t);
    void sendEndOfTransmitting();
    bool verifyReadBlock();
    void delay(int);
    void printReceivedData();

    virtual void sendAKN() = 0;
    virtual void sendNAKN() = 0;
    virtual bool checkAKN() = 0;

};