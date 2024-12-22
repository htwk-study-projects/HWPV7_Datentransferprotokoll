#include "Sender.hpp"
#include "CRC.hpp"
#include "ControlCharacter.hpp"

#include <iostream>

const CRC Sender::USED_CRC_INSTANCE = CRC();

Sender::Sender(B15f& drv){
    this->b15 = drv;
}

void Sender::createDataBlocks()
{
    std::cerr << "start block creating" << std::endl;
    std::vector<unsigned char> dataBuffer;
    char byte;
    while (std::cin.get(byte)) {
        if (isControlCharacter(byte)) {
            dataBuffer.push_back(static_cast<char>(ControlCharacter::ESC));
        }
        dataBuffer.push_back(byte);
        if (dataBuffer.size() == DataBlock::MAX_LENGTH_DATA) { 
            addBlockToOutputBuffer(dataBuffer);
            dataBuffer.clear();
        }
    }
    if (!dataBuffer.empty()) {
        addBlockToOutputBuffer(dataBuffer);
    }
    std::cerr << "finished block creating" << std::endl;
}

bool Sender::isControlCharacter(char c){
    return  c == static_cast<char>(ControlCharacter::START) || c == static_cast<char>(ControlCharacter::END) ||
            c == static_cast<char>(ControlCharacter::ACK) || c == static_cast<char>(ControlCharacter::NAK) ||
            c== static_cast<char>(ControlCharacter::ESC);
}

void Sender::addBlockToOutputBuffer(std::vector<unsigned char> dataForBlock){
    DataBlock block(dataForBlock, this->USED_CRC_INSTANCE);
    uint16_t blockNumber = block.getBlockNummer();
    this->outputBuffer[blockNumber] = std::move(block);
    this->blockNumbersToSend.push_back(blockNumber);
    this->failedBlockNumbers.insert(blockNumber);
    std::cerr << "Block " << blockNumber << " added." << std::endl;
}

void Sender::send(){
    std::cerr << "start sending" << std::endl;
    while (true) {
        if (blockNumbersToSend.empty()) {
            break;
        }
        uint16_t currentBlockNumber = blockNumbersToSend.front();
        blockNumbersToSend.pop_front();
        DataBlock block = outputBuffer[currentBlockNumber];
        std::cout << "send BLOCK: " << currentBlockNumber << std::endl;
        sendDataBlock(block);
    }
    std::cerr << "end sending" << std::endl;
}

void Sender::sendDataBlock(DataBlock block){
    std::cerr << "sendData aufgerufen" << std::endl;
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : block.getFullDataBlock()) {
        for (int j = 7; j >= 0; j--) {
            bitStream = (bitStream << 1) | ((currentChar >> j) & 0x01);
            bitCount++;
            if (bitCount == 3) {
                writeToB15(bitStream & 0x07);
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitStream = bitStream << (3 - bitCount);
        writeToB15(bitStream & 0x07);
    }
}

// IDEE: in der Pause auf ein Klassenattribut schreiben und durchprüfen nach dem Senden aller Blöcke oder ein thread schreibt und liest und der andere verarbeitet.

void Sender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data | 0b00001000);
    std::bitset<3> a = data;
    std::cerr << a;
    this->b15.delay_ms(15);
    this->b15.setRegister(&PORTA, data | 0b00000000);
    this->b15.delay_ms(15);
}


