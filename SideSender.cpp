#include "SideSender.hpp"

#include <iostream>

SideSender::SideSender(CRC crc) : Sender(crc, false) {
    std::cerr << "SideSender erstellt" << std::endl;
} 

bool SideSender::waitForMainSender(){
    uint8_t currentStatus= this->b15.getRegister(&PINA);
    return currentStatus & 0b00001000;
}