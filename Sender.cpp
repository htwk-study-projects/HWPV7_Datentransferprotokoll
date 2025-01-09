#include "Sender.hpp"
#include "DataBlock.hpp"
#include "AKNBlock.hpp"
#include <iostream>

Sender::Sender(CRC crc, bool isMain){
    this->USED_CRC_INSTANCE = crc;
    if(isMain) this->b15.setRegister(&DDRA, 0x0f);
    else this->b15.setRegister(&DDRA, 0xf0);
    this->b15.setRegister(&PORTA, 0);
}

uint16_t Sender::createDataBlocks(){
    std::cerr << "start block creating" << std::endl;
    std::vector<unsigned char> dataBuffer;
    uint16_t blockCounter = 0;
    char byte;
    while (std::cin.get(byte)) {
        if (isControlCharacter(byte)) dataBuffer.push_back(static_cast<char>(ControlCharacter::ESC));
        dataBuffer.push_back(byte);
        if (dataBuffer.size() == DataBlock::MAX_LENGTH_DATA) { 
            addDataBlockToOutputBuffer(dataBuffer);
            dataBuffer.clear();
            blockCounter++;
        }
    }
    if (!dataBuffer.empty()){
        addDataBlockToOutputBuffer(dataBuffer);
        blockCounter++;
    }
    std::cerr << "finished block creating" << std::endl;
    return blockCounter;
}

bool Sender::isControlCharacter(char c){
    return  c == static_cast<char>(ControlCharacter::START) || c == static_cast<char>(ControlCharacter::END) ||
            c == static_cast<char>(ControlCharacter::ACK) || c == static_cast<char>(ControlCharacter::NAK) ||
            c == static_cast<char>(ControlCharacter::ESC);
}

void Sender::addDataBlockToOutputBuffer(std::vector<unsigned char> dataForBlock){
    DataBlock block = DataBlock(dataForBlock, this->USED_CRC_INSTANCE);
    uint16_t blockNumber = block.getBlockNummer();
    this->outputBuffer[blockNumber] = std::move(block);
    std::cerr << "Block " << blockNumber << " added." << std::endl;
}

void Sender::sendDataBlock(uint16_t blockNumberToSend){
    std::cerr << "start sending" << std::endl;
    uint16_t currentBlockNumber = blockNumberToSend;
    Block block = outputBuffer[currentBlockNumber];
    std::cerr << "send BLOCK " << currentBlockNumber << std::endl;
    sendBlock(block);

    std::cerr << "end sending" << std::endl;
}

void Sender::sendBlock(Block block){
    std::cerr << "sendData aufgerufen" << std::endl;
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : block.getFullBlock()) {
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

void Sender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data);
    this->b15.setRegister(&PORTA, data | 0b00001000);
    std::bitset<3> a = data;
    std::cerr << a;
    this->b15.delay_ms(5);
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b10000000){
        this->inputBuffer.push_back((currentInput >> 5));
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
    this->b15.delay_ms(15);
}

void Sender::checkAKN() {
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

bool Sender::verifyReadBlock() {
    std::vector<unsigned char> streamToCharVector;
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (uint8_t i : inputBuffer) {
        bitStream = (bitStream << 3) | (i & 0b00000111);
        bitCount += 3;
        if (bitCount == 24) {
            streamToCharVector.push_back(static_cast<unsigned char>((bitStream >> 16) & 0xff));
            streamToCharVector.push_back(static_cast<unsigned char>((bitStream >> 8) & 0xff));
            streamToCharVector.push_back(static_cast<unsigned char>(bitStream & 0xff));

            bitStream = 0;
            bitCount = 0;
        }
    }
    while (bitCount >= 8) {
        streamToCharVector.push_back(static_cast<unsigned char>((bitStream >> (bitCount - 8)) & 0xff));
        bitCount -= 8;
    }
    streamToCharVector.pop_back();
    return this->USED_CRC_INSTANCE.verifyDataWithCRC(streamToCharVector);
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
