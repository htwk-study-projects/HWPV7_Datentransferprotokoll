#include "Sender.hpp"
#include "DataBlock.hpp"
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

void Sender::addDataBlockToOutputBuffer(std::vector<unsigned char> dataForBlock){
    DataBlock block = DataBlock(dataForBlock, this->USED_CRC_INSTANCE);
    uint16_t blockNumber = block.getBlockNummer();
    this->outputBuffer[blockNumber] = std::move(block);
    std::cerr << "Block " << blockNumber << " added." << std::endl;
}

void Sender::sendDataBlock(uint16_t blockNumberToSend){
    std::cerr << "start sending" << std::endl;
    uint16_t currentBlockNumber = blockNumberToSend;
    DataBlock block = outputBuffer[currentBlockNumber];
    std::cerr << "send BLOCK " << currentBlockNumber << std::endl;
    sendBlock(block);

    std::cerr << "end sending" << std::endl;
}

void Sender::sendBlock(DataBlock block){
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
    bool isCorrect = this->USED_CRC_INSTANCE.verifyDataWithCRC(streamToCharVector);
    if (isCorrect){
        saveCorrectData(streamToCharVector);
    }
    inputBuffer.clear();
    return isCorrect;
}

void Sender::saveCorrectData(std::vector<unsigned char> dataWithHeaderAndCRC){
    uint16_t blockNummer = dataWithHeaderAndCRC.at(0);
    blockNummer = (blockNummer << 8) | dataWithHeaderAndCRC.at(1);
    std::vector<unsigned char> data;
    dataWithHeaderAndCRC.erase(dataWithHeaderAndCRC.begin(), dataWithHeaderAndCRC.begin() + 2);
    dataWithHeaderAndCRC.erase(dataWithHeaderAndCRC.end() - 2, dataWithHeaderAndCRC.end());
    this->correctlyRecievedData[blockNummer] = dataWithHeaderAndCRC;
}

void Sender::delay(int delay_ms){
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < delay_ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Kleine Schritte
        end = std::chrono::steady_clock::now();
    }
}

void Sender::printReceivedData(){
    for (const auto& pair : this->correctlyRecievedData) {
        for(unsigned char c : pair.second ){
            std::cout << c;
        }
    }
}
