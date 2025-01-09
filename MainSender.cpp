#include "MainSender.hpp"
#include <iostream>

MainSender::MainSender(CRC crc) : Sender(crc, true) {
    std::cerr << "MainSender erstellt" << std::endl;
}

void MainSender::sendStartOfTransmitting(){
    this->b15.setRegister(&PORTA, 0b00001000);
    this->b15.delay_ms(10);
    this->b15.setRegister(&PORTA, 0b00000000);
}

void MainSender::writeToB15(int data) {
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

void MainSender::sendAKN(){
    this->b15.setRegister(&PORTA, 2);
}

void MainSender::sendNAKN(){
    this->b15.setRegister(&PORTA, 1);
}

bool MainSender::checkAKN(){
    bool isAKN = (this->b15.getRegister(&PINA) & 0b00100000);
    if(isAKN) return true;
    bool isAKN = (this->b15.getRegister(&PINA) & 0b00100000);
    return isAKN;
}
