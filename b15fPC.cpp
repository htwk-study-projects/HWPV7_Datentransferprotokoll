#include <iostream>
#include <b15f/b15f.h>
#include <map>
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "DataBlock.hpp"
#include "CRC.hpp"

std::map<u_int16_t, DataBlock> outputBuffer;
std::deque<uint16_t> blockNumbersToSend;

std::atomic<bool> isCreating{true};
std::mutex outputBufferMutex;
std::condition_variable cv_output;

std::mutex registerMutex;
std::condition_variable cv_clockPause;
std::atomic<bool> isClockPause{true};

std::deque<uint8_t> inputBuffer;
std::atomic<bool> isSending{true};
std::mutex inputBufferMutex;
std::condition_variable cv_input;


void writeToB15(B15F& drv, int data);
void sendData(B15F& drv, DataBlock& block);
void readFromB15(B15F& drv);
void DataBlockCreating(CRC & crc);
void DataWriting(B15F& drv);
void DataReading(B15F& drv);

int main() {
    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);
    
    CRC CRC_Instance = CRC();

    std::thread DataBlockCreater(DataBlockCreating, std::ref(CRC_Instance));
    std::thread DataWriter(DataWriting, std::ref(drv));

    DataBlockCreater.join();
    DataWriter.join();

    drv.setRegister(&PORTA, 0);
    std::cout << std::endl;
    
}

void DataBlockCreating(CRC & crc) {
    std::cout << "Thread 1" << std::endl;
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
                std::unique_lock<std::mutex> outputBufferLock(outputBufferMutex);
                outputBuffer[block.getBlockNummer()] = block;
                blockNumbersToSend.push_back(block.getBlockNummer());
                cv_output.notify_one();
            }        
            dataBuffer.clear();
        }
    }
    if (!dataBuffer.empty()) {
        DataBlock block(dataBuffer, crc);
        {
            std::unique_lock<std::mutex> outputBufferLock(outputBufferMutex);
            outputBuffer[block.getBlockNummer()] = block;
            blockNumbersToSend.push_back(block.getBlockNummer());
            cv_output.notify_one();
        }
    }
    isCreating = false;
    cv_output.notify_one();
    std::cout << "Thread 1 ende" << std::endl;
}

void DataWriting(B15F& drv) {
    std::cout << "Thread 2" << std::endl;
    while (true) {
        std::unique_lock<std::mutex> outputBufferLock(outputBufferMutex);
        cv_output.wait(outputBufferLock, [] { return !blockNumbersToSend.empty() || !isCreating; });

        if (blockNumbersToSend.empty() && !isCreating) {
            break;
        }
        uint16_t currentBlockNumber = blockNumbersToSend.front();
        blockNumbersToSend.pop_front();
        DataBlock block = outputBuffer[currentBlockNumber];
        outputBufferLock.unlock();
        sendData(drv, block);
    }
    isSending = false;
    std::cout << "Thread 2 ende" << std::endl;
}

void writeToB15(B15F& drv, int data) {
    {
        std::unique_lock<std::mutex> registerLock(registerMutex);
        drv.setRegister(&PORTA, data | 0b00001000);
    }
    drv.delay_ms(5);
    {
        std::unique_lock<std::mutex> registerLock(registerMutex);
        drv.setRegister(&PORTA, 0x00);
    }
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

void DataReading(B15F & drv){
    std::cout << "Thread 3" << std::endl;
    while(isSending){
        std::unique_lock<std::mutex> registerLock(registerMutex);
        cv_clockPause.wait(registerLock, [] { return isClockPause; });
        int currentValue = drv.getRegister(&PINA);
        if (currentValue > 127) {
            std::unique_lock<std::mutex> inputBufferLock(inputBufferMutex);
            inputBuffer.push_back(currentValue & 0b01110000);
        }
    }


    std::cout << "Thread 3 ende" << std::endl;
}

void ReadDataProcessing(){

}
