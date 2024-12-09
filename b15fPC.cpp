#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>

#include "DataBlock.hpp"
#include "CRC.hpp"

std::map<u_int16_t, DataBlock> outputBuffer;
std::deque<uint16_t> blockNumbersToSend;


std::deque<uint8_t> inputBuffer;
std::mutex inputBufferMutex;


void writeToB15(B15F& drv, int data);
void sendData(B15F& drv, DataBlock& block);
void DataBlockCreating(CRC & crc);
void DataWriting(B15F& drv);

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    //drv.setRegister(&PORTA, 0x0f);
    CRC CRC_Instance = CRC();

    DataBlockCreating(CRC_Instance);
    DataWriting(drv);

    std::cout << (int) (CRC_Instance.verifyDataWithCRC(outputBuffer[0].getFullDataBlock()))<< std::endl;

    drv.setRegister(&PORTA, 0);
    return 0;
}

void DataBlockCreating(CRC & crc) {
    std::cout << "Block creating" << std::endl;
    std::vector<unsigned char> dataBuffer;
    char byte;
    while (std::cin.get(byte)) {
        if (byte == static_cast<char>(ControlCharacter::START) ||
            byte == static_cast<char>(ControlCharacter::END) ||
            byte == static_cast<char>(ControlCharacter::ACK) ||
            byte == static_cast<char>(ControlCharacter::NAK) ||
            byte == static_cast<char>(ControlCharacter::ESC)) {
 
            dataBuffer.push_back(static_cast<char>(ControlCharacter::ESC));
        }
        dataBuffer.push_back(byte);
        if (dataBuffer.size() == 128) { 
            DataBlock block(dataBuffer, crc);
            outputBuffer[block.getBlockNummer()] = block;
            std::cout << block.getBlockNummer() << std::endl;
            blockNumbersToSend.push_back(block.getBlockNummer());
            dataBuffer.clear();
        }
    }
    if (!dataBuffer.empty()) {
        DataBlock block(dataBuffer, crc);
        outputBuffer[block.getBlockNummer()] = block;

        std::cout << block.getBlockNummer() << std::endl;
        blockNumbersToSend.push_back(block.getBlockNummer());

    }
    std::cout << "Finished block creating" << std::endl;
}

void DataWriting(B15F& drv) {
    std::cout << "start sending" << std::endl;
    while (true) {
        if (blockNumbersToSend.empty()) {
            break;
        }
        uint16_t currentBlockNumber = blockNumbersToSend.front();
        blockNumbersToSend.pop_front();
        DataBlock block = outputBuffer[currentBlockNumber];
        std::cout << "send BLOCK: " << currentBlockNumber << std::endl;
        sendData(drv, block);
    }
    std::cout << "end sending" << std::endl;
}

void writeToB15(B15F& drv, int data) {
    drv.setRegister(&PORTA, data | 0b00001000);
    drv.delay_ms(10);
    std::bitset<3> a = data;
    std::cout << a;
    drv.setRegister(&PORTA, 0x00);
}

void sendData(B15F& drv, DataBlock& block) {
    std::cout << "sendData aufgerufen" << std::endl;
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : block.getFullDataBlock()) {
        for (int j = 7; j >= 0; j--) {
            bitStream = (bitStream << 1) | ((currentChar >> j) & 0x01);
            bitCount++;
            if (bitCount == 3) {
                writeToB15(drv, bitStream & 0x07);
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitStream = bitStream << (3 - bitCount);
        writeToB15(drv, bitStream & 0x07);
    }
}

void ReadDataProcessing(){

}