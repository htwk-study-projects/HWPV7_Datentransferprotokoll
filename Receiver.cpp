#include "Receiver.hpp"

Reciever::Reciever(B15F & drv, CRC crc){
    this->b15 = drv;
    this->USED_CRC_INSTANCE = crc;
    this->needToRead = true;
}

void Reciever::read(){
    bool lastClock = false;
    while(needToRead){
        uint8_t currentData = this->b15.getRegister(&PINA);
        if(lastClock && currentData & 0b00001000){              //muss angepasst werden
            this->inputBuffer.push_back(currentData);
            lastClock = true;
        }
    }

}
