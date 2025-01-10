#include "MainSender.hpp"
#include <iostream>

MainSender::MainSender(CRC crc) : Sender(crc, true) {
    std::cerr << "MainSender erstellt" << std::endl;
}

void MainSender::sendStartOfTransmitting(){
    this->b15.setRegister(&PORTA, 0b00001000);
    delay(10);
    this->b15.setRegister(&PORTA, 0b00000000);
}

void MainSender::writeToB15(int data) {
    this->b15.setRegister(&PORTA, data);
    this->b15.setRegister(&PORTA, data | 0b00001000);
    std::bitset<3> a = data;
    std::cerr << a;
    delay(50);
    uint8_t currentInput = this->b15.getRegister(&PINA);
    if(currentInput & 0b10000000){
        this->inputBuffer.push_back((currentInput >> 4) & 0x07);
        std::cerr<< std::bitset<3>((currentInput >> 4) & 0x07);
    }
    this->b15.setRegister(&PORTA, data | 0b00000000);
    delay(50);
}

void MainSender::sendAKN(){
    std::cerr << "send akn" << std::endl;
    this->b15.setRegister(&PORTA, main_AKN);
}

void MainSender::sendNAKN(){
    std::cerr << "send nakn" << std::endl;
    this->b15.setRegister(&PORTA, main_NAKN);
}

bool MainSender::checkAKN(){
    bool isAKN = (this->b15.getRegister(&PINA) & side_AKN);
    return isAKN;
}

void MainSender::sendEndOfTransmitting(){
    this->b15.setRegister(&PORTA, main_EOT);
}
