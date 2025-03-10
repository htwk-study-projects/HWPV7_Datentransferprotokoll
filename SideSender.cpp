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
    std::cerr << "send akn" << std::endl;
    this->b15.setRegister(&PORTA, side_AKN);
}

void SideSender::sendNAKN(){
    std::cerr << "send nakn" << std::endl;
    this->b15.setRegister(&PORTA, side_NAKN);
}

bool SideSender::checkAKN(){
    bool isAKN = (this->b15.getRegister(&PINA) & main_AKN);
    return isAKN;
}

void SideSender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data);
    this->b15.setRegister(&PORTA, (data << 4) | 0b10000000);
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b00001000){
        this->inputBuffer.push_back((currentInput & 0x07));
        std::cerr << std::bitset<3>((currentInput & 0x07));
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
}
