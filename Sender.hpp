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

    const uint8_t main_EOT   = 0b00000100;
    const uint8_t main_AKN   = 0b00000010;
    const uint8_t main_NAKN  = 0b00000001;
    const uint8_t side_EOT   = 0b01000000;
    const uint8_t side_AKN   = 0b00100000;
    const uint8_t side_NAKN  = 0b00010000;

    void addDataBlockToOutputBuffer(std::vector<unsigned char> dataForBlock);
    virtual void writeToB15(int) = 0;
    void sendBlock(DataBlock);
    void saveCorrectData(std::vector<unsigned char>);

public:

    Sender(CRC crc, bool);
    uint16_t createDataBlocks();
    void sendDataBlock(uint16_t);
    bool verifyReadBlock();
    void delay(int);
    void printReceivedData();

    virtual void sendAKN() = 0;
    virtual void sendNAKN() = 0;
    virtual bool checkAKN() = 0;
    virtual void sendEndOfTransmitting() = 0;

};