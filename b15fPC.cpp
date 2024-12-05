#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "DataBlock.cpp"
#include "CRC.cpp"

std::map<u_int16_t, DataBlock> outputBuffer;
std::deque<uint16_t> blockNumbersToSend;

std::atomic<bool> isReading{true};
std::mutex mtx;
std::condition_variable cv;

void writeToB15(B15F& drv, int data);
void sendData(B15F& drv, DataBlock& block);
void readFromB15(B15F& drv);
void DataBlockCreating(CRC & crc);
void DataWriting(B15F& drv);

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    CRC CRC_Instance = CRC();

    std::thread DataBlockCreater(DataBlockCreating);
    std::thread DataWriter(DataWriting, std::ref(drv));

    DataBlockCreater.join();
    DataWriter.join();

    drv.setRegister(&PORTA, 0);
    std::cout << std::endl;
}

void DataBlockCreating(CRC & crc) {
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
            {
                std::unique_lock<std::mutex> lock(mtx);
                outputBuffer[block.getBlockNummer()] = block;
                blockNumbersToSend.push_back(block.getBlockNummer());
                cv.notify_one();
            }        
            dataBuffer.clear();
        }
    }
    isReading = false;
    cv.notify_one();
}

void DataWriting(B15F& drv) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !blockNumbersToSend.empty() || !isReading; });

        if (blockNumbersToSend.empty() && !isReading) {
            break;
        }
        uint16_t currentBlockNumber = blockNumbersToSend.front();
        blockNumbersToSend.pop_front();
        DataBlock block = outputBuffer[currentBlockNumber];
        lock.unlock();
        sendData(drv, block);
    }
}

void writeToB15(B15F& drv, int data) {
    drv.setRegister(&PORTA, data | 0b00001000);
    drv.delay_ms(10);
    drv.setRegister(&PORTA, 0x00);
}

void readFromB15(B15F& drv) {
    int currentValue = drv.getRegister(&PINA);
    if (currentValue > 127) {
        std::cout << (currentValue & 0xf0);
    }
}

void sendData(B15F& drv, DataBlock& block) {
    unsigned int bitStream = 0;
    int bitCount = 0;
    for (unsigned char currentChar : block.getFullBlock()) {
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
