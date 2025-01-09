#include "SideSender.hpp"

#include <iostream>

SideSender::SideSender(CRC crc) : Sender(crc, false) {
    std::cerr << "SideSender erstellt" << std::endl;
} 

bool SideSender::waitForMainSender(){
    uint8_t currentStatus= this->b15.getRegister(&PINA);
    return currentStatus & 0b00001000;
}

void SideSender::sendAKN(){
    this->b15.setRegister(&PORTA, 32);
}

void SideSender::sendNAKN(){
    this->b15.setRegister(&PORTA, 16);
}

bool SideSender::checkAKN(){
    bool isAKN = (this->b15.getRegister(&PINA) & 0b00000010);
    if(isAKN) return true;
    bool isAKN = (this->b15.getRegister(&PINA) & 0b00000010);
    return isAKN;
}

void SideSender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data);
    this->b15.setRegister(&PORTA, (data << 4) | 0b10000000);
    std::bitset<3> a = data;
    std::cerr << a;
    this->b15.delay_ms(5);
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b10000000){
        this->inputBuffer.push_back((currentInput & 0b00000111));
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
    this->b15.delay_ms(15);
}
