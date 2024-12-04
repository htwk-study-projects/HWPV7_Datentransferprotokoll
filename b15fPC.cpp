#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <bitset>
#include "DataBlock.cpp"  // Externe Datei für DataBlock

std::map<u_int16_t, DataBlock> outputBuffer;
std::mutex mtx;
std::condition_variable cv;

void writeToB15(B15F& drv, int data);
void sendData(B15F& drv, DataBlock& block);
void readFromB15(B15F& drv);

void DataBlockCreating();
void DataWriting(B15F& drv);

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);

    std::thread DataBlockCreater(DataBlockCreating);
    std::thread DataWriter(DataWriting, std::ref(drv));

    DataBlockCreater.join();
    DataWriter.join();

    drv.setRegister(&PORTA, 0);
    std::cout << std::endl;
}

void DataBlockCreating() {
    while(std:cin){
        std::vector<char> data;

        
    }
}

void DataWriting(B15F& drv) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !outputBuffer.empty(); });

        DataBlock block = outputBuffer.front();
        outputBuffer.pop();
        lock.unlock();
        sendData(drv, block);
    }
}

void writeToB15(B15F& drv, int data) {
    drv.setRegister(&PORTA, data | 0b00001000);
    drv.delay_ms(10);
    drv.setRegister(&PORTA, 0x00);
    std::bitset<3> bin(data);
    std::cout << bin << std::endl;
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
    for (char currentChar : block.getFullBlock()) {
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
