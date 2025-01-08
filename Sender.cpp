#include "Sender.hpp"
#include "ControlCharacter.hpp"

#include <iostream>

Sender::Sender(CRC crc){
    this->USED_CRC_INSTANCE = crc;
    this->b15.setRegister(&DDRA, 0x0f);
    this->b15.setRegister(&PORTA, 0);
}

void Sender::createDataBlocks(){
    std::cerr << "start block creating" << std::endl;
    std::vector<unsigned char> dataBuffer;
    char byte;
    while (std::cin.get(byte)) {
        if (isControlCharacter(byte)) dataBuffer.push_back(static_cast<char>(ControlCharacter::ESC));
        dataBuffer.push_back(byte);
        if (dataBuffer.size() == DataBlock::MAX_LENGTH_DATA) { 
            addBlockToOutputBuffer(dataBuffer);
            dataBuffer.clear();
        }
    }
    if (!dataBuffer.empty()) addBlockToOutputBuffer(dataBuffer);
    std::cerr << "finished block creating" << std::endl;
}

bool Sender::isControlCharacter(char c){
    return  c == static_cast<char>(ControlCharacter::START) || c == static_cast<char>(ControlCharacter::END) ||
            c == static_cast<char>(ControlCharacter::ACK) || c == static_cast<char>(ControlCharacter::NAK) ||
            c == static_cast<char>(ControlCharacter::ESC);
}

void Sender::addBlockToOutputBuffer(std::vector<unsigned char> dataForBlock){
    DataBlock block = DataBlock(dataForBlock, this->USED_CRC_INSTANCE);
    uint16_t blockNumber = block.getBlockNummer();
    this->outputBuffer[blockNumber] = std::move(block);
    this->blockNumbersToSend.push_back(blockNumber);
    this->failedBlockNumbers.insert(blockNumber);
    std::cerr << "Block " << blockNumber << " added." << std::endl;
}

void Sender::send(){
    std::cerr << "start sending" << std::endl;
    while (!blockNumbersToSend.empty()) {
        uint16_t currentBlockNumber = blockNumbersToSend.front();
        blockNumbersToSend.pop_front();
        DataBlock block = outputBuffer[currentBlockNumber];
        std::cerr << "send BLOCK " << currentBlockNumber << std::endl;
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

// Tatsächlicher clockpin für empfangen muss noch bestimmt werden

void Sender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data | 0b00001000);
    std::bitset<3> a = data;
    std::cerr << a;
    this->b15.delay_ms(15);
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b00010000){
        this->inputBuffer.push_back((currentInput >> 5));
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
    this->b15.delay_ms(20);
}

bool Sender::addBlocksForAdditionalSending(){
    if(failedBlockNumbers.empty()) return false;
    for(uint16_t i : failedBlockNumbers){
        blockNumbersToSend.push_back(i);
    }
    return true;
}

void Sender::checkAKNFromReceiver() {
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (uint8_t i : inputBuffer) {
        bitStream = (bitStream << 3) | (i & 0b00000111);
        bitCount += 3;
        if (bitCount == 24) {
            uint8_t firstByte = (bitStream >> 16) & 0xff;
            uint16_t blockNum = bitStream & 0xffff;
            if (firstByte == static_cast<uint8_t>(ControlCharacter::ACK)) {
                this->failedBlockNumbers.erase(blockNum);
            }
            bitCount = 0;
            bitStream = 0;
        }
    }
}

void Sender::sendEndOfTransmitting(){
    // 3 END-Zeichen hintereinander
    const std::vector<unsigned char> EOT = {static_cast<unsigned char>(ControlCharacter::END), static_cast<unsigned char>(ControlCharacter::END), static_cast<unsigned char>(ControlCharacter::END)};
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : EOT) {
        for (int j = 7; j >= 0; j--) {
            bitStream = (bitStream << 1) | ((currentChar >> j) & 0x01);
            bitCount++;
            if (bitCount == 3) {
                writeToB15(bitStream & 0x07);
                bitCount = 0;
            }
        }
    }
}
